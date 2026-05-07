#include "test_harness.h"
#include <math.h>
#include "actuators.h"
#include "config.h"
#include "events.h"
#include "logger.h"
#include "sensors.h"
#include "state_machine.h"
#include "thermal.h"
#include "tuning_profile.h"

static const BrickCategory FULL_RUN_SEQUENCE[TOTAL_BRICKS] = {
    BrickCategory::CAT_2x2_RED,
    BrickCategory::CAT_2x2_BLUE,
    BrickCategory::CAT_2x3_BLUE,
    BrickCategory::CAT_2x3_RED,
    BrickCategory::CAT_2x2_RED,
    BrickCategory::CAT_2x2_BLUE,
    BrickCategory::CAT_2x3_BLUE,
    BrickCategory::CAT_2x2_RED,
    BrickCategory::CAT_2x3_RED,
    BrickCategory::CAT_2x3_BLUE,
    BrickCategory::CAT_2x2_BLUE,
    BrickCategory::CAT_2x3_BLUE,
    BrickCategory::CAT_2x2_RED,
    BrickCategory::CAT_2x2_BLUE,
    BrickCategory::CAT_2x3_BLUE,
    BrickCategory::CAT_2x2_RED,
    BrickCategory::CAT_2x2_BLUE,
    BrickCategory::CAT_2x3_RED,
    BrickCategory::CAT_2x3_BLUE,
    BrickCategory::CAT_2x2_RED,
    BrickCategory::CAT_2x3_RED,
    BrickCategory::CAT_2x3_BLUE,
    BrickCategory::CAT_2x2_BLUE,
    BrickCategory::CAT_2x3_BLUE,
};

static char gLineBuf[64];
static uint8_t gLineLen = 0;

static SenseResult senseResultFor(BrickCategory cat) {
    SenseResult result;
    result.sampleCount = 6;
    result.category = cat;
    result.sizeValid = true;
    result.beltSpeedLeadMms = 230.0f;
    result.beltSpeedTrailMms = 230.0f;
    result.beltSpeedAvgMms = 230.0f;
    switch (cat) {
        case BrickCategory::CAT_2x2_RED:
            result.isTwoByThree = false;
            result.sizeSignal = 12;
            result.measuredLengthAmm = ROUTE_FALLBACK_2X2_LENGTH_MM;
            result.measuredLengthBmm = ROUTE_FALLBACK_2X2_LENGTH_MM;
            result.measuredLengthAvgMm = ROUTE_FALLBACK_2X2_LENGTH_MM;
            result.redRatio = 0.87f;
            break;
        case BrickCategory::CAT_2x2_BLUE:
            result.isTwoByThree = false;
            result.sizeSignal = 12;
            result.measuredLengthAmm = ROUTE_FALLBACK_2X2_LENGTH_MM;
            result.measuredLengthBmm = ROUTE_FALLBACK_2X2_LENGTH_MM;
            result.measuredLengthAvgMm = ROUTE_FALLBACK_2X2_LENGTH_MM;
            result.redRatio = 0.06f;
            break;
        case BrickCategory::CAT_2x3_RED:
            result.isTwoByThree = true;
            result.sizeSignal = 24;
            result.measuredLengthAmm = ROUTE_FALLBACK_2X3_LENGTH_MM;
            result.measuredLengthBmm = ROUTE_FALLBACK_2X3_LENGTH_MM;
            result.measuredLengthAvgMm = ROUTE_FALLBACK_2X3_LENGTH_MM;
            result.redRatio = 0.87f;
            break;
        case BrickCategory::CAT_2x3_BLUE:
            result.isTwoByThree = true;
            result.sizeSignal = 24;
            result.measuredLengthAmm = ROUTE_FALLBACK_2X3_LENGTH_MM;
            result.measuredLengthBmm = ROUTE_FALLBACK_2X3_LENGTH_MM;
            result.measuredLengthAvgMm = ROUTE_FALLBACK_2X3_LENGTH_MM;
            result.redRatio = 0.06f;
            break;
        default:
            break;
    }
    return result;
}

static void drainEvents() {
    Event e;
    while (gEventQueue.pop(e)) {
        gStateMachine.process(e);
    }
}

static bool waitForTimedConfirmation(uint32_t timeoutMs) {
    uint32_t startMs = millis();
    while (millis() - startMs <= timeoutMs) {
        gStateMachine.poll();
        drainEvents();
        SystemState state = gStateMachine.currentState();
        if (state == S_FEED || state == S_COMPLETE) return true;
        if (state == S_ERROR_HALT) return false;
        delay(1);
    }
    return false;
}

static bool simulateBrick(BrickCategory cat) {
    sensors::setSimulatedSenseResult(senseResultFor(cat));
    pushEvent(EventType::SIZE_ENTRY_DETECTED);
    drainEvents();

    if (!waitForTimedConfirmation(HANDOFF_WINDOW_MS + 20)) return false;
    return gStateMachine.currentState() != S_ERROR_HALT;
}

static void handleLine(const char* line);
static void handleSim(const char* args);
static void handleTest(const char* args);
static void handleLog(const char* args);
static void handleStatus();
static void printHelp();

namespace testHarness {

void begin() {
    gLineLen = 0;
}

void poll() {
    while (Serial.available()) {
        char c = Serial.read();
        if (c == '\r') continue;
        if (c == '\n') {
            gLineBuf[gLineLen] = '\0';
            if (gLineLen > 0) handleLine(gLineBuf);
            gLineLen = 0;
        } else if (gLineLen < sizeof(gLineBuf) - 1) {
            gLineBuf[gLineLen++] = c;
        }
    }
}

}  // namespace testHarness

static void handleCal(const char* args);
static void handleSet(const char* args);
static void handleSpeedMode(const char* args);

static void handleLine(const char* line) {
    if (strncmp(line, "sim ", 4) == 0) { handleSim(line + 4); return; }
    if (strncmp(line, "test ", 5) == 0) { handleTest(line + 5); return; }
    if (strncmp(line, "log ", 4) == 0) { handleLog(line + 4); return; }
    if (strncmp(line, "cal", 3) == 0) { handleCal(line + 3); return; }
    if (strncmp(line, "set ", 4) == 0) { handleSet(line + 4); return; }
    if (strncmp(line, "speedmode", 9) == 0) { handleSpeedMode(line + 9); return; }
    if (strcmp(line, "status") == 0) { handleStatus(); return; }
    if (strcmp(line, "help") == 0) { printHelp(); return; }
    Serial.printf("[harness] unknown command: %s\n", line);
}

static void handleSim(const char* args) {
    if (strcmp(args, "start") == 0) {
        pushEvent(EventType::START_BUTTON);
    } else if (strcmp(args, "brick") == 0) {
        pushEvent(EventType::SIZE_ENTRY_DETECTED);
    } else if (strncmp(args, "sensing ", 8) == 0) {
        const char* catStr = args + 8;
        BrickCategory cat = BrickCategory::UNCERTAIN;
        if (strcmp(catStr, "2x2_RED") == 0) cat = BrickCategory::CAT_2x2_RED;
        else if (strcmp(catStr, "2x2_BLUE") == 0) cat = BrickCategory::CAT_2x2_BLUE;
        else if (strcmp(catStr, "2x3_RED") == 0) cat = BrickCategory::CAT_2x3_RED;
        else if (strcmp(catStr, "2x3_BLUE") == 0) cat = BrickCategory::CAT_2x3_BLUE;
        else {
            Serial.println("[harness] usage: sim sensing <2x2_RED|2x2_BLUE|2x3_RED|2x3_BLUE>");
            return;
        }
        sensors::setSimulatedSenseResult(senseResultFor(cat));
        pushEventSensingDone(sensors::senseBrickAtStation());
    } else if (strncmp(args, "bin ", 4) == 0) {
        int bin = atoi(args + 4);
        if (bin < 1 || bin > 4) {
            Serial.println("[harness] usage: sim bin <1-4>");
            return;
        }
        pushEventBinConfirmed((uint8_t)bin);
    } else if (strcmp(args, "reset") == 0) {
        pushEvent(EventType::RESET);
    } else {
        Serial.printf("[harness] unknown sim command: %s\n", args);
    }

    drainEvents();
}

static void handleTest(const char* args) {
    if (strcmp(args, "home") == 0) {
        uint32_t startMs = millis();
        bool ok = actuators::homeSelector();
        Serial.printf("[harness] home %s in %lums\n", ok ? "ok" : "fail", millis() - startMs);
        return;
    }

    if (strncmp(args, "route ", 6) == 0) {
        int bin = atoi(args + 6);
        if (bin < 1 || bin > 4) {
            Serial.println("[harness] usage: test route <1-4>");
            return;
        }
        uint32_t startMs = millis();
        bool ok = actuators::routeServoToBin((uint8_t)bin);
        Serial.printf("[harness] route %s in %lums\n", ok ? "ok" : "fail", millis() - startMs);
        return;
    }

    if (strcmp(args, "beams") == 0) {
        Serial.println("[beams] watching beam pins for 8 seconds - block each beam now");
        uint32_t start = millis();
        bool lastA = digitalRead(PIN_SIZE_SENSOR_A) == LOW;
        bool lastB = digitalRead(PIN_SIZE_SENSOR_B) == LOW;
        Serial.printf("[beams] initial: A=%s B=%s\n", lastA ? "BLOCKED" : "open", lastB ? "BLOCKED" : "open");
        while (millis() - start < 8000) {
            bool a = digitalRead(PIN_SIZE_SENSOR_A) == LOW;
            bool b = digitalRead(PIN_SIZE_SENSOR_B) == LOW;
            if (a != lastA) {
                Serial.printf("[beams] A -> %s at %lums\n", a ? "BLOCKED" : "open", millis() - start);
                lastA = a;
            }
            if (b != lastB) {
                Serial.printf("[beams] B -> %s at %lums\n", b ? "BLOCKED" : "open", millis() - start);
                lastB = b;
            }
            delay(2);
        }
        Serial.println("[beams] done");
        return;
    }

    if (strncmp(args, "thermal", 7) == 0) {
        int n = 6;
        if (strlen(args) > 8) n = atoi(args + 8);
        ThermalModel sim;
        Serial.println("[harness] thermal evolution");
        for (int i = 1; i <= n; i++) {
            sim.onServoMove();
            sim.update();
            Serial.printf("  %d: servo=%.3f state=%s\n",
                          i, sim.servoHeat(), sim.stateName());
        }
        return;
    }

    if (strcmp(args, "fullrun") == 0) {
        pushEvent(EventType::START_BUTTON);
        drainEvents();
        if (gStateMachine.currentState() != S_FEED) {
            Serial.println("[harness] fullrun failed to enter FEED after start");
            return;
        }

        for (uint8_t i = 0; i < TOTAL_BRICKS; i++) {
            if (!simulateBrick(FULL_RUN_SEQUENCE[i])) {
                Serial.printf("[harness] fullrun aborted at brick %u\n", i + 1);
                return;
            }
        }

        Serial.println("[harness] fullrun completed");
        return;
    }

    Serial.printf("[harness] unknown test command: %s\n", args);
}

static void handleCal(const char* args) {
    // "cal" alone or "cal show"
    if (args[0] == '\0' || strcmp(args + (args[0] == ' ' ? 1 : 0), "show") == 0) {
        sensors::showCalibration();
        return;
    }
    const char* sub = args + 1;  // skip leading space
    if (strncmp(sub, "belt", 4) == 0) {
        uint8_t n = 8;
        if (strlen(sub) > 5) n = (uint8_t)atoi(sub + 5);
        sensors::captureBeltBaseline(n);
        return;
    }
    if (strcmp(sub, "save") == 0) {
        sensors::saveCalibration();
        return;
    }
    if (strcmp(sub, "wipe") == 0) {
        tuning_profile::clearProfile();
        Serial.println("[harness] NVS wiped - config.h defaults active until next cal save");
        return;
    }
    Serial.println("[harness] usage: cal show | cal belt [n] | cal save | cal wipe");
}

static void handleSet(const char* args) {
    if (strncmp(args, "color ", 6) == 0) {
        float v = atof(args + 6);
        sensors::setColorThreshold(v);
        return;
    }
    if (strncmp(args, "size ", 5) == 0) {
        float v = atof(args + 5);
        sensors::setSizeThreshold(v);
        return;
    }
    Serial.println("[harness] usage: set color <0.0-1.0> | set size <mm>");
}

static void handleSpeedMode(const char* args) {
    auto mode = actuators::conveyorSpeedMode();
    if (args[0] == '\0') {
        Serial.printf("[harness] speedmode=%s fast_sps=%lu slow_sps=%lu\n",
                      actuators::conveyorSpeedModeName(mode),
                      (unsigned long)actuators::conveyorFastSps(),
                      (unsigned long)actuators::conveyorSlowSps());
        return;
    }

    if (strcmp(args, " balanced") == 0) {
        actuators::setConveyorSpeedMode(actuators::ConveyorSpeedMode::BALANCED);
    } else if (strcmp(args, " aggressive") == 0) {
        actuators::setConveyorSpeedMode(actuators::ConveyorSpeedMode::AGGRESSIVE);
    } else if (strcmp(args, " accuracy") == 0) {
        actuators::setConveyorSpeedMode(actuators::ConveyorSpeedMode::ACCURACY);
    } else {
        Serial.println("[harness] usage: speedmode [balanced|aggressive|accuracy]");
        return;
    }

    mode = actuators::conveyorSpeedMode();
    Serial.printf("[harness] speedmode=%s fast_sps=%lu slow_sps=%lu\n",
                  actuators::conveyorSpeedModeName(mode),
                  (unsigned long)actuators::conveyorFastSps(),
                  (unsigned long)actuators::conveyorSlowSps());
}

static void handleLog(const char* args) {
    if (strcmp(args, "csv") == 0) {
        gLogger.setMode(LogMode::CSV);
    } else if (strcmp(args, "human") == 0) {
        gLogger.setMode(LogMode::HUMAN);
    } else if (strcmp(args, "quiet") == 0) {
        gLogger.setMode(LogMode::CSV);
        Serial.println("[logger] quiet mode (csv suppresses sense verbose)");
    } else {
        Serial.println("[harness] usage: log csv | log human | log quiet");
    }
}

static void handleStatus() {
    Serial.printf("[status] state=%s confirmed=%u/%u issued=%u in_flight=%u bins=[%u,%u,%u,%u] token=%s thermal=%s speedmode=%s fast_sps=%lu slow_sps=%lu\n",
                  gStateMachine.stateName(),
                  gStateMachine.brickCount(),
                  TOTAL_BRICKS,
                  gStateMachine.issuedBrickCount(),
                  gStateMachine.inFlightCount(),
                  gStateMachine.binCount(1),
                  gStateMachine.binCount(2),
                  gStateMachine.binCount(3),
                  gStateMachine.binCount(4),
                  gStateMachine.hasToken() ? "yes" : "no",
                  gThermal.stateName(),
                  actuators::conveyorSpeedModeName(actuators::conveyorSpeedMode()),
                  (unsigned long)actuators::conveyorFastSps(),
                  (unsigned long)actuators::conveyorSlowSps());
}

static void printHelp() {
    Serial.println("sim start");
    Serial.println("sim brick");
    Serial.println("sim sensing <2x2_RED|2x2_BLUE|2x3_RED|2x3_BLUE>");
    Serial.println("sim bin <1-4>");
    Serial.println("sim reset");
    Serial.println("test home");
    Serial.println("test route <1-4>");
    Serial.println("test beams     // watch beam A and B state for 8 seconds");
    Serial.println("test fullrun");
    Serial.println("test thermal [n]");
    Serial.println("cal show");
    Serial.println("cal belt [n]   // capture n belt baseline samples (default 8)");
    Serial.println("cal save       // save all thresholds + belt baseline to NVS");
    Serial.println("cal wipe       // clear NVS, revert to config.h defaults");
    Serial.println("set color <v>  // set color threshold (0.0-1.0)");
    Serial.println("set size <mm>  // set size threshold mm");
    Serial.println("speedmode      // show current conveyor speed mode and SPS");
    Serial.println("speedmode balanced");
    Serial.println("speedmode aggressive");
    Serial.println("speedmode accuracy");
    Serial.println("log csv");
    Serial.println("log human");
    Serial.println("status");
    Serial.println("help");
}
