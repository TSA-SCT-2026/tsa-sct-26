#pragma once
#include <Arduino.h>
#include "sensors.h"
#include "thermal.h"

// All serial output goes through the logger. Two modes:
//   HUMAN - aligned, labeled, easy to read in a terminal
//   CSV   - one header + one row per brick, paste directly into a spreadsheet
//
// Switch modes at runtime with: log human   /   log csv

enum class LogMode { HUMAN, CSV };

class Logger {
public:
    void begin();
    void setMode(LogMode mode);
    LogMode mode() const { return _mode; }

    // State machine events
    void stateChange(const char* from, const char* to);
    void classified(uint8_t brickNum, BrickCategory cat, uint8_t pusherIdx, uint8_t bin);
    void binConfirm(uint8_t brickNum, uint8_t expectedBin, uint8_t actualBin,
                    uint32_t transitMs, bool ok);
    void thermal();
    void runComplete(uint32_t totalMs, const uint8_t counts[4],
                     uint8_t errors, uint32_t avgTransitMs);
    void errorHalt(uint8_t brickNum, uint8_t expectedBin, const char* reason);
    void info(const char* msg);

private:
    LogMode  _mode = LogMode::HUMAN;

    void timestamp();  // prints "[  XXXXXX ms] " prefix
};

extern Logger gLogger;
