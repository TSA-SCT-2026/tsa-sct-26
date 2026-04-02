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

// ----------------------------------------------------------------
// State handlers
// ----------------------------------------------------------------

void StateMachine::onIdle(const Event& e) {
    if (e.type == EventType::START_BUTTON) {
        startRun();
        startNextBrick();
    }
}

void StateMachine::onSensing(const Event& e) {
    // SENSING_DONE is pushed by the escapement module after senseBrickInChute()
    // returns. The result is carried in the event payload.
    if (e.type == EventType::SENSING_DONE) {
        _brick.sense = e.senseResult;

        if (_brick.sense.category == BrickCategory::UNCERTAIN) {
            gLogger.errorHalt(_brick.number, 0, "UNCERTAIN_CLASSIFICATION");
            haltSystem();
            transition(State::ERROR_HALT);
            return;
        }

        _brick.pusherIdx = pusherFor(_brick.sense.category);
        _brick.targetBin = binFor(_brick.sense.category);

        gLogger.classified(_brick.number, _brick.sense.category,
                           _brick.pusherIdx, _brick.targetBin);

        // Release brick onto belt
        actuators::stepperRelease();
        gThermal.onStepperRelease();
        transition(State::RELEASING);
    }
}

void StateMachine::onReleasing(const Event& e) {
    // CHUTE_EXIT fires when the chute exit beam confirms brick left the chute.
    if (e.type == EventType::CHUTE_EXIT) {
        _brick.releaseMs = e.timestamp_ms;

        // Arm pusher timer. Delay = pusher position / belt speed.
        // Pusher 0 means default path, no push needed.
        if (_brick.pusherIdx > 0) {
            float positionMm = 0.0f;
            switch (_brick.pusherIdx) {
                case 1: positionMm = PUSHER1_POS_MM; break;
                case 2: positionMm = PUSHER2_POS_MM; break;
                case 3: positionMm = PUSHER3_POS_MM; break;
            }
            uint32_t delayMs = (uint32_t)(positionMm / BELT_TARGET_MM_S * 1000.0f);
            actuators::armPusher(_brick.pusherIdx, _brick.releaseMs + delayMs);
        }

        actuators::displaySorting(_brick.number, TOTAL_BRICKS);
        _confirmDeadlineMs = e.timestamp_ms + CONFIRM_TIMEOUT_MS;
        transition(State::TRANSIT);
    }
}

void StateMachine::onTransit(const Event& e) {
    // PUSHER_FIRED is pushed by the actuator timer when the solenoid fires.
    if (e.type == EventType::PUSHER_FIRED) {
        gThermal.onSolenoidFire(_brick.pusherIdx);
    }

    // Timeout: no bin confirmed before deadline.
    if (e.type == EventType::CONFIRM_TIMEOUT) {
        gLogger.errorHalt(_brick.number, _brick.targetBin, "TIMEOUT");
        haltSystem();
        transition(State::ERROR_HALT);
        return;
    }

    // Check bin confirmations even during transit (brick may arrive quickly).
    uint8_t arrived = binFromEvent(e);
    if (arrived == _brick.targetBin) {
        _brick.confirmMs = e.timestamp_ms;
        _brick.confirmed = true;
        uint32_t transit = _brick.confirmMs - _brick.releaseMs;
        _binCounts[_brick.targetBin - 1]++;
        _brickCount++;
        _transitSum += transit;
        gLogger.binConfirm(_brick.number, _brick.targetBin, arrived, transit, true);
        gLogger.thermal();

        if (_brickCount >= TOTAL_BRICKS) {
            endRun();
            transition(State::COMPLETE);
        } else {
            startNextBrick();
        }
        return;
    }

    if (arrived != 0 && arrived != _brick.targetBin) {
        gLogger.binConfirm(_brick.number, _brick.targetBin, arrived, 0, false);
        gLogger.errorHalt(_brick.number, _brick.targetBin, "WRONG_BIN");
        haltSystem();
        transition(State::ERROR_HALT);
    }
}

void StateMachine::onConfirm(const Event& e) {
    // Fallback confirm handling (same logic as onTransit bin check).
    uint8_t arrived = binFromEvent(e);
    if (arrived == _brick.targetBin) {
        _brick.confirmMs = e.timestamp_ms;
        _brick.confirmed = true;
        uint32_t transit = _brick.confirmMs - _brick.releaseMs;
        _binCounts[_brick.targetBin - 1]++;
        _brickCount++;
        _transitSum += transit;
        gLogger.binConfirm(_brick.number, _brick.targetBin, arrived, transit, true);
        gLogger.thermal();

        if (_brickCount >= TOTAL_BRICKS) {
            endRun();
            transition(State::COMPLETE);
        } else {
            startNextBrick();
        }
    } else if (e.type == EventType::CONFIRM_TIMEOUT) {
        gLogger.errorHalt(_brick.number, _brick.targetBin, "TIMEOUT");
        haltSystem();
        transition(State::ERROR_HALT);
    }
}

void StateMachine::onComplete(const Event& e) {
    // Wait for RESET to return to IDLE.
    if (e.type == EventType::RESET) {
        _brickCount = 0;
        memset(_binCounts, 0, sizeof(_binCounts));
        _errorCount  = 0;
        _transitSum  = 0;
        transition(State::IDLE);
        actuators::displayReady();
    }
}

void StateMachine::onErrorHalt(const Event& e) {
    if (e.type == EventType::RESET) {
        gLogger.info("RESET - returning to IDLE");
        _brickCount = 0;
        memset(_binCounts, 0, sizeof(_binCounts));
        _errorCount  = 0;
        _transitSum  = 0;
        transition(State::IDLE);
        actuators::displayReady();
    }
}

// ----------------------------------------------------------------
// Internal helpers
// ----------------------------------------------------------------

void StateMachine::startRun() {
    _brickCount = 0;
    memset(_binCounts, 0, sizeof(_binCounts));
    _errorCount = 0;
    _transitSum = 0;
    _runStartMs = millis();
    actuators::beltStart();
    gLogger.info("run started");
}

void StateMachine::endRun() {
    uint32_t totalMs    = millis() - _runStartMs;
    uint32_t avgTransit = (_brickCount > 0) ? _transitSum / _brickCount : 0;
    actuators::beltStop();
    actuators::displayComplete(totalMs, _binCounts);
    gLogger.runComplete(totalMs, _binCounts, _errorCount, avgTransit);
}

void StateMachine::startNextBrick() {
    _brick = BrickRecord{};
    _brick.number = _brickCount + 1;
    // Transition to SENSING. The escapement module will call senseBrickInChute()
    // and push SENSING_DONE when complete.
    transition(State::SENSING);
    actuators::stepperSense();  // tells escapement to hold cam and begin sensing
}

void StateMachine::checkConfirmTimeout() {
    if (_confirmDeadlineMs > 0 && millis() > _confirmDeadlineMs) {
        _confirmDeadlineMs = 0;
        Event timeout;
        timeout.type = EventType::CONFIRM_TIMEOUT;
        timeout.timestamp_ms = millis();
        gEventQueue.push(timeout);
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

void StateMachine::haltSystem() {
    actuators::beltStop();
    actuators::stepperStop();
    actuators::pusherReleaseAll();
    actuators::displayError(_brick.number, _brick.targetBin);
    _errorCount++;
}

// Bin and pusher routing table (Addendum A bin assignment)
uint8_t StateMachine::pusherFor(BrickCategory cat) {
    switch (cat) {
        case BrickCategory::CAT_2x2_RED:  return 1;
        case BrickCategory::CAT_2x2_BLUE: return 2;
        case BrickCategory::CAT_2x3_BLUE: return 3;
        case BrickCategory::CAT_2x3_RED:  return 0;  // default path, no pusher
        default:                           return 0;
    }
}

uint8_t StateMachine::binFor(BrickCategory cat) {
    switch (cat) {
        case BrickCategory::CAT_2x2_RED:  return 1;
        case BrickCategory::CAT_2x2_BLUE: return 2;
        case BrickCategory::CAT_2x3_BLUE: return 3;
        case BrickCategory::CAT_2x3_RED:  return 4;
        default:                           return 4;
    }
}

void StateMachine::transition(State next) {
    gLogger.stateChange(stateName(), stateNameFor(next));
    _state = next;
}

const char* StateMachine::stateName() const {
    return stateNameFor(_state);
}

const char* StateMachine::stateNameFor(State s) {
    switch (s) {
        case State::IDLE:       return "IDLE";
        case State::SENSING:    return "SENSING";
        case State::RELEASING:  return "RELEASING";
        case State::TRANSIT:    return "TRANSIT";
        case State::CONFIRM:    return "CONFIRM";
        case State::COMPLETE:   return "COMPLETE";
        case State::ERROR_HALT: return "ERROR_HALT";
        default:                return "UNKNOWN";
    }
}
