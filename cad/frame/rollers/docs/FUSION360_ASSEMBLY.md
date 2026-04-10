# Fusion 360 Assembly Guide: Timing Conveyor Stage

Covers: NEMA17 motor, motor mount bracket, GT2 pulleys (x2), timing belt path, supported
shaft, drive roller, idler roller, and two bearing blocks.

Part geometry is in `cad/frame/rollers/docs/`. All numeric references trace to `cad/DIMENSIONS.md`.

---

## Before you start: import issues

### STL parts appear 10x too large

Fusion 360 defaults the import unit to centimeters for STL files. The parts are authored in
millimeters. A 70mm bracket will show as 700mm if you import without changing the unit.

Fix: in the Insert Mesh dialog, set the unit dropdown to "Millimeter (mm)" before clicking OK.
Do this for every STL. If you already imported at the wrong scale, delete and re-import.

### Convert STLs to solid bodies before constraining

Mesh bodies have limited constraint surfaces. Convert each STL to a BRep solid after import:

1. Import the STL (mm units as above).
2. Right-click the component in the browser.
3. Select "Mesh to BRep".
4. Use the resulting solid for all joints.

All printed parts in this assembly are under 7,000 triangles and have been verified manifold,
so BRep conversion should succeed for all of them.

---

## 1. Files needed

| File | Location | Type | Notes |
|------|----------|------|-------|
| `stepper_17HS4401S.STEP` | `docs/datasheet/motion/nema17/` | STEP | Ground this as the assembly origin |
| `motor_mount_bracket_v1.stl` | `cad/frame/rollers/stl/` | STL | Import mm, convert to BRep |
| `bearing_block_v1.stl` | `cad/frame/rollers/stl/` | STL | Import mm, convert to BRep. Import twice. |
| `drive_roller_proto_v1.stl` | `cad/frame/rollers/stl/` | STL | Import mm, convert to BRep |
| `idler_roller_proto_v1.stl` | `cad/frame/rollers/stl/` | STL | Import mm, convert to BRep |
| `GT2_20T.STEP` | `docs/datasheet/motion/timing_pulley/` | STEP | Import twice, one instance per shaft |

### Key dimensions before constraining

| Dimension | Value |
|-----------|-------|
| Motor shaft diameter | 5mm D-profile |
| Driven shaft diameter | 5mm round, 200mm long |
| Driven shaft bore in roller/block | 5.2mm clearance |
| GT2 20T pitch diameter | 12.73mm |
| GT2 20T hub width | ~11mm (verify against received part) |
| Center distance, 100T belt | 80mm |
| Center distance, 104T belt | 84mm |
| Bracket slot travel | 8.5mm in Y |

Commit to a belt length before Step 3. Center distance is fixed by frame geometry.

---

## 2. Placement sequence

Import everything and convert STLs to BRep before constraining anything. Then work through
these steps in order - each step anchors the next.

### Step 1: Ground the motor

1. Insert > Import > select `stepper_17HS4401S.STEP`.
2. Right-click the motor component > Ground.
3. Orient so the motor shaft points in the +X direction.

This is the assembly origin. All other parts constrain to the motor or to the driven shaft.

### Step 2: Place the motor mount bracket

1. Insert > Insert Mesh > select `motor_mount_bracket_v1.stl`, unit = mm.
2. Right-click > Mesh to BRep.
3. Rigid joint: mate the bracket's motor-facing face flush to the NEMA17 mounting face.
4. Align one M3 motor hole on the bracket to the matching tapped hole on the motor face
   (Coincident joint). Add a Parallel constraint on the bracket face to lock rotation.

Physical note: the four frame slots allow 8.5mm of Y travel. In the final physical assembly
you slide the motor toward or away from the shaft to set center distance, then lock the
frame bolts. Keep the motor at mid-slot in the CAD layout so you have margin in both
directions. The slot center positions are 28mm from the bracket center in both X and Y.

See `cad/frame/rollers/docs/motor_mount_bracket.md` for full geometry.

### Step 3: Place the motor pulley

1. Insert > Import > select `GT2_20T.STEP` (Component 1 of 2, label it "motor_pulley").
2. Cylindrical joint: pulley bore axis coincident with motor shaft axis.
3. Set axial position so the pulley sits on the frame side of the bracket. The pulley must
   not be between the motor face and the bracket - it will be trapped and contact the bracket
   under load.
4. Note the axial distance from the motor shaft shoulder to the center of the pulley tooth
   section. You will need this value in Step 7 to match belt planes.

### Step 4: Create the driven shaft and set center distance

1. Insert > Create > New Component, name it "driven_shaft".
2. In the component, sketch a 5mm diameter cylinder, 200mm long.
3. Place the shaft axis parallel to the motor shaft axis (both along X).
4. Set shaft center Y-distance from motor shaft center to your target center distance:
   - 80mm for 100T (200mm) belt
   - 84mm for 104T (208mm) belt
5. Set shaft Z equal to motor shaft Z. Both shafts must be coplanar in Z.

This distance is the frame geometry commitment. Do not change it after Step 5.

### Step 5: Place the bearing blocks

1. Insert the first `bearing_block_v1.stl` instance (mm, BRep convert).
2. Insert the second instance as a separate component.
3. For each block: Cylindrical joint on the 5.2mm bore axis coincident with the driven shaft.
4. Space the blocks symmetrically about the shaft center in X:
   - Block centers at approximately +/-21.5mm from roller center
   - Keep block bodies clear of the drive roller flanges (roller edge at +/-14.5mm)
5. Orient each block so the bearing pocket face (8.15mm bore, 2.6mm deep) faces outward.
6. The two M3 mounting holes run through the Y faces. Confirm those faces point toward
   your frame walls.

See `cad/frame/rollers/docs/bearing_block.md` for full geometry.

### Step 6: Place the drive roller

1. Insert `drive_roller_proto_v1.stl` (mm, BRep convert).
2. Cylindrical joint: roller bore axis coincident with driven shaft axis.
3. Center the roller on the shaft in X. The 25mm contact face spans the belt channel.
4. Leave rotational DOF free.

See `cad/frame/rollers/docs/drive_roller.md` for bore and flange geometry.

### Step 7: Place the driven pulley and check belt plane alignment

1. Insert `GT2_20T.STEP` again (Component 2 of 2, label it "driven_pulley").
2. Cylindrical joint: pulley bore axis coincident with driven shaft axis.
3. Position axially on the motor-facing side of the outer bearing block. The pulley must
   clear the bearing block face.
4. Adjust axial position until the driven pulley belt-section center is at the same Z
   as the motor pulley belt-section center. This is the most critical constraint.

How to verify belt plane alignment:
- Inspect > Measure: distance from motor shaft axis to center of motor pulley tooth section.
- Inspect > Measure: same measurement on the driven shaft and driven pulley.
- The two values must match within 0.5mm. 0mm is the target.
- If misaligned, slide the driven pulley axially until they match.

Do not proceed past this step with misaligned belt planes. Axial misalignment causes belt
walk and is not adjustable after physical assembly.

### Step 8: Place the idler roller

1. Insert `idler_roller_proto_v1.stl` (mm, BRep convert).
2. The idler mounts on a separate M5 axle at the far end of the conveyor belt path.
   It is not on the supported shaft.
3. Set idler axle parallel to the driven shaft (both along X).
4. Position so the neoprene belt wraps from the drive roller to the idler at the channel end.
   Exact X position depends on your frame and channel geometry.
5. Set idler Z so the belt bed (top face of the neoprene belt) is at the correct channel
   height for brick transport.

See `cad/frame/rollers/docs/idler_roller.md` for crown and bearing pocket geometry.

### Step 9: Sketch the timing belt path

Fusion 360 has no native belt tool. Sketch the path manually:

1. Create a sketch on a plane containing both shaft axes.
2. Draw two tangent lines from the outside edge of the motor pulley to the outside edge
   of the driven pulley (one on each side).
3. Add arcs at each pulley closing the loop (arc radius = pulley OD / 2 = ~7.9mm).
4. Verify the path clears the bracket edges, bearing block tops, and any frame walls by
   at least 1mm.

Wrap angle check: both pulleys are 20T at 80mm center distance. Wrap angle is symmetric.
Minimum acceptable wrap: 120 degrees. Below 90 degrees is a slip risk.

---

## 3. Checks

Run in order. All must pass before physical build.

### Check 1: Shaft parallelism

- Inspect > Measure between motor shaft axis and driven shaft axis.
- Axes must be exactly parallel. Any angular offset causes belt skew.

### Check 2: Belt plane alignment

- Motor pulley tooth-section center Z = driven pulley tooth-section center Z.
- Check in the front view (looking along X).
- Max allowed offset: 0.5mm. Target: 0.

### Check 3: Center distance

- Measure Y-distance between motor shaft center and driven shaft center.
- Must match your belt choice: 80mm (100T) or 84mm (104T), within 0.5mm.

### Check 4: No hard interference

- Inspect > Interference on all components.
- Zero red interference volumes required.
- Common problem points:
  - Motor pulley vs bracket face
  - Drive roller flanges vs bearing block bodies
  - Driven pulley vs outer bearing block face
  - Idler body vs frame channel walls

### Check 5: Belt path clearance

- Belt sketch must clear all parts by at least 1mm at all points.

---

## 4. Pass / Fail table

| Check | Pass | Fail |
|-------|------|------|
| Shaft parallelism | Parallel in all views | Any visible angular offset |
| Belt plane alignment | Z match within 0.5mm | Offset visible in front view |
| Center distance | Within 0.5mm of target | Out of range, slot at limit |
| Hard interference | Zero overlapping volumes | Any red interference volume |
| Belt path clearance | 1mm+ everywhere | Belt sketch intersects any part |
| Pulley side of bracket | Pulley on frame side | Pulley between motor face and bracket |
| Bearing block orientation | Pocket face outward on both blocks | Either pocket faces inward |
| Idler belt height | Belt bed at channel height | Idler too high or too low |

Assembly is ready for physical build when all checks pass.

---

## 5. Common pitfalls

**Pulley between motor face and bracket**: the GT2 pulley goes on the motor shaft on the
frame side of the bracket. If it is between the motor face and the bracket it will be
inaccessible and will contact the bracket under belt load.

**Belt plane skipped**: shaft parallelism and belt plane alignment are independent checks.
Parallel shafts with axially offset pulleys still cause belt walk. Do not skip Check 2.

**Hub width from STEP vs received part**: the GT2_20T.STEP hub width may differ from the
WINSINN pulleys you received. Verify the received hub width before finalizing axial positions.
See `docs/datasheet/motion/timing_pulley/README.md` for the caveat on this file.

**Two bearing block instances**: the STL is one block. You need two separate imported
instances. Both pocket faces point outward from roller center.

**Motor pulley is a layout proxy**: `motor_pulley_proto_v1.stl` is a featureless cylinder
for spatial reference only. Use `GT2_20T.STEP` for both pulley instances in this assembly.
