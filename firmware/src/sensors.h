#pragma once
#include <Arduino.h>
#include <Wire.h>
#include "config.h"

// Classification happens only while the brick is static in the chamber.
// The selector chute can be any deterministic 4-index mechanism.
// Firmware only depends on target mapping and selector-ready truth.

enum class BrickCategory : uint8_t {
    CAT_2x2_RED  = 0,
    CAT_2x2_BLUE = 1,
    CAT_2x3_BLUE = 2,
    CAT_2x3_RED  = 3,
    UNCERTAIN    = 4
};

struct SenseResult {
    bool leftLaneOccupied = false;
    bool rightLaneOccupied = false;
    bool isTwoByThree = false;
    bool sizeValid = false;
    uint16_t leftClearanceMm = 0;
    uint16_t rightClearanceMm = 0;
    uint16_t widerClearanceMm = 0;
    float redRatio = 0.0f;
    uint8_t sampleCount = 0;
    BrickCategory category = BrickCategory::UNCERTAIN;
};

namespace sensors {
    void begin();
    SenseResult senseBrickInChamber();
    void setSimulatedSenseResult(const SenseResult& result);
    void clearSimulatedSenseResult();
    void attachBinBeams();
    bool chamberExitBeamClear();
    float beltSpeedMms();
    const char* categoryName(BrickCategory cat);
}
