# Build, Wiring, and Calibration

This guide consolidates the active assembly, electrical, and calibration
instructions. Verify every dimension and connection against the physical
machine before applying power.

## Constraint-driven mechanical design

I worked outward from the ground-truth objects and components the sorter had to
handle. LEGO brick width and travel orientation constrained the usable belt
width and guide clearance. That conveyor envelope constrained the sensing
station and the chute mouth; the chute sweep then set the four-bin arc, spacing,
and required collection volume. The servo foot placed the chute at a reliable
drop point, which set the conveyor height, NEMA17 support height, and reserved
electronics-underbody envelope.

The CAD therefore formed one dependency chain:

```text
brick geometry -> belt/guides -> sensing + chute mouth -> bin arc/volume
               -> servo support -> conveyor height -> motor + electronics envelope
```

Major interfaces were aligned before fabrication, reducing unnecessary
reprints and assembly rework. Slots and extra clearance were retained at real
mounting interfaces so the physical system could absorb millimeter-scale build,
belt-tension, and placement error.

## Build progression

The first two to three weeks combined sketches, dimensional calculations,
component selection, ordering, and CAD with work on another project. Most
fabrication, electronics integration, firmware, calibration, and debugging then
landed in a concentrated final two-week sprint; the constraint chain above kept
that rapid assembly from becoming a sequence of avoidable reprints.

| 1. Frame and rollers | 2. Belt curing and alignment | 3. Electronics integration |
| --- | --- | --- |
| ![Wooden frame with 3D-printed conveyor rollers](media/build/01-frame-and-rollers.jpg) | ![Dumbbell holding the stapled and taped belt assembly while epoxy cures](media/build/02-belt-curing-and-alignment.jpg) | ![Conveyor after wiring and electronics integration](media/build/03-electronics-integration.jpg) |
| Wooden boards and printed rollers establish the conveyor frame. | The dumbbell is used to cure the epoxy+staple joint | Later integration stage after the wiring was completed. |

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

### Component fit checks

The following manual measurements from the April 2026 build notes were used as
fit checks during mechanical design:

| Component | Manual measurement | Build implication |
| --- | --- | --- |
| NEMA17 | 38.1 mm body; 20.54 mm observed shaft stickout; 5 mm D-shaft class | Verify the actual motor before cutting the mount: the retained supplier drawing shows a 23.5 +/- 0.5 mm shaft envelope, so the archived sources disagree. |
| TCS3200/GY-31 module | 33.34 x 33.34 mm board; 25.4 mm overall height including pins and LEDs | Leave shroud roof and underside pin clearance; do not dimension from the bare sensor package. |
| IR break-beam bodies | 19.05 mm long and 9.53 mm thick in the manual note | Check both sides of the shroud for body and cable clearance after setting the measured beam spacing. |
| TMC2209 carrier | 20.64 x 14.29 mm board; 23.81 mm overall height with pins and heatsink | Reserve vertical cooling and connector clearance around the driver carrier. |

The original NEMA17 supplier schematic, color-sensor module manual, and related
reference files are indexed in [docs/datasheet/README.md](docs/datasheet/README.md).
Earlier NEMA11 and solenoid measurements are not relevant to the final design.

### 3. Chute and bins

1. Rigidly mount the MG995/MG996-class servo at the belt exit.
2. Attach the rotary chute and check clearance through the full angle range.
3. Add a low-friction surface where needed.
4. Position and label the bins in firmware order: `2x2 RED`, `2x3 RED`,
   `2x3 BLUE`, `2x2 BLUE`.

## Electrical

I planned the power distribution and signal paths before physical wiring. The
design places a fuse immediately after the 3S LiPo, keeps the stepper driver on
the 12 V motor path, and uses buck conversion for the 5 V ESP32/peripheral path;
the fuse limits fault current while rail separation reduces the risk of
over-voltage damage to lower-voltage electronics.

| Power and system plan | Motor and sensor detail |
| --- | --- |
| ![Hand-drawn pre-build power and signal wiring plan](media/build/04-power-and-wiring-plan.png) | ![Hand-drawn pre-build stepper and color-sensor wiring plan](media/build/05-motor-and-sensor-wiring-plan.png) |
| Fused input, voltage rails, controller, servo, stepper, color sensor, and break-beam paths. | Planned TMC2209/stepper and TCS3200 connections before assembly. |

These pre-build sketches capture the planned power and signal layout. Use the
current firmware pin map below and verify the physical machine before applying
power.

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

## Working demo and test workflow

[Watch the completed sorter run on YouTube](https://www.youtube.com/watch?v=zOtNFu6YxYQ). The final assembled-system photo is available
[here](media/final/sorter-final.jpg).

For a repeatable test session, use a consistent feed orientation and retain the
serial/CSV log alongside the selected calibration profile and speed mode. The
firmware helper at `firmware/tools/capture_serial_log.sh` records the serial
output for later review.
