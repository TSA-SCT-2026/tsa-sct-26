#pragma once
#include <Arduino.h>

namespace actuators {

    enum class ConveyorSpeedMode : uint8_t {
        BALANCED = 0,
        AGGRESSIVE = 1,
        ACCURACY = 2,
    };

    void begin();

    void startConveyorFeed();
    void stopConveyorFeed();
    void setConveyorSlow();
    void setConveyorFast();
    void setConveyorStepRate(uint32_t stepRateSps, const char* reason);
    void reverseConveyorForRetry();
    void setConveyorSpeedMode(ConveyorSpeedMode mode);
    ConveyorSpeedMode conveyorSpeedMode();
    const char* conveyorSpeedModeName(ConveyorSpeedMode mode);
    uint32_t conveyorFastSps();
    uint32_t conveyorSlowSps();
    void tuneConveyorProfile(uint8_t profile);

    bool homeSelector();
    bool routeServoToBin(uint8_t binIdx);
    void stopSelector();
    void unloadPosition();
    void readySelector();

    uint16_t servoAngleForBin(uint8_t binIdx);
    const char* selectorPositionLabel(uint8_t binIdx);

    void displayReady();
    void displaySorting(uint8_t brickNum, uint8_t total, uint8_t targetBin,
                        const uint8_t binCounts[4]);
    void displayComplete(uint32_t totalMs, const uint8_t binCounts[4]);
    void displayError(uint8_t brickNum, uint8_t expectedBin, const char* errorCode);
    void buzzerError();

}  // namespace actuators
