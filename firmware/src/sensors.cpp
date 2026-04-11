#include "sensors.h"
#include "config.h"
#include "logger.h"

namespace sensors {

namespace {

bool gHasSimulatedSense = false;
SenseResult gSimulatedSense;

void classifyFromPlaceholders(SenseResult& result) {
    // Real size-sensor integration is intentionally blocked until the
    // mechanical sensor choice is made and calibrated.
    result.sizeValid = false;
    result.category = BrickCategory::UNCERTAIN;
}

}  // namespace

void begin() {
    Wire.begin(PIN_SDA, PIN_SCL);
    Wire.setClock(I2C_FREQ_HZ);

    pinMode(PIN_BRICK_DETECT, INPUT);
    pinMode(PIN_SIZE_SENSOR_A, INPUT);
    pinMode(PIN_SIZE_SENSOR_B, INPUT);
    pinMode(PIN_BIN1_BEAM, INPUT_PULLUP);
    pinMode(PIN_BIN2_BEAM, INPUT_PULLUP);
    pinMode(PIN_BIN3_BEAM, INPUT_PULLUP);
    pinMode(PIN_BIN4_BEAM, INPUT_PULLUP);

    gLogger.info("sensors begin: size sensor open, color sensor stubbed");
}

SenseResult senseBrickAtStation() {
    if (gHasSimulatedSense) {
        gHasSimulatedSense = false;
        return gSimulatedSense;
    }

    SenseResult result;
    classifyFromPlaceholders(result);

    uint32_t startMs = millis();
    while ((millis() - startMs) < SENSING_TIMEOUT_MS && result.sampleCount < COLOR_SAMPLE_COUNT) {
        delay(24);
        result.sampleCount++;
    }

    if (result.sampleCount < COLOR_MIN_SAMPLES) {
        gLogger.info("sensing stub: insufficient samples");
        return result;
    }

    gLogger.info("sensing stub: size and color hardware not integrated, returning UNCERTAIN");
    return result;
}

void setSimulatedSenseResult(const SenseResult& result) {
    gSimulatedSense = result;
    gHasSimulatedSense = true;
}

void clearSimulatedSenseResult() {
    gHasSimulatedSense = false;
}

void attachBinBeams() {
    gLogger.info("bin beam ISR attach still stubbed");
}

bool brickDetectorClear() {
    return digitalRead(PIN_BRICK_DETECT) == HIGH;
}

float beltSpeedMms() {
    return 0.0f;
}

const char* categoryName(BrickCategory cat) {
    switch (cat) {
        case BrickCategory::CAT_2x2_RED:  return "2x2_RED";
        case BrickCategory::CAT_2x2_BLUE: return "2x2_BLUE";
        case BrickCategory::CAT_2x3_BLUE: return "2x3_BLUE";
        case BrickCategory::CAT_2x3_RED:  return "2x3_RED";
        default:                          return "UNCERTAIN";
    }
}

}  // namespace sensors
