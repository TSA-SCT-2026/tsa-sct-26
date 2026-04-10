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
- `docs/build/CHECKLIST.md`
- `docs/build/CALIBRATION.md`

## Safety and hard rules

1. Use LiPo power for any meaningful run.
2. Do not skip flyback diodes on coil-based actuators.
3. Do not skip bulk capacitors at the stepper driver motor inputs.
4. Keep total footprint at or below 610mm by 610mm.
5. Keep bricks long-side-across the conveyor.
6. Calibrate color sensing only with the shroud installed.

## What this machine does

The system sorts 24 LEGO bricks into 4 bins by size and color.

Pipeline summary:
1. Bricks start preloaded in a compressed queue.
2. One brick enters the chamber and seats at the stop wall.
3. Belt stops.
4. Size and color sensing happen while static.
5. Selector chute indexes to the target bin.
6. Release gate removes support and the brick falls.
7. Bin confirmation verifies arrival.
8. Reset state is confirmed.
9. Next brick advances by chamber pitch after reset truth is satisfied.

## Build sequence

Follow this order.

1. Chute transition validation
2. Conveyor packaging and interface lock
3. Release mechanism prototype and cycle test
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

## Stage 3: Release mechanism gate

Parts:
- Release mechanism prototype parts, still provisional until the refactor is frozen

Steps:
1. Assemble the release mechanism prototype selected for the current refactor direction.
2. Verify the release path clears support without binding.
3. Verify the reset path returns to a safe state.
4. Confirm the actuation hardware stays outside the chamber envelope.
5. Trigger repeated loaded release cycles.

Pass criteria:
- Zero failed drops in 50 loaded cycles
- Zero failed reset cycles in 50 loaded cycles
- Return to safe state under target timing

## Stage 4: Chamber sensing fit

Goal:
- Freeze chamber interfaces before wiring is finalized

Steps:
1. Print chamber fit parts.
2. Fit dual ToF mounts on the stop wall and verify their sightlines to the chamber edge zones.
3. Fit color sensor window and shroud.
4. Install the required stop-wall seat switch and verify actuator travel.
5. Keep the release-return flag and switch-mount provision in the printed parts.
6. Install the release-return switch only if the first release prototype cannot provide physical reset truth another way.
7. Insert real bricks and check motion clearance.

Pass criteria:
- No forced sensor fit
- Both ToF modules have clean line of sight and cable clearance
- No light leaks around the shroud
- No brick obstruction during seat or release

## Stage 5: Selector chute alignment and routing study

Goal:
- Guarantee a clean drop path and quantify selector cost honestly

Steps:
1. Print selector chute prototype.
2. Mount on 5mm hub or clamp interface.
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
2. Release actuator protection matches the chosen mechanism
3. Stepper motor-input bulk capacitors installed
4. Safe wire routing away from moving parts
5. Strain relief at moving interfaces

Pre-power checks:
1. No short between rails
2. Correct connector pinouts
3. MOSFET orientation verified
4. Home and stop-related sensors actuate correctly

## Stage 8: Calibration

Follow `docs/build/CALIBRATION.md`.

Order:
1. Release mechanism calibration
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

Release mechanism fails to reset:
- Check release path clearance.
- Check return bias or any reset element.
- Check friction, binding, and actuator travel.

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
1. `docs/build/ASSEMBLY.md` for full-system workflow
2. Subsystem assembly guides in subsystem folders

Current subsystem guide:
- `cad/frame/rollers/ASSEMBLY.md`
