#include "state_machine.h"
#include "actuators.h"
#include "config.h"
#include "logger.h"
#include "thermal.h"
#include <string.h>

StateMachine gStateMachine;

void StateMachine::begin() {
    _state = S_IDLE;
    _token = true;
    actuators::displayReady();
    gLogger.info("state machine ready for 4-index selector chute");
}

void StateMachine::poll() {
    uint32_t now = millis();
    if (_deadlineMs == 0) return;

    switch (_state) {
        case S_FEED:
            if (now > _deadlineMs) haltOnError(ERR_JAM_CHUTE);
            break;
        case S_APPROACH:
            if (now > _deadlineMs) haltOnError(ERR_JAM_APPROACH);
            break;
        case S_SEATED:
            if (now >= _deadlineMs) {
                _deadlineMs = 0;
                transition(S_SENSING);
                gLogger.info("chamber settled; waiting for SENSING_DONE");
            }
            break;
        case S_RELEASED:
            if (now >= _deadlineMs) {
                _deadlineMs = 0;
                pushEvent(EventType::DROP_WINDOW_DONE);
            }
            break;
        case S_CONFIRM:
            if (now > _deadlineMs) haltOnError(ERR_MISS_BIN);
            break;
        case S_RESET:
            if (now > _deadlineMs) haltOnError(ERR_PLATFORM_STUCK);
            break;
        default:
            break;
    }
}

void StateMachine::process(const Event& e) {
    if (e.type == EventType::NONE) return;

    switch (_state) {
        case S_IDLE:       onIdle(e); break;
        case S_FEED:       onFeed(e); break;
        case S_APPROACH:   onApproach(e); break;
        case S_SEATED:     onSeated(e); break;
        case S_SENSING:    onSensing(e); break;
        case S_INDEXED:    onIndexed(e); break;
        case S_RELEASED:   onReleased(e); break;
        case S_CONFIRM:    onConfirm(e); break;
        case S_RESET:      onReset(e); break;
        case S_COMPLETE:   onComplete(e); break;
        case S_ERROR_HALT: onErrorHalt(e); break;
    }
}

uint8_t StateMachine::binCount(uint8_t bin) const {
    if (bin < 1 || bin > 4) return 0;
    return _binCounts[bin - 1];
}

void StateMachine::onIdle(const Event& e) {
    if (e.type != EventType::START_BUTTON) return;
    startRun();
    startNextBrick();
}

void StateMachine::onFeed(const Event& e) {
    if (e.type == EventType::ENTRY_DETECTED) {
        _brick.entryMs = e.timestamp_ms;
        _deadlineMs = e.timestamp_ms + APPROACH_TIMEOUT_MS;
        transition(S_APPROACH);
    }
}

void StateMachine::onApproach(const Event& e) {
    if (e.type == EventType::CHAMBER_SEATED) {
        _brick.seatedMs = e.timestamp_ms;
        actuators::stopConveyorFeed();
        _token = false;
        _deadlineMs = e.timestamp_ms + SETTLE_MS;
        transition(S_SEATED);
    }
}

void StateMachine::onSeated(const Event& e) {
    if (e.type == EventType::SENSING_DONE) {
        transition(S_SENSING);
        onSensing(e);
    }
}

void StateMachine::onSensing(const Event& e) {
    if (e.type != EventType::SENSING_DONE) return;

    _brick.sense = e.senseResult;
    if (_brick.sense.category == BrickCategory::UNCERTAIN) {
        haltOnError(ERR_SENSOR_FAULT);
        return;
    }

    _brick.targetBin = binFor(_brick.sense.category);
    _brick.selectorSteps = stepsForBin(_brick.targetBin);

    gLogger.classified(_brick.number, _brick.sense.category, _brick.targetBin, _brick.sense.sampleCount);
    actuators::displaySorting(_brick.number, TOTAL_BRICKS, _brick.targetBin, _binCounts);
    transition(S_INDEXED);

    bool ok = actuators::indexSelectorToBin(_brick.targetBin);
    gThermal.onSelectorMove();
    pushEventSelectorReady(_brick.targetBin, ok, _brick.selectorSteps);
}

void StateMachine::onIndexed(const Event& e) {
    if (e.type != EventType::SELECTOR_READY) return;

    if (!e.ok) {
        haltOnError(ERR_SELECTOR_JAM);
        return;
    }

    _brick.selectorReadyMs = e.timestamp_ms;
    gLogger.selectorReady(_brick.number, _brick.targetBin, e.steps, true,
                          actuators::selectorPositionLabel(_brick.targetBin));

    transition(S_RELEASED);
    actuators::firePlatformRelease();
    gThermal.onSolenoidFire(1);
    _brick.releaseMs = millis();
    gLogger.releaseFired(_brick.number, _brick.targetBin);
    _deadlineMs = _brick.releaseMs + DROP_WINDOW_MS;
}

void StateMachine::onReleased(const Event& e) {
    if (e.type != EventType::DROP_WINDOW_DONE) return;

    gLogger.dropWindowDone(_brick.number, _brick.targetBin);
    transition(S_CONFIRM);
    _deadlineMs = e.timestamp_ms + BIN_CONFIRM_TIMEOUT_MS;
}

void StateMachine::onConfirm(const Event& e) {
    if (e.type != EventType::BIN_CONFIRMED) return;

    _brick.actualBin = e.binIdx;
    _brick.confirmMs = e.timestamp_ms;
    uint32_t transitMs = _brick.confirmMs - _brick.releaseMs;

    if (_brick.actualBin != _brick.targetBin) {
        gLogger.binConfirm(_brick.number, _brick.targetBin, _brick.actualBin, transitMs, false);
        haltOnError(ERR_MISS_BIN);
        return;
    }

    _brick.confirmed = true;
    _binCounts[_brick.targetBin - 1]++;
    gLogger.binConfirm(_brick.number, _brick.targetBin, _brick.actualBin, transitMs, true);
    transition(S_RESET);
    _deadlineMs = e.timestamp_ms + PLATFORM_LEVEL_TIMEOUT_MS;
}

void StateMachine::onReset(const Event& e) {
    if (e.type == EventType::PLATFORM_LEVEL) {
        _brick.platformLevelMs = e.timestamp_ms;
        gLogger.platformLevel(_brick.number);
        _brickCount++;
        _token = true;
        _deadlineMs = 0;

        if (_brickCount >= TOTAL_BRICKS) {
            endRun();
            transition(S_COMPLETE);
        } else {
            startNextBrick();
        }
    }
}

void StateMachine::onComplete(const Event& e) {
    if (e.type != EventType::START_BUTTON && e.type != EventType::RESET) return;
    _brickCount = 0;
    memset(_binCounts, 0, sizeof(_binCounts));
    _token = true;
    transition(S_IDLE);
    actuators::displayReady();
}

void StateMachine::onErrorHalt(const Event& e) {
    if (e.type != EventType::RESET) return;
    _brickCount = 0;
    memset(_binCounts, 0, sizeof(_binCounts));
    _token = true;
    _deadlineMs = 0;
    transition(S_IDLE);
    actuators::displayReady();
}

void StateMachine::startRun() {
    _brickCount = 0;
    memset(_binCounts, 0, sizeof(_binCounts));
    _runStartMs = millis();
    _token = true;
    actuators::homeSelector();
    gLogger.info("run started");
}

void StateMachine::endRun() {
    uint32_t totalMs = millis() - _runStartMs;
    actuators::stopConveyorFeed();
    actuators::homeSelector();
    actuators::displayComplete(totalMs, _binCounts);
    gLogger.runComplete(totalMs, _binCounts, expectedCountsMatch());
}

void StateMachine::startNextBrick() {
    if (!_token) {
        haltOnError(ERR_DOUBLE_ENTRY);
        return;
    }

    _brick = BrickRecord{};
    _brick.number = _brickCount + 1;
    actuators::startConveyorFeed();
    _deadlineMs = millis() + FEED_TIMEOUT_MS;
    transition(S_FEED);
}

void StateMachine::haltOnError(ErrorCode code) {
    _errorCode = code;
    _deadlineMs = 0;
    actuators::stopConveyorFeed();
    actuators::stopSelector();
    actuators::releaseOff();
    actuators::displayError(_brick.number, _brick.targetBin, errorName(code));
    actuators::buzzerError();
    gLogger.errorHalt(_brick.number, _brick.targetBin, errorName(code));
    transition(S_ERROR_HALT);
}

uint16_t StateMachine::stepsForBin(uint8_t bin) const {
    switch (bin) {
        case 1: return SELECTOR_BIN1_STEPS;
        case 2: return SELECTOR_BIN2_STEPS;
        case 3: return SELECTOR_BIN3_STEPS;
        case 4: return SELECTOR_BIN4_STEPS;
        default: return SELECTOR_BIN4_STEPS;
    }
}

bool StateMachine::expectedCountsMatch() const {
    return _binCounts[0] == EXPECTED_BIN1 &&
           _binCounts[1] == EXPECTED_BIN2 &&
           _binCounts[2] == EXPECTED_BIN3 &&
           _binCounts[3] == EXPECTED_BIN4;
}

uint8_t StateMachine::binFor(BrickCategory cat) {
    switch (cat) {
        case BrickCategory::CAT_2x2_RED:  return 1;
        case BrickCategory::CAT_2x2_BLUE: return 2;
        case BrickCategory::CAT_2x3_BLUE: return 3;
        case BrickCategory::CAT_2x3_RED:  return 4;
        default:                         return 4;
    }
}

void StateMachine::transition(SystemState next) {
    gLogger.stateChange(stateName(), stateNameFor(next));
    _state = next;
}

const char* StateMachine::stateName() const {
    return stateNameFor(_state);
}

const char* StateMachine::stateNameFor(SystemState s) {
    switch (s) {
        case S_IDLE: return "IDLE";
        case S_FEED: return "FEED";
        case S_APPROACH: return "APPROACH";
        case S_SEATED: return "SEATED";
        case S_SENSING: return "SENSING";
        case S_INDEXED: return "INDEXED";
        case S_RELEASED: return "RELEASED";
        case S_CONFIRM: return "CONFIRM";
        case S_RESET: return "RESET";
        case S_COMPLETE: return "COMPLETE";
        case S_ERROR_HALT: return "ERROR_HALT";
        default: return "UNKNOWN";
    }
}

const char* StateMachine::errorName(ErrorCode code) const {
    switch (code) {
        case ERR_JAM_CHUTE: return "JAM_CHUTE";
        case ERR_JAM_APPROACH: return "JAM_APPROACH";
        case ERR_SENSOR_FAULT: return "SENSOR_FAULT";
        case ERR_SELECTOR_JAM: return "SELECTOR_JAM";
        case ERR_MISS_BIN: return "MISS_BIN";
        case ERR_DOUBLE_ENTRY: return "DOUBLE_ENTRY";
        case ERR_PLATFORM_STUCK: return "PLATFORM_STUCK";
        case ERR_POSITION_DRIFT: return "POSITION_DRIFT";
        default: return "UNKNOWN_ERROR";
    }
}
