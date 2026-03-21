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

    // Solenoid pushers. pusherIdx is 1-3.
    // armPusher schedules the solenoid fire at fireAtMs using esp_timer_once.
    // pusherReleaseAll force-de-energizes all pushers immediately (error halt).
    void armPusher(uint8_t pusherIdx, uint32_t fireAtMs);
    void pusherReleaseAll();

    // Stepper escapement
    void stepperSense();                  // hold cam at dwell, begin sensing window
    void stepperRelease();                // fire one full cam revolution (releases one brick)
    void stepperStop();

    // Display (stub - real implementation in display.cpp)
    void displayReady();
    void displaySorting(uint8_t brickNum, uint8_t total);
    void displayComplete(uint32_t totalMs, const uint8_t binCounts[4]);
    void displayError(uint8_t brickNum, uint8_t expectedBin);

}  // namespace actuators
