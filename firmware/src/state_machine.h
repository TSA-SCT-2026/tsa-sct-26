#pragma once
#include <Arduino.h>
#include "events.h"
#include "sensors.h"

enum SystemState : uint8_t {
    S_IDLE,
    S_FEED,
    S_APPROACH,
    S_SEATED,
    S_SENSING,
    S_INDEXED,
    S_RELEASED,
    S_CONFIRM,
    S_RESET,
    S_COMPLETE,
    S_ERROR_HALT
};

enum ErrorCode : uint8_t {
    ERR_JAM_CHUTE,
    ERR_JAM_APPROACH,
    ERR_SENSOR_FAULT,
    ERR_SELECTOR_JAM,
    ERR_MISS_BIN,
    ERR_DOUBLE_ENTRY,
    ERR_PLATFORM_STUCK,
    ERR_POSITION_DRIFT
};

struct BrickRecord {
    uint8_t      number           = 0;
    uint32_t     entryMs          = 0;
    uint32_t     seatedMs         = 0;
    uint32_t     selectorReadyMs  = 0;
    uint32_t     releaseMs        = 0;
    uint32_t     confirmMs        = 0;
    uint32_t     platformLevelMs  = 0;
    SenseResult  sense            = SenseResult{};
    uint8_t      targetBin        = 0;
    uint8_t      actualBin        = 0;
    uint16_t     selectorSteps    = 0;
    bool         confirmed        = false;
};

class StateMachine {
public:
    void begin();
    void poll();
    void process(const Event& e);

    SystemState currentState() const { return _state; }
    uint8_t brickCount() const { return _brickCount; }
    uint8_t binCount(uint8_t bin) const;
    bool hasToken() const { return _token; }
    ErrorCode errorCode() const { return _errorCode; }

    const char* stateName() const;
    const char* errorName(ErrorCode code) const;

private:
    SystemState _state = S_IDLE;
    bool _token = true;
    BrickRecord _brick;
    uint8_t _brickCount = 0;
    uint8_t _binCounts[4] = {0, 0, 0, 0};
    uint32_t _runStartMs = 0;
    uint32_t _deadlineMs = 0;
    ErrorCode _errorCode = ERR_JAM_CHUTE;

    void transition(SystemState next);
    void startRun();
    void endRun();
    void startNextBrick();
    void haltOnError(ErrorCode code);

    void onIdle(const Event& e);
    void onFeed(const Event& e);
    void onApproach(const Event& e);
    void onSeated(const Event& e);
    void onSensing(const Event& e);
    void onIndexed(const Event& e);
    void onReleased(const Event& e);
    void onConfirm(const Event& e);
    void onReset(const Event& e);
    void onComplete(const Event& e);
    void onErrorHalt(const Event& e);

    uint16_t stepsForBin(uint8_t bin) const;
    bool expectedCountsMatch() const;

    static uint8_t binFor(BrickCategory cat);
    static const char* stateNameFor(SystemState s);
};

extern StateMachine gStateMachine;
