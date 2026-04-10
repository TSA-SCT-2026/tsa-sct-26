#include "sensors.h"
#include "config.h"
#include "logger.h"

namespace sensors {

namespace {

constexpr uint16_t SIZE_TOF_INVALID_MM = 0xFFFF;
bool gHasSimulatedSense = false;
SenseResult gSimulatedSense;

uint16_t readTofClearanceMm(uint8_t addr) {
    // Real VL53L0X bring-up still needs XSHUT-driven address assignment.
    (void)addr;
    return SIZE_TOF_INVALID_MM;
}

bool clearanceIsValid(uint16_t mm) {
    return mm != SIZE_TOF_INVALID_MM &&
           mm >= SIZE_TOF_VALID_MIN_MM &&
           mm <= SIZE_TOF_VALID_MAX_MM;
}

void classifySize(SenseResult& result) {
    const bool leftValid = clearanceIsValid(result.leftClearanceMm);
    const bool rightValid = clearanceIsValid(result.rightClearanceMm);
    result.sizeValid = leftValid && rightValid;
    if (!result.sizeValid) return;

    result.leftLaneOccupied = result.leftClearanceMm <= SIZE_TOF_OCCUPIED_MAX_MM;
    result.rightLaneOccupied = result.rightClearanceMm <= SIZE_TOF_OCCUPIED_MAX_MM;
    result.widerClearanceMm = result.leftClearanceMm;
    if (result.rightClearanceMm > result.widerClearanceMm) {
        result.widerClearanceMm = result.rightClearanceMm;
    }
    result.isTwoByThree = result.leftLaneOccupied && result.rightLaneOccupied;
}

}  // namespace

void begin() {
    Wire.begin(PIN_SDA, PIN_SCL);
    Wire.setClock(I2C_FREQ_HZ);

    pinMode(PIN_SIZE_TOF_LEFT_XSHUT, OUTPUT);
    pinMode(PIN_SIZE_TOF_RIGHT_XSHUT, OUTPUT);
    digitalWrite(PIN_SIZE_TOF_LEFT_XSHUT, LOW);
    digitalWrite(PIN_SIZE_TOF_RIGHT_XSHUT, LOW);
    delay(5);
    digitalWrite(PIN_SIZE_TOF_LEFT_XSHUT, HIGH);
    digitalWrite(PIN_SIZE_TOF_RIGHT_XSHUT, HIGH);

    pinMode(PIN_ENTRY_BEAM, INPUT);
    pinMode(PIN_BIN1_BEAM, INPUT_PULLUP);
    pinMode(PIN_BIN2_BEAM, INPUT_PULLUP);
    pinMode(PIN_BIN3_BEAM, INPUT_PULLUP);
    pinMode(PIN_BIN4_BEAM, INPUT_PULLUP);

    gLogger.info("sensors begin: dual ToF sizing reserved, color sensor still stubbed");
}

SenseResult senseBrickInChamber() {
    if (gHasSimulatedSense) {
        gHasSimulatedSense = false;
        return gSimulatedSense;
    }

    SenseResult result;

    result.leftClearanceMm = readTofClearanceMm(SIZE_TOF_LEFT_ADDR);
    result.rightClearanceMm = readTofClearanceMm(SIZE_TOF_RIGHT_ADDR);
    classifySize(result);

    if (!result.sizeValid) {
        gLogger.info("sensing stub: ToF bring-up pending, size readings invalid");
    }

    uint32_t senseBudgetMs = max((uint32_t)SIZE_TOF_TIMEOUT_MS, (uint32_t)COLOR_TIMEOUT_MS);
    uint32_t startMs = millis();
    while ((millis() - startMs) < senseBudgetMs && result.sampleCount < COLOR_SAMPLE_COUNT) {
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
    gLogger.info("sensing stub: ToF bring-up and color path not integrated, returning UNCERTAIN");
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
