#pragma once
#include <Arduino.h>

// Hardware actuator control. All functions are stubs until hardware is connected.
// Each stub logs exactly what it would do so test runs produce realistic output.

namespace actuators {

    void begin();

    // Conveyor feed axis. Keep this abstraction stable even as the hardware moved to a stepper.
    void beltStart();
    void beltStop();
    void beltSetDuty(uint8_t duty);

    // Release solenoid (class 3 lever trapdoor)
    void firePlatformRelease();

    // Chute selector disc stepper
    bool homeDisc();                      // home disc to 225 deg (bin 4), return success
    bool indexToBin(uint8_t bin_index);   // move to target bin, monitor StallGuard
    bool reHomeCheck();                   // verify position every RETHOME_PERIOD_BRICKS

    // Display (stub - real implementation in display.cpp)
    void displayReady();
    void displaySorting(uint8_t brickNum, uint8_t total, uint8_t targetBin, 
                        const uint8_t binCounts[4]);
    void displayComplete(uint32_t totalMs, const uint8_t binCounts[4]);
    void displayError(uint8_t brickNum, uint8_t expectedBin, const char* errorCode);

}  // namespace actuators
