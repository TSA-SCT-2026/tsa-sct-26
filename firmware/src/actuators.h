#pragma once
#include <Arduino.h>

namespace actuators {

    void begin();

    void startConveyorFeed();
    void stopConveyorFeed();
    void tuneConveyorProfile(uint8_t profile);

    void firePlatformRelease();
    void releaseOff();

    bool homeSelector();
    bool indexSelectorToBin(uint8_t binIdx);
    bool reHomeCheck();
    void stopSelector();

    const char* selectorPositionLabel(uint8_t binIdx);

    void displayReady();
    void displaySorting(uint8_t brickNum, uint8_t total, uint8_t targetBin,
                        const uint8_t binCounts[4]);
    void displayComplete(uint32_t totalMs, const uint8_t binCounts[4]);
    void displayError(uint8_t brickNum, uint8_t expectedBin, const char* errorCode);
    void buzzerError();

}  // namespace actuators
