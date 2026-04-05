#include "display.h"
#include "../config.h"

namespace display {

void begin() {
}

const char* selectorPositionLabel(uint8_t bin) {
    switch (bin) {
        case 1: return "BIN1_NW";
        case 2: return "BIN2_NE";
        case 3: return "BIN3_SE";
        case 4: return "BIN4_SW";
        default: return "UNKNOWN";
    }
}

void showReady() {
}

void showSorting(uint8_t brickNum, uint8_t total,
                 const char* selectorPos,
                 const uint8_t binCounts[4],
                 float driverTemp) {
    (void)brickNum;
    (void)total;
    (void)selectorPos;
    (void)binCounts;
    (void)driverTemp;
}

void showComplete(uint32_t totalMs, const uint8_t binCounts[4]) {
    (void)totalMs;
    (void)binCounts;
}

void showError(uint8_t brickNum, uint8_t targetBin, const char* errorCode) {
    (void)brickNum;
    (void)targetBin;
    (void)errorCode;
}

}  // namespace display
