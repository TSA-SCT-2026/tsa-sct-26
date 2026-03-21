#pragma once
#include <Arduino.h>
#include "events.h"
#include "sensors.h"

// State machine for escapement-based sensing design (Addendum A).
//
// New flow:
//   IDLE -> SENSING -> RELEASING -> TRANSIT -> CONFIRM -> (next brick or COMPLETE)
//
// SENSING: cam at dwell, senseBrickInChute() called, blocks ~280ms.
// RELEASING: cam rotates, brick enters belt, pusher timer scheduled.
// TRANSIT: brick traveling belt. Timer fires pusher at computed delay.
// CONFIRM: waiting for expected bin beam within timeout.
//
// The belt carries bricks from chute exit to bins.
// Classification is COMPLETE before the brick touches the belt.

enum class State : uint8_t {
    IDLE,       // waiting for start, display READY
    SENSING,    // brick stationary on cam chord, senseBrickInChute() running
    RELEASING,  // cam rotating, brick entering belt, pusher timer armed
    TRANSIT,    // brick on belt traveling toward pusher/bin end
    CONFIRM,    // waiting for bin beam confirmation within timeout
    COMPLETE,   // all 24 bricks done, display results
    ERROR_HALT, // halted on jam/miss, waiting for RESET
};

// Record of one brick's journey.
struct BrickRecord {
    uint8_t       number     = 0;
    uint32_t      releaseMs  = 0;   // when cam released the brick
    uint32_t      confirmMs  = 0;   // when bin beam confirmed arrival
    SenseResult   sense;            // full sensing result from chute
    uint8_t       targetBin  = 0;   // 1-4
    uint8_t       pusherIdx  = 0;   // 0=no pusher (default path), 1-3=pusher
    bool          confirmed  = false;
};

class StateMachine {
public:
    void begin();
    void process(const Event& e);

    State   currentState() const { return _state; }
    uint8_t brickCount()   const { return _brickCount; }
    uint8_t binCount(uint8_t bin) const;

    const char* stateName() const;

private:
    State       _state      = State::IDLE;
    BrickRecord _brick;
    uint8_t     _brickCount = 0;
    uint8_t     _binCounts[4] = {0,0,0,0};
    uint8_t     _errorCount   = 0;
    uint32_t    _runStartMs   = 0;
    uint32_t    _confirmDeadlineMs = 0;
    uint32_t    _transitSum   = 0;

    void transition(State next);
    void startRun();
    void endRun();
    void startNextBrick();

    void onIdle(const Event& e);
    void onSensing(const Event& e);
    void onReleasing(const Event& e);
    void onTransit(const Event& e);
    void onConfirm(const Event& e);
    void onComplete(const Event& e);
    void onErrorHalt(const Event& e);

    void checkConfirmTimeout();
    uint8_t binFromEvent(const Event& e) const;
    void haltSystem();

    // Compute pusher index and target bin from sense result.
    // Pusher 0 = default path (no push).
    static uint8_t pusherFor(BrickCategory cat);
    static uint8_t binFor(BrickCategory cat);

    static const char* stateNameFor(State s);
};

extern StateMachine gStateMachine;
