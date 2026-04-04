#pragma once
#include <Arduino.h>
#include "sensors.h"

// All events that drive the state machine.
// Hardware ISRs and the test harness produce events of these types.
// The state machine processes them identically regardless of source.

enum class EventType : uint8_t {
    NONE = 0,
    START_BUTTON,     // operator pressed start
    SENSING_DONE,     // senseBrickInChamber() complete: senseResult contains result
    BRICK_SEATED,     // stop switch triggered, brick ready for sensing
    INDEXED,          // disc indexed to target bin position
    PLATFORM_RELEASED,// platform release solenoid fired
    BIN1_CONFIRM,     // bin 1 confirmation beam broke
    BIN2_CONFIRM,     // bin 2 confirmation beam broke
    BIN3_CONFIRM,     // bin 3 confirmation beam broke
    BIN4_CONFIRM,     // bin 4 confirmation beam broke
    CONFIRM_TIMEOUT,  // no bin confirmed within CONFIRM_TIMEOUT_MS
    RESET,            // operator reset from ERROR_HALT
    ENCODER_PULSE,    // optional diagnostic pulse from idler Hall sensor
};

struct Event {
    EventType   type;
    uint32_t    timestamp_ms;
    union {
        SenseResult senseResult;   // SENSING_DONE
        uint8_t     binIdx;        // INDEXED: which bin (1-4)
    };

    Event() : type(EventType::NONE), timestamp_ms(0) {
        senseResult = SenseResult{};
    }
};

// Ring buffer queue. ISRs and test harness push events here.
// State machine pops in the main loop. Single consumer, multiple producers.
class EventQueue {
public:
    static constexpr uint8_t CAPACITY = 64;

    bool push(const Event& e);
    bool pop(Event& out);
    bool isEmpty() const;
    uint8_t size() const;

private:
    volatile uint8_t _head = 0;
    volatile uint8_t _tail = 0;
    Event _buf[CAPACITY];
};

extern EventQueue gEventQueue;

// Convenience helpers for test harness
void pushEvent(EventType type);
void pushEventSensingDone(const SenseResult& result);
void pushEventIndexed(uint8_t binIdx);
