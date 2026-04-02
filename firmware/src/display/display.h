#pragma once
#include <Arduino.h>

// Display management for Waveshare 1.69in ST7789V2 240x280 IPS display.
// All screens must be clear and readable from arm's length by an evaluator.

namespace display {

    void begin();
    
    // Main screen states
    void showReady();              // LOAD 24 BRICKS / PRESS START
    void showSorting(uint8_t brickNum, uint8_t total, 
                     const char* discPos, 
                     const uint8_t binCounts[4],
                     float driverTemp);
    void showComplete(uint32_t totalMs, const uint8_t binCounts[4]);
    void showError(uint8_t brickNum, uint8_t targetBin, const char* errorCode);

    // Helper: disc position label from bin number (1-4)
    const char* discPositionLabel(uint8_t bin);

}  // namespace display
