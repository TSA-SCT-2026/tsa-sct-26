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
    gEventQueue.push(e);
}

void pushEventSensingDone(const SenseResult& result) {
    Event e;
    e.type         = EventType::SENSING_DONE;
    e.timestamp_ms = millis();
    e.senseResult  = result;
    gEventQueue.push(e);
}

void pushEventPusherFired(uint8_t pusherIdx) {
    Event e;
    e.type         = EventType::PUSHER_FIRED;
    e.timestamp_ms = millis();
    e.pusherIdx    = pusherIdx;
    gEventQueue.push(e);
}
