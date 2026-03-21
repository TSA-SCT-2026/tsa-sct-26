#include "events.h"

EventQueue gEventQueue;

bool EventQueue::push(const Event& e) {
    uint8_t next = (_tail + 1) % CAPACITY;
    if (next == _head) return false;  // queue full, drop event
    _buf[_tail] = e;
    _tail = next;
    return true;
}

bool EventQueue::pop(Event& out) {
    if (_head == _tail) return false;  // empty
    out = _buf[_head];
    _head = (_head + 1) % CAPACITY;
    return true;
}

bool EventQueue::isEmpty() const {
    return _head == _tail;
}

uint8_t EventQueue::size() const {
    return (_tail - _head + CAPACITY) % CAPACITY;
}

void pushEvent(EventType type) {
    Event e;
    e.type         = type;
    e.timestamp_ms = millis();
    e.gap_us       = 0;
    gEventQueue.push(e);
}

void pushEventGap(uint32_t gap_us) {
    Event e;
    e.type         = EventType::BEAM2_BREAK;
    e.timestamp_ms = millis();
    e.gap_us       = gap_us;
    gEventQueue.push(e);
}

void pushEventColor(uint16_t r, uint16_t g, uint16_t b, uint16_t c) {
    Event e;
    e.type         = EventType::COLOR_SAMPLE;
    e.timestamp_ms = millis();
    e.color        = {r, g, b, c};
    gEventQueue.push(e);
}
