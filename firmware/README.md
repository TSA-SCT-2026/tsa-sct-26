# Firmware

ESP32 firmware for the simplified states LEGO sorter. Build with PlatformIO.

## Setup

1. Install PlatformIO
2. Open `firmware/` as the PlatformIO project
3. Run `pio run` to build
4. Run `pio run -t upload` to flash

## Terminology

- `sensing station`: the conveyor-mounted size and color sensing area before the belt exit
- `servo chute`: the MG995/MG996-class rotary chute that aims at one of four bins
- `route ready`: the servo has reached the target chute angle
- `handoff`: the brick leaves the belt and enters the chute
- `timed confirmation`: states-build confirmation after the handoff window completes
- `bin confirmed`: optional future instrumentation for bin beams

## Architecture

Read `firmware/EMBEDDED.md` first. The active control contract is:

`START_BUTTON -> BRICK_DETECTED -> SENSING_DONE -> ROUTE_READY -> HANDOFF_DONE -> timed confirmation`

`RESET` returns the machine from COMPLETE or ERROR to READY.

## Key Constraints

- Size sensing is break-beam timing with two pairs in the shroud
- Color calibration is valid only with the shroud installed
- Servo route must be ready before the brick commits to the chute
- Manual one-at-a-time feed is the active states flow
- Bin confirmation beams are deferred for states unless the base sorter is already reliable
- Logging should stay CSV-first so notebook data can be captured directly from serial

## Logging

Default logger mode is CSV. Use the serial harness to switch to human-readable logs when needed:

- `log csv`
- `log human`

Run logs should be stored under `docs/runs/` with the project naming convention.
