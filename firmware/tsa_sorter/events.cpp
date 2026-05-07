#include "events.h"

EventQueue gEventQueue;

bool EventQueue::push(const Event& e) {
    uint8_t next = (_tail + 1) % CAPACITY;
    if (next == _head) {
        _overflowed = true;
        return false;
    }
    _buf[_tail] = e;
    _tail = next;
    return true;
}

bool EventQueue::pop(Event& out) {
    if (_head == _tail) return false;
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

bool EventQueue::overflowed() const {
    return _overflowed;
}

void EventQueue::clearOverflow() {
    _overflowed = false;
}

void pushEvent(EventType type) {
    Event e;
    e.type = type;
    e.timestamp_ms = millis();
    gEventQueue.push(e);
}

void pushEventSensingDone(const SenseResult& result) {
    Event e;
    e.type = EventType::SENSING_DONE;
    e.timestamp_ms = millis();
    e.senseResult = result;
    gEventQueue.push(e);
}

void pushEventRouteReady(uint8_t binIdx, bool ok, uint16_t servoAngle) {
    Event e;
    e.type = EventType::ROUTE_READY;
    e.timestamp_ms = millis();
    e.binIdx = binIdx;
    e.ok = ok;
    e.servoAngle = servoAngle;
    gEventQueue.push(e);
}

void pushEventBinConfirmed(uint8_t binIdx) {
    Event e;
    e.type = EventType::BIN_CONFIRMED;
    e.timestamp_ms = millis();
    e.binIdx = binIdx;
    gEventQueue.push(e);
}
