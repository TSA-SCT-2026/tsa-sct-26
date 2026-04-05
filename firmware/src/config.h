#pragma once

// ================================================================
// Design: queue-fed chamber sorter with 4-index selector chute
// Static sensing in the chamber. Deterministic selector positions.
// Off-axis conveyor stage is modeled by kinematic parameters, not
// by assuming the roller sits directly on the motor shaft.
// ================================================================

// ================================================================
// Pin assignments
// GPIO 34, 35, 36, 39 are input-only and need external pull-ups.
// ================================================================

// Selector chute stepper via TMC2209
#define PIN_STEP            25
#define PIN_DIR             26
#define PIN_ENABLE          27
#define PIN_TMC_UART        23

// Conveyor stepper driver pin map is intentionally not frozen here.
// Keep the conveyor API stable until the final driver wiring is locked.

// Release solenoid via N-channel MOSFET
#define PIN_RELEASE         32

// Chamber sensing
#define PIN_SIZE_BEAM1      36
#define PIN_SIZE_BEAM2      34
#define PIN_ENTRY_BEAM      35
#define PIN_STOP_SW         33
#define PIN_HOME_SW         15
#define PIN_SHELF_LEVEL     39

// Bin confirmation beams
#define PIN_BIN1_BEAM       16
#define PIN_BIN2_BEAM       17
#define PIN_BIN3_BEAM       5
#define PIN_BIN4_BEAM       18

// I2C bus
#define PIN_SDA             21
#define PIN_SCL             22
#define PCF8574_ADDR        0x20

// Operator interface
#define PIN_START_BTN       19
#define PIN_BUZZER          2

// Optional diagnostics
#define PIN_HALL            4

// ================================================================
// Conveyor feed axis kinematics
// ================================================================
#define CONVEYOR_MOTOR_STEPS_PER_REV   200
#define CONVEYOR_MICROSTEPS            16
#define CONVEYOR_STAGE_RATIO_NUM       1
#define CONVEYOR_STAGE_RATIO_DEN       1
#define CONVEYOR_DRIVE_ROLLER_OD_MM    25.0f
#define CONVEYOR_EFFECTIVE_ROLLER_MM   78.54f
#define CONVEYOR_PITCH_ADVANCE_MM      20.0f
#define CONVEYOR_APPROACH_SLOW_ZONE_MM 8.0f
#define CONVEYOR_TARGET_SPEED_MM_S     150.0f
#define CONVEYOR_FEED_SPS              800
#define CONVEYOR_APPROACH_SPS          240
#define CONVEYOR_ACCEL_SPS             80
#define CONVEYOR_HOLD_CURRENT          250

// ================================================================
// Timeouts and dwell windows
// ================================================================
#define FEED_TIMEOUT_MS            5000
#define APPROACH_TIMEOUT_MS         500
#define SETTLE_MS                    50
#define COLOR_TIMEOUT_MS            500
#define SOLENOID_ON_MS               80
#define DROP_WINDOW_MS              400
#define PLATFORM_LEVEL_TIMEOUT_MS   300
#define BIN_CONFIRM_TIMEOUT_MS     1000

// ================================================================
// Sensing
// ================================================================
#define COLOR_SAMPLE_COUNT      8
#define COLOR_MIN_SAMPLES       4
#define COLOR_RED_THRESHOLD     0.42f
#define C_MIN_VALID             100
#define I2C_FREQ_HZ             400000

// ================================================================
// 4-index selector chute
// ================================================================
#define SELECTOR_STEPS_PER_REV      1600
#define SELECTOR_RUN_SPS             400
#define SELECTOR_START_SPS           100
#define SELECTOR_ACCEL                50
#define SELECTOR_DECEL_STEPS          50
#define SELECTOR_HOLD_CURRENT        200
#define RETHOME_PERIOD_BRICKS          8
#define RETHOME_TOLERANCE_STEPS       20

#define SELECTOR_BIN1_STEPS         1400
#define SELECTOR_BIN2_STEPS          200
#define SELECTOR_BIN3_STEPS          600
#define SELECTOR_BIN4_STEPS         1000
#define SELECTOR_HOME_BIN              4

// ================================================================
// Expected counts
// ================================================================
#define EXPECTED_BIN1           6
#define EXPECTED_BIN2           6
#define EXPECTED_BIN3           8
#define EXPECTED_BIN4           4
#define TOTAL_BRICKS           24

// ================================================================
// Thermal model
// ================================================================
#define THERMAL_HEAT_PER_SOL      0.15f
#define THERMAL_HEAT_PER_STEP     0.05f
#define THERMAL_DECAY_RATE        0.5f
#define THERMAL_WARN_LEVEL        0.60f
#define THERMAL_DANGER_LEVEL      0.85f
#define THERMAL_LOG_INTERVAL_MS   1000
#define SELECTOR_SPS_NORMAL       SELECTOR_RUN_SPS
#define SELECTOR_SPS_WARNING      280
#define SELECTOR_SPS_DANGER       180

// ================================================================
// Display
// ================================================================
#define DISPLAY_WIDTH   240
#define DISPLAY_HEIGHT  280

#define TFT_RED     0xF800
#define TFT_BLUE    0x001F
#define TFT_WHITE   0xFFFF
#define TFT_BLACK   0x0000
#define TFT_GREEN   0x07E0
#define TFT_YELLOW  0xFFE0

// ================================================================
// Serial
// ================================================================
#define SERIAL_BAUD  115200
