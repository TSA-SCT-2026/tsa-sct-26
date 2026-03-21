#pragma once
#include <Arduino.h>

// All events that drive the state machine. Hardware ISRs and the serial
// test harness both produce events of these types. The state machine
// processes them identically regardless of source.
enum class EventType : uint8_t {
    NONE = 0,
    START_BUTTON,     // operator pressed start
    BEAM1_BREAK,      // size beam 1 broken: brick entered sensing zone
    BEAM2_BREAK,      // size beam 2 broken: gap_us contains elapsed time from beam 1
    SIZE_TIMEOUT,     // beam 2 never broke within SIZE_TIMEOUT_MS: brick is 2x2
    COLOR_SAMPLE,     // one TCS34725 reading: color field contains r, g, b, c
    COLOR_DONE,       // dwell window complete: state machine should classify now
    BIN1_CONFIRM,     // bin 1 confirmation beam broke
    BIN2_CONFIRM,     // bin 2 confirmation beam broke
    BIN3_CONFIRM,     // bin 3 confirmation beam broke
    BIN4_CONFIRM,     // bin 4 confirmation beam broke
    CONFIRM_TIMEOUT,  // no bin confirmed within CONFIRM_TIMEOUT_MS
    RESET,            // operator reset from ERROR_HALT
    ENCODER_PULSE,    // belt speed encoder tick (H206 slotted disk)
};

struct ColorReading {
    uint16_t r, g, b, c;
};

struct Event {
    EventType  type;
    uint32_t   timestamp_ms;
    union {
        uint32_t     gap_us;   // BEAM2_BREAK: microseconds since beam 1 broke
        ColorReading color;    // COLOR_SAMPLE: raw TCS34725 channels
    };

    Event() : type(EventType::NONE), timestamp_ms(0), gap_us(0) {}
};

// Ring buffer queue. ISRs and test harness push events here.
// State machine pops from here. Single consumer, multiple producers.
// NOTE: push() must be called with interrupts disabled when used from ISRs.
// Use portENTER_CRITICAL_ISR(&evtMux) when adding real ISR producers.
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

// Convenience: build and push an event in one call.
void pushEvent(EventType type);
void pushEventGap(uint32_t gap_us);          // BEAM2_BREAK
void pushEventColor(uint16_t r, uint16_t g, uint16_t b, uint16_t c); // COLOR_SAMPLE
