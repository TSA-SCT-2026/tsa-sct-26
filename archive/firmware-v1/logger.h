#pragma once
#include <Arduino.h>
#include "classifier.h"
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
    void brickEnter(uint8_t brickNum);
    void sizeDetected(uint8_t brickNum, uint32_t gap_us, BrickSize size);
    void colorSample(uint8_t brickNum, uint16_t sampleIdx,
                     uint16_t r, uint16_t g, uint16_t b, uint16_t c,
                     float ratio, bool valid);
    void colorDone(uint8_t brickNum, uint16_t validSamples, uint16_t totalSamples,
                   float avgRatio, BrickColor color);
    void classified(uint8_t brickNum, BrickSize size, BrickColor color,
                    BrickType type, uint8_t plow, uint8_t bin);
    void plowExtend(uint8_t plow);
    void plowHold(uint8_t plow);
    void plowRelease(uint8_t plow);
    void binConfirm(uint8_t brickNum, uint8_t expectedBin, uint8_t actualBin,
                    uint32_t transitMs, bool ok);
    void thermal();
    void binCounts(const uint8_t counts[4], uint8_t total);
    void runComplete(uint32_t totalMs, const uint8_t counts[4],
                     uint8_t errors, uint32_t avgTransitMs);
    void errorHalt(uint8_t brickNum, uint8_t expectedBin, const char* reason);
    void info(const char* msg);

    // CSV: emit header row. Call once when entering CSV mode.
    void csvHeader();

    // CSV: emit one row per confirmed brick. Call on bin confirmation.
    void csvBrickRow(uint8_t brickNum, uint32_t tsEnterMs, uint32_t tsConfirmMs,
                     uint32_t gap_us, BrickSize size,
                     uint32_t sumR, uint32_t sumG, uint32_t sumB,
                     uint16_t validSamples, float avgRatio,
                     BrickColor color, BrickType type,
                     uint8_t plow, uint8_t bin, bool ok);

private:
    LogMode  _mode = LogMode::HUMAN;
    bool     _csvHeaderPrinted = false;

    void timestamp();  // prints "[  XXXXXX ms] " prefix
};

extern Logger gLogger;
