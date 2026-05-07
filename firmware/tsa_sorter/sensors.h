#pragma once
#include <Arduino.h>
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
    float rawRedHz = 0.0f;
    float rawBlueHz = 0.0f;
    float rawClearHz = 0.0f;
    float redNetHz = 0.0f;
    float blueNetHz = 0.0f;
    float beltSpeedLeadMms = 0.0f;
    float beltSpeedTrailMms = 0.0f;
    float beltSpeedAvgMms = 0.0f;
    float measuredLengthAmm = 0.0f;
    float measuredLengthBmm = 0.0f;
    float measuredLengthAvgMm = 0.0f;
    float speedSkewPct = 0.0f;
    float lengthSkewPct = 0.0f;
    uint8_t sampleCount = 0;
    bool lowConfidence = false;
    bool guessed = false;
    bool retried = false;
    BrickCategory category = BrickCategory::UNCERTAIN;
};

namespace sensors {
    void begin();
    void pollInputs();
    SenseResult senseBrickAtStation();
    void setSimulatedSenseResult(const SenseResult& result);
    void clearSimulatedSenseResult();
    void resetPassCapture();
    void attachBinBeams();
    float beltSpeedMms();
    float activeBeamSpacingMm();
    const char* categoryName(BrickCategory cat);

    float activeColorThreshold();
    float activeSizeThresholdMm();
    void captureBeltBaseline(uint8_t samples = 8);
    void setColorThreshold(float t);
    void setSizeThreshold(float mm);
    bool saveCalibration();
    void showCalibration();
}
