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

void plowFire(uint8_t plow) {
    gLogger.plowExtend(plow);
    // Real: ledcWrite(plow pin, SOL_FULL_PWM)
    // Timer to drop to hold at SOL_FULL_MS, de-energize at SOL_DEENERGIZE_MS
    gLogger.plowHold(plow);
    gLogger.plowRelease(plow);
}

void plowRelease(uint8_t plow) {
    char buf[32];
    snprintf(buf, sizeof(buf), "STUB plow %d FORCE RELEASE", plow);
    gLogger.info(buf);
}

void plowReleaseAll() {
    gLogger.info("STUB all plows FORCE RELEASE");
}

void stepperRelease() {
    gLogger.info("STUB stepper RELEASE one brick");
    // Real: step STEPPER_STEPS_PER_REV steps at adjustedStepperRpm()
}

void stepperSetRpm(uint16_t rpm) {
    char buf[40];
    snprintf(buf, sizeof(buf), "STUB stepper rpm=%d", rpm);
    gLogger.info(buf);
}

void stepperHold() {
    gLogger.info("STUB stepper HOLD (reduced current)");
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
    char buf[48];
    snprintf(buf, sizeof(buf), "STUB display ERROR brick=%d expected_bin=%d",
             brickNum, expectedBin);
    gLogger.info(buf);
}

}  // namespace actuators
