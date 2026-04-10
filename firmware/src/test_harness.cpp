#include "test_harness.h"
#include <math.h>
#include "actuators.h"
#include "config.h"
#include "events.h"
#include "logger.h"
#include "sensors.h"
#include "state_machine.h"
#include "thermal.h"

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
    switch (cat) {
        case BrickCategory::CAT_2x2_RED:
            result.leftLaneOccupied = true;
            result.rightLaneOccupied = false;
            result.isTwoByThree = false;
            result.sizeValid = true;
            result.leftClearanceMm = 8;
            result.rightClearanceMm = 12;
            result.widerClearanceMm = 12;
            result.redRatio = 0.87f;
            break;
        case BrickCategory::CAT_2x2_BLUE:
            result.leftLaneOccupied = false;
            result.rightLaneOccupied = true;
            result.isTwoByThree = false;
            result.sizeValid = true;
            result.leftClearanceMm = 12;
            result.rightClearanceMm = 8;
            result.widerClearanceMm = 12;
            result.redRatio = 0.06f;
            break;
        case BrickCategory::CAT_2x3_RED:
            result.leftLaneOccupied = true;
            result.rightLaneOccupied = true;
            result.isTwoByThree = true;
            result.sizeValid = true;
            result.leftClearanceMm = 2;
            result.rightClearanceMm = 3;
            result.widerClearanceMm = 3;
            result.redRatio = 0.87f;
            break;
        case BrickCategory::CAT_2x3_BLUE:
            result.leftLaneOccupied = true;
            result.rightLaneOccupied = true;
            result.isTwoByThree = true;
            result.sizeValid = true;
            result.leftClearanceMm = 3;
            result.rightClearanceMm = 2;
            result.widerClearanceMm = 3;
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

static bool waitForState(SystemState target, uint32_t timeoutMs) {
    uint32_t startMs = millis();
    while (millis() - startMs <= timeoutMs) {
        gStateMachine.poll();
        drainEvents();
        if (gStateMachine.currentState() == target) return true;
        if (gStateMachine.currentState() == S_ERROR_HALT) return false;
        delay(1);
    }
    return false;
}

static uint8_t binForCat(BrickCategory cat) {
    switch (cat) {
        case BrickCategory::CAT_2x2_RED:  return 1;
        case BrickCategory::CAT_2x2_BLUE: return 2;
        case BrickCategory::CAT_2x3_BLUE: return 3;
        default:                          return 4;
    }
}

static bool simulateBrick(BrickCategory cat) {
    sensors::setSimulatedSenseResult(senseResultFor(cat));
    pushEvent(EventType::ENTRY_DETECTED);
    drainEvents();

    pushEvent(EventType::CHAMBER_SEATED);
    drainEvents();

    if (!waitForState(S_CONFIRM, DROP_WINDOW_MS + 20)) return false;

    pushEventBinConfirmed(binForCat(cat));
    drainEvents();

    pushEvent(EventType::PLATFORM_LEVEL);
    drainEvents();
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

static void handleLine(const char* line) {
    if (strncmp(line, "sim ", 4) == 0) { handleSim(line + 4); return; }
    if (strncmp(line, "test ", 5) == 0) { handleTest(line + 5); return; }
    if (strncmp(line, "log ", 4) == 0) { handleLog(line + 4); return; }
    if (strcmp(line, "status") == 0) { handleStatus(); return; }
    if (strcmp(line, "help") == 0) { printHelp(); return; }
    Serial.printf("[harness] unknown command: %s\n", line);
}

static void handleSim(const char* args) {
    if (strcmp(args, "start") == 0) {
        pushEvent(EventType::START_BUTTON);
    } else if (strcmp(args, "entry") == 0) {
        pushEvent(EventType::ENTRY_DETECTED);
    } else if (strcmp(args, "seated") == 0) {
        pushEvent(EventType::CHAMBER_SEATED);
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
        pushEventSensingDone(sensors::senseBrickInChamber());
    } else if (strncmp(args, "bin ", 4) == 0) {
        int bin = atoi(args + 4);
        if (bin < 1 || bin > 4) {
            Serial.println("[harness] usage: sim bin <1-4>");
            return;
        }
        pushEventBinConfirmed((uint8_t)bin);
    } else if (strcmp(args, "platform") == 0) {
        pushEvent(EventType::PLATFORM_LEVEL);
    } else if (strcmp(args, "reset") == 0) {
        pushEvent(EventType::RESET);
    } else {
        Serial.printf("[harness] unknown sim command: %s\n", args);
    }

    drainEvents();
}

static void handleTest(const char* args) {
    if (strcmp(args, "release") == 0) {
        uint32_t startMs = millis();
        actuators::firePlatformRelease();
        Serial.printf("[harness] release fired in %lums\n", millis() - startMs);
        return;
    }

    if (strcmp(args, "home") == 0) {
        uint32_t startMs = millis();
        bool ok = actuators::homeSelector();
        Serial.printf("[harness] home %s in %lums\n", ok ? "ok" : "fail", millis() - startMs);
        return;
    }

    if (strncmp(args, "index ", 6) == 0) {
        int bin = atoi(args + 6);
        if (bin < 1 || bin > 4) {
            Serial.println("[harness] usage: test index <1-4>");
            return;
        }
        uint32_t startMs = millis();
        bool ok = actuators::indexSelectorToBin((uint8_t)bin);
        Serial.printf("[harness] index %s in %lums\n", ok ? "ok" : "fail", millis() - startMs);
        return;
    }

    if (strncmp(args, "thermal", 7) == 0) {
        int n = 6;
        if (strlen(args) > 8) n = atoi(args + 8);
        ThermalModel sim;
        Serial.println("[harness] thermal evolution");
        for (int i = 1; i <= n; i++) {
            sim.onSolenoidFire(1);
            sim.onSelectorMove();
            sim.update();
            Serial.printf("  %d: sol=%.3f selector=%.3f state=%s\n",
                          i, sim.solenoidHeat(1), sim.stepperHeat(), sim.stateName());
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

static void handleLog(const char* args) {
    if (strcmp(args, "csv") == 0) {
        gLogger.setMode(LogMode::CSV);
    } else if (strcmp(args, "human") == 0) {
        gLogger.setMode(LogMode::HUMAN);
    } else {
        Serial.println("[harness] usage: log csv | log human");
    }
}

static void handleStatus() {
    Serial.printf("[status] state=%s bricks=%u/%u bins=[%u,%u,%u,%u] token=%s thermal=%s\n",
                  gStateMachine.stateName(),
                  gStateMachine.brickCount(),
                  TOTAL_BRICKS,
                  gStateMachine.binCount(1),
                  gStateMachine.binCount(2),
                  gStateMachine.binCount(3),
                  gStateMachine.binCount(4),
                  gStateMachine.hasToken() ? "yes" : "no",
                  gThermal.stateName());
}

static void printHelp() {
    Serial.println("sim start");
    Serial.println("sim entry");
    Serial.println("sim seated");
    Serial.println("sim sensing <2x2_RED|2x2_BLUE|2x3_RED|2x3_BLUE>");
    Serial.println("sim bin <1-4>");
    Serial.println("sim platform");
    Serial.println("sim reset");
    Serial.println("test release");
    Serial.println("test home");
    Serial.println("test index <1-4>");
    Serial.println("test fullrun");
    Serial.println("test thermal [n]");
    Serial.println("log csv");
    Serial.println("log human");
    Serial.println("status");
    Serial.println("help");
}
