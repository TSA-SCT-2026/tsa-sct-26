#pragma once
#include <Arduino.h>

namespace display {

    void begin();

    void showReady();
    void showSorting(uint8_t brickNum, uint8_t total,
                     const char* selectorPos,
                     const uint8_t binCounts[4],
                     float driverTemp);
    void showComplete(uint32_t totalMs, const uint8_t binCounts[4]);
    void showError(uint8_t brickNum, uint8_t targetBin, const char* errorCode);

    const char* selectorPositionLabel(uint8_t bin);

}  // namespace display
