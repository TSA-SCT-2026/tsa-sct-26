#include "state_machine.h"
#include "actuators.h"
#include "config.h"
#include "logger.h"
#include "sensors.h"
#include "thermal.h"
#include <string.h>

StateMachine gStateMachine;

void StateMachine::begin() {
    _state = S_IDLE;
    _token = true;
    actuators::displayReady();
    gLogger.info("state machine ready for states servo chute sorter");
}

void StateMachine::poll() {
    uint32_t now = millis();
    if (_deadlineMs == 0) return;

    switch (_state) {
        case S_FEED:
            if (now > _deadlineMs) haltOnError(ERR_FEED_TIMEOUT);
            break;
        case S_HANDOFF:
            if (now >= _deadlineMs) {
                _deadlineMs = 0;
                pushEvent(EventType::HANDOFF_DONE);
            }
            break;
        case S_CONFIRM:
            if (_deadlineMs > 0 && now > _deadlineMs) haltOnError(ERR_MISS_BIN);
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
        case S_SENSING:    onSensing(e); break;
        case S_ROUTING:    onRouting(e); break;
        case S_HANDOFF:    onHandoff(e); break;
        case S_CONFIRM:    onConfirm(e); break;
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
    if (e.type != EventType::BRICK_DETECTED) return;

    _brick.detectedMs = e.timestamp_ms;
    transition(S_SENSING);
    pushEventSensingDone(sensors::senseBrickAtStation());
}

void StateMachine::onSensing(const Event& e) {
    if (e.type != EventType::SENSING_DONE) return;

    _brick.sense = e.senseResult;
    if (_brick.sense.category == BrickCategory::UNCERTAIN) {
        haltOnError(ERR_SENSOR_FAULT);
        return;
    }

    _brick.targetBin = binFor(_brick.sense.category);
    _brick.servoAngle = actuators::servoAngleForBin(_brick.targetBin);

    gLogger.classified(_brick.number, _brick.sense.category, _brick.targetBin, _brick.sense.sampleCount);
    actuators::displaySorting(_brick.number, TOTAL_BRICKS, _brick.targetBin, _binCounts);
    transition(S_ROUTING);

    bool ok = actuators::routeServoToBin(_brick.targetBin);
    gThermal.onServoMove();
    pushEventRouteReady(_brick.targetBin, ok, _brick.servoAngle);
}

void StateMachine::onRouting(const Event& e) {
    if (e.type != EventType::ROUTE_READY) return;

    if (!e.ok) {
        haltOnError(ERR_ROUTE_FAIL);
        return;
    }

    _brick.routeReadyMs = e.timestamp_ms;
    gLogger.routeReady(_brick.number, _brick.targetBin, e.servoAngle, true,
                       actuators::selectorPositionLabel(_brick.targetBin));

    transition(S_HANDOFF);
    _deadlineMs = e.timestamp_ms + HANDOFF_WINDOW_MS;
}

void StateMachine::onHandoff(const Event& e) {
    if (e.type != EventType::HANDOFF_DONE) return;

    _brick.handoffMs = e.timestamp_ms;
    gLogger.handoffDone(_brick.number, _brick.targetBin);
    transition(S_CONFIRM);
    _deadlineMs = 0;
    pushEventBinConfirmed(_brick.targetBin);
}

void StateMachine::onConfirm(const Event& e) {
    if (e.type != EventType::BIN_CONFIRMED) return;

    _brick.actualBin = e.binIdx;
    _brick.confirmMs = e.timestamp_ms;
    uint32_t transitMs = _brick.confirmMs - _brick.handoffMs;

    if (_brick.actualBin != _brick.targetBin) {
        gLogger.binConfirm(_brick.number, _brick.targetBin, _brick.actualBin, transitMs, false);
        haltOnError(ERR_MISS_BIN);
        return;
    }

    _brick.confirmed = true;
    _binCounts[_brick.targetBin - 1]++;
    gLogger.binConfirm(_brick.number, _brick.targetBin, _brick.actualBin, transitMs, true);
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

    _token = false;
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
    actuators::displayError(_brick.number, _brick.targetBin, errorName(code));
    actuators::buzzerError();
    gLogger.errorHalt(_brick.number, _brick.targetBin, errorName(code));
    transition(S_ERROR_HALT);
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
        case BrickCategory::CAT_2x3_RED:  return 3;
        case BrickCategory::CAT_2x3_BLUE: return 4;
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
        case S_SENSING: return "SENSING";
        case S_ROUTING: return "ROUTING";
        case S_HANDOFF: return "HANDOFF";
        case S_CONFIRM: return "CONFIRM";
        case S_COMPLETE: return "COMPLETE";
        case S_ERROR_HALT: return "ERROR_HALT";
        default: return "UNKNOWN";
    }
}

const char* StateMachine::errorName(ErrorCode code) const {
    switch (code) {
        case ERR_FEED_TIMEOUT: return "FEED_TIMEOUT";
        case ERR_SENSOR_FAULT: return "SENSOR_FAULT";
        case ERR_ROUTE_FAIL: return "ROUTE_FAIL";
        case ERR_MISS_BIN: return "MISS_BIN";
        case ERR_DOUBLE_ENTRY: return "DOUBLE_ENTRY";
        case ERR_POSITION_DRIFT: return "POSITION_DRIFT";
        default: return "UNKNOWN_ERROR";
    }
}
