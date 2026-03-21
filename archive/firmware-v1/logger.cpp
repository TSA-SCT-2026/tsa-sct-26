#include "logger.h"
#include "router.h"
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
    if (mode == LogMode::CSV && !_csvHeaderPrinted) {
        csvHeader();
        _csvHeaderPrinted = true;
    }
    if (mode == LogMode::HUMAN) {
        Serial.println("[logger] switched to HUMAN mode");
    }
}

void Logger::timestamp() {
    Serial.printf("[%7lums] ", millis());
}

// Human mode helpers

static void cat(const char* label) {
    // Left-pad category to CAT_WIDTH chars
    Serial.printf("%-18s", label);
}

// State machine events

void Logger::stateChange(const char* from, const char* to) {
    if (_mode != LogMode::HUMAN) return;
    timestamp(); cat("STATE");
    Serial.printf("%s -> %s\n", from, to);
}

void Logger::brickEnter(uint8_t brickNum) {
    if (_mode != LogMode::HUMAN) return;
    timestamp(); cat("BRICK_ENTER");
    Serial.printf("brick=%-2d\n", brickNum);
}

void Logger::sizeDetected(uint8_t brickNum, uint32_t gap_us, BrickSize size) {
    if (_mode != LogMode::HUMAN) return;
    timestamp(); cat("SIZE_DETECTED");
    if (gap_us == 0) {
        Serial.printf("brick=%-2d  size=%-3s  gap=TIMEOUT  (threshold=%uus)\n",
                       brickNum, sizeName(size), SIZE_THRESHOLD_US);
    } else {
        Serial.printf("brick=%-2d  size=%-3s  gap_us=%-6lu  (threshold=%uus)\n",
                       brickNum, sizeName(size), gap_us, SIZE_THRESHOLD_US);
    }
}

void Logger::colorSample(uint8_t brickNum, uint16_t sampleIdx,
                          uint16_t r, uint16_t g, uint16_t b, uint16_t c,
                          float ratio, bool valid) {
    if (_mode != LogMode::HUMAN) return;
    // Only log every 5th sample in human mode to avoid flooding the terminal.
    // All samples are counted and averaged regardless.
    if (sampleIdx % 5 != 0 && sampleIdx != 0) return;
    timestamp(); cat("COLOR_SAMPLE");
    Serial.printf("brick=%-2d  samp=%-2d  r=%-4d  g=%-4d  b=%-4d  c=%-4d  ratio=%.3f  %s\n",
                   brickNum, sampleIdx, r, g, b, c, ratio, valid ? "VALID" : "BELT");
}

void Logger::colorDone(uint8_t brickNum, uint16_t validSamples, uint16_t totalSamples,
                        float avgRatio, BrickColor color) {
    if (_mode != LogMode::HUMAN) return;
    timestamp(); cat("COLOR_DONE");
    Serial.printf("brick=%-2d  valid=%d/%d  avg_ratio=%.3f  -> %s\n",
                   brickNum, validSamples, totalSamples, avgRatio, colorName(color));
}

void Logger::classified(uint8_t brickNum, BrickSize size, BrickColor color,
                         BrickType type, uint8_t plow, uint8_t bin) {
    if (_mode != LogMode::HUMAN) return;
    timestamp(); cat("CLASSIFIED");
    const char* plowStr = (plow == 0) ? "none (default)" : "";
    if (plow > 0) {
        Serial.printf("brick=%-2d  %s %s  -> %s  plow=%d  bin=%d\n",
                       brickNum, sizeName(size), colorName(color), typeName(type), plow, bin);
    } else {
        Serial.printf("brick=%-2d  %s %s  -> %s  plow=none  bin=%d\n",
                       brickNum, sizeName(size), colorName(color), typeName(type), bin);
    }
}

void Logger::plowExtend(uint8_t plow) {
    if (_mode != LogMode::HUMAN) return;
    timestamp(); cat("PLOW_EXTEND");
    Serial.printf("plow=%d  full_pwm=%d  will_hold_at=%dms\n",
                   plow, SOL_FULL_PWM, SOL_FULL_MS);
}

void Logger::plowHold(uint8_t plow) {
    if (_mode != LogMode::HUMAN) return;
    timestamp(); cat("PLOW_HOLD");
    Serial.printf("plow=%d  hold_pwm=%d (~40%%)\n", plow, SOL_HOLD_PWM);
}

void Logger::plowRelease(uint8_t plow) {
    if (_mode != LogMode::HUMAN) return;
    timestamp(); cat("PLOW_RELEASE");
    Serial.printf("plow=%d  total_on_ms=%d\n", plow, SOL_DEENERGIZE_MS);
}

void Logger::binConfirm(uint8_t brickNum, uint8_t expectedBin, uint8_t actualBin,
                         uint32_t transitMs, bool ok) {
    if (_mode != LogMode::HUMAN) return;
    timestamp(); cat("BIN_CONFIRM");
    if (ok) {
        Serial.printf("brick=%-2d  bin=%d  transit=%lums  -> OK\n",
                       brickNum, actualBin, transitMs);
    } else {
        Serial.printf("brick=%-2d  expected_bin=%d  got_bin=%d  transit=%lums  -> FAIL\n",
                       brickNum, expectedBin, actualBin, transitMs);
    }
}

void Logger::thermal() {
    if (_mode != LogMode::HUMAN) return;
    timestamp(); cat("THERMAL");
    Serial.printf("sol=[%.2f, %.2f, %.2f]  step=%.2f  -> %s\n",
                   gThermal.solenoidHeat(1), gThermal.solenoidHeat(2),
                   gThermal.solenoidHeat(3), gThermal.stepperHeat(),
                   gThermal.stateName());
}

void Logger::binCounts(const uint8_t counts[4], uint8_t total) {
    if (_mode != LogMode::HUMAN) return;
    timestamp(); cat("BIN_COUNTS");
    Serial.printf("b1=%-2d  b2=%-2d  b3=%-2d  b4=%-2d  total=%d/%d\n",
                   counts[0], counts[1], counts[2], counts[3], total, TOTAL_BRICKS);
}

void Logger::runComplete(uint32_t totalMs, const uint8_t counts[4],
                          uint8_t errors, uint32_t avgTransitMs) {
    Serial.println();
    Serial.println("=== RUN COMPLETE =================================================");

    auto check = [](uint8_t actual, uint8_t expected) -> const char* {
        return (actual == expected) ? "OK" : "FAIL";
    };

    Serial.printf("  total_time_ms   : %lu\n",   totalMs);
    Serial.printf("  bricks_sorted   : %d\n",    TOTAL_BRICKS);
    Serial.printf("  bin1 (2x2 BLU)  : %2d  (expected %2d)  %s\n",
                   counts[0], EXPECTED_BIN1, check(counts[0], EXPECTED_BIN1));
    Serial.printf("  bin2 (2x2 RED)  : %2d  (expected %2d)  %s\n",
                   counts[1], EXPECTED_BIN2, check(counts[1], EXPECTED_BIN2));
    Serial.printf("  bin3 (2x3 RED)  : %2d  (expected %2d)  %s\n",
                   counts[2], EXPECTED_BIN3, check(counts[2], EXPECTED_BIN3));
    Serial.printf("  bin4 (2x3 BLU)  : %2d  (expected %2d)  %s\n",
                   counts[3], EXPECTED_BIN4, check(counts[3], EXPECTED_BIN4));

    uint8_t correct = counts[0] + counts[1] + counts[2] + counts[3];
    Serial.printf("  accuracy        : %d/%d (%.1f%%)\n",
                   correct, TOTAL_BRICKS, 100.0f * correct / TOTAL_BRICKS);
    Serial.printf("  avg_transit_ms  : %lu\n", avgTransitMs);
    Serial.printf("  peak_heat       : %.2f (%s)\n",
                   gThermal.maxHeat(), gThermal.stateName());
    Serial.printf("  errors          : %d\n", errors);
    Serial.println("==================================================================");
    Serial.println();
}

void Logger::errorHalt(uint8_t brickNum, uint8_t expectedBin, const char* reason) {
    timestamp(); cat("ERROR_HALT");
    Serial.printf("brick=%-2d  expected_bin=%d  reason=%s\n",
                   brickNum, expectedBin, reason);
    Serial.println("  -> belt stopped, stepper stopped, all solenoids off");
    Serial.println("  -> type 'sim reset' to clear and return to IDLE");
}

void Logger::info(const char* msg) {
    if (_mode != LogMode::HUMAN) return;
    timestamp(); cat("INFO");
    Serial.println(msg);
}

// CSV mode - useful for benchmarks, tuning, and simulator 

void Logger::csvHeader() {
    Serial.println("brick,ts_enter_ms,ts_confirm_ms,transit_ms,"
                   "gap_us,size,"
                   "r_avg,g_avg,b_avg,ratio_avg,valid_samples,"
                   "color,type,plow,bin,ok,"
                   "heat_sol1,heat_sol2,heat_sol3,heat_step,thermal_state");
}

void Logger::csvBrickRow(uint8_t brickNum, uint32_t tsEnterMs, uint32_t tsConfirmMs,
                          uint32_t gap_us, BrickSize size,
                          uint32_t sumR, uint32_t sumG, uint32_t sumB,
                          uint16_t validSamples, float avgRatio,
                          BrickColor color, BrickType type,
                          uint8_t plow, uint8_t bin, bool ok) {
    uint32_t transit = (tsConfirmMs > tsEnterMs) ? tsConfirmMs - tsEnterMs : 0;
    uint32_t rAvg = validSamples > 0 ? sumR / validSamples : 0;
    uint32_t gAvg = validSamples > 0 ? sumG / validSamples : 0;
    uint32_t bAvg = validSamples > 0 ? sumB / validSamples : 0;

    Serial.printf("%d,%lu,%lu,%lu,%lu,%s,%lu,%lu,%lu,%.3f,%d,%s,%s,%d,%d,%s,%.3f,%.3f,%.3f,%.3f,%s\n",
                   brickNum, tsEnterMs, tsConfirmMs, transit,
                   gap_us, sizeName(size),
                   rAvg, gAvg, bAvg, avgRatio, validSamples,
                   colorName(color), typeName(type), plow, bin,
                   ok ? "1" : "0",
                   gThermal.solenoidHeat(1), gThermal.solenoidHeat(2),
                   gThermal.solenoidHeat(3), gThermal.stepperHeat(),
                   gThermal.stateName());
}
