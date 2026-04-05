#include "logger.h"
#include "config.h"

Logger gLogger;

void Logger::begin() {
    Serial.begin(SERIAL_BAUD);
    while (!Serial) delay(10);
    printCsvHeader();
}

void Logger::setMode(LogMode mode) {
    _mode = mode;
    if (_mode == LogMode::CSV) {
        printCsvHeader();
    } else {
        Serial.println("[logger] switched to HUMAN mode");
    }
}

void Logger::printCsvHeader() {
    Serial.println("event,ts_ms,brick,state,category,target_bin,actual_bin,selector_steps,samples,ok,error,heat,message");
}

void Logger::printHumanPrefix(const char* label) {
    Serial.printf("[%7lums] %-16s", millis(), label);
}

void Logger::printCsvRow(const char* eventName,
                         uint8_t brickNum,
                         const char* stateName,
                         const char* category,
                         uint8_t targetBin,
                         uint8_t actualBin,
                         uint16_t selectorSteps,
                         uint8_t samples,
                         const char* okField,
                         const char* errorCode,
                         const char* message) {
    Serial.printf("%s,%lu,%u,%s,%s,",
                  eventName,
                  millis(),
                  brickNum,
                  stateName ? stateName : "",
                  category ? category : "");

    if (targetBin > 0) Serial.printf("%u,", targetBin); else Serial.print(",");
    if (actualBin > 0) Serial.printf("%u,", actualBin); else Serial.print(",");
    if (selectorSteps > 0) Serial.printf("%u,", selectorSteps); else Serial.print(",");
    if (samples > 0) Serial.printf("%u,", samples); else Serial.print(",");

    Serial.printf("%s,%s,%.3f,%s\n",
                  okField ? okField : "",
                  errorCode ? errorCode : "",
                  gThermal.maxHeat(),
                  message ? message : "");
}

void Logger::stateChange(const char* from, const char* to) {
    if (_mode == LogMode::HUMAN) {
        printHumanPrefix("STATE");
        Serial.printf("%s -> %s\n", from, to);
        return;
    }

    char msg[48];
    snprintf(msg, sizeof(msg), "%s->%s", from, to);
    printCsvRow("state_change", 0, to, "", 0, 0, 0, 0, "", "", msg);
}

void Logger::classified(uint8_t brickNum, BrickCategory cat, uint8_t targetBin, uint8_t samples) {
    const char* category = sensors::categoryName(cat);
    if (_mode == LogMode::HUMAN) {
        printHumanPrefix("CLASSIFIED");
        Serial.printf("brick=%u category=%s target_bin=%u samples=%u\n",
                      brickNum, category, targetBin, samples);
        return;
    }
    printCsvRow("classified", brickNum, "SENSING", category, targetBin, 0, 0, samples, "1", "", "");
}

void Logger::selectorReady(uint8_t brickNum, uint8_t targetBin, uint16_t steps, bool ok,
                           const char* positionLabel) {
    if (_mode == LogMode::HUMAN) {
        printHumanPrefix("SELECTOR_READY");
        Serial.printf("brick=%u target_bin=%u pos=%s steps=%u ok=%s\n",
                      brickNum, targetBin, positionLabel, steps, ok ? "yes" : "no");
        return;
    }
    printCsvRow("selector_ready", brickNum, "INDEXED", "", targetBin, 0, steps, 0,
                ok ? "1" : "0", ok ? "" : "SELECTOR_JAM", positionLabel);
}

void Logger::releaseFired(uint8_t brickNum, uint8_t targetBin) {
    if (_mode == LogMode::HUMAN) {
        printHumanPrefix("RELEASE");
        Serial.printf("brick=%u target_bin=%u\n", brickNum, targetBin);
        return;
    }
    printCsvRow("release_fired", brickNum, "RELEASED", "", targetBin, 0, 0, 0, "1", "", "");
}

void Logger::dropWindowDone(uint8_t brickNum, uint8_t targetBin) {
    if (_mode == LogMode::HUMAN) {
        printHumanPrefix("DROP_WINDOW");
        Serial.printf("brick=%u target_bin=%u\n", brickNum, targetBin);
        return;
    }
    printCsvRow("drop_window_done", brickNum, "CONFIRM", "", targetBin, 0, 0, 0, "1", "", "");
}

void Logger::binConfirm(uint8_t brickNum, uint8_t expectedBin, uint8_t actualBin,
                        uint32_t transitMs, bool ok) {
    if (_mode == LogMode::HUMAN) {
        printHumanPrefix("BIN_CONFIRM");
        Serial.printf("brick=%u expected=%u actual=%u transit_ms=%lu ok=%s\n",
                      brickNum, expectedBin, actualBin, transitMs, ok ? "yes" : "no");
        return;
    }

    char msg[32];
    snprintf(msg, sizeof(msg), "transit_ms=%lu", transitMs);
    printCsvRow("bin_confirm", brickNum, "CONFIRM", "", expectedBin, actualBin, 0, 0,
                ok ? "1" : "0", ok ? "" : "MISS_BIN", msg);
}

void Logger::platformLevel(uint8_t brickNum) {
    if (_mode == LogMode::HUMAN) {
        printHumanPrefix("PLATFORM_LEVEL");
        Serial.printf("brick=%u\n", brickNum);
        return;
    }
    printCsvRow("platform_level", brickNum, "RESET", "", 0, 0, 0, 0, "1", "", "");
}

void Logger::thermal() {
    if (_mode == LogMode::HUMAN) {
        printHumanPrefix("THERMAL");
        Serial.printf("sol=[%.2f, %.2f, %.2f] selector=%.2f state=%s\n",
                      gThermal.solenoidHeat(1), gThermal.solenoidHeat(2),
                      gThermal.solenoidHeat(3), gThermal.stepperHeat(),
                      gThermal.stateName());
        return;
    }
    printCsvRow("thermal", 0, "", "", 0, 0, 0, 0, "", "", gThermal.stateName());
}

void Logger::runComplete(uint32_t totalMs, const uint8_t counts[4], bool countsMatch) {
    if (_mode == LogMode::HUMAN) {
        Serial.println();
        Serial.println("=== RUN COMPLETE ================================================");
        Serial.printf("total_ms: %lu\n", totalMs);
        Serial.printf("bin1: %u/%u\n", counts[0], EXPECTED_BIN1);
        Serial.printf("bin2: %u/%u\n", counts[1], EXPECTED_BIN2);
        Serial.printf("bin3: %u/%u\n", counts[2], EXPECTED_BIN3);
        Serial.printf("bin4: %u/%u\n", counts[3], EXPECTED_BIN4);
        Serial.printf("count_check: %s\n", countsMatch ? "PASS" : "FAIL");
        Serial.println("===============================================================");
        return;
    }

    char msg[96];
    snprintf(msg, sizeof(msg), "total_ms=%lu counts=[%u,%u,%u,%u]",
             totalMs, counts[0], counts[1], counts[2], counts[3]);
    printCsvRow("run_complete", TOTAL_BRICKS, "COMPLETE", "", 0, 0, 0, 0,
                countsMatch ? "1" : "0", countsMatch ? "" : "COUNT_MISMATCH", msg);
}

void Logger::errorHalt(uint8_t brickNum, uint8_t expectedBin, const char* reason) {
    if (_mode == LogMode::HUMAN) {
        printHumanPrefix("ERROR_HALT");
        Serial.printf("brick=%u target_bin=%u reason=%s\n", brickNum, expectedBin, reason);
        return;
    }
    printCsvRow("error_halt", brickNum, "ERROR_HALT", "", expectedBin, 0, 0, 0, "0", reason, "");
}

void Logger::info(const char* msg) {
    if (_mode == LogMode::HUMAN) {
        printHumanPrefix("INFO");
        Serial.println(msg);
        return;
    }
    printCsvRow("info", 0, "", "", 0, 0, 0, 0, "", "", msg);
}
