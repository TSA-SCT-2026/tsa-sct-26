# Firmware Sprint: What to Build Before Parts Arrive

No hardware needed for any of this. Do it now.
Parts arrive ~April 1. You have ~10 days of pure firmware time.

---

## Day 1-2: Project scaffold

Set up PlatformIO project, verify it compiles, get serial output working.

```
firmware/
  platformio.ini
  src/
    main.cpp          entry point, setup() and loop()
    config.h          all tunable constants in one place
    state_machine.cpp state definitions and transition logic
    state_machine.h
    events.h          event type enum used everywhere
    stepper.cpp       escapement step/dir control
    stepper.h
    belt.cpp          L298N PWM control
    belt.h
    solenoid.cpp      fire, PWM hold, de-energize scheduling
    solenoid.h
    color_sensor.cpp  TCS34725 I2C driver, sampling, classifier
    color_sensor.h
    size_sensor.cpp   hardware timer interrupt, beam logic
    size_sensor.h
    bin_confirm.cpp   confirmation beam handling, timeout
    bin_confirm.h
    thermal.cpp       exponential decay model
    thermal.h
    display.cpp       ST7789 SPI driver stub, state screens
    display.h
    logger.cpp        serial timestamp logging, CSV mode
    logger.h
    test_harness.cpp  serial command interface
    test_harness.h
```

platformio.ini minimum:
```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200
```

config.h is where every magic number lives. Nothing hardcoded in .cpp files.

---

## Day 2-3: config.h and events.h first

Build these two files before anything else. Every other module depends on them.

config.h must contain:
- STEPPER_SPS (default 800, comment says "tune up in 200 sps increments after reliability proven")
- STEPPER_STEPS_PER_REV (200)
- BELT_PWM_DEFAULT (start at 128, tune empirically)
- COLOR_THRESHOLD (0.45, placeholder, calibrate with shroud installed)
- COLOR_BLACK_FLOOR (500, placeholder, calibrate against bare belt)
- SIZE_TIMEOUT_MS (150)
- SIZE_BEAM_SPACING_MM (19)
- SOLENOID_FULL_POWER_MS (20)
- SOLENOID_HOLD_DUTY (40)
- SOLENOID_DEENERGIZE_MS (280)
- BIN_CONFIRM_TIMEOUT_MS (500)
- THERMAL_COOLING_FACTOR (0.95)
- THERMAL_WARNING_THRESHOLD (0.6)
- THERMAL_DANGER_THRESHOLD (0.85)
- Pin assignments for every GPIO

events.h must define:
- EventType enum: BEAM1_BREAK, BEAM2_BREAK, BEAM1_RESTORE, SIZE_TIMEOUT, COLOR_SAMPLE_READY, BIN_CONFIRM, BIN_TIMEOUT, BUTTON_PRESS, THERMAL_WARNING, THERMAL_DANGER
- Event struct: type + timestamp + optional payload

---

## Day 3-4: State machine and logger

The state machine processes events from a queue. ISRs push events in. Loop pulls them out.
This architecture means ISRs are minimal (push and return) and all logic is in the main loop.

State machine:
- Enum of all states
- Current state variable
- process_event(Event e) function: big switch on current state + event type
- Transition functions that log the transition over serial

Logger:
- log_event(EventType, uint32_t timestamp, const char* detail)
- log_brick(int num, const char* size, const char* color, int bin)
- log_run_summary(int total, int* bin_counts, uint32_t elapsed_ms)
- csv_mode flag: changes output format to one row per brick

Get serial output working and readable before touching any hardware module.
Run `test fullrun` from test_harness and confirm state transitions print correctly.

---

## Day 4-5: Sensor stubs with test harness injection

Write all sensor modules as stubs first. They expose the same interface as real hardware
but return values injected from the test harness. The state machine never knows the difference.

size_sensor.cpp stub:
- start_timer() / stop_timer() are no-ops
- inject_beam1_break() and inject_beam2_break() push events directly into the queue
- Real ISR implementation marked TODO, wired in later

color_sensor.cpp stub:
- start_sampling() begins a counter
- get_classification() returns injected color value
- inject_reading(r, g, b) for test harness use

This lets you run the full 24-brick sequence from serial commands before any hardware arrives.

---

## Day 5-6: Thermal model

Pure math, no hardware needed, fully testable over serial.

```cpp
struct ThermalState {
  float heat;
  uint32_t last_update_ms;
};

void thermal_update(ThermalState* t, uint32_t now_ms) {
  float elapsed = (now_ms - t->last_update_ms) / 1000.0f;
  t->heat *= powf(THERMAL_COOLING_FACTOR, elapsed);
  t->last_update_ms = now_ms;
}

void thermal_on_fire(ThermalState* t, uint32_t duration_ms) {
  t->heat += duration_ms * HEAT_PER_MS;
}
```

One ThermalState per solenoid, one for stepper. Update every loop iteration.
Test with `test thermal 50` from serial: simulate 50 firing cycles and verify
WARNING and DANGER thresholds trigger at correct accumulated heat.

---

## Day 6-7: Solenoid timing and stepper sequencer

Solenoid module: fire(int id) starts the sequence
- t=0: GPIO high (full power)
- t=SOLENOID_FULL_POWER_MS: switch to PWM at SOLENOID_HOLD_DUTY
- t=SOLENOID_DEENERGIZE_MS: GPIO low

Implement with ESP32 hardware timers, not delay(). Three timers minimum, one per solenoid
so they can overlap if two plows fire close together (they should not, but be defensive).

Stepper sequencer:
- release_one_brick(): fires STEPPER_STEPS_PER_REV steps at STEPPER_SPS
- Uses ESP32 hardware timer for step pulses, not a software loop
- On completion: pushes ESCAPEMENT_DONE event to queue
- Between releases: reduce hold current (trim pot on driver, set once, not dynamic)

---

## Day 7-8: Display module stub

Write the display module against the ST7789 interface but with a hardware flag
that compiles out the actual SPI calls when DISPLAY_HARDWARE_PRESENT is not defined.
This lets the rest of the firmware reference display functions without needing the panel.

States to implement (stub them as serial prints for now):
- display_ready(): "READY - press button to sort"
- display_sorting(int brick_num, const char* size, const char* color, int dest_bin)
- display_sort_complete(uint32_t elapsed_ms, int* bin_counts)
- display_error(int brick_num, int expected_bin)
- display_thermal_bar(float heat_level)

Brick animation: pre-rendered bitmaps in flash. RGB565 format.
2x2 red: 0xF800, 2x2 blue: 0x001F, 2x3 red: 0xF800 (wider), 2x3 blue: 0x001F (wider).
All display updates must be non-blocking. Use a display task or async queue.

---

## Day 8-9: Full integration test over serial

With all stubs in place, run this sequence from test_harness and verify:
1. `test fullrun` - all 24 bricks route correctly, summary shows (6, 6, 4, 8)
2. `test classify 200 50 80 1000 0` - red brick (high R ratio), confirm RED
3. `test classify 50 80 200 1000 0` - blue brick (low R ratio), confirm BLUE
4. `test thermal 50` - verify WARNING and DANGER trigger
5. Inject a bin timeout (no BEAM_BREAK after routing): verify ERROR_HALT
6. `log csv` then `test fullrun`: confirm CSV output has one row per brick

If all pass: firmware skeleton is complete. Hardware drops in when parts arrive.

---

## Day 9-10: Parallel color sampling

Implement the beam1-break parallel sampling optimization:
- BEAM1_BREAK ISR: start hardware timer for size detection AND push START_COLOR_SAMPLING event
- Color sampling loop starts immediately, discards belt readings via black floor filter
- SIZE_TIMEOUT or BEAM2_BREAK: size classification resolves, color samples already banked
- Classify both simultaneously, push to routing

This is a meaningful firmware optimization with zero hardware changes.
Document it in the engineering notebook as a design decision.

---

## What not to touch yet

- Real I2C driver for TCS34725: wait for hardware
- Real hardware timer ISR for break-beams: wait for hardware
- PI controller: implement after belt speed is measured empirically
- Display SPI: wire up when panel arrives
- Any calibration constants: all placeholders until parts arrive

The stub architecture means none of this blocks you. Every module compiles and runs
in simulation mode. Real hardware slots in by replacing the stub functions.
