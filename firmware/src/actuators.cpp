#include "actuators.h"
#include "logger.h"
#include "config.h"

// STUBS: All functions log what they would do. Replace with real hardware
// calls when electronics are connected. The state machine calls these
// identically whether stubs or real - no changes required above this layer.

namespace actuators {

void begin() {
    gLogger.info("STUB actuators::begin - pins not configured yet");
}

void beltStart() {
    gLogger.info("STUB belt START");
}

void beltStop() {
    gLogger.info("STUB belt STOP");
}

void beltSetDuty(uint8_t duty) {
    char buf[48];
    snprintf(buf, sizeof(buf), "STUB belt duty=%d", duty);
    gLogger.info(buf);
}

void armPusher(uint8_t pusherIdx, uint32_t fireAtMs) {
    char buf[64];
    snprintf(buf, sizeof(buf), "STUB armPusher idx=%d fireAt=%lums on_ms=%d",
             pusherIdx, fireAtMs, SOL_ON_MS);
    gLogger.info(buf);
    // Real: esp_timer_once callback fires solenoid pin HIGH at fireAtMs,
    //       then LOW after SOL_ON_MS. Spring returns rod.
}

void pusherReleaseAll() {
    gLogger.info("STUB all pushers FORCE DE-ENERGIZE");
}

void stepperSense() {
    gLogger.info("STUB stepper SENSE (hold cam at dwell, sensing begins)");
    // Real: hold current cam position, signal sensing module to start sampling
}

void stepperRelease() {
    gLogger.info("STUB stepper RELEASE one brick (one full cam revolution)");
    // Real: step STEPPER_STEPS_PER_REV steps at adjustedStepperSps()
}

void stepperStop() {
    gLogger.info("STUB stepper STOP");
}

void displayReady() {
    gLogger.info("STUB display READY screen");
}

void displaySorting(uint8_t brickNum, uint8_t total) {
    char buf[48];
    snprintf(buf, sizeof(buf), "STUB display SORTING brick=%d/%d", brickNum, total);
    gLogger.info(buf);
}

void displayComplete(uint32_t totalMs, const uint8_t binCounts[4]) {
    char buf[80];
    snprintf(buf, sizeof(buf),
             "STUB display COMPLETE time=%lums bins=[%d,%d,%d,%d]",
             totalMs, binCounts[0], binCounts[1], binCounts[2], binCounts[3]);
    gLogger.info(buf);
}

void displayError(uint8_t brickNum, uint8_t expectedBin) {
    char buf[56];
    snprintf(buf, sizeof(buf), "STUB display ERROR brick=%d expected_bin=%d",
             brickNum, expectedBin);
    gLogger.info(buf);
}

}  // namespace actuators
