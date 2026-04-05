# Full System Assembly Guide

This is the primary assembly guide for the complete sorter.

Audience:
- A beginner builder
- A teammate who did not design every part
- A first-time operator preparing the machine for validation

Use this guide with:
- `docs/ARCHITECTURE.md`
- `cad/DIMENSIONS.md`
- `cad/MECHANICAL.md`
- `wiring/ELECTRICAL.md`
- `docs/CHECKLIST.md`
- `docs/CALIBRATION.md`

## Safety and hard rules

1. Use LiPo power for any meaningful run.
2. Do not skip flyback diodes on solenoids.
3. Do not skip bulk capacitors at the stepper driver motor inputs.
4. Keep total footprint at or below 610mm by 610mm.
5. Keep bricks widthwise across the conveyor.
6. Calibrate color sensing only with the shroud installed.

## What this machine does

The system sorts 24 LEGO bricks into 4 bins by size and color.

Pipeline summary:
1. Bricks start preloaded in a compressed queue.
2. One brick enters the chamber and seats at the stop wall.
3. Belt stops.
4. Size and color sensing happen while static.
5. Selector chute indexes to the target bin.
6. Trapdoor removes support and the brick falls.
7. Bin confirmation verifies arrival.
8. Platform returns level.
9. Next brick advances by chamber pitch after reset truth is satisfied.

## Build sequence

Follow this order.

1. Chute transition validation
2. Conveyor packaging and interface lock
3. Trapdoor gate parts and cycle test
4. Chamber sensing geometry fit
5. Selector chute alignment and routing-cost study
6. Bin and drop-path fit check
7. Full mechanical packaging and labels
8. Wiring and power checks
9. Calibration
10. Full 24-brick reliability runs

## Stage 1: Chute transition first

Goal:
- Prove reliable feed before broader printing

Steps:
1. Print chute transition prototype only.
2. Deburr and smooth contact surfaces.
3. Feed real bricks under expected stack load.
4. Verify no jam and no double-feed behavior.

Pass criteria:
- Zero jams in 50 feed attempts
- Zero double-feed events in 50 feed attempts

## Stage 2: Conveyor packaging and interface lock

Goal:
- Lock the production conveyor path before chamber and frame work drift

The active conveyor path is:
- NEMA17 motor
- Toothed timing-belt stage
- Supported drive roller shaft
- Smooth drive roller
- Neoprene conveyor belt

Steps:
1. Run roller and fit coupons first.
2. Package the motor, pulleys, belt envelope, supported shaft, bearings, idler, and tension adjust travel.
3. Confirm chamber interface height and belt plane.
4. Confirm service access, belt guard clearance, and cable-routing path.
5. Confirm footprint impact before printing larger structural parts.

Pass criteria:
- No hard interference in packaging CAD
- Shaft support and tension ranges documented
- Conveyor module fits the full-system envelope with margin

## Stage 3: Trapdoor mechanism gate

Parts:
- Platform
- Hinge bracket
- Lever arm
- Lever pivot bracket
- Solenoid bracket

Steps:
1. Assemble hinge with 3mm rod.
2. Verify free platform pivot.
3. Install lever and pivot fastener.
4. Verify lever tip engages the platform tab at rest.
5. Install solenoid and return springs.
6. Trigger repeated loaded release cycles.

Pass criteria:
- Zero failed drops in 50 loaded cycles
- Zero failed re-latch cycles in 50 loaded cycles
- Return to level under target timing

## Stage 4: Chamber sensing fit

Goal:
- Freeze chamber interfaces before wiring is finalized

Steps:
1. Print chamber fit parts.
2. Fit size beam mounts at target chamber positions.
3. Fit color sensor window and shroud.
4. Install stop switch and verify actuator travel.
5. Verify platform-level sensing path if used.
6. Insert real bricks and check motion clearance.

Pass criteria:
- No forced sensor fit
- No light leaks around the shroud
- No brick obstruction during seat or release

## Stage 5: Selector chute alignment and routing study

Goal:
- Guarantee a clean drop path and quantify selector cost honestly

Steps:
1. Print selector chute prototype.
2. Mount on 5mm aluminum hub.
3. Mount to the stepper and homing hardware.
4. Cycle each selector position.
5. Perform repeated drop tests per bin.
6. Record adjacent, worst-case, and weighted selector motion cost for the real brick mix.

Pass criteria:
- Clean path in all four positions
- No edge catch in at least 25 drops per bin
- Selector gate study created for the notebook

## Stage 6: Bins, labels, and operator UX

Install and verify:
1. Bin labels:
- 2x2 RED
- 2x2 BLUE
- 2x3 BLUE
- 2x3 RED
2. Start control label
3. Display states:
- READY
- SORTING
- SORT COMPLETE
- ERROR
4. Orientation cue at chute entrance:
- studs up
- length along travel axis
5. Cable routing and strain relief

Goal:
- A first-time evaluator can operate the system without assistance

## Stage 7: Wiring and power integration

Follow `wiring/ELECTRICAL.md`.

Checklist:
1. Common ground across logic and actuator rails
2. Flyback diode polarity on the solenoid path
3. Stepper motor-input bulk capacitors installed
4. Safe wire routing away from moving parts
5. Strain relief at moving interfaces

Pre-power checks:
1. No short between rails
2. Correct connector pinouts
3. MOSFET orientation verified
4. Home, stop, and reset-related sensors actuate correctly

## Stage 8: Calibration

Follow `docs/CALIBRATION.md`.

Order:
1. Lever release calibration
2. Chamber seat and pitch-advance timing calibration
3. Color threshold calibration with shroud installed
4. Selector timing and routing study
5. Full-system validation runs

Data rule:
- Keep run data in CSV logs for notebook use

## Stage 9: Full-run acceptance

Before calling the system ready:
1. Run at least 10 complete 24-brick sets.
2. Log all failures by type and stage.
3. Confirm repeatability, not just one successful run.
4. Verify packaging and footprint compliance again.
5. Verify the selector evidence gate decision is recorded.

## Common build mistakes and fixes

Belt walks sideways:
- Check roller parallel alignment.
- Reduce over-tension.
- Confirm crown orientation and quality.

Lever fails to re-latch:
- Check tip and tab chamfer contact.
- Check return spring preload.
- Check pivot friction and binding.

Color classification unstable:
- Confirm the shroud is installed.
- Recalibrate threshold from fresh sample windows.
- Shield color sensor wiring from motor wiring.

Chute feed jams:
- Reinspect transition geometry.
- Verify printed clearances with calipers.
- Smooth friction points.

## Where to put subsystem assembly guides

Use this pattern:
1. `docs/ASSEMBLY.md` for full-system workflow
2. Subsystem assembly guides in subsystem folders

Current subsystem guide:
- `cad/frame/rollers/ASSEMBLY.md`
