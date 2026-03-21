#include "sensors.h"
#include "logger.h"
#include "config.h"

// STUBS: Functions log what they would set up. Replace with real ISR attachment
// and hardware polling when electronics are connected.
// The events these functions push into gEventQueue are identical in format
// to what the test harness pushes. The state machine sees no difference.

namespace sensors {

void begin() {
    gLogger.info("STUB sensors::begin - I2C and GPIO not configured");
    // Real:
    //   Wire.begin(PIN_COLOR_SDA, PIN_COLOR_SCL);
    //   Wire.setClock(I2C_FREQ_HZ);
    //   tcs.begin();
    //   tcs.setIntegrationTime(TCS34725_INTEGRATIONTIME_2_4MS);
}

void attachSizeBeams() {
    gLogger.info("STUB size beams - ISRs not attached");
    // Real:
    //   attachInterrupt(PIN_BEAM1, onBeam1Break, FALLING);
    //   attachInterrupt(PIN_BEAM2, onBeam2Break, FALLING);
    // ISR onBeam1Break: starts hardware timer, pushes BEAM1_BREAK
    // ISR onBeam2Break: reads timer, pushes BEAM2_BREAK with gap_us
    // Timer overflow callback: pushes SIZE_TIMEOUT
}

void attachBinBeams() {
    gLogger.info("STUB bin beams - ISRs not attached");
    // Real:
    //   attachInterrupt(PIN_BIN1, onBin1, FALLING);  // pushes BIN1_CONFIRM
    //   attachInterrupt(PIN_BIN2, onBin2, FALLING);
    //   attachInterrupt(PIN_BIN3, onBin3, FALLING);
    //   attachInterrupt(PIN_BIN4, onBin4, FALLING);
}

void attachButton() {
    gLogger.info("STUB button - ISR not attached");
    // Real:
    //   pinMode(PIN_BUTTON, INPUT_PULLUP);
    //   attachInterrupt(PIN_BUTTON, onButton, FALLING);
    // ISR: pushes START_BUTTON or RESET depending on current state
}

void attachEncoder() {
    gLogger.info("STUB encoder - pulse counter not configured");
    // Real: use ESP32 PCNT peripheral for high-frequency pulse counting
    //   pcnt_config_t cfg = {...};
    //   pcnt_unit_config(&cfg);
}

void startColorSampling() {
    gLogger.info("STUB color sampling START");
    // Real: start a hardware timer at COLOR_INTEGRATION_MS interval
    // Each tick: read TCS34725, push COLOR_SAMPLE event
    // After dwell window (determined by beam timing or fixed duration): push COLOR_DONE
}

void stopColorSampling() {
    gLogger.info("STUB color sampling STOP");
    // Real: stop the sampling timer, push COLOR_DONE if not already sent
}

float beltSpeedMms() {
    // Real: read PCNT count, convert pulse rate to mm/s
    // pulses_per_second / ENCODER_SLOTS_PER_REV * PULLEY_CIRCUMFERENCE_MM
    return 0.0f;  // 0 = no reading available (stub)
}

}  // namespace sensors
