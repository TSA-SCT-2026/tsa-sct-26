#pragma once
#include <Arduino.h>
#include <Wire.h>
#include "config.h"

// Escapement-based sensing (Addendum A).
// Classification happens while brick is stationary on cam chord, before release.
// Belt speed has no effect on sensing quality.

enum class BrickCategory : uint8_t {
    CAT_2x2_RED  = 0,   // -> pusher 1 -> bin 1
    CAT_2x2_BLUE = 1,   // -> pusher 2 -> bin 2
    CAT_2x3_BLUE = 2,   // -> pusher 3 -> bin 3
    CAT_2x3_RED  = 3,   // -> default  -> bin 4
    UNCERTAIN    = 4    // error: stop and flag
};

struct SenseResult {
    bool isLarge;           // true = 2x3, false = 2x2
    float rRatio;           // R/(R+G+B) averaged over samples
    uint8_t sampleCount;    // number of valid color samples taken
    BrickCategory category; // final classification
};

namespace sensors {
    void begin();
    SenseResult senseBrickInChute();  // call once per cam cycle, blocks for COLOR_SENSE_MS
    void attachBinBeams();
    bool chuteExitBeamClear();
    float beltSpeedMms();             // optional, 0.0 if Hall sensor not installed
    const char* categoryName(BrickCategory cat);
}
