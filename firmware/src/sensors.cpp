#include "sensors.h"
#include "events.h"
#include "logger.h"
#include "config.h"

// V6 static chamber sensing.
// The brick is stationary in isolated chamber when sensed.
// Size: two beams at X=5mm and X=21mm. Both blocked = 2x3, one blocked = 2x2.
// Color: blocking sample loop bounded by COLOR_TIMEOUT_MS.
// Classification is complete BEFORE the disc is indexed.
// The disc and belt are stopped during sensing.

namespace sensors {

void begin() {
    Wire.begin(PIN_SDA, PIN_SCL);
    Wire.setClock(I2C_FREQ_HZ);

    pinMode(PIN_SIZE_BEAM1, INPUT);      // external 10k pull-up to 3.3V required
    pinMode(PIN_SIZE_BEAM2, INPUT);      // external 10k pull-up to 3.3V required
    pinMode(PIN_ENTRY_BEAM, INPUT);      // external 10k pull-up to 3.3V required
    pinMode(PIN_BIN1_BEAM, INPUT_PULLUP);
    pinMode(PIN_BIN2_BEAM, INPUT_PULLUP);
    pinMode(PIN_BIN3_BEAM, INPUT_PULLUP);
    pinMode(PIN_BIN4_BEAM, INPUT_PULLUP);

    gLogger.info("sensors::begin - I2C configured, beam pins ready");
    // Real color sensor init depends on the final purchased module.
}

// Called once per brick, while brick is stationary in isolation chamber.
// Blocks until COLOR_TIMEOUT_MS or COLOR_SAMPLE_COUNT samples are reached.
// Returns a fully classified BrickCategory before disc indexing begins.
SenseResult senseBrickInChamber() {
    SenseResult result;
    result.sampleCount = 0;
    result.rRatio = 0.0f;
    result.category = BrickCategory::UNCERTAIN;

    // Size: two beams at X=5mm and X=21mm in chamber width dimension.
    // Both blocked = 2x3 (23.7mm width).
    // One blocked = 2x2 (15.8mm width).
    bool beam1 = (digitalRead(PIN_SIZE_BEAM1) == LOW);
    bool beam2 = (digitalRead(PIN_SIZE_BEAM2) == LOW);
    result.isLarge = (beam1 && beam2);

    gLogger.info(result.isLarge ? "SIZE: 2x3 (both beams blocked)" : "SIZE: 2x2 (one beam)");

    // Color: accumulate samples for COLOR_TIMEOUT_MS or until the target sample count is reached.
    // The brick is stationary in isolated chamber - no motion blur, no ambient light.
    // At 24ms integration: ~6 samples in 150ms.
    // At 2.4ms integration: ~62 samples in 150ms.
    uint32_t rSum = 0, gSum = 0, bSum = 0;
    uint32_t start = millis();

    while ((millis() - start < COLOR_TIMEOUT_MS) && (result.sampleCount < COLOR_SAMPLE_COUNT)) {
        // Real color sensor read depends on the final purchased module.
        // Replace this stub after the TCS3200 GY-31 wiring and calibration plan is locked.

        // Stub: yields so other tasks can run during dwell
        delay(24);
        result.sampleCount++;   // remove when real reads are implemented
    }

    if (result.sampleCount < COLOR_MIN_SAMPLES) {
        gLogger.info("COLOR: insufficient samples -> UNCERTAIN");
        return result;
    }

    // Classify color from averaged ratio
    if (rSum + gSum + bSum > 0) {
        result.rRatio = (float)rSum / (float)(rSum + gSum + bSum);
    }

    bool isRed;
    if (result.rRatio > COLOR_RED_THRESHOLD) {
        isRed = true;
    } else {
        isRed = false;
    }

    // Assign category
    if (!result.isLarge && isRed)  result.category = BrickCategory::CAT_2x2_RED;
    if (!result.isLarge && !isRed) result.category = BrickCategory::CAT_2x2_BLUE;
    if (result.isLarge  && !isRed) result.category = BrickCategory::CAT_2x3_BLUE;
    if (result.isLarge  && isRed)  result.category = BrickCategory::CAT_2x3_RED;

    char buf[64];
    snprintf(buf, sizeof(buf), "CLASSIFIED: %s ratio=%.3f samples=%d",
             categoryName(result.category), result.rRatio, result.sampleCount);
    gLogger.info(buf);

    return result;
}

// Attach bin confirmation beam ISRs.
// Called after begin(). ISRs push BINx_CONFIRM events into gEventQueue.
void attachBinBeams() {
    gLogger.info("STUB bin beams - attach ISRs here when hardware ready");
    // Real:
    //   attachInterrupt(PIN_BIN1_BEAM, onBin1, FALLING);
    //   attachInterrupt(PIN_BIN2_BEAM, onBin2, FALLING);
    //   attachInterrupt(PIN_BIN3_BEAM, onBin3, FALLING);
    //   attachInterrupt(PIN_BIN4_BEAM, onBin4, FALLING);
}

// Chamber exit beam: confirms brick was released by platform.
bool chamberExitBeamClear() {
    return digitalRead(PIN_ENTRY_BEAM) == HIGH;
}

// Belt speed from Hall sensor on idler roller (optional diagnostic only).
float beltSpeedMms() {
    // Real: count Hall pulses per second, convert via roller circumference.
    return 0.0f;
}

const char* categoryName(BrickCategory cat) {
    switch (cat) {
        case BrickCategory::CAT_2x2_RED:  return "2x2_RED";
        case BrickCategory::CAT_2x2_BLUE: return "2x2_BLUE";
        case BrickCategory::CAT_2x3_BLUE: return "2x3_BLUE";
        case BrickCategory::CAT_2x3_RED:  return "2x3_RED";
        default:                           return "UNCERTAIN";
    }
}

} // namespace sensors
