#include "logger.h"
#include "config.h"

Logger gLogger;

// Column widths for human mode alignment
static constexpr uint8_t TS_WIDTH  = 8;   // "12345678" ms
static constexpr uint8_t CAT_WIDTH = 18;  // category label

void Logger::begin() {
    Serial.begin(SERIAL_BAUD);
    while (!Serial) delay(10);
    Serial.println();
    Serial.println("==============================================");
    Serial.println("  TSA SCT 2026 - Brick Sorter Firmware");
    Serial.println("  Type 'help' for test harness commands");
    Serial.println("==============================================");
    Serial.println();
}

void Logger::setMode(LogMode mode) {
    _mode = mode;
    if (mode == LogMode::HUMAN) {
        Serial.println("[logger] switched to HUMAN mode");
    }
}

void Logger::timestamp() {
    Serial.printf("[%7lums] ", millis());
}

static void label(const char* s) {
    Serial.printf("%-18s", s);
}

void Logger::stateChange(const char* from, const char* to) {
    if (_mode != LogMode::HUMAN) return;
    timestamp(); label("STATE");
    Serial.printf("%s -> %s\n", from, to);
}

void Logger::classified(uint8_t brickNum, BrickCategory cat, uint8_t discPos, uint8_t bin) {
    if (_mode != LogMode::HUMAN) return;
    timestamp(); label("CLASSIFIED");
    const char* posLabel = "UNKNOWN";
    switch (discPos) {
        case 1: posLabel = "NW (315°)"; break;
        case 2: posLabel = "NE (45°)"; break;
        case 3: posLabel = "SE (135°)"; break;
        case 4: posLabel = "SW (225°)"; break;
    }
    Serial.printf("brick=%-2d  %s  disc=%s  bin=%d\n",
                   brickNum, sensors::categoryName(cat), posLabel, bin);
}

void Logger::platformReleased(uint8_t brickNum, uint8_t bin) {
    if (_mode != LogMode::HUMAN) return;
    timestamp(); label("PLATFORM_RELEASED");
    Serial.printf("brick=%-2d  bin=%d\n", brickNum, bin);
}

void Logger::discIndexed(uint8_t brickNum, uint8_t bin, const char* position) {
    if (_mode != LogMode::HUMAN) return;
    timestamp(); label("DISC_INDEXED");
    Serial.printf("brick=%-2d  bin=%d  position=%s\n", brickNum, bin, position);
}

void Logger::binConfirm(uint8_t brickNum, uint8_t expectedBin, uint8_t actualBin,
                         uint32_t feedMs, bool ok) {
    if (_mode != LogMode::HUMAN) return;
    timestamp(); label("BIN_CONFIRM");
    if (ok) {
        Serial.printf("brick=%-2d  bin=%d  feed=%lums  -> OK\n",
                       brickNum, actualBin, feedMs);
    } else {
        Serial.printf("brick=%-2d  expected=%d  got=%d  feed=%lums  -> FAIL\n",
                       brickNum, expectedBin, actualBin, feedMs);
    }
}

void Logger::thermal() {
    if (_mode != LogMode::HUMAN) return;
    timestamp(); label("THERMAL");
    Serial.printf("sol=[%.2f, %.2f, %.2f]  step=%.2f  -> %s\n",
                   gThermal.solenoidHeat(1), gThermal.solenoidHeat(2),
                   gThermal.solenoidHeat(3), gThermal.stepperHeat(),
                   gThermal.stateName());
}

void Logger::runComplete(uint32_t totalMs, const uint8_t counts[4],
                          uint8_t errors, uint32_t avgFeedMs) {
    Serial.println();
    Serial.println("=== RUN COMPLETE =================================================");

    auto check = [](uint8_t actual, uint8_t expected) -> const char* {
        return (actual == expected) ? "OK" : "FAIL";
    };

    Serial.printf("  total_time_ms   : %lu\n",   totalMs);
    Serial.printf("  bricks_sorted   : %d\n",    TOTAL_BRICKS);
    Serial.printf("  bin1 (2x2 RED)  : %2d  (expected %2d)  %s\n",
                   counts[0], EXPECTED_BIN1, check(counts[0], EXPECTED_BIN1));
    Serial.printf("  bin2 (2x2 BLU)  : %2d  (expected %2d)  %s\n",
                   counts[1], EXPECTED_BIN2, check(counts[1], EXPECTED_BIN2));
    Serial.printf("  bin3 (2x3 BLU)  : %2d  (expected %2d)  %s\n",
                   counts[2], EXPECTED_BIN3, check(counts[2], EXPECTED_BIN3));
    Serial.printf("  bin4 (2x3 RED)  : %2d  (expected %2d)  %s\n",
                   counts[3], EXPECTED_BIN4, check(counts[3], EXPECTED_BIN4));

    uint8_t correct = counts[0] + counts[1] + counts[2] + counts[3];
    Serial.printf("  accuracy        : %d/%d (%.1f%%)\n",
                   correct, TOTAL_BRICKS, 100.0f * correct / TOTAL_BRICKS);
    Serial.printf("  avg_feed_ms     : %lu\n", avgFeedMs);
    Serial.printf("  peak_heat       : %.2f (%s)\n",
                   gThermal.maxHeat(), gThermal.stateName());
    Serial.printf("  errors          : %d\n", errors);
    Serial.println("==================================================================");
    Serial.println();
}

void Logger::errorHalt(uint8_t brickNum, uint8_t expectedBin, const char* reason) {
    timestamp(); label("ERROR_HALT");
    Serial.printf("brick=%-2d  expected_bin=%d  reason=%s\n",
                   brickNum, expectedBin, reason);
    Serial.println("  -> belt stopped, stepper stopped, platform locked");
    Serial.println("  -> type 'sim reset' to clear and return to IDLE");
}

void Logger::info(const char* msg) {
    if (_mode != LogMode::HUMAN) return;
    timestamp(); label("INFO");
    Serial.println(msg);
}
