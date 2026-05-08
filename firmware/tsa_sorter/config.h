#pragma once

// Conveyor feed, conveyor-mounted sensing station, and
// MG995/MG996-class servo rotary chute selector.

// Pin assignments
// GPIO 34, 35, 36, and 39 are input-only and need external pull-ups.

// Conveyor stepper driver pin map is intentionally not frozen here.
// Keep the conveyor API stable until the final driver wiring is locked.
#define PIN_CONVEYOR_STEP    25
#define PIN_CONVEYOR_DIR     27
#define PIN_CONVEYOR_ENABLE  26
#define PIN_TMC_UART         23

// Servo chute selector
#define PIN_SELECTOR_SERVO   32

// Sensing station
#define PIN_SIZE_SENSOR_A    33
#define PIN_SIZE_SENSOR_B    15
#define PIN_COLOR_OUT        34
#define PIN_COLOR_S2         17
#define PIN_COLOR_S3         18
// TCS3200 S0 is tied to 3V3, S1 is tied to GND, and OE is tied to GND in hardware.

// I2C bus
#define PIN_SDA              21
#define PIN_SCL              22
#define PCF8574_ADDR         0x20

// Operator interface
#define PIN_START_BTN        19
#define PIN_BUZZER           14

// Conveyor
// Fallback belt speed for length calculation when beam B transit timing is unavailable.
// Tune this by running a known 2x3 brick and adjusting until len ~ 22mm.
#define CONVEYOR_SENSING_FALLBACK_SPEED_MM_S  230.0f
#define CONVEYOR_MOTOR_STEPS_PER_REV   200
#define CONVEYOR_MICROSTEPS            16
#define CONVEYOR_TARGET_SPEED_MM_S     120.0f
// Speed modes:
// balanced: default two-speed profile for normal operation
// aggressive: highest throughput profile intended to remain accurate
// accuracy: conservative profile with maximum accuracy margin
#define CONVEYOR_BALANCED_FAST_SPS     7000UL
#define CONVEYOR_BALANCED_SLOW_SPS     2100UL
#define CONVEYOR_AGGRESSIVE_FAST_SPS   8500UL
#define CONVEYOR_AGGRESSIVE_SLOW_SPS   2100UL
#define CONVEYOR_ACCURACY_FAST_SPS     5000UL
#define CONVEYOR_ACCURACY_SLOW_SPS     1500UL
// 0=balanced, 1=aggressive, 2=accuracy
#define CONVEYOR_DEFAULT_MODE_ID       0
#define CONVEYOR_FEED_SPS              CONVEYOR_BALANCED_FAST_SPS
#define CONVEYOR_MIN_SPACING_SPS       1200UL
#define CONVEYOR_SPACING_RECOVERY_MARGIN_MS  5
#define CONVEYOR_FORWARD_DIR_LEVEL     HIGH
#define CONVEYOR_REVERSE_DIR_LEVEL     LOW
#define CONVEYOR_RETRY_REVERSE_SPS     2600UL
#define CONVEYOR_RETRY_REVERSE_MS      450
#define SENSING_RETRY_LIMIT            0
#define CONVEYOR_ACCEL_SPS             80
#define CONVEYOR_HOLD_CURRENT          250

// Timeouts and dwell windows
#define FEED_TIMEOUT_MS          5000
#define SENSING_TIMEOUT_MS        600
#define SERVO_SETTLE_MS           250
#define BOOT_POWER_SETTLE_MS     3000
#define AUTO_START_DELAY_MS      2000
#define STARTUP_PIN_CLAMP_MS      200
#define SERVO_STARTUP_ATTACH_DELAY_MS  1200
#define CONVEYOR_FIRST_START_RAMP_MS    600
#define HANDOFF_WINDOW_MS        1500
#define TIMED_CONFIRM_WINDOW_MS    20
#define BIN_CONFIRM_TIMEOUT_MS   1200
// Route timing from real sensing data:
// beam A detect -> beam B center spacing -> measured distance from shroud to servo.
#define ROUTE_DISTANCE_BEAM_B_TO_SERVO_MM   50.0f
#define ROUTE_SWITCH_MARGIN_MS                15
#define ROUTE_FALLBACK_2X2_LENGTH_MM       16.0f
#define ROUTE_FALLBACK_2X3_LENGTH_MM       24.0f

// Sensing
#define COLOR_SAMPLE_COUNT      8
#define COLOR_MIN_SAMPLES       4
// Preliminary shroud-installed static calibration on May 1, 2026:
// red cluster 0.5551 to 0.5561, blue cluster 0.5257 to 0.5260.
// Validate against moving-brick passes before treating this as final.
// Moving-pass calibration: blue ~0.467, red ~0.512. Midpoint 0.489.
#define COLOR_RED_THRESHOLD     0.490f
#define COLOR_PASS_SAMPLE_INTERVAL_US   70000UL
#define COLOR_PASS_MIN_SAMPLES          3
#define COLOR_BELT_MIN_NET_SUM_HZ       0.5f
#define COLOR_BELT_MIN_NET_CLEAR_HZ     0.0f
#define COLOR_MIN_REASONABLE_RATIO      0.05f
#define COLOR_MAX_REASONABLE_RATIO      0.95f
#define C_MIN_VALID             100
#define I2C_FREQ_HZ             400000

// Initial thresholds for two-pair break-beam timing. Calibrate with shroud installed.
#define SIZE_SENSOR_TIMEOUT_MS       80
#define SIZE_TWO_BY_THREE_MIN_UNITS  20
// Preliminary moving-pass size calibration on May 1, 2026:
// clean 2x2 lengthAvg cluster mean 13.523 mm, clean 2x3 lengthAvg cluster mean 22.459 mm.
// Midpoint threshold 17.991 mm. Exclude high-skew outliers when recalibrating.
#define SIZE_BEAM_CENTER_SPACING_MM     40.436f
#define SIZE_TWO_BY_THREE_MIN_LENGTH_MM  18.0f
#define SIZE_CAL_REJECT_SPEED_SKEW_PCT   8.0f
#define SIZE_CAL_REJECT_LENGTH_SKEW_PCT  12.0f
#define SIZE_MIN_REASONABLE_LENGTH_MM     8.0f
#define SIZE_MAX_REASONABLE_LENGTH_MM    35.0f

// Servo chute selector
#define SELECTOR_BIN1_ANGLE_DEG   60
#define SELECTOR_BIN2_ANGLE_DEG   90
#define SELECTOR_BIN3_ANGLE_DEG   130
#define SELECTOR_BIN4_ANGLE_DEG   155
#define SELECTOR_HOME_BIN         1
#define SELECTOR_DEFAULT_BIN      1
#define SELECTOR_UNLOAD_ANGLE_DEG 90
#define SELECTOR_JIGGLE_ENABLED   1
#define SELECTOR_JIGGLE_AMPLITUDE_DEG  5
#define SELECTOR_JIGGLE_STEP_MS        45
#define SELECTOR_JIGGLE_PULSES         4
#define SELECTOR_JIGGLE_TIME_MS        (SELECTOR_JIGGLE_ENABLED ? ((SELECTOR_JIGGLE_PULSES + 1) * SELECTOR_JIGGLE_STEP_MS) : 0)
#define SELECTOR_ROUTE_READY_MS        (SERVO_SETTLE_MS + SELECTOR_JIGGLE_TIME_MS)

// Expected counts: BIN1=2x2_RED(6), BIN2=2x3_RED(4), BIN3=2x3_BLUE(8), BIN4=2x2_BLUE(6)
#define EXPECTED_BIN1           6
#define EXPECTED_BIN2           4
#define EXPECTED_BIN3           8
#define EXPECTED_BIN4           6
#define TOTAL_BRICKS           24

// Thermal and power watch model
#define THERMAL_HEAT_PER_SERVO_MOVE  0.05f
#define THERMAL_DECAY_RATE           0.5f
#define THERMAL_WARN_LEVEL           0.60f
#define THERMAL_DANGER_LEVEL         0.85f
#define THERMAL_LOG_INTERVAL_MS      1000

// Display
#define DISPLAY_WIDTH   240
#define DISPLAY_HEIGHT  320

#define TFT_RED     0xF800
#define TFT_BLUE    0x001F
#define TFT_WHITE   0xFFFF
#define TFT_BLACK   0x0000
#define TFT_GREEN   0x07E0
#define TFT_YELLOW  0xFFE0

#define SERIAL_BAUD  115200
