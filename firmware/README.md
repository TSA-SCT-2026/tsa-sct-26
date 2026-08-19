# Firmware

PlatformIO firmware for the ESP32-controlled LEGO brick sorter.

## Build

```sh
pio run
pio run -e servo_tuning
```

The default `esp32dev` environment builds `tsa_sorter/`. The
`servo_tuning` environment builds `tools/servo_tuning/main.cpp`.

## Flash and monitor

```sh
pio run -t upload
pio device monitor -b 115200
```

The serial harness supports component checks, human-readable or CSV logging,
operator-assisted calibration, speed-profile selection, and simulated events.
Run `help` in the monitor for the current command list.

Common calibration commands:

```text
cal show
cal belt 8
set color <ratio>
set size <millimeters>
cal save
```

`cal belt` captures a belt baseline. The operator must still choose the color
and size thresholds from physical-run evidence; the firmware does not derive
them automatically.

Use `tools/capture_serial_log.sh` to retain calibration or acceptance evidence.
See [BUILD.md](../BUILD.md) for the full procedure.

## Structure

- `tsa_sorter/`: production controller, sensing, routing, logging, and harness.
- `tools/servo_tuning/`: independent servo-position utility.
- `tools/capture_serial_log.sh`: serial log capture helper.
