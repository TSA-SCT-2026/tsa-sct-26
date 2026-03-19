#include <Arduino.h>
#include "config.h"
#include "events.h"
#include "thermal.h"
#include "logger.h"
#include "actuators.h"
#include "sensors.h"
#include "state_machine.h"
#include "test_harness.h"

// ----------------------------------------------------------------
// Periodic thermal logging while idle
// ----------------------------------------------------------------
static uint32_t _lastThermalLogMs = 0;

void setup() {
    gLogger.begin();
    testHarness::begin();

    gThermal.update();  // initialize last-update timestamp

    actuators::begin();
    sensors::begin();

    gStateMachine.begin();
}

void loop() {
    // Update thermal model decay every iteration
    gThermal.update();

    // Process all queued events
    Event e;
    while (gEventQueue.pop(e)) {
        gStateMachine.process(e);
    }

    // Read and handle serial commands from test harness
    testHarness::poll();

    // Periodically log thermal state while idle (useful during between-run cooling)
    if (gStateMachine.currentState() == State::IDLE) {
        uint32_t now = millis();
        if (now - _lastThermalLogMs >= THERMAL_LOG_INTERVAL_MS) {
            _lastThermalLogMs = now;
            if (gThermal.maxHeat() > 0.01f) {
                gLogger.thermal();
            }
        }
    }
}
