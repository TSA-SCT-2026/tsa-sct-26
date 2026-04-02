#include "actuators.h"
#include "logger.h"
#include "config.h"

// STUBS: All functions log what they would do. Replace with real hardware
// calls when electronics are connected. The state machine calls these
// identically whether stubs or real - no changes required above this layer.

namespace actuators {

// Internal state tracking
static int16_t current_position_steps = 0;  // current disc position

void begin() {
    gLogger.info("STUB actuators::begin - pins not configured yet");
    // Real: pinMode for all GPIO, set TB6612FNG STBY HIGH, TMC2209 UART init
}

void beltStart() {
    gLogger.info("STUB belt START (TB6612FNG)");
    // Real: digitalWrite(PIN_MOTOR_STBY, HIGH), set direction, analogWrite PWM
}

void beltStop() {
    gLogger.info("STUB belt STOP");
    // Real: analogWrite(PIN_MOTOR_ENA, 0), optional STBY LOW for power save
}

void beltSetDuty(uint8_t duty) {
    char buf[48];
    snprintf(buf, sizeof(buf), "STUB belt duty=%d", duty);
    gLogger.info(buf);
    // Real: analogWrite(PIN_MOTOR_ENA, duty)
}

void firePlatformRelease() {
    char buf[64];
    snprintf(buf, sizeof(buf), "STUB firePlatformRelease on_ms=%d", SOLENOID_ON_MS);
    gLogger.info(buf);
    // Real:
    // digitalWrite(PIN_RELEASE, HIGH);
    // delay(SOLENOID_ON_MS);  // 80ms
    // digitalWrite(PIN_RELEASE, LOW);
    // Class 3 lever sweeps outward, platform drops, brick falls
}

bool homeDisc() {
    gLogger.info("STUB homeDisc - run CW until PIN_HOME_SW LOW");
    // Real:
    // digitalWrite(PIN_ENABLE, LOW);  // enable stepper
    // digitalWrite(PIN_DIR, HIGH);     // set CW
    // while (digitalRead(PIN_HOME_SW) == HIGH) {
    //     digitalWrite(PIN_STEP, HIGH);
    //     delayMicroseconds(1000000 / STEPPER_START_SPS / 2);
    //     digitalWrite(PIN_STEP, LOW);
    //     delayMicroseconds(1000000 / STEPPER_START_SPS / 2);
    //     // timeout check
    // }
    // current_position_steps = BIN4_STEPS;  // 225 deg home position
    // Set TMC2209 hold current to STEPPER_HOLD_CURRENT via UART
    current_position_steps = BIN4_STEPS;
    gLogger.info("STUB homeDisc SUCCESS - position set to BIN4_STEPS (1000)");
    return true;
}

bool indexToBin(uint8_t bin_index) {
    char buf[96];
    
    // Get target steps from bin_index
    int16_t target_steps;
    switch(bin_index) {
        case 1: target_steps = BIN1_STEPS; break;  // 1400
        case 2: target_steps = BIN2_STEPS; break;  // 200
        case 3: target_steps = BIN3_STEPS; break;  // 600
        case 4: target_steps = BIN4_STEPS; break;  // 1000
        default:
            gLogger.info("STUB indexToBin INVALID bin_index");
            return false;
    }
    
    snprintf(buf, sizeof(buf), 
             "STUB indexToBin bin=%d target=%d current=%d", 
             bin_index, target_steps, current_position_steps);
    gLogger.info(buf);
    
    // Real: Compute shortest arc, set DIR, ramp profile
    // int16_t delta = target_steps - current_position_steps;
    // if (delta > STEPPER_STEPS_PER_REV / 2) delta -= STEPPER_STEPS_PER_REV;
    // if (delta < -STEPPER_STEPS_PER_REV / 2) delta += STEPPER_STEPS_PER_REV;
    // digitalWrite(PIN_DIR, delta > 0 ? HIGH : LOW);
    // 
    // Ramp: start at STEPPER_START_SPS, accel to STEPPER_RUN_SPS
    // Run at STEPPER_RUN_SPS
    // Decel: STEPPER_DECEL_STEPS before target, ramp down to STEPPER_START_SPS
    // Final steps at STEPPER_START_SPS
    // Set hold current via TMC2209 UART
    // Read SGRESULT via UART for StallGuard detection
    // if (stall_detected) return false;
    
    current_position_steps = target_steps;
    gLogger.info("STUB indexToBin SUCCESS - StallGuard clear");
    return true;
}

bool reHomeCheck() {
    gLogger.info("STUB reHomeCheck - rotate to home by step count, verify PIN_HOME_SW");
    // Real:
    // Compute steps to home position BIN4_STEPS
    // Rotate using step count
    // Verify PIN_HOME_SW triggers within RETHOME_TOLERANCE_STEPS
    // if (abs(expected_steps - actual_steps) > RETHOME_TOLERANCE_STEPS) {
    //     return false;  // POSITION_DRIFT error
    // }
    // current_position_steps = BIN4_STEPS;
    gLogger.info("STUB reHomeCheck SUCCESS - position verified");
    return true;
}

void displayReady() {
    gLogger.info("STUB display READY screen (LOAD 24 BRICKS / PRESS START)");
}

void displaySorting(uint8_t brickNum, uint8_t total, uint8_t targetBin, 
                    const uint8_t binCounts[4]) {
    char buf[96];
    const char* discPos = "UNKNOWN";
    switch (targetBin) {
        case 1: discPos = "NW (315°)"; break;
        case 2: discPos = "NE (45°)"; break;
        case 3: discPos = "SE (135°)"; break;
        case 4: discPos = "SW (225°)"; break;
    }
    snprintf(buf, sizeof(buf), 
             "STUB display SORTING brick=%d/%d disc=%s bins=[%d,%d,%d,%d]",
             brickNum, total, discPos, 
             binCounts[0], binCounts[1], binCounts[2], binCounts[3]);
    gLogger.info(buf);
}

void displayComplete(uint32_t totalMs, const uint8_t binCounts[4]) {
    char buf[80];
    snprintf(buf, sizeof(buf),
             "STUB display COMPLETE time=%lums bins=[%d,%d,%d,%d]",
             totalMs, binCounts[0], binCounts[1], binCounts[2], binCounts[3]);
    gLogger.info(buf);
}

void displayError(uint8_t brickNum, uint8_t expectedBin, const char* errorCode) {
    char buf[64];
    snprintf(buf, sizeof(buf), "STUB display ERROR brick=%d bin=%d code=%s",
             brickNum, expectedBin, errorCode);
    gLogger.info(buf);
}

}  // namespace actuators
