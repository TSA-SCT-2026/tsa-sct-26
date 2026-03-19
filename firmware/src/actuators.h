#pragma once
#include <Arduino.h>

// Hardware actuator control. All functions are stubs until hardware is connected.
// Each stub logs exactly what it would do so test runs produce realistic output.

namespace actuators {

    void begin();

    // Belt motor
    void beltStart();
    void beltStop();
    void beltSetDuty(uint8_t duty);

    // Solenoid plows. plow is 1-3.
    // Full PWM for SOL_FULL_MS, then drops to hold PWM, then auto-releases at SOL_DEENERGIZE_MS.
    void plowFire(uint8_t plow);
    void plowRelease(uint8_t plow);       // force immediate release (error halt)
    void plowReleaseAll();

    // Stepper escapement
    void stepperRelease();                // fire one full revolution (releases one brick)
    void stepperSetRpm(uint16_t rpm);
    void stepperHold();                   // reduced current hold between releases
    void stepperStop();

    // Display (stub - real implementation in display.cpp)
    void displayReady();
    void displaySorting(uint8_t brickNum, uint8_t total);
    void displayComplete(uint32_t totalMs, const uint8_t binCounts[4]);
    void displayError(uint8_t brickNum, uint8_t expectedBin);

}  // namespace actuators
