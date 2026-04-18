#include "actuators.h"
#include "config.h"
#include "logger.h"

namespace actuators {

static uint16_t current_servo_angle = SELECTOR_BIN4_ANGLE_DEG;

uint16_t servoAngleForBin(uint8_t binIdx) {
    switch (binIdx) {
        case 1: return SELECTOR_BIN1_ANGLE_DEG;
        case 2: return SELECTOR_BIN2_ANGLE_DEG;
        case 3: return SELECTOR_BIN3_ANGLE_DEG;
        case 4: return SELECTOR_BIN4_ANGLE_DEG;
        default: return SELECTOR_BIN4_ANGLE_DEG;
    }
}

const char* selectorPositionLabel(uint8_t binIdx) {
    switch (binIdx) {
        case 1: return "BIN1_2x2_RED";
        case 2: return "BIN2_2x2_BLUE";
        case 3: return "BIN3_2x3_RED";
        case 4: return "BIN4_2x3_BLUE";
        default: return "UNKNOWN";
    }
}

void begin() {
    gLogger.info("stub actuators begin: conveyor and servo not configured yet");
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

bool homeSelector() {
    current_servo_angle = servoAngleForBin(SELECTOR_HOME_BIN);
    gLogger.info("stub selector servo moved to home bin angle");
    return true;
}

bool routeServoToBin(uint8_t binIdx) {
    if (binIdx < 1 || binIdx > 4) {
        gLogger.info("stub servo route failed: invalid target bin");
        return false;
    }

    uint16_t targetAngle = servoAngleForBin(binIdx);
    char buf[96];
    snprintf(buf, sizeof(buf), "stub servo route bin=%u target_angle=%u current_angle=%u",
             binIdx, targetAngle, current_servo_angle);
    gLogger.info(buf);
    current_servo_angle = targetAngle;
    return true;
}

void stopSelector() {
    gLogger.info("stub selector servo stop");
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
