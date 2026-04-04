# Firmware

ESP32 embedded firmware for the sorting system. Written in C++, built with PlatformIO.

## Setup

1. Install PlatformIO (VS Code extension or CLI)
2. Open the `firmware/` directory as the PlatformIO project
3. `pio run` to build, `pio run --target upload` to flash

## Directory structure

```
firmware/
  src/
    main.cpp            entry point, hardware init, main loop
    state_machine.*     state machine: all states and transitions
    sensors.*           size detection (break-beams) and color detection
    actuators.*         selector indexing, release solenoid timing, conveyor feed axis
    events.*            event queue and event types
    logger.*            serial logging
    test_harness.*      serial command test interface
    thermal.*           thermal model for actuators
    display/            display module
  include/              shared headers
  platformio.ini        build config
```

## Architecture

Read `firmware/EMBEDDED.md` before writing any firmware. The state machine, sensor logic, thermal model, and display behavior are specified there.

## Programming structure is scored

Programming Structure is 20 of 100 competition points. Code quality, module separation, and logical organization are evaluated by judges.

Required coding rules:
- Each module does one thing and has a clear interface
- The state machine is the obvious control-flow entry
- Constants and thresholds are named in one place (`config.h`)
- Comments explain why, not what

## Key constraints

- Classification sensing happens only in static `SENSING` state
- Do not document sensing as interrupt-only: Hall speed capture is interrupt-driven, classification sensing is state-driven in `SENSING`
- Display updates must not block sensing or routing
- I2C clock must be set to 400kHz before color sensor communication
- Thermal model is required for actuator protection
- Serial output must be structured logs usable as notebook data

## Logging

During calibration: connect serial at 115200 baud and log CSV output. Every brick event, confirmation, and end-of-run summary should be timestamped.

Run log filename convention:
- `run_YYYYMMDD_HHMM_session_label.csv`

TODO:
- Enforce this filename convention in firmware logging scripts and examples.
