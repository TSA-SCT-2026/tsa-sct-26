#pragma once
#include <Arduino.h>
#include <Wire.h>
#include "config.h"

// Size sensing uses two-pair break-beam timing in the shrouded station.
// Keep this interface stable while thresholds are calibrated on real hardware.

enum class BrickCategory : uint8_t {
    CAT_2x2_RED  = 0,
    CAT_2x2_BLUE = 1,
    CAT_2x3_BLUE = 2,
    CAT_2x3_RED  = 3,
    UNCERTAIN    = 4
};

struct SenseResult {
    bool isTwoByThree = false;
    bool sizeValid = false;
    uint16_t sizeSignal = 0;
    float redRatio = 0.0f;
    uint8_t sampleCount = 0;
    BrickCategory category = BrickCategory::UNCERTAIN;
};

namespace sensors {
    void begin();
    SenseResult senseBrickAtStation();
    void setSimulatedSenseResult(const SenseResult& result);
    void clearSimulatedSenseResult();
    void attachBinBeams();
    bool brickDetectorClear();
    float beltSpeedMms();
    const char* categoryName(BrickCategory cat);
}
