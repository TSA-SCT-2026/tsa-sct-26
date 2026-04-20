# Full System Assembly Guide

This is the primary assembly guide for the simplified states sorter.

Audience:
- A beginner builder
- A teammate or builder who did not design every part
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

## Current Build Phase

CAD is the critical path again. The active mechanical order is bins, then servo chute, then conveyor height and handoff, then permanent woodworking.

Do not start permanent woodworking or fixed-height mounting around the conveyor feet, servo feet, or exit handoff until final bin and chute geometry exists.

## Mechanical Build Order

1. Dry-fit conveyor core.
2. Confirm belt tracking and support clearance.
3. Fit sensing station after the conveyor path is believable.
4. Confirm bin and chute geometry.
5. Derive conveyor feet, servo feet, and exit handoff from the real stack.
6. Cut and drill permanent wood only after the handoff stack is known.
7. Install bins, labels, start control, display, and cable routing.
8. Wire and power-check.
9. Calibrate.
10. Run full 24-brick validation.

## Conveyor Core

Goal:
- Prove the conveyor has a reliable belt path before adding precision sensing and handoff details.

Steps:
1. Assemble side plates, shafts, rollers, belt, support block, motor board, motor, and pulley path.
2. Use real dry-fit alignment instead of old typed coordinates.
3. Confirm the belt tracks without rubbing.
4. Confirm the support block does not touch the return path.
5. Confirm motor and pulley adjustment remain accessible.
6. Confirm wiring exits away from moving paths.
7. Keep exit handoff geometry as a placeholder until bin and chute geometry exists.

Pass criteria:
- Belt path clears rollers, wood support, side plates, and likely screw heads.
- Motor and timing belt adjustment remain serviceable.
- The assembly fits inside the footprint with chute and bin space reserved.
- No permanent holes depend on open geometry.

## Sensing Station

Goal:
- Mount the upstream sensing station without blocking the brick path.

Steps:
1. Mount the shroud near the conveyor start on the wood conveyor bed or a rigid frame member.
2. Keep the first-build side rails separate from the shroud until real-brick centering is proven.
3. Install both break-beam pairs through the shroud side walls.
4. Mount the TCS3200/GY-31 color sensor in the shroud.
5. Verify brick clearance through the shroud.
6. Verify the shroud can be removed without disassembling the conveyor core.
7. Route sensor wiring away from motor and servo wiring where practical.

Pass criteria:
- Size sensor has a clean measurement path.
- Color shroud has no obvious light leaks.
- Bricks do not catch on sensor brackets.
- Color calibration can be repeated without moving the sensor.
- Shroud can be unscrewed for adjustment, cleaning, or reprint.

## Servo Chute And Bins

Goal:
- Align the rotating chute with all four bin entries.

Steps:
1. Fit the MG995/MG996-class servo in its verified mount.
2. Align the output shaft vertically with the chute pivot.
3. Attach the chute through a horn adapter or pivot boss.
4. Install the bins and funnels.
5. Test each servo target position with real bricks.
6. Adjust servo positions from actual catches, not old angle tables.

Pass criteria:
- Chute exit lands inside each bin funnel with margin.
- Servo horn and screws remain accessible.
- Chute clears frame and wires across the full sweep.
- Bins are labeled and removable.

## Operator UX

Install and verify:
1. Bin labels:
- 2x2 RED
- 2x2 BLUE
- 2x3 RED
- 2x3 BLUE
2. Start control label.
3. Display states:
- READY
- SORTING
- SORT COMPLETE
- ERROR
4. Feed orientation cue:
- studs up
- long side along travel
5. Cable routing and strain relief.

Goal:
- A first-time evaluator can operate the system without assistance.

## Wiring And Power Integration

Follow `wiring/ELECTRICAL.md`.

Checklist:
1. Common ground across logic and motor or servo power rails.
2. Stepper motor-input bulk capacitor installed.
3. Servo power checked for current capacity.
4. Fan installed if driver cooling needs it.
5. Safe wire routing away from moving parts.
6. Strain relief at moving interfaces.
7. Electronics placement checked against final conveyor, chute, and bins before permanent mounting.

Pre-power checks:
1. No short between rails.
2. Correct connector pinouts.
3. Servo signal and power polarity verified.
4. Conveyor motor direction verified at low speed.
5. Sensor readings visible in serial output.

## Calibration

Follow `docs/build/CALIBRATION.md`.

Order:
1. Conveyor speed and handoff calibration.
2. Servo chute position calibration.
3. Break-beam size timing calibration.
4. Color threshold calibration with shroud installed.
5. Full-system validation runs.

Data rule:
- Keep run data in CSV logs for notebook use.

## Full-Run Acceptance

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
- Adjust chute angle from UHMW-lined real-brick tests.
- Smooth the channel.
- Recheck clearance.

Color classification unstable:
- Confirm the shroud is installed.
- Recalibrate threshold from fresh sample windows.
- Keep color sensor wiring away from motor wiring where practical.

Servo buzzes or undershoots:
- Check servo power voltage and current capacity.
- Reduce chute friction or horn slop.
- Verify target angles are not hitting hard stops.
