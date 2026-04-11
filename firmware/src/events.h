#pragma once
#include <Arduino.h>
#include "sensors.h"

// Hardware inputs and the test harness both produce these events.
// The state machine treats them the same way so the control contract
// stays tied to physical truth instead of a special test path.

enum class EventType : uint8_t {
    NONE = 0,
    START_BUTTON,
    BRICK_DETECTED,
    SENSING_DONE,
    ROUTE_READY,
    HANDOFF_DONE,
    BIN_CONFIRMED,
    RESET,
    ENCODER_PULSE,
};

struct Event {
    EventType   type         = EventType::NONE;
    uint32_t    timestamp_ms = 0;
    SenseResult senseResult  = SenseResult{};
    uint8_t     binIdx       = 0;
    bool        ok           = true;
    uint16_t    servoAngle   = 0;
};

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

void pushEvent(EventType type);
void pushEventSensingDone(const SenseResult& result);
void pushEventRouteReady(uint8_t binIdx, bool ok, uint16_t servoAngle);
void pushEventBinConfirmed(uint8_t binIdx);
