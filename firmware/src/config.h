#pragma once

// ================================================================
// Pin assignments
// ================================================================
#define PIN_BUTTON          0       // start button (GPIO0, active LOW, internal pullup)
#define PIN_BEAM1           34      // size detection beam 1 receiver
#define PIN_BEAM2           35      // size detection beam 2 receiver
#define PIN_COLOR_SDA       21      // I2C SDA - color sensor
#define PIN_COLOR_SCL       22      // I2C SCL - color sensor
#define PIN_SOL1            25      // solenoid 1 - plow 1 (2x2 blue)
#define PIN_SOL2            26      // solenoid 2 - plow 2 (2x2 red)
#define PIN_SOL3            27      // solenoid 3 - plow 3 (2x3 red)
#define PIN_BELT_PWM        32      // belt motor PWM (L298N enable pin)
#define PIN_BELT_DIR1       33      // belt motor direction 1
#define PIN_BELT_DIR2       14      // belt motor direction 2
#define PIN_STEPPER_STEP    18      // TMC2209 STEP
#define PIN_STEPPER_DIR     19      // TMC2209 DIR
// TMC2209 EN pin: wire to GPIO and pull LOW in begin() to enable driver
// EN is active-low on TMC2209 (opposite of A4988 SLEEP which was active-high)
#define PIN_STEPPER_EN      23      // TMC2209 EN (pull LOW to enable)
#define PIN_BIN1            36      // bin 1 confirmation beam (2x2 blue)
#define PIN_BIN2            39      // bin 2 confirmation beam (2x2 red)
#define PIN_BIN3            4       // bin 3 confirmation beam (2x3 red)
#define PIN_BIN4            5       // bin 4 confirmation beam (2x3 blue)
#define PIN_ENCODER         15      // belt speed encoder (H206 slot optocoupler)
#define PIN_TFT_CS          13      // display chip select
#define PIN_TFT_DC          12      // display data/command
#define PIN_TFT_RST         2       // display reset

// ================================================================
// Belt
// ================================================================
#define BELT_TARGET_MM_S        200     // target operating speed
#define BELT_PWM_FREQ_HZ        5000    // L298N PWM frequency (1-10kHz range)
#define BELT_PWM_RESOLUTION     8       // bits (0-255)
#define BELT_PWM_INIT_DUTY      180     // initial duty - calibrate to reach 200mm/s

// ================================================================
// Size detection  (calibrate empirically after hardware installed)
// ================================================================
#define BEAM_GAP_MM             19      // physical distance between size beams
// At 200mm/s a 2x3 brick (23.7mm) reaches beam 2 in ~95ms.
// A 2x2 brick (15.8mm) never reaches beam 2.
#define SIZE_TIMEOUT_MS         150     // beam 2 timeout: above 95ms, below any real gap
#define SIZE_THRESHOLD_US       50000   // gap_us below this = 2x3; 0 (timeout) = 2x2
                                        // placeholder - calibrate from actual timing data

// ================================================================
// Color detection  (calibrate empirically with shroud installed)
// ================================================================
#define I2C_FREQ_HZ             400000  // must be set explicitly before any sensor read
#define COLOR_INTEGRATION_MS    2       // TCS34725 min integration time ~2.4ms per sample
// At 200mm/s over 15.8mm brick width, dwell window = ~79ms -> ~32 samples max
#define COLOR_BLACK_FLOOR       50      // total (r+g+b+c) below this = bare belt, discard
#define COLOR_RED_RATIO         0.45f   // R/(R+G+B): above = red, below = blue
                                        // placeholder - calibrate from actual brick readings

// ================================================================
// Routing
// ================================================================
// Plow assignment by brick type (BrickType enum order: 2x2_BLUE, 2x2_RED, 2x3_RED, 2x3_BLUE)
// Plow 0 = default path, no solenoid fires
#define EXPECTED_BIN1           6       // 2x2 blue -> plow 1 -> bin 1
#define EXPECTED_BIN2           6       // 2x2 red  -> plow 2 -> bin 2
#define EXPECTED_BIN3           4       // 2x3 red  -> plow 3 -> bin 3
#define EXPECTED_BIN4           8       // 2x3 blue -> default -> bin 4
#define TOTAL_BRICKS            24

// ================================================================
// Solenoid timing
// ================================================================
#define SOL_FULL_PWM            255     // 8-bit full extension duty cycle
#define SOL_HOLD_PWM            102     // ~40% hold duty - reduces heat ~60%
#define SOL_FULL_MS             20      // full power phase duration
#define SOL_DEENERGIZE_MS       280     // total on time: brick clears plow by this point
#define SOL_LEAD_MS             150     // sensor-to-plow travel time (calibrate from CAD)

// ================================================================
// Stepper (escapement)
// ================================================================
#define STEPPER_STEPS_PER_REV   200     // full step, NEMA 11 1.8 deg/step

// Speed is set in steps/sec (sps). RPM = sps / STEPPER_STEPS_PER_REV * 60.
// Start at 800 sps (4 bricks/sec, ~9s run). Tune up in 200 sps increments
// after reliability is proven at each level. Stop when accuracy degrades
// or step-skipping is audible. Document the accuracy vs sps curve.
// TMC2209 handles 2000+ sps. NEMA 11 torque curve limits reliable speed
// to ~1200-1600 sps under load. 8 color samples per brick is the floor.
#define STEPPER_SPS_NORMAL      800     // 4.0 bricks/sec - starting point
#define STEPPER_SPS_WARNING     533     // 2.7 bricks/sec - thermal WARNING rate
#define STEPPER_SPS_DANGER      267     // 1.3 bricks/sec - thermal DANGER rate

// Legacy RPM defines kept for reference (used in log messages only, not hardware)
#define STEPPER_RPM_NORMAL      30      // = 800 sps / 200 steps * 60
#define STEPPER_RPM_WARNING     20
#define STEPPER_RPM_DANGER      10

// ================================================================
// Bin confirmation
// ================================================================
// Timeout starts when routing fires (solenoid command or skip for default path).
// At 200mm/s the brick still needs to travel from its classified position to
// the bin entrance. Max path (bin 4, default): ~330mm remaining at classify time.
// 330mm / 200mm/s = 1650ms. Add 350ms belt speed variation margin = 2000ms.
//
// Tuning: after dry assembly, measure sensor-to-bin-entrance for all 4 bins.
// Compute: max(bin_dist_mm) / belt_speed_mm_s * 1000 + 400ms margin.
// The 400ms accounts for speed variation and actuation timing. Tighten it
// if you want faster ERROR_HALT on genuine jams. Do not go below 200ms margin
// or you will see false halts on normal belt speed variation.
#define CONFIRM_TIMEOUT_MS      2000    // calibrate from actual bin distances after dry assembly

// ================================================================
// Thermal model  (all values need calibration - tune during reliability runs)
// ================================================================
#define THERMAL_HEAT_PER_SOL    0.15f   // heat added per solenoid fire
#define THERMAL_HEAT_PER_STEP   0.05f   // heat added per stepper release
#define THERMAL_DECAY_RATE      0.5f    // exponential decay constant (per second)
#define THERMAL_WARN_LEVEL      0.60f   // above this: reduce stepper RPM
#define THERMAL_DANGER_LEVEL    0.85f   // above this: reduce further
#define THERMAL_LOG_INTERVAL_MS 1000    // how often to log thermal state while idle

// ================================================================
// Display (Waveshare 1.69in ST7789V2, 240x280 IPS, SPI)
// ================================================================
// Driver: TFT_eSPI library. Set in User_Setup.h:
//   #define ST7789_DRIVER
//   #define TFT_WIDTH  240
//   #define TFT_HEIGHT 280
//   #define TFT_MOSI   PIN_TFT_MOSI  (default SPI MOSI - check ESP32 pinout)
//   #define TFT_SCLK   PIN_TFT_SCLK  (default SPI SCLK)
//   #define TFT_CS     PIN_TFT_CS
//   #define TFT_DC     PIN_TFT_DC
//   #define TFT_RST    PIN_TFT_RST
//   #define SPI_FREQUENCY 40000000
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
