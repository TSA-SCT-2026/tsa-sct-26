# Firmware

ESP32 firmware for the queue-fed chamber sorter. Build with PlatformIO.

## Setup

1. Install PlatformIO
2. Open `firmware/` as the PlatformIO project
3. Run `pio run` to build
4. Run `pio run -t upload` to flash

## Terminology

- `selector chute`: the 4-index downstream routing mechanism. Firmware does not require it to be a circular disc.
- `indexed position`: one of the 4 deterministic selector positions mapped to the 4 output bins.
- `selector ready`: the move to the target indexed position completed and is safe for release.
- `drop window`: the bounded release-and-fall interval before bin confirmation is expected.
- `platform level`: the reset truth that allows the next queued brick to enter the chamber.
- `long-side-across`: the brick orientation assumption that keys the chamber footprint to the long side before final geometry is frozen.

## Architecture

Read `firmware/EMBEDDED.md` first. The active control contract is:

`START_BUTTON -> ENTRY_DETECTED -> CHAMBER_SEATED -> SENSING_DONE -> SELECTOR_READY -> DROP_WINDOW_DONE -> BIN_CONFIRMED -> PLATFORM_LEVEL`

Optional future events remain available for tighter instrumentation:
- `CHAMBER_CLEAR`
- `PITCH_ADVANCE_DONE`

## Key constraints

- Classification happens only while the brick is static in the chamber
- Chamber size is determined from dual rear-wall ToF readings, not cross-chamber break-beams
- The selector chute must provide 4 deterministic positions plus a home reference
- Release is allowed only after selector-ready truth
- Feed restart is allowed only after platform-level truth
- Logging should stay CSV-first so notebook data can be captured directly from serial

## Logging

Default logger mode is CSV. Use the serial harness to switch to human-readable logs when needed:

- `log csv`
- `log human`

Run logs should be stored under `docs/runs/` with the project naming convention.
