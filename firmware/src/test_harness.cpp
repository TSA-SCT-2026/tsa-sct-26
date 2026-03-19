#include "test_harness.h"
#include "events.h"
#include "state_machine.h"
#include "classifier.h"
#include "router.h"
#include "thermal.h"
#include "logger.h"
#include "config.h"

// ----------------------------------------------------------------
// Full-run brick sequence: 6x 2x2_BLUE, 6x 2x2_RED, 4x 2x3_RED, 8x 2x3_BLUE
// Interleaved so back-to-back same-plow firings stress the thermal model.
// ----------------------------------------------------------------
static const BrickType FULL_RUN_SEQUENCE[TOTAL_BRICKS] = {
    BrickType::TYPE_2x2_BLUE,  // bin 1
    BrickType::TYPE_2x2_RED,   // bin 2
    BrickType::TYPE_2x3_BLUE,  // bin 4 (default)
    BrickType::TYPE_2x3_RED,   // bin 3
    BrickType::TYPE_2x2_BLUE,
    BrickType::TYPE_2x2_RED,
    BrickType::TYPE_2x3_BLUE,
    BrickType::TYPE_2x2_BLUE,
    BrickType::TYPE_2x3_RED,
    BrickType::TYPE_2x3_BLUE,
    BrickType::TYPE_2x2_RED,
    BrickType::TYPE_2x3_BLUE,
    BrickType::TYPE_2x2_BLUE,
    BrickType::TYPE_2x2_RED,
    BrickType::TYPE_2x3_BLUE,
    BrickType::TYPE_2x2_BLUE,
    BrickType::TYPE_2x2_RED,
    BrickType::TYPE_2x3_RED,
    BrickType::TYPE_2x3_BLUE,
    BrickType::TYPE_2x2_BLUE,
    BrickType::TYPE_2x3_RED,
    BrickType::TYPE_2x3_BLUE,
    BrickType::TYPE_2x2_RED,
    BrickType::TYPE_2x3_BLUE,
};

// Simulated sensor values for each brick type.
// These are placeholder values - real values come from calibration.
// Red:  high R, low G/B  (ratio ~0.87, well above COLOR_RED_RATIO threshold)
// Blue: low R, high B    (ratio ~0.06, well below threshold)
struct SimSensorValues {
    uint16_t r, g, b, c;
    uint32_t gap_us;     // 0 = SIZE_TIMEOUT (2x2)
};

static SimSensorValues sensorsFor(BrickType type) {
    switch (type) {
        case BrickType::TYPE_2x2_BLUE: return {15,  25,  220, 260, 0};       // timeout
        case BrickType::TYPE_2x2_RED:  return {240, 18,  20,  278, 0};       // timeout
        case BrickType::TYPE_2x3_BLUE: return {15,  25,  220, 260, 95000};   // ~95ms gap
        case BrickType::TYPE_2x3_RED:  return {240, 18,  20,  278, 95000};   // ~95ms gap
        default:                       return {0,   0,   0,   0,   0};
    }
}

// Inject all events for one brick and process them through the state machine.
static void simulateBrick(BrickType type) {
    SimSensorValues sv = sensorsFor(type);

    // Beam 1: brick entered sensing zone
    pushEvent(EventType::BEAM1_BREAK);
    Event e;
    while (gEventQueue.pop(e)) gStateMachine.process(e);

    // Size detection
    if (sv.gap_us > 0) {
        pushEventGap(sv.gap_us);
    } else {
        pushEvent(EventType::SIZE_TIMEOUT);
    }
    while (gEventQueue.pop(e)) gStateMachine.process(e);

    // Color samples (simulate ~10 valid samples per brick)
    for (int i = 0; i < 10; i++) {
        pushEventColor(sv.r, sv.g, sv.b, sv.c);
        while (gEventQueue.pop(e)) gStateMachine.process(e);
    }

    // Color done: dwell window ended
    pushEvent(EventType::COLOR_DONE);
    while (gEventQueue.pop(e)) gStateMachine.process(e);

    // Bin confirmation for the expected bin
    uint8_t bin = targetBin(type);
    EventType binEvt = (EventType)((uint8_t)EventType::BIN1_CONFIRM + (bin - 1));
    pushEvent(binEvt);
    while (gEventQueue.pop(e)) gStateMachine.process(e);

    gThermal.update();
}

// ----------------------------------------------------------------
// Command parsing
// ----------------------------------------------------------------

static char  _lineBuf[64];
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

    } else if (strcmp(args, "beam1") == 0) {
        pushEvent(EventType::BEAM1_BREAK);

    } else if (strncmp(args, "beam2 ", 6) == 0) {
        uint32_t gap = (uint32_t)atol(args + 6);
        pushEventGap(gap);

    } else if (strcmp(args, "timeout") == 0) {
        pushEvent(EventType::SIZE_TIMEOUT);

    } else if (strncmp(args, "color ", 6) == 0) {
        uint16_t r, g, b, c;
        if (sscanf(args + 6, "%hu %hu %hu %hu", &r, &g, &b, &c) == 4) {
            pushEventColor(r, g, b, c);
        } else {
            Serial.println("[harness] usage: sim color <r> <g> <b> <c>");
        }

    } else if (strcmp(args, "colordone") == 0) {
        pushEvent(EventType::COLOR_DONE);

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

    if (strncmp(args, "classify ", 9) == 0) {
        uint16_t r, g, b, c;
        uint32_t gap;
        int parsed = sscanf(args + 9, "%hu %hu %hu %hu %lu", &r, &g, &b, &c, &gap);
        if (parsed < 4) {
            Serial.println("[harness] usage: test classify <r> <g> <b> <c> [gap_us]");
            return;
        }
        if (parsed < 5) gap = 0;

        BrickSize  sz    = classifySize(gap);
        float      ratio = colorRatio(r, g, b);
        bool       valid = isValidColorSample(r, g, b, c);
        BrickColor col   = valid ? classifyColor(r, g, b, 1) : BrickColor::UNKNOWN;
        BrickType  type  = classifyBrick(sz, col);
        uint8_t    plow  = routePlow(type);
        uint8_t    bin   = targetBin(type);

        Serial.println();
        Serial.println("--- classify result ----------------------------");
        Serial.printf("  size        : %s  (gap_us=%lu, threshold=%u)\n",
                       sizeName(sz), gap, SIZE_THRESHOLD_US);
        Serial.printf("  color_ratio : %.3f  (threshold=%.3f -> %s = %s)\n",
                       ratio, COLOR_RED_RATIO,
                       ratio >= COLOR_RED_RATIO ? "above" : "below",
                       ratio >= COLOR_RED_RATIO ? "RED" : "BLUE");
        Serial.printf("  sample_valid: %s  (floor=%d, total=%d)\n",
                       valid ? "YES" : "NO (bare belt)", COLOR_BLACK_FLOOR, r+g+b+c);
        Serial.printf("  brick_type  : %s\n", typeName(type));
        Serial.printf("  plow        : %s\n", plow ? String(plow).c_str() : "none (default)");
        Serial.printf("  bin         : %d\n", bin);
        Serial.println("------------------------------------------------");
        Serial.println();

    } else if (strncmp(args, "route ", 6) == 0) {
        // test route <size> <color>  e.g. "test route 2x3 RED"
        char szStr[8], colStr[8];
        if (sscanf(args + 6, "%7s %7s", szStr, colStr) != 2) {
            Serial.println("[harness] usage: test route <2x2|2x3> <RED|BLUE>");
            return;
        }
        BrickSize sz = (strcmp(szStr, "2x3") == 0) ? BrickSize::SIZE_2x3 : BrickSize::SIZE_2x2;
        BrickColor col = (strcmp(colStr, "RED") == 0) ? BrickColor::RED : BrickColor::BLUE;
        BrickType type = classifyBrick(sz, col);
        Serial.printf("  %s %s -> type=%s  plow=%d  bin=%d\n",
                       sizeName(sz), colorName(col), typeName(type),
                       routePlow(type), targetBin(type));

    } else if (strncmp(args, "thermal", 7) == 0) {
        int n = 6;
        if (strlen(args) > 8) n = atoi(args + 8);

        Serial.println();
        Serial.println("--- thermal model evolution --------------------");
        Serial.printf("  Firing all 3 solenoids + stepper %d times each:\n", n);
        Serial.println("  fire#   sol1    sol2    sol3    step    state");

        ThermalModel sim;  // local instance, does not affect gThermal
        for (int i = 1; i <= n; i++) {
            sim.onSolenoidFire(1);
            sim.onSolenoidFire(2);
            sim.onSolenoidFire(3);
            sim.onStepperRelease();
            sim.update();
            Serial.printf("  %-6d  %.3f   %.3f   %.3f   %.3f   %s\n",
                           i, sim.solenoidHeat(1), sim.solenoidHeat(2),
                           sim.solenoidHeat(3), sim.stepperHeat(), sim.stateName());
        }
        Serial.println("  (decaying for 5 simulated seconds...)");
        // Simulate 5 seconds of decay by calling update repeatedly
        // We can't actually wait, so we manually apply the factor
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

        if (gStateMachine.currentState() != State::FEEDING) {
            Serial.println("[harness] ERROR: state machine did not enter FEEDING after start");
            return;
        }

        for (uint8_t i = 0; i < TOTAL_BRICKS; i++) {
            simulateBrick(FULL_RUN_SEQUENCE[i]);

            // If we errored out mid-run, stop
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
    Serial.printf("  bin1 (2x2B) : %d  (expected %d)\n", gStateMachine.binCount(1), EXPECTED_BIN1);
    Serial.printf("  bin2 (2x2R) : %d  (expected %d)\n", gStateMachine.binCount(2), EXPECTED_BIN2);
    Serial.printf("  bin3 (2x3R) : %d  (expected %d)\n", gStateMachine.binCount(3), EXPECTED_BIN3);
    Serial.printf("  bin4 (2x3B) : %d  (expected %d)\n", gStateMachine.binCount(4), EXPECTED_BIN4);
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
    Serial.println("    sim start               inject START_BUTTON");
    Serial.println("    sim beam1               inject BEAM1_BREAK");
    Serial.println("    sim beam2 <gap_us>      inject BEAM2_BREAK (2x3 gap ~95000)");
    Serial.println("    sim timeout             inject SIZE_TIMEOUT (2x2)");
    Serial.println("    sim color <r> <g> <b> <c>  inject COLOR_SAMPLE");
    Serial.println("    sim colordone           inject COLOR_DONE");
    Serial.println("    sim bin <1-4>           inject BIN_CONFIRM");
    Serial.println("    sim jam                 inject CONFIRM_TIMEOUT");
    Serial.println("    sim reset               inject RESET");
    Serial.println();
    Serial.println("  Run tests:");
    Serial.println("    test classify <r> <g> <b> <c> [gap_us]");
    Serial.println("    test route <2x2|2x3> <RED|BLUE>");
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
