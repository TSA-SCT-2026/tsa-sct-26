#include "actuators.h"
#include "config.h"
#include "logger.h"

namespace actuators {

static int16_t current_selector_steps = 0;

const char* selectorPositionLabel(uint8_t binIdx) {
    switch (binIdx) {
        case 1: return "BIN1_NW";
        case 2: return "BIN2_NE";
        case 3: return "BIN3_SE";
        case 4: return "BIN4_SW";
        default: return "UNKNOWN";
    }
}

void begin() {
    gLogger.info("stub actuators begin: pins and drivers not configured yet");
}

void startConveyorFeed() {
    gLogger.info("stub conveyor feed start");
}

void stopConveyorFeed() {
    gLogger.info("stub conveyor feed stop");
}

void tuneConveyorProfile(uint8_t profile) {
    char buf[64];
    snprintf(buf, sizeof(buf), "stub conveyor profile=%u", profile);
    gLogger.info(buf);
}

void firePlatformRelease() {
    char buf[64];
    snprintf(buf, sizeof(buf), "stub release pulse on_ms=%d", SOLENOID_ON_MS);
    gLogger.info(buf);
}

void releaseOff() {
    gLogger.info("stub release off");
}

bool homeSelector() {
    current_selector_steps = SELECTOR_BIN4_STEPS;
    gLogger.info("stub selector homed to reference position");
    return true;
}

bool indexSelectorToBin(uint8_t binIdx) {
    int16_t targetSteps = 0;
    switch (binIdx) {
        case 1: targetSteps = SELECTOR_BIN1_STEPS; break;
        case 2: targetSteps = SELECTOR_BIN2_STEPS; break;
        case 3: targetSteps = SELECTOR_BIN3_STEPS; break;
        case 4: targetSteps = SELECTOR_BIN4_STEPS; break;
        default:
            gLogger.info("stub selector index failed: invalid target bin");
            return false;
    }

    char buf[96];
    snprintf(buf, sizeof(buf), "stub selector index bin=%u target_steps=%d current_steps=%d",
             binIdx, targetSteps, current_selector_steps);
    gLogger.info(buf);
    current_selector_steps = targetSteps;
    return true;
}

bool reHomeCheck() {
    gLogger.info("stub selector re-home check passed");
    current_selector_steps = SELECTOR_BIN4_STEPS;
    return true;
}

void stopSelector() {
    gLogger.info("stub selector stop");
}

void displayReady() {
    gLogger.info("display READY");
}

void displaySorting(uint8_t brickNum, uint8_t total, uint8_t targetBin,
                    const uint8_t binCounts[4]) {
    char buf[128];
    snprintf(buf, sizeof(buf),
             "display SORTING brick=%u/%u selector=%s bins=[%u,%u,%u,%u]",
             brickNum, total, selectorPositionLabel(targetBin),
             binCounts[0], binCounts[1], binCounts[2], binCounts[3]);
    gLogger.info(buf);
}

void displayComplete(uint32_t totalMs, const uint8_t binCounts[4]) {
    char buf[96];
    snprintf(buf, sizeof(buf), "display COMPLETE time_ms=%lu bins=[%u,%u,%u,%u]",
             totalMs, binCounts[0], binCounts[1], binCounts[2], binCounts[3]);
    gLogger.info(buf);
}

void displayError(uint8_t brickNum, uint8_t expectedBin, const char* errorCode) {
    char buf[96];
    snprintf(buf, sizeof(buf), "display ERROR brick=%u target_bin=%u code=%s",
             brickNum, expectedBin, errorCode);
    gLogger.info(buf);
}

void buzzerError() {
    gLogger.info("stub buzzer error");
}

}  // namespace actuators
