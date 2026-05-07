# Firmware

ESP32 firmware for the LEGO brick sorter.

## Build And Flash

```sh
cd firmware
pio run
pio run -t upload
pio device monitor
```

The main firmware lives in `tsa_sorter/`.

## Commands

Use the serial monitor for:

- servo angle tuning
- break-beam size logging
- color sensor sampling
- full 24-brick run logging

Capture calibration runs as CSV with `tools/capture_serial_log.sh`.

## Structure

- `tsa_sorter/`: sorter firmware.
- `tools/`: serial logging helpers.
