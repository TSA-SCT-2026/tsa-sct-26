#include "state_machine.h"
#include "actuators.h"
#include "sensors.h"
#include "thermal.h"
#include "logger.h"
#include "config.h"

StateMachine gStateMachine;

void StateMachine::begin() {
    _state = S_IDLE;
    _token = true;
    actuators::displayReady();
    gLogger.info("V6 state machine ready");
}

void StateMachine::process(const Event& e) {
    if (e.type == EventType::NONE) return;

    checkTimeout();

    switch (_state) {
        case S_IDLE:       onIdle(e);       break;
        case S_FEED:       onFeed(e);       break;
        case S_APPROACH:   onApproach(e);   break;
        case S_SEATED:     onSeated(e);     break;
        case S_SENSING:    onSensing(e);    break;
        case S_INDEXED:    onIndexed(e);    break;
        case S_RELEASED:   onReleased(e);   break;
        case S_CONFIRM:    onConfirm(e);    break;
        case S_RESET:      onReset(e);      break;
        case S_COMPLETE:   onComplete(e);   break;
        case S_ERROR_HALT: onErrorHalt(e);  break;
    }
}

uint8_t StateMachine::binCount(uint8_t bin) const {
    if (bin < 1 || bin > 4) return 0;
    return _binCounts[bin - 1];
}

// State handlers

void StateMachine::onIdle(const Event& e) {
    if (e.type == EventType::START_BUTTON) {
        startRun();
        startNextBrick();
    }
}

void StateMachine::onFeed(const Event& e) {
    if (e.type == EventType::ENTRY_BEAM_TRIGGERED) {
        _brick.entryMs = e.timestamp_ms;
        gLogger.info("entry beam triggered");
        _timeoutMs = e.timestamp_ms + APPROACH_TIMEOUT_MS;
        transition(S_APPROACH);
    }
    else if (millis() > _timeoutMs) {
        haltOnError(ERR_JAM_CHUTE);
    }
}

void StateMachine::onApproach(const Event& e) {
    if (e.type == EventType::STOP_SW_TRIGGERED) {
        _brick.seatedMs = e.timestamp_ms;
        gLogger.info("stop switch triggered");
        actuators::beltStop();
        _token = false;
        _timeoutMs = e.timestamp_ms + SETTLE_MS;
        transition(S_SEATED);
    }
    else if (millis() > _timeoutMs) {
        haltOnError(ERR_JAM_APPROACH);
    }
}

void StateMachine::onSeated(const Event& e) {
    if (millis() >= _timeoutMs) {
        transition(S_SENSING);
        actuators::startSensing();
    }
}

void StateMachine::onSensing(const Event& e) {
    if (e.type == EventType::SENSING_DONE) {
        _brick.sense = e.senseResult;

        if (_brick.sense.category == BrickCategory::UNCERTAIN) {
            haltOnError(ERR_SENSOR_FAULT);
            return;
        }

        _brick.targetBin = binFor(_brick.sense.category);
        _brick.binSteps = stepsForBin(_brick.targetBin);

        gLogger.classified(_brick.number, _brick.sense.category,
                           0, _brick.targetBin);

        transition(S_INDEXED);
        actuators::indexToBin(_brick.binSteps);
    }
}

void StateMachine::onIndexed(const Event& e) {
    if (e.type == EventType::INDEXED) {
        if (e.value != 0) {
            haltOnError(ERR_DISC_JAM);
            return;
        }
        gLogger.info("disc indexed");
        transition(S_RELEASED);
        actuators::releasePlatform();
        _brick.releaseMs = e.timestamp_ms;
        _timeoutMs = e.timestamp_ms + FALL_SETTLE_MS;
    }
}

void StateMachine::onReleased(const Event& e) {
    if (millis() >= _timeoutMs) {
        _timeoutMs = millis() + BIN_CONFIRM_TIMEOUT_MS;
        transition(S_CONFIRM);
    }
}

void StateMachine::onConfirm(const Event& e) {
    uint8_t arrived = binFromEvent(e);
    
    if (arrived == _brick.targetBin) {
        _brick.confirmMs = e.timestamp_ms;
        _brick.confirmed = true;
        uint32_t transit = _brick.confirmMs - _brick.releaseMs;
        _binCounts[_brick.targetBin - 1]++;
        gLogger.binConfirm(_brick.number, _brick.targetBin, arrived, transit, true);
        transition(S_RESET);
    }
    else if (arrived != 0 && arrived != _brick.targetBin) {
        gLogger.binConfirm(_brick.number, _brick.targetBin, arrived, 0, false);
        haltOnError(ERR_MISS_BIN);
    }
    else if (millis() > _timeoutMs) {
        haltOnError(ERR_MISS_BIN);
    }
}

void StateMachine::onReset(const Event& e) {
    if (e.type == EventType::PLATFORM_RELEASED) {
        _brickCount++;
        _token = true;
        gLogger.thermal();

        if (_brickCount >= TOTAL_BRICKS) {
            endRun();
            transition(S_COMPLETE);
        } else {
            startNextBrick();
        }
    }
    else if (millis() > _timeoutMs) {
        haltOnError(ERR_PLATFORM_STUCK);
    }
}

void StateMachine::onComplete(const Event& e) {
    if (e.type == EventType::START_BUTTON) {
        _brickCount = 0;
        memset(_binCounts, 0, sizeof(_binCounts));
        transition(S_IDLE);
        actuators::displayReady();
    }
}

void StateMachine::onErrorHalt(const Event& e) {
    if (e.type == EventType::RESET) {
        gLogger.info("RESET - returning to IDLE");
        _brickCount = 0;
        memset(_binCounts, 0, sizeof(_binCounts));
        _token = true;
        transition(S_IDLE);
        actuators::displayReady();
    }
}

// Internal helpers

void StateMachine::startRun() {
    _brickCount = 0;
    memset(_binCounts, 0, sizeof(_binCounts));
    _runStartMs = millis();
    _token = true;
    gLogger.info("V6 run started");
}

void StateMachine::endRun() {
    uint32_t totalMs = millis() - _runStartMs;
    actuators::beltStop();
    actuators::homeDisc();
    actuators::displayComplete(totalMs, _binCounts);
    gLogger.runComplete(totalMs, _binCounts, 0, 0);
}

void StateMachine::startNextBrick() {
    if (!_token) {
        gLogger.info("ERROR: token=0, cannot feed next brick");
        haltOnError(ERR_DOUBLE_ENTRY);
        return;
    }

    _brick = BrickRecord{};
    _brick.number = _brickCount + 1;
    actuators::beltStart();
    _timeoutMs = millis() + FEED_TIMEOUT_MS;
    transition(S_FEED);
}

void StateMachine::haltOnError(ErrorCode code) {
    _errorCode = code;
    actuators::beltStop();
    actuators::stepperStop();
    actuators::releaseOff();
    actuators::displayError(_brick.number, (uint8_t)code);
    actuators::buzzerError();
    gLogger.errorHalt(_brick.number, _brick.targetBin, errorName(code));
    transition(S_ERROR_HALT);
}

void StateMachine::checkTimeout() {
    if (_timeoutMs > 0 && millis() > _timeoutMs) {
        Event timeout;
        timeout.type = EventType::TIMEOUT;
        timeout.timestamp_ms = millis();
        gEventQueue.push(timeout);
        _timeoutMs = 0;
    }
}

uint8_t StateMachine::binFromEvent(const Event& e) const {
    switch (e.type) {
        case EventType::BIN1_CONFIRM: return 1;
        case EventType::BIN2_CONFIRM: return 2;
        case EventType::BIN3_CONFIRM: return 3;
        case EventType::BIN4_CONFIRM: return 4;
        default:                      return 0;
    }
}

uint16_t StateMachine::stepsForBin(uint8_t bin) const {
    switch (bin) {
        case 1:  return BIN1_STEPS;
        case 2:  return BIN2_STEPS;
        case 3:  return BIN3_STEPS;
        case 4:  return BIN4_STEPS;
        default: return BIN4_STEPS;
    }
}

// V6 bin routing table
uint8_t StateMachine::binFor(BrickCategory cat) {
    switch (cat) {
        case BrickCategory::CAT_2x2_RED:  return 1;  // NW bin (315 deg)
        case BrickCategory::CAT_2x2_BLUE: return 2;  // NE bin (45 deg)
        case BrickCategory::CAT_2x3_BLUE: return 3;  // SE bin (135 deg)
        case BrickCategory::CAT_2x3_RED:  return 4;  // SW bin (225 deg, home, default)
        default:                           return 4;
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
        case S_IDLE:       return "IDLE";
        case S_FEED:       return "FEED";
        case S_APPROACH:   return "APPROACH";
        case S_SEATED:     return "SEATED";
        case S_SENSING:    return "SENSING";
        case S_INDEXED:    return "INDEXED";
        case S_RELEASED:   return "RELEASED";
        case S_CONFIRM:    return "CONFIRM";
        case S_RESET:      return "RESET";
        case S_COMPLETE:   return "COMPLETE";
        case S_ERROR_HALT: return "ERROR_HALT";
        default:           return "UNKNOWN";
    }
}

const char* StateMachine::errorName(ErrorCode code) const {
    switch (code) {
        case ERR_JAM_CHUTE:       return "JAM_CHUTE";
        case ERR_JAM_APPROACH:    return "JAM_APPROACH";
        case ERR_SENSOR_FAULT:    return "SENSOR_FAULT";
        case ERR_DISC_JAM:        return "DISC_JAM";
        case ERR_MISS_BIN:        return "MISS_BIN";
        case ERR_DOUBLE_ENTRY:    return "DOUBLE_ENTRY";
        case ERR_PLATFORM_STUCK:  return "PLATFORM_STUCK";
        case ERR_POSITION_DRIFT:  return "POSITION_DRIFT";
        default:                  return "UNKNOWN_ERROR";
    }
}
