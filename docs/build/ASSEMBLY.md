# Full System Assembly Guide

This is the primary assembly guide for the simplified states sorter.

Audience:
- A beginner builder
- A teammate who did not design every part
- A first-time operator preparing the machine for validation

Use this guide with:
- `docs/ARCHITECTURE.md`
- `cad/DIMENSIONS.md`
- `cad/MECHANICAL.md`
- `wiring/ELECTRICAL.md`
- `docs/build/CHECKLIST.md`
- `docs/build/CALIBRATION.md`

## Safety And Hard Rules

1. Use LiPo power for any meaningful run.
2. Keep total footprint at or below 610mm by 610mm.
3. Calibrate color sensing only with the shroud installed.
4. Keep the stepper driver motor-input bulk capacitor installed.
5. Keep wiring clear of the conveyor, chute, servo horn, and bins.
6. Do not add feed automation until one-at-a-time sorting is reliable.

## What This Machine Does

The system sorts 24 LEGO bricks into 4 bins by size and color.

Pipeline summary:
1. Operator places one brick on the conveyor in the marked orientation.
2. Conveyor carries the brick through the size sensor.
3. Conveyor carries the brick through the shrouded color sensor.
4. Firmware classifies the target bin.
5. Servo chute rotates to the target position.
6. Brick exits the belt, enters the chute, and slides into the labeled bin.
7. System returns to READY for the next brick.

## Build Sequence

Follow this order.

1. Conveyor import and frame layout
2. Servo chute angle and pivot prototype
3. Servo mount and bin arc alignment
4. Sensing station and color shroud
5. Bins, labels, and operator UX
6. Wiring and power checks
7. Calibration
8. Full 24-brick reliability runs

## Stage 1: Conveyor And Frame Layout

Goal:
- Establish a working belt path inside the footprint

Steps:
1. Import or model the NEMA17 conveyor.
2. Set belt width around the current 25mm neoprene strip.
3. Set usable top-run length to about 300mm to 400mm.
4. Place the conveyor on a wood frame with printed brackets where useful.
5. Keep the 610mm x 610mm boundary visible.
6. Leave space under the belt exit for chute sweep and bins.

Pass criteria:
- Conveyor fits footprint with bins and chute space reserved
- Motor and belt tension adjustment remain serviceable
- Belt exit can hand off to the chute without a large gap

## Stage 2: Servo Chute Prototype

Goal:
- Prove bricks slide reliably before designing the full selector

Steps:
1. Print a short chute section.
2. Test real bricks at 30, 35, 40, and 45 degrees.
3. Choose the lowest angle that slides reliably.
4. Record the result for the inventor's log.

Pass criteria:
- Brick slides without sticking
- No studs, corners, or edges catch in the channel
- Chosen angle is recorded

## Stage 3: Servo Mount And Bin Arc

Goal:
- Align the rotating chute with all four bin entries

Steps:
1. Fit the MG995/MG996-class servo in its mount.
2. Align the output shaft vertically with the chute pivot.
3. Attach the chute through a horn adapter or pivot boss.
4. Create four construction rays from the pivot.
5. Place the four bins and guides under the chute arc.
6. Rotate the chute to each target position in CAD.

Pass criteria:
- Chute exit overlaps each bin guide
- Servo horn and screws remain accessible
- Chute clears frame and wires across the full sweep

## Stage 4: Sensing Station

Goal:
- Mount sensors without blocking the brick path

Steps:
1. Reserve adjustable space for the size sensor.
2. Install the chosen size sensor only after the decision is documented.
3. Mount the TCS3200/GY-31 color sensor in the shroud.
4. Verify brick clearance through the shroud.
5. Route sensor wiring away from motor and servo wiring where practical.

Pass criteria:
- Size sensor has a clean measurement path
- Color shroud has no obvious light leaks
- Bricks do not catch on sensor brackets
- Color calibration can be repeated without moving the sensor

## Stage 5: Bins, Labels, And Operator UX

Install and verify:
1. Bin labels:
- 2x2 RED
- 2x2 BLUE
- 2x3 RED
- 2x3 BLUE
2. Start control label
3. Display states:
- READY
- SORTING
- SORT COMPLETE
- ERROR
4. Feed orientation cue:
- studs up
- long side along travel
5. Cable routing and strain relief

Goal:
- A first-time evaluator can operate the system without assistance

## Stage 6: Wiring And Power Integration

Follow `wiring/ELECTRICAL.md`.

Checklist:
1. Common ground across logic and motor or servo power rails
2. Stepper motor-input bulk capacitor installed
3. Servo power checked for current capacity
4. Fan installed if driver cooling needs it
5. Safe wire routing away from moving parts
6. Strain relief at moving interfaces

Pre-power checks:
1. No short between rails
2. Correct connector pinouts
3. Servo signal and power polarity verified
4. Conveyor motor direction verified at low speed
5. Sensor readings visible in serial output

## Stage 7: Calibration

Follow `docs/build/CALIBRATION.md`.

Order:
1. Conveyor speed and handoff calibration
2. Servo chute position calibration
3. Size sensor calibration after the sensor choice is made
4. Color threshold calibration with shroud installed
5. Full-system validation runs

Data rule:
- Keep run data in CSV logs for notebook use

## Stage 8: Full-Run Acceptance

Before calling the system ready:
1. Run at least 10 complete 24-brick sets.
2. Log all failures by type and stage.
3. Confirm repeatability, not just one successful run.
4. Verify packaging and footprint compliance again.
5. Verify labels, display states, and feed instructions are installed.

## Common Build Mistakes And Fixes

Belt walks sideways:
- Check roller parallel alignment.
- Reduce over-tension.
- Confirm the belt path is not rubbing a guide.

Brick misses chute:
- Reduce belt-exit gap.
- Raise or widen the chute entry.
- Recheck servo pivot alignment.

Brick sticks in chute:
- Increase chute angle.
- Smooth the channel.
- Recheck width and height clearance.

Color classification unstable:
- Confirm the shroud is installed.
- Recalibrate threshold from fresh sample windows.
- Keep color sensor wiring away from motor wiring where practical.

Servo buzzes or undershoots:
- Check servo power voltage and current capacity.
- Reduce chute friction or horn slop.
- Verify target angles are not hitting hard stops.
