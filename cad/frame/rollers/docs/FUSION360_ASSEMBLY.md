# Fusion 360 Assembly Guide: Timing Conveyor Stage

Covers: integrated trough body, NEMA17 motor, GT2 pulleys, timing belt path, supported
drive shaft, drive roller, fixed-axle idler, and the neoprene transport belt path.

Part geometry is in `cad/frame/rollers/docs/`. All numeric references trace to `cad/DIMENSIONS.md`.

---

## Before you start: import issues

### STL parts appear 10x too large

Fusion 360 defaults the import unit to centimeters for STL files. The parts are authored in
millimeters. A 110mm trough will show as 1100mm if you import without changing the unit.

Fix: in the Insert Mesh dialog, set the unit dropdown to "Millimeter (mm)" before clicking OK.
Do this for every STL. If you already imported at the wrong scale, delete and re-import.

### Convert STLs to solid bodies before constraining

Mesh bodies have limited constraint surfaces. Convert each STL to a BRep solid after import:

1. Import the STL with mm units.
2. Right-click the component in the browser.
3. Select "Mesh to BRep".
4. Use the resulting solid for all joints.

---

## 1. Files needed

| File | Location | Type | Notes |
|------|----------|------|-------|
| `conveyor_trough_v1.stl` | `cad/frame/rollers/stl/` | STL | Import mm, convert to BRep, ground this first |
| `stepper_17HS4401S.STEP` | `docs/datasheet/motion/nema17/` | STEP | Align to the integrated motor face |
| `drive_roller_proto_v1.stl` | `cad/frame/rollers/stl/` | STL | Import mm, convert to BRep |
| `idler_roller_proto_v1.stl` | `cad/frame/rollers/stl/` | STL | Import mm, convert to BRep |
| `GT2_20T.STEP` | `docs/datasheet/motion/timing_pulley/` | STEP | Import twice, one instance per shaft |

### Key dimensions before constraining

| Dimension | Value |
|-----------|-------|
| Trough transport length | 110mm |
| Internal channel width | 25mm |
| Channel wall height above shelf | 15mm |
| Supported shaft diameter | 5mm round with D-drive at roller |
| Idler axle diameter | 5mm clearance path through trough |
| GT2 20T pitch diameter | 12.73mm |
| Motor to drive-shaft center distance | 80mm |
| Motor slot travel | 8mm in the belt direction |

---

## 2. Placement sequence

Import everything and convert the printed parts to BRep before constraining anything. Then work through these steps in order.

### Step 1: Ground the trough

1. Insert > Insert Mesh > select `conveyor_trough_v1.stl`, unit = mm.
2. Right-click > Mesh to BRep.
3. Ground the trough component.
4. Orient the channel so transport runs in the positive Y direction and the shaft axes run in X.

This is the assembly origin. All other parts constrain to the trough.

### Step 2: Create the supported drive shaft

1. Insert > Create > New Component, name it `drive_shaft`.
2. In that component, sketch a 5mm diameter cylinder long enough to span the trough and pulley.
3. Constrain the shaft axis through the two drive-end trough bores.
4. Confirm the shaft axis stays parallel to the future idler axle and motor shaft.

The drive shaft is supported by the two drive-end MR85 pockets built into the trough walls.

### Step 3: Place the drive roller

1. Insert `drive_roller_proto_v1.stl`, unit = mm, then convert it to BRep.
2. Cylindrical joint: roller bore axis coincident with the drive-shaft axis.
3. Center the 25mm contact face on the 25mm trough channel.
4. Leave rotational DOF free.

### Step 4: Place the driven pulley

1. Insert `GT2_20T.STEP` and label it `drive_pulley`.
2. Cylindrical joint: pulley bore axis coincident with the drive-shaft axis.
3. Place the pulley on the motor side of the trough, outside the right wall.
4. Leave enough shaft length between the trough outer face and pulley hub for real hardware access.

### Step 5: Place the motor

1. Insert `stepper_17HS4401S.STEP`.
2. Align the motor shaft axis parallel to the drive shaft axis.
3. Mate the motor face to the integrated motor plate on the trough.
4. Place the motor at mid-slot so you keep tension margin in both directions.
5. Measure motor-shaft to drive-shaft center distance in Y. It should be 80mm nominal.

### Step 6: Place the motor pulley and match belt planes

1. Insert `GT2_20T.STEP` again and label it `motor_pulley`.
2. Cylindrical joint: pulley bore axis coincident with the motor shaft axis.
3. Slide the motor pulley axially until its tooth-section center matches the drive pulley tooth-section center.

How to verify belt-plane alignment:
- Inspect > Measure the X position of the center of the motor pulley tooth section.
- Measure the same on the drive pulley.
- Match them within 0.5mm. Zero offset is the target.

### Step 7: Create the idler axle and place the idler roller

1. Insert > Create > New Component, name it `idler_axle`.
2. Sketch a 5mm diameter cylinder spanning the two idler-end trough holes.
3. Insert `idler_roller_proto_v1.stl`, unit = mm, then convert it to BRep.
4. Constrain the idler roller to the M5 axle.
5. Center the roller in the channel.

The idler uses its own internal MR85 bearings. The trough only locates the fixed axle.

### Step 8: Sketch the timing belt path

Fusion 360 has no native belt tool. Sketch the path manually:

1. Create a sketch on a plane containing both pulley axes.
2. Draw two tangent lines from the outside edge of the motor pulley to the outside edge of the drive pulley.
3. Add arcs at each pulley to close the loop.
4. Verify the path passes cleanly through the bridge belt window.
5. Verify the path clears the motor plate, trough wall, and pulley hubs by at least 1mm.

### Step 9: Sketch the transport belt path

1. Create a second sketch on a plane through the drive-roller and idler axes.
2. Draw the top run tangent to both rollers at the belt-bed support plane.
3. Confirm the top run stays centered between the trough walls for the full 110mm transport length.
4. Confirm the idler and drive roller tangent heights match the trough shelf support plane.

---

## 3. Checks

Run in order. All must pass before physical build.

### Check 1: Shaft parallelism

- Motor shaft axis, drive-shaft axis, and idler axle axis must be parallel.
- Any angular offset causes belt skew.

### Check 2: Timing-belt plane alignment

- Motor pulley tooth-section center must match the drive pulley tooth-section center within 0.5mm.
- Check in the front view looking along Y.

### Check 3: Center distance

- Measure Y-distance from motor shaft center to drive-shaft center.
- Target: 80mm nominal, with the motor placed near the middle of the available slot travel.

### Check 4: Transport belt height

- The top tangent of the drive roller and the top tangent of the idler must match the trough shelf support plane.
- Any mismatch here changes brick transport height and can create a step in the belt path.

### Check 5: No hard interference

- Run Inspect > Interference on all components.
- Zero red interference volumes required.
- Common problem points:
- Motor body vs trough motor leg
- Timing belt path vs bridge window
- Drive pulley vs trough outer wall
- Idler body vs trough wall tops
- Drive-roller flanges vs wall interior

### Check 6: Belt path clearance

- Timing belt sketch must clear all solids by at least 1mm.
- Transport belt sketch must stay centered in the trough channel.

---

## 4. Pass / Fail table

| Check | Pass | Fail |
|-------|------|------|
| Shaft parallelism | All three axes parallel | Any visible angular offset |
| Timing-belt plane | Pulley tooth centers match within 0.5mm | Axial offset visible |
| Center distance | 80mm nominal with slot margin left | Slot already near one limit |
| Transport belt height | Both roller tangents match trough shelf plane | Visible step in the top run |
| Hard interference | Zero overlapping volumes | Any red interference volume |
| Belt path clearance | 1mm+ everywhere | Belt sketch intersects any part |

Assembly is ready for physical build when all checks pass.

---

## 5. Common pitfalls

**Motor not centered in slot travel**: if you assemble the CAD with the motor already near one end of the slot, you hide whether the real belt will still have usable tension range.

**Pulley plane skipped**: parallel shafts alone are not enough. Axially offset pulleys still cause belt walk.

**Drive pulley too close to the wall**: leave real wrench and set-screw access. CAD that only barely clears is not serviceable during testing.

**Idler treated like a supported shaft**: the idler keeps its own internal bearings. The trough only needs the fixed M5 axle line at that end.

**Motor pulley is a layout proxy**: `motor_pulley_proto_v1.stl` is still only a quick envelope model. Use `GT2_20T.STEP` for the actual CAD assembly.
