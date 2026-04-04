# Embedded Design

## State machine

```
IDLE -> FEED -> APPROACH -> SEATED -> SENSING -> INDEXED -> RELEASED -> CONFIRM -> RESET -> IDLE
                                                                                    |
                                                                             COMPLETE (after brick 24)
                                                          ERROR_HALT from any state on fault
```

---

## State definitions

**IDLE (S0)**
Token = 1. Platform level (lever tip under tab). Disc at home (225 deg, bin 4).
Display: READY. Waiting for START button.

**FEED (S1)**
Token = 1. Conveyor feed axis runs a stepper motion profile toward the chamber.
Monitor PIN_ENTRY_BEAM. Triggered -> APPROACH.
Timeout FEED_TIMEOUT_MS -> ERROR_HALT (JAM_CHUTE).

**APPROACH (S2)**
Entry beam triggered. Belt continues pushing brick to stop wall.
Monitor PIN_STOP_SW. Triggered -> SEATED.
Timeout APPROACH_TIMEOUT_MS -> ERROR_HALT (JAM_APPROACH).

**SEATED (S3)**
Stop switch triggered. Belt immediately disabled. Token = 0.
Settle: SETTLE_MS (50ms). Nothing moves.

**SENSING (S4)**
Read both size beams: both LOW = 2x3, else = 2x2.
Accumulate color samples from the purchased color module until COLOR_SAMPLE_COUNT or COLOR_TIMEOUT_MS.
valid_sample_count < COLOR_MIN_SAMPLES -> ERROR_HALT (SENSOR_FAULT).
Lock category and bin_index.

**INDEXED (S5)**
Stepper moves disc to target bin position with ramp profile.
StallGuard monitored. Stall -> ERROR_HALT (DISC_JAM).
Stop at target. Reduce hold current.

**RELEASED (S6)**
Disc at target. Lever tip under platform tab. Brick seated.
PIN_RELEASE HIGH for SOLENOID_ON_MS (80ms).
During pulse: solenoid plunger extends parallel to belt, pushes lever arm at effort
point (8mm from fulcrum), lever tip sweeps outward 30mm, clears platform tab,
platform far edge unsupported, gravity drops platform, brick falls straight down
through aligned disc funnel.
After 80ms: PIN_RELEASE LOW. Solenoid spring retracts plunger.
Lever return spring pulls tip back inward.
Platform return spring pulls far edge back to level.
Lever tip re-engages platform tab via chamfer (slam latch).
FALL_SETTLE_MS timer covers fall + both spring returns.

**CONFIRM (S7)**
Monitor expected bin beam up to BIN_CONFIRM_TIMEOUT_MS.
Trigger -> increment bin counter.
Timeout -> ERROR_HALT (MISS_BIN_N).

**RESET (S8)**
If PIN_SHELF_LEVEL installed: confirm platform level within PLATFORM_RETURN_MS.
Timeout -> ERROR_HALT (PLATFORM_STUCK).
Clear brick state. brick_count++.
brick_count == 24 -> COMPLETE. Else -> FEED.

**COMPLETE (S9)**
Display SORT COMPLETE. Buzzer success. Stepper homes to 225 deg.
Verify totals (6, 6, 8, 4). Wait for START -> IDLE.

**ERROR_HALT**
Belt stop. Stepper stop. PIN_RELEASE LOW (solenoid off, lever return spring
pulls tip back, platform held by lever). Display error. Buzzer 3 beeps.
Wait for manual reset.

Error codes: JAM_CHUTE, JAM_APPROACH, SENSOR_FAULT, DISC_JAM, MISS_BIN_N,
DOUBLE_ENTRY, PLATFORM_STUCK, POSITION_DRIFT.

---

## Sensing sequence (SENSING state)

```
1. Belt off. Brick against stop wall. Stop switch active. Token = 0.
2. Wait SETTLE_MS.
3. Read GPIO 36 (beam 1) and GPIO 34 (beam 2).
   Both LOW -> SIZE_2x3. Else -> SIZE_2x2.
4. Loop until COLOR_SAMPLE_COUNT valid samples or COLOR_TIMEOUT_MS:
   - Read current color-module output
   - Convert to calibrated red versus blue classification metric
   - Accumulate valid samples
5. valid_sample_count < COLOR_MIN_SAMPLES -> ERROR_HALT (SENSOR_FAULT)
6. avg_r_ratio = sum / count
7. color = avg_r_ratio > COLOR_RED_THRESHOLD ? COLOR_RED : COLOR_BLUE
8. category = lookup(size, color) -> BrickCategory
9. bin_index = BIN_MAP[category]
10. Lock.
```

---

## Stepper indexing sequence (INDEXED state)

```
1. Compute target_steps from bin_index.
2. Compute shortest arc delta. Set DIR.
3. EN LOW. Ramp from STEPPER_START_SPS to STEPPER_RUN_SPS.
4. Run at STEPPER_RUN_SPS. Decel STEPPER_DECEL_STEPS before target.
5. Final steps to target at STEPPER_START_SPS.
6. Stop. Set hold current = STEPPER_HOLD_CURRENT.
7. Read SGRESULT via UART. Stall -> ERROR_HALT (DISC_JAM).
8. Update current_position_steps.
```

Periodic re-home every RETHOME_PERIOD_BRICKS:
- Rotate to expected home by step count.
- Verify PIN_HOME_SW triggers.
- Mismatch > RETHOME_TOLERANCE_STEPS -> ERROR_HALT (POSITION_DRIFT).

---

## Release sequence (RELEASED state)

```
1. Verify disc at target (step count).
2. PIN_RELEASE HIGH.
3. Wait SOLENOID_ON_MS (80ms).
   Solenoid extends plunger parallel to belt.
   Lever arm (class 3) rotates about M3 pivot bolt.
   Lever tip sweeps outward ~30mm, clears 8-10mm tab.
   Platform far edge unsupported. Gravity drops platform.
   Brick falls through aligned funnel.
4. PIN_RELEASE LOW.
   Solenoid spring retracts plunger.
   Lever return spring pulls tip back inward.
   Platform return spring pulls far edge to level.
   Lever tip chamfer re-engages tab (slam latch).
5. Wait remaining FALL_SETTLE_MS (400ms total from step 2).
```

Note on solenoid load: the solenoid plunger pushes the lever arm against the lever
return spring only (~0.05-0.10N). It does not support platform weight at any point.
This is the minimum possible solenoid load in this design. Thermal impact is negligible.

---

## Module structure

| Module | Responsibility |
|--------|----------------|
| main.cpp | Init, loop, event dispatch |
| config.h | All constants. Nothing hardcoded elsewhere. |
| sensors.h/.cpp | senseBrickInChamber(): dual beams + color + classify. |
| actuators.h/.cpp | All hardware. Target split before competition: belt.h, chute_selector.h, release.h, audio.h, selftest.h. |
| display.h/.cpp | All display states. |
| thermal.h/.cpp | Solenoid thermal model (minimal load in current mechanism). |
| logger.h/.cpp | Serial CSV. Every event timestamped. |
| events.h/.cpp | Event queue. |
| state_machine.h/.cpp | Transitions and routing table. |
| test_harness.h/.cpp | Serial command interface. |

---

## config.h constants

```c
// Belt
#define CONVEYOR_TARGET_SPEED_MM_S  100.0f
#define CONVEYOR_RUN_SPS            800
#define CONVEYOR_START_SPS          120
#define CONVEYOR_ACCEL_SPS          80
#define CONVEYOR_HOLD_CURRENT       250
#define ROLLER_OD_MM                25.0f

// Timeouts (ms)
#define FEED_TIMEOUT_MS         5000
#define APPROACH_TIMEOUT_MS     500
#define SETTLE_MS               50
#define COLOR_TIMEOUT_MS        500
#define SOLENOID_ON_MS          80
#define FALL_SETTLE_MS          400
#define PLATFORM_RETURN_MS      300
#define BIN_CONFIRM_TIMEOUT_MS  1000

// Sensing
#define COLOR_SAMPLE_COUNT      8
#define COLOR_MIN_SAMPLES       4
#define COLOR_RED_THRESHOLD     0.42f     // SET FROM CALIBRATION DATA
#define C_MIN_VALID             100

// Stepper
#define STEPPER_STEPS_PER_REV   1600
#define STEPPER_RUN_SPS         400
#define STEPPER_START_SPS       100
#define STEPPER_ACCEL            50
#define STEPPER_DECEL_STEPS     50
#define STEPPER_HOLD_CURRENT    200
#define RETHOME_PERIOD_BRICKS   8
#define RETHOME_TOLERANCE_STEPS 20

// Bin positions (steps 0-1599)
#define BIN1_STEPS              1400
#define BIN2_STEPS               200
#define BIN3_STEPS               600
#define BIN4_STEPS              1000      // home

// Expected counts
#define EXPECTED_BIN1            6
#define EXPECTED_BIN2            6
#define EXPECTED_BIN3            8
#define EXPECTED_BIN4            4
```

---

## GPIO assignments

```c
PIN_STEP        = GPIO 25    // stepper step
PIN_DIR         = GPIO 26    // stepper direction
PIN_ENABLE      = GPIO 27    // stepper enable, active low
PIN_TMC_UART    = GPIO 23    // TMC2209 UART, 1k series
PIN_RELEASE     = GPIO 32    // lever solenoid, IRLZ44N gate via 1k
PIN_SIZE_BEAM1  = GPIO 36    // X=5mm. 10k ext pull-up. Input-only.
PIN_SIZE_BEAM2  = GPIO 34    // X=21mm. 10k ext pull-up. Input-only.
PIN_ENTRY_BEAM  = GPIO 35    // chamber entry. 10k ext pull-up. Input-only.
PIN_STOP_SW     = GPIO 33    // stop wall switch. Internal pull-up.
PIN_HOME_SW     = GPIO 15    // disc home switch. Internal pull-up.
PIN_SHELF_LEVEL = GPIO 39    // platform-level switch. 10k ext pull-up. Input-only.
PIN_BIN1_BEAM   = GPIO 16
PIN_BIN2_BEAM   = GPIO 17
PIN_BIN3_BEAM   = GPIO 5
PIN_BIN4_BEAM   = GPIO 18
PIN_SDA         = GPIO 21
PIN_SCL         = GPIO 22
PIN_HALL        = GPIO 4     // optional diagnostic Hall input if retained later
PIN_START_BTN   = GPIO 19    // active low, external pull-up
PIN_BUZZER      = GPIO 2
```

GPIO 34, 35, 36, 39: input-only, no internal pull-ups, require 10k external to 3.3V.
ADC1 never used. GPIO 36 and 39 safe as digital inputs.

---

## Self-test sequence (power-on)

1. Color sensor: verify the actual purchased module responds as expected for the chosen interface.
2. Display: I2C ACK at 0x3C or SPI init.
3. Size beam 1 (GPIO 36): HIGH (unblocked).
4. Size beam 2 (GPIO 34): HIGH.
5. Entry beam (GPIO 35): HIGH.
6. Lever release: 20ms pulse. Verify lever does not fully clear tab (sub-threshold test).
7. Stepper: 5 steps CW, 5 CCW. SGRESULT clean.
8. Home: run until PIN_HOME_SW triggers. Set position = BIN4_STEPS.
9. Conveyor feed axis: short forward and stop command succeeds.
10. Display SELF-TEST PASS. -> IDLE.

Failure on any step: display SELF-TEST FAIL [component]. Halt.

---

## Implementation alignment notes

sensors.h/.cpp:
  Keep `senseBrickInChamber` as the single classification entry point.

actuators.h/.cpp:
  Use `PIN_RELEASE` as the only release output.
  Keep `SOLENOID_ON_MS = 80` unless calibration data supports change.
  Chute selector behavior:
    Home: run until `PIN_HOME_SW`, set position = `BIN4_STEPS`.
    `indexToBin(bin_index)`: shortest arc, ramp, StallGuard read after move.
    Periodic re-home every `RETHOME_PERIOD_BRICKS`.
  Conveyor target starts at `CONVEYOR_TARGET_SPEED_MM_S = 100.0f`.
  Keep `PIN_SHELF_LEVEL` check in `RESET` when installed.

state_machine.h/.cpp:
  Maintain current states and selector-before-release order.
  Do not add legacy transit or pusher timing paths.

config.h:
  Keep all constants listed above in one location.
  Update GPIO map only with matching doc updates.

events.h/.cpp:
  Keep event set aligned to current state flow: `BRICK_SEATED`, `INDEXED`, `PLATFORM_RELEASED`.

test_harness.h/.cpp:
  Supported test commands: `test release`, `test home`, `test index N` (N=1-4), `test drop`.
