#pragma once

// ================================================================
// Design: V3 + Addendum A (March 21 2026)
// Sensing moved to escapement zone. Brick classified while stationary.
// Belt is transport only. Speed decoupled from sensing quality.
// Diverter: direct solenoid push, no lever arm.
// Default bin: 2x3 RED (rarest category, minimizes error accumulation).
// ================================================================

// ================================================================
// Pin assignments
// Avoid GPIO 36 and 39 - phantom interrupt errata with ESP32 ADC.
// GPIO 34, 35: input-only, no internal pull-ups. Need external 10k to 3.3V.
// ================================================================

// Stepper (escapement) via TMC2209
#define PIN_STEPPER_STEP    25
#define PIN_STEPPER_DIR     26
#define PIN_STEPPER_EN      27      // active-low on TMC2209 (pull LOW to enable)
#define PIN_TMC_UART        23      // single-wire UART, needs 1k series resistor

// Belt motor via L298N
#define PIN_BELT_PWM        14      // PWM speed control (L298N ENA)
#define PIN_BELT_DIR1       12      // L298N IN1
#define PIN_BELT_DIR2       13      // L298N IN2

// Solenoids via N-channel MOSFETs (IRLZ44N)
#define PIN_SOL1            32      // pusher 1 -> bin 1 (2x2 red)
#define PIN_SOL2            33      // pusher 2 -> bin 2 (2x2 blue)
#define PIN_SOL3            15      // pusher 3 -> bin 3 (2x3 blue)

// Sensing - chute/escapement zone
// GPIO 34 and 35 are input-only. External 10k pull-ups to 3.3V required.
#define PIN_SIZE_BEAM       34      // chute size beam: blocked=2x3, clear=2x2
#define PIN_CHUTE_EXIT      35      // chute exit confirmation beam
// TCS34725 color sensor on I2C (pins below)

// Bin confirmation beams
#define PIN_BIN1_BEAM       16      // bin 1 confirm (2x2 red)
#define PIN_BIN2_BEAM       17      // bin 2 confirm (2x2 blue)
#define PIN_BIN3_BEAM       5       // bin 3 confirm (2x3 blue)
#define PIN_BIN4_BEAM       18      // bin 4 confirm (2x3 red, default)

// I2C bus (TCS34725 at 0x29)
#define PIN_COLOR_SDA       21
#define PIN_COLOR_SCL       22

// Display (Waveshare 1.69in ST7789V2, SPI)
#define PIN_TFT_CS          13      // NOTE: shared with belt DIR2 - reassign if conflict
#define PIN_TFT_DC          12
#define PIN_TFT_RST         2

// Operator interface
#define PIN_START_BTN       19      // momentary button, active low, internal pull-up
#define PIN_BUZZER          0       // passive piezo, 100 ohm series resistor

// Optional: belt speed Hall sensor
#define PIN_HALL            4       // A3144 Hall sensor on idler roller

// ================================================================
// Belt
// ================================================================
#define BELT_TARGET_MM_S        200     // target speed (sensing decoupled, can push up)
#define BELT_PWM_FREQ_HZ        5000    // L298N PWM frequency (1-10kHz range)
#define BELT_PWM_RESOLUTION     8       // bits (0-255)
#define BELT_PWM_INIT_DUTY      128     // start at 50%, tune up to 200mm/s

// Belt geometry (Addendum A - shorter belt, 640mm loop)
#define BELT_ACTIVE_MM          290.0f  // chute exit to belt end
#define BELT_CIRCUMFERENCE_MM   640     // closed-loop GT2 belt

// ================================================================
// Escapement-based sensing (Addendum A)
// Brick is stationary on cam chord. Size: single digital read.
// Color: accumulate samples during cam dwell time.
// ================================================================
#define CHUTE_WIDTH_MM          27.0f   // chute internal dimension (27mm axis)
#define SIZE_BEAM_POS_MM        20.0f   // beam position from wall in 27mm dimension
                                        // 2x3=23.7mm blocks it, 2x2=15.8mm does not
#define COLOR_SENSE_MS          150     // color sampling duration per brick (ms)
#define COLOR_INTEGRATION_MS    24      // TCS34725 integration time per sample
#define COLOR_MIN_SAMPLES       3       // below this: CAT_UNCERTAIN, flag error
#define I2C_FREQ_HZ             400000  // set explicitly before first sensor read

// Color thresholds (calibrate with real bricks and shroud installed)
#define COLOR_RED_HIGH          0.50f   // R/(R+G+B) above this = RED
#define COLOR_RED_LOW           0.30f   // R/(R+G+B) below this = BLUE
                                        // between = UNCERTAIN

// ================================================================
// Pusher positions (mm from belt start / chute exit)
// ================================================================
#define PUSHER1_POS_MM          75.0f   // bin 1: 2x2 red
#define PUSHER2_POS_MM          150.0f  // bin 2: 2x2 blue
#define PUSHER3_POS_MM          225.0f  // bin 3: 2x3 blue
#define BELT_END_MM             290.0f  // bin 4: 2x3 red (default, no pusher)

// Travel time from chute exit to each pusher at BELT_TARGET_MM_S
// T_ms = pos_mm / belt_mm_s * 1000
// Pusher 1: 375ms, Pusher 2: 750ms, Pusher 3: 1125ms

// ================================================================
// Bin assignment
// ================================================================
#define EXPECTED_BIN1           6       // 2x2 red  -> pusher 1 -> bin 1
#define EXPECTED_BIN2           6       // 2x2 blue -> pusher 2 -> bin 2
#define EXPECTED_BIN3           8       // 2x3 blue -> pusher 3 -> bin 3
#define EXPECTED_BIN4           4       // 2x3 red  -> default  -> bin 4 (RAREST)
#define TOTAL_BRICKS            24

// ================================================================
// Solenoid timing
// ================================================================
#define SOL_ON_MS               40      // energize duration (brick clears in ~2ms, 40ms is safe)
// No lever, no PWM hold needed - direct push, spring return
// Face plate: 20mm wide. Window = (15.8+20)/200*1000 = 179ms. Margin: 139ms.

// ================================================================
// Escapement (NEMA 11 + cam)
// ================================================================
#define STEPPER_STEPS_PER_REV   200     // full step, 1.8 deg/step
#define CAM_LOBE_DEG            50      // lobe arc (tune +/-5 deg to prevent double-release)

// Cycle time per brick: SENSE_MS + release + settle = ~280ms = 3.6 bricks/sec
// Reduce COLOR_SENSE_MS to 100ms for ~3.8 bricks/sec if needed
#define ESCAPEMENT_CYCLE_MS     280     // total cam cycle time

// Thermal throttle (uses same model, different sps values)
#define STEPPER_SPS_NORMAL      800     // 3.6 bricks/sec at 280ms cycle
#define STEPPER_SPS_WARNING     533     // thermal WARNING throttle
#define STEPPER_SPS_DANGER      267     // thermal DANGER throttle

// ================================================================
// Bin confirmation
// ================================================================
#define CONFIRM_TIMEOUT_MS      2000    // max wait after routing before ERROR_HALT
                                        // calibrate from actual bin distances after assembly

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
