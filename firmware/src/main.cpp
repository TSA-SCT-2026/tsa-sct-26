#include <Arduino.h>
#include "actuators.h"
#include "config.h"
#include "events.h"
#include "logger.h"
#include "sensors.h"
#include "state_machine.h"
#include "test_harness.h"
#include "thermal.h"

static uint32_t gLastThermalLogMs = 0;

void setup() {
    gLogger.begin();
    testHarness::begin();

    gThermal.update();
    actuators::begin();
    sensors::begin();
    gStateMachine.begin();
}

void loop() {
    gThermal.update();
    gStateMachine.poll();

    Event e;
    while (gEventQueue.pop(e)) {
        gStateMachine.process(e);
    }

    testHarness::poll();

    if (gStateMachine.currentState() == S_IDLE) {
        uint32_t now = millis();
        if (now - gLastThermalLogMs >= THERMAL_LOG_INTERVAL_MS) {
            gLastThermalLogMs = now;
            if (gThermal.maxHeat() > 0.01f) {
                gLogger.thermal();
            }
        }
    }
}
