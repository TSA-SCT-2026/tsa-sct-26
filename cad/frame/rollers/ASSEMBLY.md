# Rollers Assembly Guide

This guide is for beginners and focuses on the parts in `cad/frame/rollers`.

Use this when you are assembling:
- Timing pulley on the motor shaft
- Supported drive roller on the conveyor shaft
- Idler roller on the axle
- Belt tracking test setup

Read this with:
- `cad/frame/rollers/README.md`
- `cad/frame/rollers/docs/`
- `cad/DIMENSIONS.md`

## What each STL does

`motor_pulley_proto_v1.stl`
- This is the timing pulley on the NEMA17 motor shaft.
- It drives the toothed belt stage.
- It uses a shaft-fit bore and a split clamp so it can grip the shaft.
- It is a timing envelope model, not a final tooth profile.

`drive_roller_proto_v1.stl`
- This is the smooth roller on the supported conveyor shaft.
- It contacts the neoprene conveyor belt.
- It uses a shaft-fit bore and a split clamp so it can grip the shaft.
- It has side flanges to help keep the belt centered.

`idler_roller_proto_v1.stl`
- This is the free spinning roller on the other end.
- It has a slight crown in the middle.
- The crown helps the belt self center.
- It has bearing pockets for MR85 bearings.
- It has optional magnet pockets for diagnostic Hall sensing.

`coupon_bearing_fit_v1.stl`
- This is a test block for bearing fit.
- You press a real bearing into each test hole.
- You pick the hole size that gives secure fit without damage.

`coupon_shaft_fit_v1.stl`
- This is a test block for shaft fit.
- Each bore version has a different shaft-fit size.
- You pick the bore that slides on by hand and does not wobble.

`coupon_crown_v1.stl`
- This is a test strip for crown height.
- It lets you compare belt tracking behavior before printing full rollers.

## What the split clamp is

The split clamp is a slot in the hub that lets the hub squeeze closed.

Why it exists:
- Printed plastic bores can vary by printer and material.
- The clamp gives controlled grip on the shaft.
- It is more reliable than friction only fit.

How it works:
1. The hub has a split gap.
2. A screw crosses the gap beside the shaft bore, not through the shaft center.
3. Tightening the screw pulls both sides together.
4. The bore grips the shaft.

Beginner picture in words:
- Imagine the shaft hole in the middle.
- Now imagine a thin slit from that hole out to one side of the hub.
- The screw is next to that hole, crossing the slit.
- Tightening the screw pinches the slit closed a small amount.
- That small motion is what grips the shaft.

## Extra hardware needed for split clamp

Yes, it needs extra hardware.

Minimum hardware:
- One M3 socket head clamp screw
- One M3 hex nut

Recommended:
- M3 screw length chosen so full threads engage the nut without excess overhang
- Test fit hardware in the printed recesses before final assembly

## Hardware needed for idler assembly

- Two MR85ZZ bearings
- One M5 axle bolt or shaft
- Washer and nut set for your frame style
- Two 3mm magnets only if you choose to keep diagnostic Hall sensing

## Hardware needed for the timing stage

- One NEMA17 motor pulley, either purchased or printed to the envelope here
- One supported conveyor shaft with bearings in the frame
- One smooth drive roller on the supported shaft
- One toothed timing belt with the chosen tooth count ratio
- One tension adjustment feature with the travel called out in `cad/DIMENSIONS.md`

## Print setup for first assembly pass

Use your current baseline, then tune from coupons:
- Layer height: 0.2mm
- Material: PLA
- Walls: at least 4
- Infill:
- Motor pulley and drive roller: 100 percent
- Idler: 80 to 100 percent
- Orientation:
- Print rollers standing on axis if bore and bearing circularity are priority
- Print coupons flat

## Step by step assembly
 
### Step 1: Run coupon fits first

1. Print all coupon STL files.
2. Test bearing coupon with real MR85 bearings.
3. Test the shaft-fit coupon with the real shafts that will carry the timing pulley and drive roller.
4. Test crown coupon with your belt strip.
5. Record which option gives best fit and tracking.

Do not skip this step. It is the fastest way to avoid failed full size prints.

### Step 2: Print final timing stage parts

1. If needed, update `src/rollers_params.scad`.
2. Run `./scripts/build_rollers.sh`.
3. Print `motor_pulley_proto_v1.stl`, `drive_roller_proto_v1.stl`, and `idler_roller_proto_v1.stl`.

### Step 3: Build drive roller

1. Slide the drive roller onto the supported shaft.
2. Align the roller centerline to the conveyor channel centerline.
3. Insert the clamp hardware and tighten gradually while checking rotation.
4. Verify the roller does not slip under expected belt load.

Pass target:
- Roller spins true.
- No hub crack at split line.
- No shaft slip under expected belt load.

### Step 4: Build idler roller

1. Press first MR85 bearing into one side pocket.
2. Press second MR85 bearing into the opposite side pocket.
3. Confirm both bearings seat square and flush to pocket shoulders.
4. Add magnets to pockets only if using diagnostic Hall sensing.
5. Place idler on M5 axle and check free spin.

Pass target:
- Free spin is smooth.
- No scraping noise.
- No visible wobble.

### Step 5: Build timing pulley

1. Slide the timing pulley onto the motor shaft.
2. Align pulley so the belt will run centered in the timing stage.
3. Press an M3 hex nut into the recessed nut trap.
4. Insert the M3 screw from the recessed head side.
5. Tighten gradually while checking rotation.
6. Verify pulley does not slip by applying light motor torque.

Pass target:
- Pulley spins true.
- No hub crack at split line.
- No shaft slip under expected belt load.

### Step 6: Install belt and align

1. Mount both rollers in frame.
2. Install belt strip and set tension.
3. Run motor at low speed first.
4. Watch belt position for 30 to 60 seconds.
5. Adjust alignment or tension if belt walks to one side.

Pass target:
- Belt stays centered during run.
- Belt does not rub walls or flanges continuously.
- Tensioner travel stays inside the documented range.

## Validation checks before system integration

Mechanical checks:
- Motor pulley clamp remains tight after repeated start stop cycles.
- Idler bearings remain seated.
- No abnormal wear dust near flanges.
- Supported shaft bearings remain aligned.

Tracking checks:
- Belt remains centered across low and moderate speed.
- Crown behavior is stable with expected belt tension.
- Center distance and ratio stay within the model envelope.

Electrical checks:
- Hall sensor reads clean pulses if optional diagnostic magnets are installed.
- No contact between moving belt and sensor body.

## Common beginner issues and fixes

Bore too tight:
- Use the coupon-selected larger shaft-fit size.
- Clean first layer artifacts.

Bore too loose:
- Use the coupon-selected smaller shaft-fit size.
- Increase clamp grip, or increase hub wall thickness in params.

Bearing too tight:
- Use coupon selected larger bearing pocket.
- Add light chamfer cleanup only if needed.

Bearing too loose:
- Use coupon selected smaller pocket.
- If urgent, use retaining compound as temporary measure.

Belt walks to one side:
- Check roller parallel alignment.
- Check crown orientation and magnitude.
- Reduce over tension.

Hub crack near split:
- Increase hub outer diameter or wall thickness.
- Use lower clamp torque.
- Verify the nut trap is not forcing the hub open before tightening.

Motor pulley chatters under load:
- Check belt alignment and center distance.
- Reduce over tension.
- Confirm the pulley ratio matches the timing-stage model.

I do not understand why we use a split clamp:
- It is okay to ask that. The reason is repeatability, not complexity for its own sake.
- A plain printed shaft bore can work for a test print, but if it comes out a little loose you have to reprint.
- A split clamp gives one extra adjustment after printing.
- For a competition machine, that adjustability is useful.
- If you want the simplest possible prototype, you can print the pulley and test the shaft fit first without clamp hardware. For the competition version, keep the split clamp.

## Document what you choose

After first good assembly:
1. Record chosen bore and pocket values in `src/rollers_params.scad`.
2. Save notes about fit outcomes in your run logs.
3. Re export STL files from locked parameters.

This gives repeatable prints for future replacements and competition spares.

---

## Fusion 360 Assembly Guide

This section walks through assembling the full timing stage and conveyor roller system in Fusion. Work from the drive shaft outward.

### Reference coordinate system

- X axis: driven shaft axis (left to right when facing the machine front)
- Y axis: belt travel direction (bricks move in +Y, toward the chamber)
- Z axis: vertical (up)
- Origin: center of the driven shaft, at the midpoint of the drive roller face width

### Files to import

| Component | File | Format |
|-----------|------|--------|
| NEMA17 motor body | `docs/datasheet/motion/nema17/Stepper Motor 17HS4401S.STEP` | STEP |
| GT2 20T pulley (x2 instances) | `docs/datasheet/motion/timing_pulley/Шкив GT2 20T b5.STEP` | STEP |
| MR85ZZ bearing (reference) | `docs/datasheet/fasteners/bearing/NSK_MR85ZZ.stp` | STEP |
| GT2 belt (layout only) | `docs/datasheet/motion/timing_belt/gt2 belt C200.STEP` | STEP |
| Drive roller | `cad/frame/rollers/stl/drive_roller_proto_v1.stl` | STL |
| Idler roller | `cad/frame/rollers/stl/idler_roller_proto_v1.stl` | STL |
| Motor mount bracket | `cad/frame/rollers/stl/motor_mount_bracket_v1.stl` | STL |
| Bearing block (x2 instances) | `cad/frame/rollers/stl/bearing_block_v1.stl` | STL |

Create the driven shaft as a simple 5mm diameter, 200mm long cylinder directly in Fusion. No STEP file is needed.

### Step 1: Ground the driven shaft

1. Create a cylinder: 5mm diameter, 200mm long, axis along X.
2. Center it at the origin (midpoint of shaft at X=0, Y=0, Z=0).
3. Ground this component. Everything else constrains to it.

### Step 2: Place the drive roller

1. Import `drive_roller_proto_v1.stl` as a new component.
2. Align the roller bore axis to the shaft X axis.
3. Center the roller face width at X=0 (roller extends from X=-14.5 to X=+14.5mm, flanges add 2mm each side to X=-16.5 to X=+16.5mm).
4. Constraint: roller bore axis coincident with shaft axis. Axial position locked at X=0.

### Step 3: Place the bearing blocks

1. Import `bearing_block_v1.stl` twice as two separate components.
2. Bearing block A: place so bearing pocket (+X face) faces the nearest roller flange.
   - Bearing block center at approximately X=-21mm to X=-22mm (adjust so inner block face clears roller flange by 1-2mm).
   - The bearing center is at the block center, at X=-21mm from the roller center.
3. Bearing block B: mirror position on the +X side at approximately X=+21mm.
4. Both blocks: Y=0, Z=0 (shaft center runs through bearing bore).
5. Constraint for each: bearing bore axis coincident with shaft axis. Lock axial position.

The exact X positions depend on your frame wall positions. Adjust so bearing block outer faces align with (or are flush inside) the frame side walls.

### Step 4: Place the driven pulley

1. Import the GT2 20T STEP as a new component.
2. Place it outboard of Bearing Block B (the far side from the motor side, or whichever side the motor approaches from in your frame layout).
3. Driven pulley center at X = (bearing_block_B_outer_face + pulley_half_width + 1mm clearance).
   For a typical 11mm wide pulley: place at approximately X = +30mm to +35mm.
4. The belt center plane of the driven pulley must align with the belt center plane of the motor pulley in step 6. Note the belt plane X position.
5. Constraint: pulley bore axis coincident with shaft axis.

### Step 5: Place the motor mount bracket

1. Import `motor_mount_bracket_v1.stl` as a new component.
2. The bracket center (motor shaft center) goes at:
   - Y = -80mm from origin (80mm from driven shaft center, upstream in belt direction)
   - Z = same as driven shaft Z (coplanar, for horizontal timing belt)
   - X = same as driven shaft X center (motor shaft parallel to driven shaft)
3. The bracket is a flat plate (4mm thick, Z dimension).
4. Orient the bracket so the slot direction (Y axis of the bracket) aligns with the Y axis of the assembly.
5. Constraint: bracket slot centerline parallel to assembly Y. Motor center hole at (X=0, Y=-80mm, Z=0).

Note: if your frame places the motor lower than the drive shaft, adjust Z accordingly. Belt will run at an angle, which is acceptable. Keep the angle under 15 degrees to avoid belt edge loading.

### Step 6: Place the NEMA17 motor

1. Import `Stepper Motor 17HS4401S.STEP` as a new component.
2. The motor face mates to the bracket back face (-Z face of bracket when bracket is vertical).
3. Motor pilot boss (22mm OD) registers in the bracket center hole.
4. Motor shaft extends through the bracket center hole in the +Z direction (toward the driven shaft).
5. Constraint: motor pilot boss center coincident with bracket center hole. Motor face flush to bracket back.

### Step 7: Place the motor pulley

1. Import the GT2 20T STEP again as a second pulley component.
2. Place it on the motor shaft, on the driven-shaft side of the bracket.
3. Motor pulley belt center plane must align with driven pulley belt center plane (same X position).
4. Constraint: motor pulley bore axis coincident with motor shaft axis. X position locked to match driven pulley belt plane.

### Step 8: Place the idler roller

1. Import `idler_roller_proto_v1.stl` as a new component.
2. Position the idler at Y = -110mm to -120mm from origin (100-120mm upstream from the drive roller, toward the chute).
3. Idler axis along X, centered at same X=0 and Z as the drive roller.
4. Constraint: idler bore axis parallel to X. Y position sets the effective belt run length.

The idler uses an M5 bolt axle. In Fusion, create a simple 5mm cylinder representing the M5 bolt through the idler bore as a layout reference.

### Step 9: Belt representation

For the GT2 timing belt (between pulleys):
1. Import `gt2 belt C200.STEP` for visual reference, or sketch the belt path.
2. Position so the belt loops around both 20T pulleys at 80mm center distance.
3. For layout: verify the belt does not interfere with the motor body, frame walls, or the bracket.

For the neoprene conveyor belt:
- Sketch a rectangle 25mm wide representing the belt top surface, running from the drive roller to the idler roller.
- Verify the belt top plane Z position aligns with the chute exit height.

### Step 10: Verify the assembly

Check these conditions before printing:

| Check | What to look for |
|-------|-----------------|
| Pulley alignment | Motor pulley belt plane and driven pulley belt plane at identical X |
| Belt clearance | Timing belt path does not touch motor body, bracket, or frame walls |
| Shaft clearance | Drive roller flanges do not contact bearing blocks |
| Driven pulley clearance | No contact between driven pulley and bearing block |
| Bracket slot access | Frame bolt positions accessible for a screwdriver after assembly |
| Conveyor belt plane | Neoprene belt top surface at correct height for chute interface |
| Footprint | Full assembly stays inside 610mm x 610mm base plate |

### What "done right" looks like

- Motor shaft axis and driven shaft axis are parallel and at the same Z height (or intentionally offset).
- Both 20T pulleys are in the same plane (their belt teeth are aligned in X).
- The timing belt wraps both pulleys with ~4mm of straight section on each side of each pulley.
- The drive roller centerline is aligned with the 25mm conveyor channel centerline.
- The idler roller face width center is also aligned with the channel centerline.
- No interference shown in Fusion sectional view through the timing belt plane.
- The motor mount bracket slot centers are accessible after the motor is bolted in place.

### Center distance summary

| Belt option | Center distance | Bracket slot position |
|-------------|----------------|-----------------------|
| 100T (200mm) | 80mm | Slot centered (nominal) |
| 104T (208mm) | 84mm | Slot extended +4mm in Y |

Design the frame hole positions for the 100T belt (80mm center distance). The bracket slots then cover the 104T option without frame modification.
