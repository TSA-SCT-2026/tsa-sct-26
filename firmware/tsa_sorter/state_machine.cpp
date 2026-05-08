#include "state_machine.h"
#include "actuators.h"
#include "config.h"
#include "logger.h"
#include "sensors.h"
#include "thermal.h"
#include <Arduino.h>
#include <string.h>

StateMachine gStateMachine;

void StateMachine::begin() {
    _state = S_IDLE;
    _token = true;
    _brickCount = 0;
    _issuedBrickCount = 0;
    _routeProtectedUntilMs = 0;
    _lastDetectMs = 0;
    _nextPipelineId = 1;
    gEventQueue.clearOverflow();
    sensors::resetPassCapture();
    memset(_binCounts, 0, sizeof(_binCounts));
    clearInFlight();
    _deadlineMs = 0;
    actuators::displayReady();
    gLogger.info("state machine ready for servo chute sorter");
    gLogger.info("state machine: waiting before auto-start");
    delay(AUTO_START_DELAY_MS);
    startRun();
    startNextBrick();
}

void StateMachine::poll() {
    uint32_t now = millis();
    if (gEventQueue.overflowed()) {
        gEventQueue.clearOverflow();
        haltOnError(ERR_EVENT_QUEUE_OVERFLOW);
        return;
    }

    confirmReadyInFlight(now);
    if (_state != S_COMPLETE &&
        _issuedBrickCount >= TOTAL_BRICKS &&
        _inFlightCount == 0 &&
        _brickCount >= TOTAL_BRICKS) {
        endRun();
        transition(S_COMPLETE);
        return;
    }

    if (_deadlineMs == 0) return;

    switch (_state) {
        case S_FEED:
            if (now > _deadlineMs) {
                _deadlineMs = now + FEED_TIMEOUT_MS;
                gLogger.info("feed: waiting for next brick");
            }
            break;
        case S_HANDOFF:
            if (now >= _deadlineMs) {
                _deadlineMs = 0;
                pushEvent(EventType::HANDOFF_DONE);
            }
            break;
        case S_CONFIRM:
            if (now >= _deadlineMs) {
                _deadlineMs = 0;
                pushEventBinConfirmed(_brick.targetBin);
            }
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

uint16_t StateMachine::binCount(uint8_t bin) const {
    if (bin < 1 || bin > 4) return 0;
    return _binCounts[bin - 1];
}

void StateMachine::onIdle(const Event& e) {
    if (e.type != EventType::START_BUTTON) return;
    startRun();
    startNextBrick();
}

void StateMachine::onFeed(const Event& e) {
    if (e.type != EventType::SIZE_ENTRY_DETECTED) return;
    if (_issuedBrickCount >= TOTAL_BRICKS) {
        gLogger.info("feed: extra brick detected after run limit");
        return;
    }

    _brick.detectedMs = e.timestamp_ms;
    if (_lastDetectMs > 0 && _brick.detectedMs > _lastDetectMs) {
        char buf[72];
        snprintf(buf, sizeof(buf), "feed: inter-arrival dt=%lums",
                 (unsigned long)(_brick.detectedMs - _lastDetectMs));
        gLogger.info(buf);
    }
    _lastDetectMs = _brick.detectedMs;
    actuators::stopSelectorJiggle();
    transition(S_SENSING);
    actuators::setConveyorSlow();
    pushEventSensingDone(sensors::senseBrickAtStation());
}

void StateMachine::onSensing(const Event& e) {
    if (e.type != EventType::SENSING_DONE) return;

    _brick.sense = e.senseResult;
    if (_brick.sense.category == BrickCategory::UNCERTAIN) {
        if (retryCurrentBrick()) return;

        _brick.targetBin = chooseBestGuessBin(_brick.sense);
        _brick.sense.category = categoryForBin(_brick.targetBin);
        _brick.sense.lowConfidence = true;
        _brick.sense.guessed = true;
        _brick.sense.retried = _brick.retryCount > 0;
        char buf[96];
        snprintf(buf, sizeof(buf), "sense: best guess brick=%u bin=%u retried=%u",
                 _brick.number, _brick.targetBin, _brick.retryCount);
        gLogger.info(buf);
    } else {
        _brick.targetBin = binFor(_brick.sense.category);
        _brick.sense.retried = _brick.retryCount > 0;
    }
    _brick.servoAngle = actuators::servoAngleForBin(_brick.targetBin);

    gLogger.classified(_brick.number, _brick.sense.category, _brick.targetBin, _brick.sense.sampleCount);
    if (gLogger.mode() == LogMode::HUMAN) gLogger.senseVerbose(_brick.number, _brick.sense);
    actuators::displaySorting(_brick.number, TOTAL_BRICKS, _brick.targetBin, _binCounts);
    transition(S_ROUTING);

    float speedMms = _brick.sense.beltSpeedAvgMms;
    if (speedMms <= 0.0f) speedMms = sensors::beltSpeedMms();
    if (speedMms <= 0.0f) speedMms = CONVEYOR_SENSING_FALLBACK_SPEED_MM_S;

    float lengthMm = _brick.sense.measuredLengthAvgMm;
    if (lengthMm <= 0.0f) {
        lengthMm = _brick.sense.isTwoByThree ? ROUTE_FALLBACK_2X3_LENGTH_MM
                                             : ROUTE_FALLBACK_2X2_LENGTH_MM;
    }

    const float distanceToServoMm = sensors::activeBeamSpacingMm() + ROUTE_DISTANCE_BEAM_B_TO_SERVO_MM;
    const uint32_t travelToServoMs = (uint32_t)((distanceToServoMm / speedMms) * 1000.0f);
    uint32_t estimatedCommitMs = _brick.detectedMs + travelToServoMs;
    uint32_t clearTimeMs = (uint32_t)((lengthMm / speedMms) * 1000.0f);
    uint32_t estimatedClearMs = estimatedCommitMs + clearTimeMs;
    const uint32_t earliestRouteMs = _routeProtectedUntilMs == 0
                                       ? 0
                                       : _routeProtectedUntilMs + ROUTE_SWITCH_MARGIN_MS;
    const uint32_t latestRouteMs = estimatedCommitMs > (SELECTOR_ROUTE_READY_MS + ROUTE_SWITCH_MARGIN_MS)
                                     ? estimatedCommitMs - (SELECTOR_ROUTE_READY_MS + ROUTE_SWITCH_MARGIN_MS)
                                     : 0;

    uint32_t now = millis();
    uint32_t effectiveLatestRouteMs = latestRouteMs;
    if (latestRouteMs == 0 || latestRouteMs < earliestRouteMs) {
        uint32_t routeAtMs = earliestRouteMs > now ? earliestRouteMs : now;
        uint32_t desiredCommitMs = routeAtMs + SELECTOR_ROUTE_READY_MS +
                                   ROUTE_SWITCH_MARGIN_MS +
                                   CONVEYOR_SPACING_RECOVERY_MARGIN_MS;
        if (desiredCommitMs <= now) desiredCommitMs = now + 1;

        float elapsedMs = _brick.detectedMs < now ? (float)(now - _brick.detectedMs) : 0.0f;
        float traveledMm = speedMms * (elapsedMs / 1000.0f);
        float remainingDistanceMm = distanceToServoMm - traveledMm;
        if (remainingDistanceMm < 0.0f) remainingDistanceMm = 0.0f;

        uint32_t availableMs = desiredCommitMs - now;
        float requiredSpeedMms = availableMs > 0
                                   ? remainingDistanceMm / ((float)availableMs / 1000.0f)
                                   : 0.0f;
        uint32_t baseSps = actuators::conveyorSlowSps();
        uint32_t requiredSps = baseSps;
        if (speedMms > 0.0f) {
            requiredSps = (uint32_t)((requiredSpeedMms / speedMms) * (float)baseSps);
        }
        if (requiredSps > baseSps) requiredSps = baseSps;

        if (requiredSps < CONVEYOR_MIN_SPACING_SPS || remainingDistanceMm <= 0.0f) {
            char buf[144];
            snprintf(buf, sizeof(buf),
                     "route: spacing unrecoverable brick=%u speed=%.1f req_sps=%lu remaining=%.1f earliest=%lu latest=%lu",
                     _brick.number, speedMms, (unsigned long)requiredSps,
                     remainingDistanceMm,
                     (unsigned long)earliestRouteMs,
                     (unsigned long)latestRouteMs);
            gLogger.info(buf);
            haltOnError(ERR_POSITION_DRIFT);
            return;
        }

        actuators::setConveyorStepRate(requiredSps, "spacing");
        estimatedCommitMs = desiredCommitMs;
        if (requiredSpeedMms > 0.0f) {
            clearTimeMs = (uint32_t)((lengthMm / requiredSpeedMms) * 1000.0f);
        }
        estimatedClearMs = estimatedCommitMs + clearTimeMs;
        effectiveLatestRouteMs = routeAtMs;

        char buf[160];
        snprintf(buf, sizeof(buf),
                 "route: spacing recovery brick=%u req_sps=%lu remaining=%.1f route_at=%lu commit_ms=%lu",
                 _brick.number,
                 (unsigned long)requiredSps,
                 remainingDistanceMm,
                 (unsigned long)routeAtMs,
                 (unsigned long)estimatedCommitMs);
        gLogger.info(buf);
    }
    if (now > effectiveLatestRouteMs) {
        char buf[112];
        snprintf(buf, sizeof(buf),
                 "route: late brick=%u now=%lu latest=%lu speed=%.1f len=%.1f",
                 _brick.number,
                 (unsigned long)now,
                 (unsigned long)effectiveLatestRouteMs,
                 speedMms,
                 lengthMm);
        gLogger.info(buf);
        haltOnError(ERR_POSITION_DRIFT);
        return;
    }

    uint32_t routeAtMs = effectiveLatestRouteMs;
    if (routeAtMs < earliestRouteMs) routeAtMs = earliestRouteMs;
    if (now < routeAtMs) {
        uint32_t waitMs = routeAtMs - now;
        char buf[72];
        snprintf(buf, sizeof(buf), "route: timed wait=%lums brick=%u",
                 (unsigned long)waitMs, _brick.number);
        gLogger.info(buf);
        delay(waitMs);
    }

    bool ok = actuators::routeServoToBin(_brick.targetBin);
    gThermal.onServoMove();
#if SELECTOR_JIGGLE_ENABLED
    for (uint8_t i = 0; i < SELECTOR_JIGGLE_PULSES; i++) {
        gThermal.onServoMove();
    }
#endif
    if (ok) actuators::setConveyorFast();
    pushEventRouteReady(_brick.targetBin, ok, _brick.servoAngle);
    _routeProtectedUntilMs = estimatedClearMs;
    pushInFlight(_brick, millis(), estimatedCommitMs, estimatedClearMs, speedMms);
    if (_state == S_ERROR_HALT) return;
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
    _token = true;
    _deadlineMs = 0;

    if (_issuedBrickCount >= TOTAL_BRICKS) {
        transition(S_FEED);
    } else {
        startNextBrick();
    }
}

void StateMachine::onHandoff(const Event& e) {
    if (e.type != EventType::HANDOFF_DONE) return;

    _brick.handoffMs = e.timestamp_ms;
    gLogger.handoffDone(_brick.number, _brick.targetBin);
    transition(S_CONFIRM);
    _deadlineMs = e.timestamp_ms + TIMED_CONFIRM_WINDOW_MS;
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
    _issuedBrickCount = 0;
    _routeProtectedUntilMs = 0;
    _lastDetectMs = 0;
    gEventQueue.clearOverflow();
    clearInFlight();
    memset(_binCounts, 0, sizeof(_binCounts));
    _token = true;
    transition(S_IDLE);
    actuators::displayReady();
}

void StateMachine::onErrorHalt(const Event& e) {
    if (e.type != EventType::RESET) return;
    _brickCount = 0;
    _issuedBrickCount = 0;
    _routeProtectedUntilMs = 0;
    _lastDetectMs = 0;
    gEventQueue.clearOverflow();
    clearInFlight();
    memset(_binCounts, 0, sizeof(_binCounts));
    _token = true;
    _deadlineMs = 0;
    transition(S_IDLE);
    actuators::displayReady();
}

void StateMachine::startRun() {
    _brickCount = 0;
    _issuedBrickCount = 0;
    _routeProtectedUntilMs = 0;
    _lastDetectMs = 0;
    _nextPipelineId = 1;
    gEventQueue.clearOverflow();
    sensors::resetPassCapture();
    memset(_binCounts, 0, sizeof(_binCounts));
    clearInFlight();
    _runStartMs = millis();
    _token = true;
    gLogger.info("run started");
}

void StateMachine::endRun() {
    uint32_t totalMs = millis() - _runStartMs;
    actuators::stopConveyorFeed();
    actuators::displayComplete(totalMs, _binCounts);
    gLogger.runComplete(totalMs, _binCounts, expectedCountsMatch());
}

void StateMachine::startNextBrick() {
    if (!_token) {
        haltOnError(ERR_DOUBLE_ENTRY);
        return;
    }
    if (_issuedBrickCount >= TOTAL_BRICKS) {
        return;
    }

    _token = false;
    _brick = BrickRecord{};
    _issuedBrickCount++;
    _brick.number = _issuedBrickCount;
    sensors::resetPassCapture();
    // Do NOT call readySelector here: it would yank the servo back to home
    // before the previous brick has physically dropped into its bin.
    actuators::startConveyorFeed();
    _deadlineMs = millis() + FEED_TIMEOUT_MS;
    transition(S_FEED);
}

void StateMachine::pushInFlight(const BrickRecord& brick,
                                uint32_t routeReadyMs,
                                uint32_t estimatedCommitMs,
                                uint32_t estimatedClearMs,
                                float speedMms) {
    uint32_t startMs = millis();
    while (_inFlightCount >= MAX_IN_FLIGHT) {
        confirmReadyInFlight(millis());
        if (_inFlightCount < MAX_IN_FLIGHT) break;
        if (millis() - startMs > HANDOFF_WINDOW_MS) break;
        delay(1);
    }
    if (_inFlightCount >= MAX_IN_FLIGHT) {
        haltOnError(ERR_POSITION_DRIFT);
        return;
    }

    PipelineRecord rec;
    rec.id = _nextPipelineId++;
    rec.brickNumber = brick.number;
    rec.targetBin = brick.targetBin;
    rec.servoAngle = brick.servoAngle;
    rec.routeReadyMs = routeReadyMs;
    rec.detectedMs = brick.detectedMs;
    rec.estimatedCommitMs = estimatedCommitMs;
    rec.estimatedClearMs = estimatedClearMs;
    rec.speedMms = speedMms;
    rec.lengthMm = brick.sense.measuredLengthAvgMm > 0.0f
                     ? brick.sense.measuredLengthAvgMm
                     : (brick.sense.isTwoByThree ? ROUTE_FALLBACK_2X3_LENGTH_MM
                                                 : ROUTE_FALLBACK_2X2_LENGTH_MM);
    _inFlight[_inFlightCount++] = rec;

    char buf[128];
    snprintf(buf, sizeof(buf),
             "pipeline: queued id=%u brick=%u bin=%u commit_ms=%lu clear_ms=%lu in_flight=%u",
             rec.id, rec.brickNumber, rec.targetBin,
             (unsigned long)rec.estimatedCommitMs,
             (unsigned long)rec.estimatedClearMs,
             _inFlightCount);
    gLogger.info(buf);
}

void StateMachine::confirmReadyInFlight(uint32_t nowMs) {
    uint8_t writeIdx = 0;
    for (uint8_t i = 0; i < _inFlightCount; i++) {
        PipelineRecord rec = _inFlight[i];
        uint32_t confirmAtMs = rec.estimatedClearMs + TIMED_CONFIRM_WINDOW_MS;
        if (nowMs < confirmAtMs) {
            _inFlight[writeIdx++] = rec;
            continue;
        }

        uint32_t transitMs = nowMs - rec.detectedMs;
        gLogger.handoffDone(rec.brickNumber, rec.targetBin);
        gLogger.binConfirm(rec.brickNumber, rec.targetBin, rec.targetBin, transitMs, true);
        _binCounts[rec.targetBin - 1]++;
        _brickCount++;
    }
    _inFlightCount = writeIdx;
}

void StateMachine::clearInFlight() {
    memset(_inFlight, 0, sizeof(_inFlight));
    _inFlightCount = 0;
}

bool StateMachine::retryCurrentBrick() {
    if (_brick.retryCount >= SENSING_RETRY_LIMIT) return false;

    _brick.retryCount++;
    char buf[80];
    snprintf(buf, sizeof(buf), "sense: retry brick=%u attempt=%u",
             _brick.number, _brick.retryCount);
    gLogger.info(buf);

    actuators::reverseConveyorForRetry();
    sensors::resetPassCapture();
    actuators::startConveyorFeed();
    _deadlineMs = millis() + FEED_TIMEOUT_MS;
    transition(S_FEED);
    return true;
}

uint8_t StateMachine::remainingForBin(uint8_t bin) const {
    uint8_t expected = 0;
    switch (bin) {
        case 1: expected = EXPECTED_BIN1; break;
        case 2: expected = EXPECTED_BIN2; break;
        case 3: expected = EXPECTED_BIN3; break;
        case 4: expected = EXPECTED_BIN4; break;
        default: return 0;
    }
    uint16_t actual = _binCounts[bin - 1];
    return actual >= expected ? 0 : (uint8_t)(expected - actual);
}

uint8_t StateMachine::chooseProbabilityBin(bool useSize, bool isTwoByThree,
                                           bool useColor, bool isRed) const {
    uint8_t bestBin = SELECTOR_DEFAULT_BIN;
    uint8_t bestRemaining = 0;

    for (uint8_t bin = 1; bin <= 4; bin++) {
        BrickCategory cat = categoryForBin(bin);
        bool binIsTwoByThree = (cat == BrickCategory::CAT_2x3_BLUE ||
                                cat == BrickCategory::CAT_2x3_RED);
        bool binIsRed = (cat == BrickCategory::CAT_2x2_RED ||
                         cat == BrickCategory::CAT_2x3_RED);

        if (useSize && binIsTwoByThree != isTwoByThree) continue;
        if (useColor && binIsRed != isRed) continue;

        uint8_t rem = remainingForBin(bin);
        if (rem > bestRemaining) {
            bestRemaining = rem;
            bestBin = bin;
        }
    }

    return bestBin;
}

uint8_t StateMachine::chooseBestGuessBin(const SenseResult& sense) const {
    float len = sense.measuredLengthAvgMm;
    bool sizeUsable = len >= SIZE_MIN_REASONABLE_LENGTH_MM &&
                      len <= SIZE_MAX_REASONABLE_LENGTH_MM;
    bool isTwoByThree = len >= sensors::activeSizeThresholdMm();

    float netSum = sense.redNetHz + sense.blueNetHz;
    bool colorUsable = sense.sampleCount > 0 &&
                       sense.rawRedHz > 0.0f &&
                       sense.rawBlueHz > 0.0f &&
                       netSum >= COLOR_BELT_MIN_NET_SUM_HZ &&
                       sense.redRatio > COLOR_MIN_REASONABLE_RATIO &&
                       sense.redRatio < COLOR_MAX_REASONABLE_RATIO;
    bool isRed = sense.redRatio >= sensors::activeColorThreshold();

    if (sizeUsable && colorUsable) {
        if (isTwoByThree) {
            return isRed ? 4 : 3;
        }
        return isRed ? 1 : 2;
    }

    return chooseProbabilityBin(sizeUsable, isTwoByThree, colorUsable, isRed);
}

void StateMachine::haltOnError(ErrorCode code) {
    _errorCode = code;
    _deadlineMs = 0;
    clearInFlight();
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
        case BrickCategory::CAT_2x2_BLUE: return 4;
        case BrickCategory::CAT_2x3_BLUE: return 3;
        case BrickCategory::CAT_2x3_RED:  return 2;
        default:                          return SELECTOR_DEFAULT_BIN;
    }
}

BrickCategory StateMachine::categoryForBin(uint8_t bin) {
    switch (bin) {
        case 1: return BrickCategory::CAT_2x2_RED;
        case 2: return BrickCategory::CAT_2x3_RED;
        case 3: return BrickCategory::CAT_2x3_BLUE;
        case 4: return BrickCategory::CAT_2x2_BLUE;
        default: return BrickCategory::UNCERTAIN;
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
        case ERR_EVENT_QUEUE_OVERFLOW: return "EVENT_QUEUE_OVERFLOW";
        default: return "UNKNOWN_ERROR";
    }
}
