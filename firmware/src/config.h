#pragma once

// ================================================================
// Design: V6 (chamber-based sorting)
// Chamber sensing with dual size beams. Chute selector disc with stepper.
// Single trapdoor release via class 3 lever. TB6612FNG belt motor driver.
// ================================================================

// ================================================================
// Pin assignments
// GPIO 34, 35, 36, 39: input-only, no internal pull-ups. Need external 10k to 3.3V.
// ================================================================

// Stepper (chute selector disc) via TMC2209
#define PIN_STEP            25
#define PIN_DIR             26
#define PIN_ENABLE          27      // active-low on TMC2209 (pull LOW to enable)
#define PIN_TMC_UART        23      // single-wire UART, needs 1k series resistor

// Belt motor via TB6612FNG
#define PIN_MOTOR_ENA       14      // PWM speed control
#define PIN_MOTOR_IN1       12      // direction
#define PIN_MOTOR_IN2       13      // direction
#define PIN_MOTOR_STBY      0       // standby (must be HIGH for operation)

// Release solenoid via N-channel MOSFET (IRLZ44N)
#define PIN_RELEASE         32      // class 3 lever solenoid, gate via 1k resistor

// Sensing - chamber zone
// GPIO 34, 35, 36, 39 are input-only. External 10k pull-ups to 3.3V required.
#define PIN_SIZE_BEAM1      36      // X=5mm, 10k ext pull-up, INPUT-ONLY
#define PIN_SIZE_BEAM2      34      // X=21mm, 10k ext pull-up, INPUT-ONLY
#define PIN_ENTRY_BEAM      35      // chamber entry, 10k ext pull-up, INPUT-ONLY
#define PIN_STOP_SW         33      // stop wall micro-switch, internal pull-up
#define PIN_HOME_SW         15      // disc home micro-switch, internal pull-up
#define PIN_SHELF_LEVEL     39      // platform-level switch, 10k ext pull-up, INPUT-ONLY
// TCS34725 color sensor on I2C (pins below)

// Bin confirmation beams
#define PIN_BIN1_BEAM       16
#define PIN_BIN2_BEAM       17
#define PIN_BIN3_BEAM       5
#define PIN_BIN4_BEAM       18

// I2C bus (TCS34725 at 0x29, PCF8574 at 0x20)
#define PIN_SDA             21
#define PIN_SCL             22
#define PCF8574_ADDR        0x20

// Operator interface
#define PIN_START_BTN       19      // momentary button, active low, external pull-up
#define PIN_BUZZER          2       // passive piezo, 100 ohm series resistor

// Belt speed Hall sensor
#define PIN_HALL            4       // A3144 Hall sensor on idler roller, 10k pull-up

// ================================================================
// Belt
// ================================================================
#define BELT_TARGET_SPEED       100.0f  // target speed (mm/s)
#define BELT_KP                 0.5f
#define BELT_KI                 0.1f
#define BELT_BASE_PWM           128
#define ROLLER_OD_MM            25.0f
#define BELT_MAGNETS            2

// ================================================================
// Timeouts (ms)
// ================================================================
#define FEED_TIMEOUT_MS         5000
#define APPROACH_TIMEOUT_MS     500
#define SETTLE_MS               50
#define COLOR_TIMEOUT_MS        500
#define SOLENOID_ON_MS          80
#define FALL_SETTLE_MS          400
#define PLATFORM_RETURN_MS      300
#define BIN_CONFIRM_TIMEOUT_MS  1000

// ================================================================
// Sensing
// ================================================================
#define COLOR_SAMPLE_COUNT      8
#define COLOR_MIN_SAMPLES       4
#define COLOR_RED_THRESHOLD     0.42f   // SET FROM CALIBRATION DATA
#define C_MIN_VALID             100
#define I2C_FREQ_HZ             400000  // set explicitly before first sensor read

// ================================================================
// Stepper (chute selector disc)
// ================================================================
#define STEPPER_STEPS_PER_REV   1600
#define STEPPER_RUN_SPS         400
#define STEPPER_START_SPS       100
#define STEPPER_ACCEL           50
#define STEPPER_DECEL_STEPS     50
#define STEPPER_HOLD_CURRENT    200
#define RETHOME_PERIOD_BRICKS   8
#define RETHOME_TOLERANCE_STEPS 20

// Bin positions (steps 0-1599)
#define BIN1_STEPS              1400
#define BIN2_STEPS              200
#define BIN3_STEPS              600
#define BIN4_STEPS              1000    // home

// ================================================================
// Expected counts
// ================================================================
#define EXPECTED_BIN1           6
#define EXPECTED_BIN2           6
#define EXPECTED_BIN3           8
#define EXPECTED_BIN4           4
#define TOTAL_BRICKS            24

// ================================================================
// Thermal model
// ================================================================
#define THERMAL_HEAT_PER_SOL    0.15f
#define THERMAL_HEAT_PER_STEP   0.05f
#define THERMAL_DECAY_RATE      0.5f
#define THERMAL_WARN_LEVEL      0.60f
#define THERMAL_DANGER_LEVEL    0.85f
#define THERMAL_LOG_INTERVAL_MS 1000

// ================================================================
// Display (Waveshare 1.69in ST7789V2, 240x280 IPS, SPI)
// Use TFT_eSPI library. Set in User_Setup.h:
//   #define ST7789_DRIVER
//   #define TFT_WIDTH  240
//   #define TFT_HEIGHT 280
//   #define SPI_FREQUENCY 40000000
// ================================================================
#define DISPLAY_WIDTH   240
#define DISPLAY_HEIGHT  280

// RGB565 colors
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
