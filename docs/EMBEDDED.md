# Embedded Design

## State machine

```
IDLE -> SENSING -> RELEASING -> TRANSIT -> CONFIRM -> COMPLETE
                                                  |
                                             ERROR_HALT
```

**IDLE:** Display shows READY. Waiting for START button press. Thermal model cooling
between runs. Self-test must pass before IDLE is reached.

**SENSING:** Brick N is stationary on the cam chord. Size beam read (instantaneous digital
read). Color sensor accumulates samples for 150-200ms. Classification completes. Pusher
index is determined and stored for this brick.

**RELEASING:** Cam rotates 360 degrees (one full rotation), releasing brick N onto belt.
Chute exit beam confirms release. If not confirmed within timeout: ERROR_HALT.

**TRANSIT:** Brick is on the belt. Firmware counts down to the scheduled pusher fire time
(based on classification from SENSING). If the brick is 2x3 red (default path): no pusher
fires. After pusher-fire time (or belt-end time for default): transition to CONFIRM.

**CONFIRM:** Wait up to 500ms for the expected bin's break-beam to trigger. On break:
increment bin counter, check if 24 bricks done. If 24 done: COMPLETE. Else: back to SENSING
for brick N+1. On timeout: ERROR_HALT.

**COMPLETE:** Display shows SORT COMPLETE with per-bin counts and run time. Buzzer plays
success tone. System waits for reset.

**ERROR_HALT:** Belt stops. Stepper stops. All solenoids de-energize. Display shows which
error occurred and which brick number triggered it. Waits for manual reset.

---

## Sensing sequence (per brick)

```
1. Previous brick has exited chute (chute exit beam cleared)
2. Brick N settles onto cam chord (~50ms settling time)
3. Read PIN_SIZE_BEAM: LOW = 2x3 (beam blocked), HIGH = 2x2 (beam clear)
4. Accumulate TCS34725 color samples for 150-200ms
5. Compute r_ratio = R / (R + G + B), averaged over all valid samples
6. Classify: (size, r_ratio) -> BrickCategory
7. If CAT_UNCERTAIN and sample_count < 3: ERROR_HALT
8. Store pusher index for this brick
9. Trigger RELEASING state
```

Classification is complete before the brick touches the belt. Belt speed has zero effect
on classification quality.

**I2C clock:** must be set to 400kHz before any TCS34725 communication. See ELECTRICAL.md.

---

## Pusher timing

Pre-calculated at startup from BELT_SPEED_MM_S and pusher positions in config.h:

```
T_TO_PUSHER1_MS = PUSHER1_POS_MM / BELT_SPEED_MM_S * 1000   // 375ms at 200mm/s
T_TO_PUSHER2_MS = PUSHER2_POS_MM / BELT_SPEED_MM_S * 1000   // 750ms
T_TO_PUSHER3_MS = PUSHER3_POS_MM / BELT_SPEED_MM_S * 1000   // 1125ms
T_TO_BELT_END_MS = BELT_LENGTH_MM / BELT_SPEED_MM_S * 1000  // 1450ms
```

Timer starts when chute exit beam breaks (brick enters belt). At T_TO_PUSHERn_MS, fire
the scheduled solenoid for SOLENOID_ON_MS (40ms), then de-energize. Spring returns.

Pusher assignment lookup (from state_machine.cpp):
```
CAT_2x2_RED  -> pusher index 1 (GPIO 32, at 75mm)
CAT_2x2_BLUE -> pusher index 2 (GPIO 33, at 150mm)
CAT_2x3_BLUE -> pusher index 3 (GPIO 15, at 225mm)
CAT_2x3_RED  -> pusher index 0 (no pusher, default path to bin 4)
```

---

## Module structure

All modules are in firmware/src/. Each has a header and implementation file.
Code must compile with zero warnings. Constants go in config.h, never hardcoded.

| Module | Responsibility |
|--------|---------------|
| main.cpp | Init and loop: poll event queue, dispatch to state machine |
| config.h | All tunable constants with units in comments |
| selftest.h/.cpp | Power-on test sequence: solenoid pulse, beam verify, I2C ACK, stepper step |
| escapement.h/.cpp | Stepper control: step pulse generation, cam rotation, StallGuard monitoring |
| sensing.h/.cpp | senseBrickInChute(): size read + color accumulate + classify |
| diverter.h/.cpp | armPusher(idx, fireAtMs): schedules solenoid fire using esp_timer_once |
| display.h/.cpp | State display updates, brick animation, bin counters, thermal bar |
| belt.h/.cpp | Belt motor PWM control, speed target management |
| audio.h/.cpp | Buzzer tones for events (ready, sorted, error) |
| thermal.h/.cpp | Thermal model for solenoid temperature. WARNING and DANGER thresholds. |
| logger.h/.cpp | Serial CSV output: every event timestamped. Source of notebook data. |
| events.h/.cpp | Event queue: SENSING_DONE, CHUTE_EXIT, PUSHER_FIRED, BIN_CONFIRM |
| state_machine.h/.cpp | State transitions, pusher routing table, run completion check |
| test_harness.h/.cpp | Serial command interface for simulation and subsystem testing |

---

## Display requirements

Display updates must not block the sensing or routing pipeline. Animation runs
asynchronously. Updates happen in the main loop, not inside interrupt handlers or
state machine transitions.

Display states:
- READY: large text, shows bricks loaded count if known
- SORTING: animated brick silhouette in the correct color and size for the current brick,
  live bin counters (N/6, N/6, N/8, N/4), thermal bar in corner
- SORT COMPLETE: per-bin totals vs expected, run time, any missed confirmations
- ERROR: error type, brick number, which bin timed out

---

## Thermal model

The thermal model is a real engineering requirement. It tracks estimated solenoid
temperature based on firing rate and cool-down time between runs. It is NOT optional.

At WARNING threshold: reduce release rate (stepper SPS) to allow cool-down.
At DANGER threshold: halt sorting, display WARNING on screen. Do not fire solenoids.
The thermal state is visible on the display during sorting (small bar indicator).

---

## Self-test sequence (on power-on, before IDLE)

1. Each solenoid: 10ms pulse, verify no fault (current sense or timing)
2. Each break-beam: emitter on, verify receiver reads LOW (beam present and connected)
3. Color sensor: I2C ACK at address 0x29, read ID register, verify expected value
4. Stepper: one full rotation, verify StallGuard does not flag (no jam)
5. Belt motor: 200ms spin, verify (optional) hall pulse or timer completes
6. Display SELF-TEST PASS. Proceed to IDLE.
7. On any failure: display SELF-TEST FAIL with which component failed. Do not proceed.
