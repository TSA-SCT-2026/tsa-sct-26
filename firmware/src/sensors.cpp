#include "sensors.h"
#include "config.h"
#include "logger.h"

namespace sensors {

void begin() {
    Wire.begin(PIN_SDA, PIN_SCL);
    Wire.setClock(I2C_FREQ_HZ);

    pinMode(PIN_SIZE_BEAM1, INPUT);
    pinMode(PIN_SIZE_BEAM2, INPUT);
    pinMode(PIN_ENTRY_BEAM, INPUT);
    pinMode(PIN_BIN1_BEAM, INPUT_PULLUP);
    pinMode(PIN_BIN2_BEAM, INPUT_PULLUP);
    pinMode(PIN_BIN3_BEAM, INPUT_PULLUP);
    pinMode(PIN_BIN4_BEAM, INPUT_PULLUP);

    gLogger.info("sensors begin: beam inputs ready, color sensor still stubbed");
}

SenseResult senseBrickInChamber() {
    SenseResult result;

    bool beam1Blocked = (digitalRead(PIN_SIZE_BEAM1) == LOW);
    bool beam2Blocked = (digitalRead(PIN_SIZE_BEAM2) == LOW);
    result.isLarge = beam1Blocked && beam2Blocked;

    uint32_t startMs = millis();
    while ((millis() - startMs) < COLOR_TIMEOUT_MS && result.sampleCount < COLOR_SAMPLE_COUNT) {
        delay(24);
        result.sampleCount++;
    }

    // Until the purchased module is integrated, the firmware should not
    // pretend this path produces a valid color classification on hardware.
    if (result.sampleCount < COLOR_MIN_SAMPLES) {
        gLogger.info("sensing stub: insufficient samples");
        return result;
    }

    result.category = BrickCategory::UNCERTAIN;
    gLogger.info("sensing stub: color path not integrated, returning UNCERTAIN");
    return result;
}

void attachBinBeams() {
    gLogger.info("bin beam ISR attach still stubbed");
}

bool chamberExitBeamClear() {
    return digitalRead(PIN_ENTRY_BEAM) == HIGH;
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
