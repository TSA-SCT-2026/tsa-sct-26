# Full System Assembly Guide

This is the primary assembly guide for the complete sorter.

Audience:
- A beginner builder
- A teammate who did not design every part
- A first time operator preparing the machine for validation

Use this guide with:
- `docs/ARCHITECTURE.md`
- `cad/DIMENSIONS.md`
- `cad/MECHANICAL.md`
- `wiring/ELECTRICAL.md`
- `docs/CHECKLIST.md`
- `docs/CALIBRATION.md`

This guide follows the current CAD first build strategy:
1. Validate highest risk geometry first
2. Freeze interfaces
3. Assemble complete mechanism
4. Wire and calibrate
5. Run repeatability tests

## Safety and hard rules

1. Use LiPo power for any meaningful run.
2. Do not skip flyback diodes on solenoids.
3. Do not skip bulk capacitor at stepper motor input.
4. Keep total footprint at or below 610mm by 610mm.
5. Keep bricks widthwise across conveyor.
6. Calibrate color sensing only with shroud installed.

## What this machine does

The system sorts 24 LEGO bricks into 4 bins by size and color.

Pipeline summary:
1. Bricks feed from chute to narrow belt.
2. One brick enters chamber and seats at stop wall.
3. Belt stops.
4. Size and color sensing happens while static.
5. Selector indexes to target bin.
6. Trapdoor releases brick by removing support.
7. Brick falls to selected bin.
8. Platform and lever reset.
9. Belt restarts for next brick.

## Build sequence

Follow this order. Do not jump ahead.

1. Chute transition validation
2. Trapdoor gate parts and cycle test
3. Chamber sensing geometry fit
4. Selector disc alignment test
5. Belt channel and roller module
6. Bin and drop path fit check
7. Full mechanical packaging and labels
8. Wiring and power checks
9. Calibration
10. Full 24 brick reliability runs

## Stage 1: Chute transition first

Goal:
- Prove reliable feed before broader printing

Steps:
1. Print chute transition prototype only.
2. Deburr and smooth contact surfaces.
3. Feed real bricks under expected stack load.
4. Verify no jam and no double feed behavior.

Pass criteria:
- Zero jams in 50 feed attempts
- Zero double feed events in 50 feed attempts

## Stage 2: Trapdoor mechanism gate

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
4. Verify lever tip engages platform tab at rest.
5. Install solenoid and return springs.
6. Trigger release cycle repeatedly.

Pass criteria:
- Zero failed drops in 50 loaded cycles
- Zero failed re latch in 50 loaded cycles
- Return to level under 200ms target

## Stage 3: Chamber sensing fit

Goal:
- Freeze mechanical interfaces before wiring is finalized

Steps:
1. Print chamber fit parts.
2. Fit size beam mounts at target X positions.
3. Fit color sensor window and shroud.
4. Install stop switch and verify actuator travel.
5. Insert real bricks and check motion clearance.

Pass criteria:
- No forced sensor fit
- No light leaks around shroud
- No brick obstruction during seat and release

## Stage 4: Selector disc alignment

Goal:
- Guarantee clear drop path for all bin targets

Steps:
1. Print selector disc prototype.
2. Mount on 5mm aluminum hub.
3. Mount to stepper and home position hardware.
4. Cycle each selector position.
5. Perform repeated drop tests per bin.

Pass criteria:
- Clean path in all four positions
- No edge catch in at least 25 drops per bin

## Stage 5: Belt and roller module

This stage uses the roller subsystem guide:
- `cad/frame/rollers/ASSEMBLY.md`

CAD file layout for this subsystem:
- `src/` holds OpenSCAD source
- `scripts/` holds build helpers
- `stl/` holds exported print files

Key points:
1. Print and test coupons first.
2. Choose final D bore and bearing fit from coupon results.
3. Print final drive pulley and idler.
4. Build split clamp hub with the offset M3 screw and hex nut hardware.
5. Set belt tension and confirm stable tracking.

Pass criteria:
- Pulley does not slip on shaft
- Idler spins freely with no wobble
- Belt tracks centered during run

## Stage 6: Bins, labels, and operator UX

Install and verify:
1. Bin labels:
- 2x2 RED
- 2x2 BLUE
- 2x3 BLUE
- 2x3 RED
2. Start control label.
3. Display states:
- READY
- SORTING
- SORT COMPLETE
- ERROR
4. Orientation cue at chute entrance:
- studs up
- length along travel axis

Goal:
- A first time evaluator can operate without assistance

## Stage 7: Wiring and power integration

Follow `wiring/ELECTRICAL.md`.

Checklist:
1. Common ground across logic and actuator rails.
2. Flyback diode polarity on solenoid path.
3. Stepper motor input bulk capacitor installed.
4. Safe wire routing away from moving parts.
5. Strain relief at moving interfaces.

Pre power checks:
1. No short between rails.
2. Correct connector pinouts.
3. MOSFET orientation verified.
4. Home and stop switches actuate correctly.

## Stage 8: Calibration

Follow `docs/CALIBRATION.md`.

Order:
1. Lever release calibration
2. Color threshold calibration with shroud installed
3. Belt approach and settle timing
4. Full system validation runs

Data rule:
- Keep run data in CSV logs for notebook use.

## Stage 9: Full run acceptance

Before calling the system ready:
1. Run at least 10 complete 24 brick sets.
2. Log all failures by type and stage.
3. Confirm repeatability, not just one successful run.
4. Verify packaging and footprint compliance again.

## Split clamp beginner note

The drive pulley uses a split clamp hub.

What it does:
- Lets the pulley grip a D shaft with controlled pressure.

Why this is used:
- Printed bores vary between prints.
- Clamp tension lets you tune grip without reprinting.

Important clarification:
- This is for the conveyor drive motor pulley.
- The conveyor uses the NEMA 17 feed stepper.
- The selector disc uses a separate NEMA 11 stepper.

Hardware needed:
- M3 screw
- M3 hex nut

Installation tip:
- Tighten in small steps while checking pulley runout.

## Common build mistakes and fixes

Belt walks sideways:
- Check roller parallel alignment.
- Reduce over tension.
- Confirm crown orientation and quality.

Lever fails to re latch:
- Check tip and tab chamfer contact.
- Check return spring preload.
- Check pivot friction and binding.

Color classification unstable:
- Confirm shroud is installed.
- Re calibrate threshold from fresh sample windows.
- Shield color sensor wiring from motor wiring.

Chute feed jams:
- Re inspect transition geometry.
- Verify printed clearances with calipers.
- Smooth friction points.

## Where to put subsystem assembly guides

Use this pattern:
1. `docs/ASSEMBLY.md` for full system workflow.
2. Subsystem assembly guides in subsystem folders.

Current subsystem guide:
- `cad/frame/rollers/ASSEMBLY.md`

This keeps system flow in one place while preserving deep instructions where parts live.
