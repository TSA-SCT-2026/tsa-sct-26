#include "test_harness.h"
#include "events.h"
#include "sensors.h"
#include "actuators.h"
#include "state_machine.h"
#include "thermal.h"
#include "logger.h"
#include "config.h"

// ----------------------------------------------------------------
// Full-run brick sequence: 6x 2x2_RED, 6x 2x2_BLUE, 8x 2x3_BLUE, 4x 2x3_RED
// Interleaved so back-to-back same-bin indexing tests thermal model.
// ----------------------------------------------------------------
static const BrickCategory FULL_RUN_SEQUENCE[TOTAL_BRICKS] = {
    BrickCategory::CAT_2x2_RED,   // bin 1
    BrickCategory::CAT_2x2_BLUE,  // bin 2
    BrickCategory::CAT_2x3_BLUE,  // bin 3
    BrickCategory::CAT_2x3_RED,   // bin 4 (default)
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

// Build a plausible SenseResult for a given category.
// rRatio values are placeholder calibration targets.
//   RED:  rRatio ~0.87 (high R, low G/B)
//   BLUE: rRatio ~0.06 (low R, high B)
static SenseResult senseResultFor(BrickCategory cat) {
    SenseResult r;
    r.sampleCount = 6;
    r.category    = cat;
    switch (cat) {
        case BrickCategory::CAT_2x2_RED:
            r.isLarge = false; r.rRatio = 0.87f; break;
        case BrickCategory::CAT_2x2_BLUE:
            r.isLarge = false; r.rRatio = 0.06f; break;
        case BrickCategory::CAT_2x3_RED:
            r.isLarge = true;  r.rRatio = 0.87f; break;
        case BrickCategory::CAT_2x3_BLUE:
            r.isLarge = true;  r.rRatio = 0.06f; break;
        default:
            r.isLarge = false; r.rRatio = 0.0f;  break;
    }
    return r;
}

// Routing mirrors state_machine.cpp - used only for driving confirmations in fullrun.
static uint8_t binForCat(BrickCategory cat) {
    switch (cat) {
        case BrickCategory::CAT_2x2_RED:  return 1;
        case BrickCategory::CAT_2x2_BLUE: return 2;
        case BrickCategory::CAT_2x3_BLUE: return 3;
        default:                           return 4;
    }
}

// Inject all events for one brick and drive them through the state machine.
static void simulateBrick(BrickCategory cat) {
    Event e;

    // Brick seated in chamber
    pushEvent(EventType::BRICK_SEATED);
    while (gEventQueue.pop(e)) gStateMachine.process(e);

    // Sensing: brick is stationary in chamber, classification completes
    pushEventSensingDone(senseResultFor(cat));
    while (gEventQueue.pop(e)) gStateMachine.process(e);

    // Disc indexed to target bin
    uint8_t bin = binForCat(cat);
    pushEventIndexed(bin);
    while (gEventQueue.pop(e)) gStateMachine.process(e);

    // Platform released
    pushEvent(EventType::PLATFORM_RELEASED);
    while (gEventQueue.pop(e)) gStateMachine.process(e);

    // Bin confirmation
    pushEvent((EventType)((uint8_t)EventType::BIN1_CONFIRM + (bin - 1)));
    while (gEventQueue.pop(e)) gStateMachine.process(e);

    gThermal.update();
}

// ----------------------------------------------------------------
// Command parsing
// ----------------------------------------------------------------

static char    _lineBuf[64];
static uint8_t _lineLen = 0;

static void handleLine(const char* line);
static void handleSim(const char* args);
static void handleTest(const char* args);
static void handleLog(const char* args);
static void handleStatus();
static void printHelp();

namespace testHarness {

void begin() {
    _lineLen = 0;
}

void poll() {
    while (Serial.available()) {
        char c = Serial.read();
        if (c == '\r') continue;
        if (c == '\n') {
            _lineBuf[_lineLen] = '\0';
            if (_lineLen > 0) handleLine(_lineBuf);
            _lineLen = 0;
        } else if (_lineLen < (sizeof(_lineBuf) - 1)) {
            _lineBuf[_lineLen++] = c;
        }
    }
}

}  // namespace testHarness

static void handleLine(const char* line) {
    if (strncmp(line, "sim ", 4) == 0)   { handleSim(line + 4);  return; }
    if (strncmp(line, "test ", 5) == 0)  { handleTest(line + 5); return; }
    if (strncmp(line, "log ", 4) == 0)   { handleLog(line + 4);  return; }
    if (strcmp(line, "status") == 0)     { handleStatus();        return; }
    if (strcmp(line, "help") == 0)       { printHelp();           return; }
    Serial.printf("[harness] unknown command: '%s'  (type 'help')\n", line);
}

static void handleSim(const char* args) {
    if (strcmp(args, "start") == 0) {
        pushEvent(EventType::START_BUTTON);

    } else if (strncmp(args, "sensing ", 8) == 0) {
        const char* catStr = args + 8;
        SenseResult result;
        result.sampleCount = 6;
        result.category = BrickCategory::UNCERTAIN;
        if (strcmp(catStr, "2x2_RED") == 0) {
            result.category = BrickCategory::CAT_2x2_RED;
            result.isLarge = false; result.rRatio = 0.87f;
        } else if (strcmp(catStr, "2x2_BLUE") == 0) {
            result.category = BrickCategory::CAT_2x2_BLUE;
            result.isLarge = false; result.rRatio = 0.06f;
        } else if (strcmp(catStr, "2x3_RED") == 0) {
            result.category = BrickCategory::CAT_2x3_RED;
            result.isLarge = true; result.rRatio = 0.87f;
        } else if (strcmp(catStr, "2x3_BLUE") == 0) {
            result.category = BrickCategory::CAT_2x3_BLUE;
            result.isLarge = true; result.rRatio = 0.06f;
        } else {
            Serial.println("[harness] usage: sim sensing <2x2_RED|2x2_BLUE|2x3_RED|2x3_BLUE>");
            return;
        }
        pushEventSensingDone(result);

    } else if (strcmp(args, "seated") == 0) {
        pushEvent(EventType::BRICK_SEATED);

    } else if (strncmp(args, "indexed ", 8) == 0) {
        int bin = atoi(args + 8);
        if (bin >= 1 && bin <= 4) {
            pushEventIndexed((uint8_t)bin);
        } else {
            Serial.println("[harness] usage: sim indexed <1-4>");
        }

    } else if (strcmp(args, "released") == 0) {
        pushEvent(EventType::PLATFORM_RELEASED);

    } else if (strncmp(args, "bin ", 4) == 0) {
        int bin = atoi(args + 4);
        if (bin >= 1 && bin <= 4) {
            pushEvent((EventType)((uint8_t)EventType::BIN1_CONFIRM + (bin - 1)));
        } else {
            Serial.println("[harness] usage: sim bin <1-4>");
        }

    } else if (strcmp(args, "jam") == 0) {
        pushEvent(EventType::CONFIRM_TIMEOUT);

    } else if (strcmp(args, "reset") == 0) {
        pushEvent(EventType::RESET);

    } else {
        Serial.printf("[harness] unknown sim command: '%s'\n", args);
    }
}

static void handleTest(const char* args) {

    if (strcmp(args, "release") == 0) {
        Serial.println();
        Serial.println("--- test platform release --------------------------");
        uint32_t start = millis();
        actuators::firePlatformRelease();
        uint32_t duration = millis() - start;
        Serial.printf("  Platform release fired. Duration: %lums\n", duration);
        Serial.println("----------------------------------------------------");
        Serial.println();

    } else if (strcmp(args, "home") == 0) {
        Serial.println();
        Serial.println("--- test home disc ---------------------------------");
        uint32_t start = millis();
        bool success = actuators::homeDisc();
        uint32_t duration = millis() - start;
        Serial.printf("  Home result: %s. Duration: %lums\n", 
                       success ? "SUCCESS" : "FAIL", duration);
        Serial.println("----------------------------------------------------");
        Serial.println();

    } else if (strncmp(args, "index ", 6) == 0) {
        int bin = atoi(args + 6);
        if (bin >= 1 && bin <= 4) {
            Serial.println();
            Serial.printf("--- test index to bin %d -----------------------------\n", bin);
            uint32_t start = millis();
            bool success = actuators::indexToBin((uint8_t)bin);
            uint32_t duration = millis() - start;
            Serial.printf("  Index result: %s. Duration: %lums\n", 
                           success ? "SUCCESS" : "FAIL", duration);
            Serial.println("----------------------------------------------------");
            Serial.println();
        } else {
            Serial.println("[harness] usage: test index <1-4>");
        }

    } else if (strcmp(args, "drop") == 0) {
        Serial.println();
        Serial.println("--- test drop (index + release + confirm) ---------");
        Serial.println("  Indexing to bin 1...");
        uint32_t start = millis();
        bool indexed = actuators::indexToBin(1);
        if (!indexed) {
            Serial.println("  ERROR: index failed");
            Serial.println("----------------------------------------------------");
            Serial.println();
            return;
        }
        uint32_t indexDuration = millis() - start;
        Serial.printf("  Index complete: %lums\n", indexDuration);
        
        Serial.println("  Firing platform release...");
        start = millis();
        actuators::firePlatformRelease();
        uint32_t releaseDuration = millis() - start;
        Serial.printf("  Release fired: %lums\n", releaseDuration);
        
        Serial.println("  Waiting for bin 1 beam confirmation...");
        Serial.println("  (inject with: sim bin 1)");
        Serial.println("----------------------------------------------------");
        Serial.println();

    } else if (strncmp(args, "thermal", 7) == 0) {
        int n = 6;
        if (strlen(args) > 8) n = atoi(args + 8);

        Serial.println();
        Serial.println("--- thermal model evolution --------------------");
        Serial.printf("  Firing platform release + stepper %d times:\n", n);
        Serial.println("  fire#   release  step    state");

        ThermalModel sim;
        for (int i = 1; i <= n; i++) {
            sim.onSolenoidFire(1);
            sim.onStepperRelease();
            sim.update();
            Serial.printf("  %-6d  %.3f    %.3f   %s\n",
                           i, sim.solenoidHeat(1), sim.stepperHeat(), sim.stateName());
        }
        float factor = expf(-THERMAL_DECAY_RATE * 5.0f);
        Serial.printf("  after 5s decay factor = %.4f\n", factor);
        Serial.println("------------------------------------------------");
        Serial.println();

    } else if (strcmp(args, "fullrun") == 0) {
        Serial.println();
        Serial.println("[harness] starting simulated 24-brick full run...");
        Serial.println("[harness] injecting START_BUTTON");

        pushEvent(EventType::START_BUTTON);
        Event e;
        while (gEventQueue.pop(e)) gStateMachine.process(e);

        if (gStateMachine.currentState() != State::FEED) {
            Serial.println("[harness] ERROR: state machine did not enter FEED after start");
            return;
        }

        for (uint8_t i = 0; i < TOTAL_BRICKS; i++) {
            simulateBrick(FULL_RUN_SEQUENCE[i]);

            if (gStateMachine.currentState() == State::ERROR_HALT) {
                Serial.printf("[harness] run aborted at brick %d - ERROR_HALT\n", i + 1);
                return;
            }
        }

        Serial.println("[harness] full run simulation complete");

    } else {
        Serial.printf("[harness] unknown test command: '%s'\n", args);
    }
}

static void handleLog(const char* args) {
    if (strcmp(args, "human") == 0) {
        gLogger.setMode(LogMode::HUMAN);
    } else if (strcmp(args, "csv") == 0) {
        gLogger.setMode(LogMode::CSV);
    } else {
        Serial.println("[harness] usage: log human  |  log csv");
    }
}

static void handleStatus() {
    Serial.println();
    Serial.println("--- status -------------------------------------");
    Serial.printf("  state       : %s\n", gStateMachine.stateName());
    Serial.printf("  bricks      : %d/%d\n", gStateMachine.brickCount(), TOTAL_BRICKS);
    Serial.printf("  bin1 (2x2R) : %d  (expected %d)\n", gStateMachine.binCount(1), EXPECTED_BIN1);
    Serial.printf("  bin2 (2x2B) : %d  (expected %d)\n", gStateMachine.binCount(2), EXPECTED_BIN2);
    Serial.printf("  bin3 (2x3B) : %d  (expected %d)\n", gStateMachine.binCount(3), EXPECTED_BIN3);
    Serial.printf("  bin4 (2x3R) : %d  (expected %d)\n", gStateMachine.binCount(4), EXPECTED_BIN4);
    Serial.printf("  thermal     : sol=[%.2f, %.2f, %.2f]  step=%.2f  -> %s\n",
                   gThermal.solenoidHeat(1), gThermal.solenoidHeat(2),
                   gThermal.solenoidHeat(3), gThermal.stepperHeat(),
                   gThermal.stateName());
    Serial.printf("  log_mode    : %s\n", gLogger.mode() == LogMode::CSV ? "CSV" : "HUMAN");
    Serial.println("------------------------------------------------");
    Serial.println();
}

static void printHelp() {
    Serial.println();
    Serial.println("=== Test Harness Commands =====================================");
    Serial.println("  Inject events (same as hardware ISRs):");
    Serial.println("    sim start                         inject START_BUTTON");
    Serial.println("    sim sensing <cat>                 inject SENSING_DONE");
    Serial.println("      cat: 2x2_RED | 2x2_BLUE | 2x3_RED | 2x3_BLUE");
    Serial.println("    sim chute_exit                    inject CHUTE_EXIT");
    Serial.println("    sim pusher <1-3>                  inject PUSHER_FIRED");
    Serial.println("    sim bin <1-4>                     inject BIN_CONFIRM");
    Serial.println("    sim jam                           inject CONFIRM_TIMEOUT");
    Serial.println("    sim reset                         inject RESET");
    Serial.println();
    Serial.println("  Run tests:");
    Serial.println("    test thermal [n_fires]  (default 6)");
    Serial.println("    test fullrun            simulate complete 24-brick run");
    Serial.println();
    Serial.println("  Logging:");
    Serial.println("    log human               human-readable output (default)");
    Serial.println("    log csv                 CSV output for spreadsheets");
    Serial.println();
    Serial.println("  Other:");
    Serial.println("    status                  print current state and counts");
    Serial.println("    help                    print this list");
    Serial.println("==============================================================");
    Serial.println();
}
