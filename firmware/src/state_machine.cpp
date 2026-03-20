#include "state_machine.h"
#include "classifier.h"
#include "router.h"
#include "thermal.h"
#include "actuators.h"
#include "sensors.h"
#include "logger.h"
#include "config.h"

StateMachine gStateMachine;

// ----------------------------------------------------------------
// Public interface
// ----------------------------------------------------------------

void StateMachine::begin() {
    _state = State::IDLE;
    actuators::displayReady();
    gLogger.info("state machine ready - press start button or type 'sim start'");
}

void StateMachine::process(const Event& e) {
    if (e.type == EventType::NONE) return;

    // CONFIRM timeout must be checked on any event while in CONFIRM state.
    if (_state == State::CONFIRM) checkConfirmTimeout();

    switch (_state) {
        case State::IDLE:        onIdle(e);        break;
        case State::FEEDING:     onFeeding(e);     break;
        case State::SIZE_DETECT: onSizeDetect(e);  break;
        case State::COLOR_DETECT:onColorDetect(e); break;
        case State::ROUTING:     /* entered and exited synchronously, not event-driven */ break;
        case State::CONFIRM:     onConfirm(e);     break;
        case State::ERROR_HALT:  onErrorHalt(e);   break;
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
        transition(State::FEEDING);
        actuators::stepperRelease();
        gThermal.onStepperRelease();
    }
}

void StateMachine::onFeeding(const Event& e) {
    // Waiting for beam 1 to break (brick arrived at sensing zone).
    if (e.type == EventType::BEAM1_BREAK) {
        resetBrick();
        _brick.number    = _brickCount + 1;
        _brick.tsEnterMs = e.timestamp_ms;
        gLogger.brickEnter(_brick.number);
        // Start color sampling immediately on beam 1 break, not after size resolves.
        // The black belt filter discards any samples taken before the brick reaches
        // the color sensor. By the time size classification completes (~95-150ms),
        // samples are already banked and classification fires immediately.
        resetColorAccumulator();
        sensors::startColorSampling();
        transition(State::SIZE_DETECT);
    }
}

void StateMachine::onSizeDetect(const Event& e) {
    if (e.type == EventType::BEAM2_BREAK) {
        _brick.gap_us = e.gap_us;
        _brick.size   = classifySize(e.gap_us);
        gLogger.sizeDetected(_brick.number, _brick.gap_us, _brick.size);
        // Color sampling already running since BEAM1_BREAK. Do not restart.
        transition(State::COLOR_DETECT);

    } else if (e.type == EventType::SIZE_TIMEOUT) {
        _brick.gap_us = 0;
        _brick.size   = classifySize(0);
        gLogger.sizeDetected(_brick.number, 0, _brick.size);
        // Color sampling already running since BEAM1_BREAK. Do not restart.
        transition(State::COLOR_DETECT);
    }
}

void StateMachine::onColorDetect(const Event& e) {
    if (e.type == EventType::COLOR_SAMPLE) {
        uint16_t r = e.color.r, g = e.color.g, b = e.color.b, c = e.color.c;
        float ratio = colorRatio(r, g, b);
        bool valid  = isValidColorSample(r, g, b, c);

        gLogger.colorSample(_brick.number, _brick.totalSamples, r, g, b, c, ratio, valid);

        _brick.totalSamples++;
        if (valid) {
            _brick.colorSumR += r;
            _brick.colorSumG += g;
            _brick.colorSumB += b;
            _brick.validSamples++;
        }

    } else if (e.type == EventType::COLOR_DONE) {
        sensors::stopColorSampling();

        _brick.color = classifyColor(_brick.colorSumR, _brick.colorSumG,
                                      _brick.colorSumB, _brick.validSamples);

        // Compute average ratio for logging
        uint32_t total = _brick.colorSumR + _brick.colorSumG + _brick.colorSumB;
        _brick.avgRatio = (total > 0 && _brick.validSamples > 0)
                          ? (float)_brick.colorSumR / (float)total
                          : 0.0f;

        gLogger.colorDone(_brick.number, _brick.validSamples,
                           _brick.totalSamples, _brick.avgRatio, _brick.color);

        _brick.type = classifyBrick(_brick.size, _brick.color);
        _brick.plow      = routePlow(_brick.type);
        _brick.targetBin = targetBin(_brick.type);

        gLogger.classified(_brick.number, _brick.size, _brick.color,
                            _brick.type, _brick.plow, _brick.targetBin);

        transition(State::ROUTING);
        onRouting();  // routing is synchronous: fire plow and immediately move to CONFIRM
    }
}

void StateMachine::onRouting() {
    // Pre-set plow before brick arrives. At 200mm/s, brick is SOL_LEAD_MS away.
    if (_brick.plow > 0) {
        actuators::plowFire(_brick.plow);
        gThermal.onSolenoidFire(_brick.plow);
    }

    actuators::displaySorting(_brick.number, TOTAL_BRICKS);

    // Start confirm deadline
    _confirmDeadlineMs = millis() + CONFIRM_TIMEOUT_MS;
    transition(State::CONFIRM);
}

void StateMachine::onConfirm(const Event& e) {
    uint8_t binFromEvt = binFromEvent(e);

    if (binFromEvt == _brick.targetBin) {
        // Expected bin confirmed
        _brick.tsConfirmMs = e.timestamp_ms;
        _brick.confirmed   = true;
        uint32_t transit   = _brick.tsConfirmMs - _brick.tsEnterMs;

        _binCounts[_brick.targetBin - 1]++;
        _brickCount++;
        _transitSum += transit;

        gLogger.binConfirm(_brick.number, _brick.targetBin, binFromEvt, transit, true);
        gLogger.thermal();
        gLogger.binCounts(_binCounts, _brickCount);

        // CSV row: complete brick record
        if (gLogger.mode() == LogMode::CSV) {
            gLogger.csvBrickRow(_brick.number, _brick.tsEnterMs, _brick.tsConfirmMs,
                                 _brick.gap_us, _brick.size,
                                 _brick.colorSumR, _brick.colorSumG, _brick.colorSumB,
                                 _brick.validSamples, _brick.avgRatio,
                                 _brick.color, _brick.type,
                                 _brick.plow, _brick.targetBin, true);
        }

        if (_brickCount >= TOTAL_BRICKS) {
            endRun();
            transition(State::IDLE);
        } else {
            // Release next brick immediately
            actuators::stepperRelease();
            gThermal.onStepperRelease();
            transition(State::FEEDING);
        }

    } else if (binFromEvt != 0) {
        // Wrong bin confirmed - misroute or jam
        gLogger.binConfirm(_brick.number, _brick.targetBin, binFromEvt, 0, false);
        gLogger.errorHalt(_brick.number, _brick.targetBin, "WRONG_BIN");
        haltSystem();
        transition(State::ERROR_HALT);

    } else if (e.type == EventType::CONFIRM_TIMEOUT) {
        gLogger.errorHalt(_brick.number, _brick.targetBin, "TIMEOUT");
        haltSystem();
        transition(State::ERROR_HALT);
    }
}

void StateMachine::onErrorHalt(const Event& e) {
    if (e.type == EventType::RESET) {
        gLogger.info("RESET received - returning to IDLE");
        _brickCount = 0;
        memset(_binCounts, 0, sizeof(_binCounts));
        _errorCount   = 0;
        _transitSum   = 0;
        transition(State::IDLE);
        actuators::displayReady();
    }
}

// ----------------------------------------------------------------
// Internal helpers
// ----------------------------------------------------------------

void StateMachine::transition(State next) {
    gLogger.stateChange(stateName(), stateNameFor(next));
    _state = next;
}

void StateMachine::startRun() {
    _brickCount = 0;
    memset(_binCounts, 0, sizeof(_binCounts));
    _errorCount  = 0;
    _transitSum  = 0;
    _runStartMs  = millis();
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

void StateMachine::resetBrick() {
    _brick = BrickRecord{};
}

void StateMachine::resetColorAccumulator() {
    _brick.colorSumR    = 0;
    _brick.colorSumG    = 0;
    _brick.colorSumB    = 0;
    _brick.validSamples = 0;
    _brick.totalSamples = 0;
    _brick.avgRatio     = 0.0f;
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

bool StateMachine::isExpectedBinConfirm(const Event& e) const {
    return binFromEvent(e) == _brick.targetBin;
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
    actuators::plowReleaseAll();
    actuators::displayError(_brick.number, _brick.targetBin);
    _errorCount++;
}

const char* StateMachine::stateName() const {
    return stateNameFor(_state);
}

const char* StateMachine::stateNameFor(State s) {
    switch (s) {
        case State::IDLE:         return "IDLE";
        case State::FEEDING:      return "FEEDING";
        case State::SIZE_DETECT:  return "SIZE_DETECT";
        case State::COLOR_DETECT: return "COLOR_DETECT";
        case State::ROUTING:      return "ROUTING";
        case State::CONFIRM:      return "CONFIRM";
        case State::ERROR_HALT:   return "ERROR_HALT";
        default:                  return "UNKNOWN";
    }
}
