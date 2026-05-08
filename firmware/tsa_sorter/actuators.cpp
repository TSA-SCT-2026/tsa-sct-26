#include "actuators.h"
#include "config.h"
#include "logger.h"
#include "thermal.h"

#include <Arduino.h>
#include <ESP32Servo.h>
#include <esp_arduino_version.h>

namespace actuators {

// Stepper using LEDC hardware PWM with no ISR.
// PlatformIO uses the ESP32 Arduino core channel-based LEDC API here.
static constexpr uint8_t  STEP_LEDC_CHANNEL  = 4;
static constexpr uint8_t  STEP_LEDC_RES_BITS = 8;
static constexpr uint32_t STEP_LEDC_DUTY_50  = 128;  // 50% duty at 8-bit

static void setupStepPwm(uint32_t stepRateSps) {
#if ESP_ARDUINO_VERSION_MAJOR >= 3
    ledcAttach(PIN_CONVEYOR_STEP, stepRateSps, STEP_LEDC_RES_BITS);
#else
    ledcSetup(STEP_LEDC_CHANNEL, stepRateSps, STEP_LEDC_RES_BITS);
    ledcAttachPin(PIN_CONVEYOR_STEP, STEP_LEDC_CHANNEL);
#endif
}

static void writeStepPwm(uint32_t duty) {
#if ESP_ARDUINO_VERSION_MAJOR >= 3
    ledcWrite(PIN_CONVEYOR_STEP, duty);
#else
    ledcWrite(STEP_LEDC_CHANNEL, duty);
#endif
}

// Servo state.
static Servo gServo;
static bool gSelectorJiggleActive = false;
static bool gSelectorJigglePositive = true;
static uint16_t gSelectorJiggleTarget = SELECTOR_UNLOAD_ANGLE_DEG;
static uint32_t gSelectorJiggleNextMs = 0;
static bool gFirstConveyorStart = true;
static ConveyorSpeedMode gConveyorMode = ConveyorSpeedMode::BALANCED;
static uint32_t gConveyorFastSps = CONVEYOR_BALANCED_FAST_SPS;
static uint32_t gConveyorSlowSps = CONVEYOR_BALANCED_SLOW_SPS;

static uint16_t clampServoAngle(int angle) {
    if (angle < 0) return 0;
    if (angle > 180) return 180;
    return (uint16_t)angle;
}

static void applyConveyorSpeedMode(ConveyorSpeedMode mode) {
    gConveyorMode = mode;
    switch (mode) {
        case ConveyorSpeedMode::BALANCED:
            gConveyorFastSps = CONVEYOR_BALANCED_FAST_SPS;
            gConveyorSlowSps = CONVEYOR_BALANCED_SLOW_SPS;
            break;
        case ConveyorSpeedMode::AGGRESSIVE:
            gConveyorFastSps = CONVEYOR_AGGRESSIVE_FAST_SPS;
            gConveyorSlowSps = CONVEYOR_AGGRESSIVE_SLOW_SPS;
            break;
        case ConveyorSpeedMode::ACCURACY:
            gConveyorFastSps = CONVEYOR_ACCURACY_FAST_SPS;
            gConveyorSlowSps = CONVEYOR_ACCURACY_SLOW_SPS;
            break;
    }
}

static void startSelectorJiggle(uint16_t targetAngle) {
#if SELECTOR_JIGGLE_ENABLED
    const uint16_t amplitude = SELECTOR_JIGGLE_AMPLITUDE_DEG;
    if (amplitude == 0) return;

    gSelectorJiggleTarget = targetAngle;
    gSelectorJigglePositive = true;
    gSelectorJiggleNextMs = millis();
    gSelectorJiggleActive = true;
#else
    (void)targetAngle;
#endif
}

static void serviceSelectorJiggle() {
#if SELECTOR_JIGGLE_ENABLED
    if (!gSelectorJiggleActive) return;
    uint32_t now = millis();
    if ((int32_t)(now - gSelectorJiggleNextMs) < 0) return;

    int direction = gSelectorJigglePositive ? 1 : -1;
    gServo.write(clampServoAngle((int)gSelectorJiggleTarget +
                                 direction * (int)SELECTOR_JIGGLE_AMPLITUDE_DEG));
    gThermal.onServoMove();
    gSelectorJigglePositive = !gSelectorJigglePositive;
    gSelectorJiggleNextMs = now + SELECTOR_JIGGLE_STEP_MS;
#endif
}

// Public helpers.

uint16_t servoAngleForBin(uint8_t binIdx) {
    switch (binIdx) {
        case 1: return SELECTOR_BIN1_ANGLE_DEG;
        case 2: return SELECTOR_BIN2_ANGLE_DEG;
        case 3: return SELECTOR_BIN3_ANGLE_DEG;
        case 4: return SELECTOR_BIN4_ANGLE_DEG;
        default: return SELECTOR_BIN3_ANGLE_DEG;
    }
}

const char* selectorPositionLabel(uint8_t binIdx) {
    switch (binIdx) {
        case 1: return "BIN1_2x2_RED";
        case 2: return "BIN2_2x3_RED";
        case 3: return "BIN3_2x3_BLUE";
        case 4: return "BIN4_2x2_BLUE";
        default: return "UNKNOWN";
    }
}

// Init.

static void ensureServoAttached() {
    if (gServo.attached()) return;
    // Timers must be allocated before attach so the stepper PWM cannot corrupt
    // the servo 50Hz signal.
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    gServo.attach(PIN_SELECTOR_SERVO);
}

void begin() {
    switch (CONVEYOR_DEFAULT_MODE_ID) {
        case 1: applyConveyorSpeedMode(ConveyorSpeedMode::AGGRESSIVE); break;
        case 2: applyConveyorSpeedMode(ConveyorSpeedMode::ACCURACY); break;
        default: applyConveyorSpeedMode(ConveyorSpeedMode::BALANCED); break;
    }

    // Servo must attach first to physically claim LEDC timers 0 and 1.
    // The stepper uses an explicit LEDC channel so the servo PWM setup stays
    // isolated from the conveyor step pulse generator.
    pinMode(PIN_SELECTOR_SERVO, OUTPUT);
    digitalWrite(PIN_SELECTOR_SERVO, LOW);
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    delay(SERVO_STARTUP_ATTACH_DELAY_MS);
    gServo.attach(PIN_SELECTOR_SERVO);
    // No write here. Hold movement until first brick is routed.

    // Stepper
    pinMode(PIN_CONVEYOR_ENABLE, OUTPUT);
    digitalWrite(PIN_CONVEYOR_ENABLE, HIGH);  // disabled (active-low)
    setupStepPwm(CONVEYOR_FEED_SPS);
    writeStepPwm(0);

    // Buzzer
    pinMode(PIN_BUZZER, OUTPUT);
    digitalWrite(PIN_BUZZER, LOW);

    gLogger.info("actuators: ready");
    char buf[96];
    snprintf(buf, sizeof(buf), "conveyor: mode=%s fast_sps=%lu slow_sps=%lu",
             conveyorSpeedModeName(gConveyorMode),
             (unsigned long)gConveyorFastSps,
             (unsigned long)gConveyorSlowSps);
    gLogger.info(buf);
}

void poll() {
    serviceSelectorJiggle();
}

// Conveyor.

void startConveyorFeed() {
    digitalWrite(PIN_CONVEYOR_DIR, CONVEYOR_FORWARD_DIR_LEVEL);
    digitalWrite(PIN_CONVEYOR_ENABLE, LOW);   // enable driver (active-low)
    if (gFirstConveyorStart) {
        setupStepPwm(gConveyorSlowSps);
        writeStepPwm(STEP_LEDC_DUTY_50);
        delay(CONVEYOR_FIRST_START_RAMP_MS);
        gFirstConveyorStart = false;
        gLogger.info("conveyor: first-start ramp complete");
    }
    setupStepPwm(gConveyorFastSps);
    writeStepPwm(STEP_LEDC_DUTY_50);
    char buf[72];
    snprintf(buf, sizeof(buf), "conveyor: feed started mode=%s fast_sps=%lu",
             conveyorSpeedModeName(gConveyorMode),
             (unsigned long)gConveyorFastSps);
    gLogger.info(buf);
}

void stopConveyorFeed() {
    writeStepPwm(0);
    digitalWrite(PIN_CONVEYOR_ENABLE, HIGH);  // disable driver (hold off)
    gLogger.info("conveyor: feed stopped");
}

void reverseConveyorForRetry() {
    writeStepPwm(0);
    digitalWrite(PIN_CONVEYOR_DIR, CONVEYOR_REVERSE_DIR_LEVEL);
    digitalWrite(PIN_CONVEYOR_ENABLE, LOW);
    setupStepPwm(CONVEYOR_RETRY_REVERSE_SPS);
    writeStepPwm(STEP_LEDC_DUTY_50);
    delay(CONVEYOR_RETRY_REVERSE_MS);
    writeStepPwm(0);
    digitalWrite(PIN_CONVEYOR_DIR, CONVEYOR_FORWARD_DIR_LEVEL);
    gLogger.info("conveyor: retry reverse complete");
}

void setConveyorSlow() {
    setupStepPwm(gConveyorSlowSps);
    writeStepPwm(STEP_LEDC_DUTY_50);
    char buf[64];
    snprintf(buf, sizeof(buf), "conveyor: slow mode=%s sps=%lu",
             conveyorSpeedModeName(gConveyorMode),
             (unsigned long)gConveyorSlowSps);
    gLogger.info(buf);
}

void setConveyorFast() {
    setupStepPwm(gConveyorFastSps);
    writeStepPwm(STEP_LEDC_DUTY_50);
    char buf[64];
    snprintf(buf, sizeof(buf), "conveyor: fast mode=%s sps=%lu",
             conveyorSpeedModeName(gConveyorMode),
             (unsigned long)gConveyorFastSps);
    gLogger.info(buf);
}

void setConveyorStepRate(uint32_t stepRateSps, const char* reason) {
    setupStepPwm(stepRateSps);
    writeStepPwm(STEP_LEDC_DUTY_50);
    char buf[80];
    snprintf(buf, sizeof(buf), "conveyor: %s sps=%lu",
             reason != nullptr ? reason : "custom",
             (unsigned long)stepRateSps);
    gLogger.info(buf);
}

void setConveyorSpeedMode(ConveyorSpeedMode mode) {
    applyConveyorSpeedMode(mode);
    char buf[96];
    snprintf(buf, sizeof(buf), "conveyor: mode set to %s fast_sps=%lu slow_sps=%lu",
             conveyorSpeedModeName(gConveyorMode),
             (unsigned long)gConveyorFastSps,
             (unsigned long)gConveyorSlowSps);
    gLogger.info(buf);
}

ConveyorSpeedMode conveyorSpeedMode() {
    return gConveyorMode;
}

const char* conveyorSpeedModeName(ConveyorSpeedMode mode) {
    switch (mode) {
        case ConveyorSpeedMode::BALANCED: return "balanced";
        case ConveyorSpeedMode::AGGRESSIVE: return "aggressive";
        case ConveyorSpeedMode::ACCURACY: return "accuracy";
        default: return "unknown";
    }
}

uint32_t conveyorFastSps() {
    return gConveyorFastSps;
}

uint32_t conveyorSlowSps() {
    return gConveyorSlowSps;
}

void tuneConveyorProfile(uint8_t profile) {
    if (profile == 0) {
        setConveyorSpeedMode(ConveyorSpeedMode::BALANCED);
    } else if (profile == 1) {
        setConveyorSpeedMode(ConveyorSpeedMode::AGGRESSIVE);
    } else if (profile == 2) {
        setConveyorSpeedMode(ConveyorSpeedMode::ACCURACY);
    } else {
        char buf[64];
        snprintf(buf, sizeof(buf), "conveyor: invalid profile=%u", profile);
        gLogger.info(buf);
    }
}

// Selector servo.

bool homeSelector() {
    ensureServoAttached();
    stopSelectorJiggle();
    uint16_t angle = servoAngleForBin(SELECTOR_DEFAULT_BIN);
    gServo.write(angle);
    delay(SERVO_SETTLE_MS);
    char buf[48];
    snprintf(buf, sizeof(buf), "servo: home -> bin%u (%u deg)", SELECTOR_DEFAULT_BIN, angle);
    gLogger.info(buf);
    return true;
}

bool routeServoToBin(uint8_t binIdx) {
    if (binIdx < 1 || binIdx > 4) {
        gLogger.info("servo: invalid bin");
        return false;
    }
    ensureServoAttached();
    uint16_t angle = servoAngleForBin(binIdx);
    gServo.write(angle);
    delay(SERVO_SETTLE_MS);
    startSelectorJiggle(angle);
    char buf[48];
    snprintf(buf, sizeof(buf), "servo: bin%u -> %u deg jiggle=%u", binIdx, angle,
             SELECTOR_JIGGLE_ENABLED ? 1 : 0);
    gLogger.info(buf);
    return true;
}

void stopSelectorJiggle() {
#if SELECTOR_JIGGLE_ENABLED
    if (!gSelectorJiggleActive) return;
    gSelectorJiggleActive = false;
    if (gServo.attached()) {
        gServo.write(gSelectorJiggleTarget);
    }
#endif
}

void stopSelector() {
    stopSelectorJiggle();
    // Servo holds position; nothing to stop.
}

void unloadPosition() {
    ensureServoAttached();
    stopSelectorJiggle();
    gServo.write(SELECTOR_UNLOAD_ANGLE_DEG);
    delay(SERVO_SETTLE_MS);
    gLogger.info("servo: unload position (90 deg)");
}

void readySelector() {
    ensureServoAttached();
    stopSelectorJiggle();
    gServo.write(servoAngleForBin(SELECTOR_DEFAULT_BIN));
}

// Buzzer.

void buzzerError() {
    for (int i = 0; i < 3; i++) {
        digitalWrite(PIN_BUZZER, HIGH);
        delay(150);
        digitalWrite(PIN_BUZZER, LOW);
        delay(150);
    }
}

// Display is log-only until a display is connected.

void displayReady() {
    gLogger.info("state: READY");
}

void displaySorting(uint16_t brickNum, uint16_t total, uint8_t targetBin,
                    const uint16_t binCounts[4]) {
    (void)binCounts;
    char buf[64];
    snprintf(buf, sizeof(buf), "state: SORTING brick=%u/%u bin=%u", brickNum, total, targetBin);
    gLogger.info(buf);
}

void displayComplete(uint32_t totalMs, const uint16_t binCounts[4]) {
    (void)binCounts;
    char buf[64];
    snprintf(buf, sizeof(buf), "state: COMPLETE time_ms=%lu", totalMs);
    gLogger.info(buf);
}

void displayError(uint16_t brickNum, uint8_t expectedBin, const char* errorCode) {
    char buf[64];
    snprintf(buf, sizeof(buf), "state: ERROR brick=%u bin=%u code=%s", brickNum, expectedBin, errorCode);
    gLogger.info(buf);
}

}  // namespace actuators
