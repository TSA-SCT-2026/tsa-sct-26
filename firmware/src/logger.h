#pragma once
#include <Arduino.h>
#include "sensors.h"
#include "thermal.h"

enum class LogMode { CSV, HUMAN };

class Logger {
public:
    void begin();
    void setMode(LogMode mode);
    LogMode mode() const { return _mode; }

    void stateChange(const char* from, const char* to);
    void classified(uint8_t brickNum, BrickCategory cat, uint8_t targetBin, uint8_t samples);
    void selectorReady(uint8_t brickNum, uint8_t targetBin, uint16_t steps, bool ok,
                       const char* positionLabel);
    void releaseFired(uint8_t brickNum, uint8_t targetBin);
    void dropWindowDone(uint8_t brickNum, uint8_t targetBin);
    void binConfirm(uint8_t brickNum, uint8_t expectedBin, uint8_t actualBin,
                    uint32_t transitMs, bool ok);
    void platformLevel(uint8_t brickNum);
    void thermal();
    void runComplete(uint32_t totalMs, const uint8_t counts[4], bool countsMatch);
    void errorHalt(uint8_t brickNum, uint8_t expectedBin, const char* reason);
    void info(const char* msg);

private:
    LogMode _mode = LogMode::CSV;

    void printCsvHeader();
    void printHumanPrefix(const char* label);
    void printCsvRow(const char* eventName,
                     uint8_t brickNum,
                     const char* stateName,
                     const char* category,
                     uint8_t targetBin,
                     uint8_t actualBin,
                     uint16_t selectorSteps,
                     uint8_t samples,
                     const char* okField,
                     const char* errorCode,
                     const char* message);
};

extern Logger gLogger;
