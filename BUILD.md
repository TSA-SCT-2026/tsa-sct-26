# Build, Wiring, and Calibration

This guide consolidates the active assembly, electrical, and calibration
instructions. Verify every dimension and connection against the physical
machine before applying power.

## Mechanical assembly

### 1. Frame and conveyor

1. Build the frame within the event footprint limit.
2. Mount the conveyor side plates, shaft supports, and bearings.
3. Install the rollers, GT2 belt and pulleys, NEMA17 motor, and tensioning
   hardware.
4. Turn the system by hand and correct belt tracking or binding before power is
   connected.

### 2. Sensing station

1. Mount two break-beam pairs across the conveyor at a measured spacing.
2. Mount the TCS3200/GY-31 above the belt inside a light-blocking shroud.
3. Add low guides so each brick enters studs up and aligned with travel.
4. Confirm both beam transitions and all three color-filter readings over
   serial before continuing.

### 3. Chute and bins

1. Rigidly mount the MG995/MG996-class servo at the belt exit.
2. Attach the rotary chute and check clearance through the full angle range.
3. Add a low-friction surface where needed.
4. Position and label the bins in firmware order: `2x2 RED`, `2x3 RED`,
   `2x3 BLUE`, `2x2 BLUE`.

## Electrical

The design uses a fused 3S LiPo input, a motor rail for the stepper driver, a
buck-converted 5 V rail for suitable peripherals, and a common ground. Confirm
the real regulator rating, servo current path, fuse, wire gauge, polarity, and
grounding before connecting the battery.

### Firmware pin map

| Function | ESP32 GPIO | Notes |
| --- | ---: | --- |
| Conveyor STEP | 25 | LEDC-generated pulse output |
| Conveyor DIR | 27 | Forward/reverse level from `config.h` |
| Conveyor ENABLE | 26 | Active low; clamped disabled at startup |
| TMC UART | 23 | Reserved in configuration |
| Chute servo PWM | 32 | MG995/MG996-class servo |
| Break-beam A | 33 | Active-low input with pull-up |
| Break-beam B | 15 | Active-low input with pull-up |
| TCS3200 OUT | 34 | Input-only GPIO; external pull-up as required |
| TCS3200 S2 | 17 | Color-filter select |
| TCS3200 S3 | 18 | Color-filter select |
| Buzzer | 14 | Error indication |

The firmware comments specify that TCS3200 S0 is tied to 3.3 V, S1 to ground,
and OE to ground. GPIO 34 is input-only. Validate these choices against the
actual sensor board and wiring before use.

Pins 21/22 and PCF8574 address `0x20` remain reserved in configuration, but the
current display functions are log-only. Do not treat a display wiring plan as
implemented firmware.

> **Wiring placeholder:** add `hardware/wiring.png` only after checking it
> wire-by-wire against both the assembled sorter and `firmware/tsa_sorter/config.h`.

## Firmware

Install PlatformIO, then build from the repository root:

```sh
cd firmware
pio run
pio run -e servo_tuning
```

To flash and monitor the main controller:

```sh
pio run -t upload
pio device monitor -b 115200
```

Use `firmware/tools/servo_tuning/` when centering the chute or establishing bin
angles. Copy confirmed angles into `firmware/tsa_sorter/config.h`, rebuild, and
repeat the physical clearance check.

## Calibration

Calibration is operator-assisted. The firmware stores values but does not fit
color or size thresholds from the recorded clusters.

### 1. Inspect defaults and capture the belt

With the final belt and shroud installed, open the serial monitor and run:

```text
cal show
cal belt 8
cal show
```

The belt capture averages valid red, blue, and clear readings for later
subtraction. If samples are invalid, correct lighting, alignment, or wiring
before proceeding.

### 2. Select the size threshold

1. Feed several 2x2 and 2x3 bricks at the intended conveyor setting.
2. Capture the serial output with `firmware/tools/capture_serial_log.sh` or a
   terminal logger.
3. Review measured length and skew; exclude physically explained outliers.
4. Choose a threshold that separates the stable clusters and enter it:

```text
set size <millimeters>
```

### 3. Select the color threshold

1. Record several moving red and blue passes with the shroud installed.
2. Review the belt-subtracted `red / (red + blue)` values.
3. Choose a separating threshold and enter it:

```text
set color <ratio>
```

### 4. Tune chute angles

1. Build and flash the `servo_tuning` environment.
2. Move to each bin position with commands `1` through `4`, or enter an angle
   from `0` to `180`.
3. Test with the longest supported brick and record the centered angle for each
   bin.
4. Update `config.h`, rebuild the main environment, and repeat the drops.

### 5. Save and verify

After reviewing all selected values:

```text
cal save
cal show
```

`cal save` persists the thresholds and belt baseline to ESP32 NVS. Use
`cal wipe` only when intentionally returning to the compiled defaults.

## Acceptance evidence

Run the configured set of 24 bricks in the documented orientation and retain:

- an uninterrupted video showing feed, sensing, chute, and bins,
- the corresponding serial/CSV log,
- the selected calibration values and speed mode,
- the count of correct landings, misroutes, retries, and incomplete passes, and
- enough timing data to calculate throughput from the same run.

Do not label the run `24/24`, publish an accuracy percentage, or publish a
throughput number until those artifacts have been reviewed together.

## Remaining verification

- TODO: validate and add the real wiring diagram.
- TODO: confirm the exact final frame dimensions.
- TODO: perform upload, power-integrity, sensor, servo, and full-run checks on
  the physical machine.
