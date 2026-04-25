#pragma once

// States build: one-at-a-time conveyor feed, conveyor-mounted sensing station,
// and MG995/MG996-class servo rotary chute selector.

// Pin assignments
// GPIO 35, 36, and 39 are input-only and need external pull-ups.

// Conveyor stepper driver pin map is intentionally not frozen here.
// Keep the conveyor API stable until the final driver wiring is locked.
#define PIN_CONVEYOR_STEP    25
#define PIN_CONVEYOR_DIR     26
#define PIN_CONVEYOR_ENABLE  27
#define PIN_TMC_UART         23

// Servo chute selector
#define PIN_SELECTOR_SERVO   32

// Sensing station
#define PIN_BRICK_DETECT     35
#define PIN_SIZE_SENSOR_A    33
#define PIN_SIZE_SENSOR_B    15

// Bin confirmation beams are deferred for states and reserved for future instrumentation.
#define PIN_BIN1_BEAM        16
#define PIN_BIN2_BEAM        17
#define PIN_BIN3_BEAM        5
#define PIN_BIN4_BEAM        18

// I2C bus
#define PIN_SDA              21
#define PIN_SCL              22
#define PCF8574_ADDR         0x20

// Operator interface
#define PIN_START_BTN        19
#define PIN_BUZZER           2

// Optional diagnostics
#define PIN_HALL             4

// Conveyor
#define CONVEYOR_MOTOR_STEPS_PER_REV   200
#define CONVEYOR_MICROSTEPS            16
#define CONVEYOR_TARGET_SPEED_MM_S     120.0f
#define CONVEYOR_FEED_SPS              600
#define CONVEYOR_ACCEL_SPS             80
#define CONVEYOR_HOLD_CURRENT          250

// Timeouts and dwell windows
#define FEED_TIMEOUT_MS          5000
#define SENSING_TIMEOUT_MS        600
#define SERVO_SETTLE_MS           250
#define HANDOFF_WINDOW_MS         500
#define BIN_CONFIRM_TIMEOUT_MS   1200

// Sensing
#define COLOR_SAMPLE_COUNT      8
#define COLOR_MIN_SAMPLES       4
#define COLOR_RED_THRESHOLD     0.42f
#define C_MIN_VALID             100
#define I2C_FREQ_HZ             400000

// Initial thresholds for two-pair break-beam timing. Calibrate with shroud installed.
#define SIZE_SENSOR_TIMEOUT_MS       80
#define SIZE_TWO_BY_THREE_MIN_UNITS  20

// Servo chute selector
#define SELECTOR_BIN1_ANGLE_DEG   37
#define SELECTOR_BIN2_ANGLE_DEG   72
#define SELECTOR_BIN3_ANGLE_DEG   107
#define SELECTOR_BIN4_ANGLE_DEG   142
#define SELECTOR_HOME_BIN         4

// Expected counts
#define EXPECTED_BIN1           6
#define EXPECTED_BIN2           6
#define EXPECTED_BIN3           4
#define EXPECTED_BIN4           8
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

// Serial
#define SERIAL_BAUD  115200
