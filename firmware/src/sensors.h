#pragma once
#include <Arduino.h>
#include "events.h"

// Hardware sensor initialization and ISR attachment.
// All functions are stubs until hardware is connected.
// When real hardware exists, these functions attach the ISRs that push
// events into gEventQueue. The state machine is unchanged.

namespace sensors {

    void begin();

    // Attach ISR for size detection beams.
    // On beam 1 break: pushEvent(EventType::BEAM1_BREAK)
    // On beam 2 break: pushEventGap(gap_us since beam 1)
    // On SIZE_TIMEOUT_MS with no beam 2: pushEvent(EventType::SIZE_TIMEOUT)
    void attachSizeBeams();

    // Attach ISR for bin confirmation beams (1-4).
    void attachBinBeams();

    // Attach ISR for start button.
    void attachButton();

    // Attach encoder pulse counter (H206 slotted disk).
    void attachEncoder();

    // Begin color sensor I2C polling. Must be called after Wire.begin() at 400kHz.
    // Polls on a timer and pushes COLOR_SAMPLE events into gEventQueue.
    // Call stopColorSampling() to push COLOR_DONE and end the window.
    void startColorSampling();
    void stopColorSampling();

    // Read belt speed from encoder pulse count. Returns mm/s.
    float beltSpeedMms();

}  // namespace sensors
