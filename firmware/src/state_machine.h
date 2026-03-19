#pragma once
#include <Arduino.h>
#include "events.h"
#include "classifier.h"

enum class State : uint8_t {
    IDLE,         // waiting for start button
    FEEDING,      // stepper releasing one brick, waiting for beam 1
    SIZE_DETECT,  // waiting for beam 2 break or timeout
    COLOR_DETECT, // accumulating color samples, waiting for COLOR_DONE
    ROUTING,      // plow set, transitioning to CONFIRM (no wait)
    CONFIRM,      // waiting for expected bin beam within timeout
    ERROR_HALT,   // belt/stepper stopped, waiting for RESET
};

// Snapshot of one brick's journey through the pipeline.
// Populated incrementally as the brick moves through each state.
struct BrickRecord {
    uint8_t  number       = 0;
    uint32_t tsEnterMs    = 0;    // when beam 1 broke
    uint32_t tsConfirmMs  = 0;    // when bin beam broke

    uint32_t  gap_us      = 0;    // 0 = timeout (2x2)
    BrickSize size        = BrickSize::UNKNOWN;

    uint32_t colorSumR    = 0;
    uint32_t colorSumG    = 0;
    uint32_t colorSumB    = 0;
    uint16_t validSamples = 0;
    uint16_t totalSamples = 0;
    float    avgRatio     = 0.0f;
    BrickColor color      = BrickColor::UNKNOWN;

    BrickType type        = BrickType::UNKNOWN;
    uint8_t   plow        = 0;
    uint8_t   targetBin   = 0;
    bool      confirmed   = false;
};

class StateMachine {
public:
    void begin();

    // Process one event. Call from the main loop whenever gEventQueue has events.
    void process(const Event& e);

    State   currentState()  const { return _state; }
    uint8_t brickCount()    const { return _brickCount; }
    uint8_t binCount(uint8_t bin) const;  // bin 1-4

    const char* stateName() const;

private:
    State       _state      = State::IDLE;
    BrickRecord _brick;                     // current in-flight brick
    uint8_t     _brickCount = 0;
    uint8_t     _binCounts[4] = {0,0,0,0};
    uint8_t     _errorCount   = 0;
    uint32_t    _runStartMs   = 0;
    uint32_t    _confirmDeadlineMs = 0;
    uint32_t    _transitSum   = 0;          // sum of transit times for averaging

    void transition(State next);
    void startRun();
    void endRun();
    void resetBrick();
    void resetColorAccumulator();

    // Per-state event handlers
    void onIdle(const Event& e);
    void onFeeding(const Event& e);
    void onSizeDetect(const Event& e);
    void onColorDetect(const Event& e);
    void onRouting();
    void onConfirm(const Event& e);
    void onErrorHalt(const Event& e);

    // Returns true if event e is a bin confirmation for the expected bin.
    bool isExpectedBinConfirm(const Event& e) const;
    // Returns bin number if event is any bin confirm, else 0.
    uint8_t binFromEvent(const Event& e) const;

    // Poll for confirm timeout. Call every loop when in CONFIRM.
    void checkConfirmTimeout();

    void haltSystem();
    static const char* stateNameFor(State s);
};

extern StateMachine gStateMachine;
