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

static void clampStartupPins() {
    // Force low-load safe outputs before any long delay or init.
    pinMode(PIN_CONVEYOR_ENABLE, OUTPUT);
    digitalWrite(PIN_CONVEYOR_ENABLE, HIGH);  // driver disabled (active-low)
    pinMode(PIN_CONVEYOR_STEP, OUTPUT);
    digitalWrite(PIN_CONVEYOR_STEP, LOW);
    pinMode(PIN_CONVEYOR_DIR, OUTPUT);
    digitalWrite(PIN_CONVEYOR_DIR, LOW);
    pinMode(PIN_SELECTOR_SERVO, INPUT_PULLDOWN);
    pinMode(PIN_BUZZER, OUTPUT);
    digitalWrite(PIN_BUZZER, LOW);
}

void setup() {
    clampStartupPins();
    delay(STARTUP_PIN_CLAMP_MS);
    gLogger.begin();
    gLogger.info("boot: settling power rails");
    delay(BOOT_POWER_SETTLE_MS);
    testHarness::begin();
    gThermal.update();
    actuators::begin();
    sensors::begin();
    gStateMachine.begin();
}

void loop() {
    gThermal.update();
    gStateMachine.poll();
    sensors::pollInputs();

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
