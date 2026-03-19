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
    main.cpp          entry point, hardware init, main loop
    state/            state machine: all states and transitions
    sensors/          size detection (break-beams) and color detection
    actuators/        stepper cam control, solenoid timing, belt motor
    display/          TFT animation and screen management
    thermal/          thermal model for all actuators
  include/            shared headers and constants
  platformio.ini      build config
```

## Architecture

Read EMBEDDED.md before writing any firmware. The state machine, sensor logic, thermal model, and display spec are all documented there. Do not invent new architecture. Implement what is specified, and update the spec if you find a better approach.

## Programming structure is scored

Programming Structure is 20 of 100 competition points. Code quality, module separation, and logical organization are evaluated by judges. This means:
- Each module (state, sensors, actuators, display, thermal) does one thing and has a clear interface
- The state machine is the obvious entry point and control flow is readable
- Constants and thresholds are named and defined in one place, not scattered as magic numbers
- Comments explain why, not what. The code explains what

Write code that a judge reading it for the first time can follow.

## Key constraints

- All sensing happens via hardware interrupts, not polling
- Display updates are asynchronous and must never block sensing or routing
- I2C clock must be explicitly set to 400kHz before any color sensor communication
- The thermal model is not optional: it protects hardware during back-to-back judge demos
- Serial output is structured logging, not debug prints. It becomes the engineering notebook

## Logging

During calibration: connect to serial at 115200 baud and pipe output to a CSV file. Every brick event, confirmation, and end-of-run summary is timestamped. Do not manually record run results.
