#include "display.h"
#include "../config.h"

// STUB implementations. Replace with ST7789V2 SPI calls when hardware connected.
// Layout for Waveshare 1.69in (240x280):
// - Large state text at top (24pt font)
// - Brick count (e.g., "BRICK 12/24")
// - Current disc position (e.g., "-> BIN 2 NE" or "DISC: NE (45°)")
// - Bin tallies (4 lines, updated live)
// - Thermal bar at bottom (driver temperature gauge)

namespace display {

void begin() {
    // TODO: Initialize SPI bus for ST7789V2
    // SPI.begin(PIN_TFT_CS, PIN_TFT_DC, PIN_TFT_RST)
    // Display resolution: 240x280
    // All stubs until hardware connected
}

const char* discPositionLabel(uint8_t bin) {
    switch (bin) {
        case 1: return "NW (315°)";  // bin 1: 2x2 red
        case 2: return "NE (45°)";   // bin 2: 2x2 blue
        case 3: return "SE (135°)";  // bin 3: 2x3 blue
        case 4: return "SW (225°)";  // bin 4: 2x3 red (default/home)
        default: return "UNKNOWN";
    }
}

void showReady() {
    // STUB: Real implementation should display:
    // - Large "READY" at top (green background)
    // - "LOAD 24 BRICKS" (centered, large font)
    // - "PRESS START" (centered below)
    // - System status: thermal OK, all bins empty
}

void showSorting(uint8_t brickNum, uint8_t total, 
                 const char* discPos, 
                 const uint8_t binCounts[4],
                 float driverTemp) {
    // STUB: Real implementation should display:
    // Line 1: "SORTING" (large, white on blue)
    // Line 2: "BRICK 12/24" (current progress)
    // Line 3: "-> BIN 2 NE" or "DISC: NE (45°)" (current target)
    // Line 4-7: Bin tallies:
    //   "BIN 1 (2x2 RED): 3"
    //   "BIN 2 (2x2 BLU): 2"
    //   "BIN 3 (2x3 BLU): 5"
    //   "BIN 4 (2x3 RED): 2"
    // Line 8: Thermal bar showing driver temperature
    //   Green = OK, Yellow = WARNING, Red = DANGER
}

void showComplete(uint32_t totalMs, const uint8_t binCounts[4]) {
    // STUB: Real implementation should display:
    // Line 1: "SORT COMPLETE" (large, green background)
    // Line 2: "TIME: XX.X sec"
    // Line 3-6: Final bin tallies with expected counts:
    //   "BIN 1: 6/6 OK" or "BIN 1: 5/6 FAIL" (green or red)
    //   "BIN 2: 6/6 OK"
    //   "BIN 3: 8/8 OK"
    //   "BIN 4: 4/4 OK"
    // Line 7: "ACCURACY: 24/24 (100%)"
    // Line 8: "PRESS RESET" (if applicable)
}

void showError(uint8_t brickNum, uint8_t targetBin, const char* errorCode) {
    // STUB: Real implementation should display:
    // Line 1: "ERROR" (large, red background)
    // Line 2: Error code name (e.g., "JAM_CHUTE", "MISS_BIN", "TIMEOUT")
    // Line 3: "BRICK: XX"
    // Line 4: "TARGET BIN: X"
    // Line 5: "SYSTEM HALTED"
    // Line 6: "CHECK CHAMBER"
    // Line 7: "PRESS RESET"
    
    // Known error codes:
    // - JAM_CHUTE: brick stuck in feed or chamber
    // - MISS_BIN: brick did not arrive at expected bin beam
    // - TIMEOUT: bin confirm timeout (no beam within window)
    // - UNCERTAIN_CLASSIFICATION: color sensor failed to classify
    // - THERMAL_DANGER: driver overheating
}

}  // namespace display
