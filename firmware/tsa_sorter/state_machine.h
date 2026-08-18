#pragma once
#include <Arduino.h>
#include "events.h"
#include "sensors.h"

enum SystemState : uint8_t {
    S_IDLE,
    S_FEED,
    S_SENSING,
    S_ROUTING,
    S_HANDOFF,
    S_CONFIRM,
    S_COMPLETE,
    S_ERROR_HALT
};

enum ErrorCode : uint8_t {
    ERR_FEED_TIMEOUT,
    ERR_SENSOR_FAULT,
    ERR_ROUTE_FAIL,
    ERR_MISS_BIN,
    ERR_DOUBLE_ENTRY,
    ERR_POSITION_DRIFT,
    ERR_EVENT_QUEUE_OVERFLOW
};

struct BrickRecord {
    uint16_t     number          = 0;
    uint32_t     detectedMs      = 0;
    uint32_t     routeReadyMs    = 0;
    uint32_t     handoffMs       = 0;
    uint32_t     confirmMs       = 0;
    SenseResult  sense           = SenseResult{};
    uint8_t      targetBin       = 0;
    uint8_t      actualBin       = 0;
    uint16_t     servoAngle      = 0;
    uint8_t      retryCount      = 0;
    bool         confirmed       = false;
};

class StateMachine {
public:
    void begin();
    void poll();
    void process(const Event& e);

    SystemState currentState() const { return _state; }
    uint16_t brickCount() const { return _brickCount; }
    uint16_t issuedBrickCount() const { return _issuedBrickCount; }
    // Retained for serial-status compatibility. The sequential controller
    // never queues a second brick, so the legacy in_flight field is always 0.
    uint8_t inFlightCount() const { return 0; }
    uint16_t binCount(uint8_t bin) const;
    bool hasToken() const { return _token; }
    ErrorCode errorCode() const { return _errorCode; }

    const char* stateName() const;
    const char* errorName(ErrorCode code) const;

private:
    SystemState _state = S_IDLE;
    bool _token = true;
    BrickRecord _brick;
    uint16_t _brickCount = 0;
    uint16_t _issuedBrickCount = 0;
    uint16_t _binCounts[4] = {0, 0, 0, 0};
    uint32_t _runStartMs = 0;
    uint32_t _deadlineMs = 0;
    uint32_t _routeProtectedUntilMs = 0;
    uint32_t _lastDetectMs = 0;
    ErrorCode _errorCode = ERR_FEED_TIMEOUT;

    void transition(SystemState next);
    void startRun();
    void endRun();
    void startNextBrick();
    void haltOnError(ErrorCode code);
    bool retryCurrentBrick();
    uint8_t chooseBestGuessBin(const SenseResult& sense) const;
    uint8_t chooseProbabilityBin(bool useSize, bool isTwoByThree,
                                 bool useColor, bool isRed) const;
    uint8_t remainingForBin(uint8_t bin) const;

    void onIdle(const Event& e);
    void onFeed(const Event& e);
    void onSensing(const Event& e);
    void onRouting(const Event& e);
    void onHandoff(const Event& e);
    void onConfirm(const Event& e);
    void onComplete(const Event& e);
    void onErrorHalt(const Event& e);

    bool expectedCountsMatch() const;

    static uint8_t binFor(BrickCategory cat);
    static BrickCategory categoryForBin(uint8_t bin);
    static const char* stateNameFor(SystemState s);
};

extern StateMachine gStateMachine;
