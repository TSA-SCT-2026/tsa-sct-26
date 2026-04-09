# Fusion 360 Assembly Guide: Timing Conveyor Stage

Quick reference for assembling the full timing stage in Fusion 360.
Covers: NEMA17 motor, motor mount bracket, both GT2 pulleys, timing belt, supported shaft,
drive roller, idler roller, and two bearing blocks.

Read the part cards in `cad/frame/rollers/docs/` for detailed geometry.
Read `cad/DIMENSIONS.md` lines 53-80 for all numeric references.

---

## Known import issues (read before starting)

### STL unit mismatch - parts appear 10x too large

Fusion 360 defaults to centimeters when importing STL files. The printed parts are authored
in millimeters in OpenSCAD. If you import an STL without changing the unit, a 70mm bracket
will appear as 700mm.

Fix: when the Insert Mesh dialog opens, change the unit dropdown to "Millimeter (mm)" before
clicking OK. Do this for every STL file in this assembly.

If you already imported at the wrong scale, delete the component and re-import with the
correct unit selected.

### STL gives a mesh body, not a solid - constraints are limited

STL files import as mesh bodies. Fusion 360 can constrain to mesh faces but the workflow
is less reliable than constraining to solid STEP geometry, especially for coaxial joints
on bores.

Convert mesh to BRep in Fusion 360 (no extra tools needed):

1. Import the STL with mm units as above.
2. In the browser, right-click the mesh component.
3. Select "Mesh to BRep". Fusion will convert the mesh to a solid body.
4. For simple parts like the bracket and bearing block this works cleanly.
5. Use the converted solid body for all subsequent joint constraints.

Note: complex curved surfaces (drive roller crown, idler crown) may produce many faces but
the bore and flat reference surfaces are usable.

---

## 1. Prerequisites

### Files to have open or imported before starting

| File | Location | Notes |
|------|----------|-------|
| `motor_mount_bracket_v1.stl` | `cad/frame/rollers/stl/` | Import with unit = mm. Convert to BRep after import. |
| `bearing_block_v1.stl` | `cad/frame/rollers/stl/` | Import with unit = mm. Print two copies. |
| `drive_roller_proto_v1.stl` | `cad/frame/rollers/stl/` | Import with unit = mm. |
| `idler_roller_proto_v1.stl` | `cad/frame/rollers/stl/` | Import with unit = mm. |
| `GT2_20T.STEP` | `docs/datasheet/motion/timing_pulley/` | Use for both pulleys |
| NEMA17 STEP | Your motor datasheet folder | Community model acceptable for layout |


### Hardware dimensions to know before constraining

- Driven shaft: 5mm diameter, 200mm long
- Motor shaft: 5mm D-profile, NEMA17 standard
- Pulley pitch diameter: 12.73mm (GT2 20T)
- Pulley hub width: approximately 11mm (verify against received part before finalizing)
- Center distance: 80mm nominal (100T belt) or 84mm nominal (104T belt)
- Bracket slot travel: 8.5mm, oriented in the Y direction

---

## 2. Import and Placement Order

Work in this order. Each step anchors the next.

### Step 1: Ground the NEMA17 motor

1. Insert > Import > select your NEMA17 STEP file.
2. Ground the motor body (right-click component > Ground).
3. Orient so the motor shaft points in the +X direction (shaft axis = X).
4. This is your assembly origin. Everything else constrains to this.

### Step 2: Place the motor mount bracket

1. Import `motor_mount_bracket_v1.stl` and convert to a Fusion body if needed
   (Insert > Insert Mesh, or re-export the SCAD as STEP for clean constraint surfaces).
2. Mate the bracket face (motor-facing side) flush to the motor mounting face.
3. Align the four M3 holes on the bracket to the four M3 holes on the NEMA17 face.
   Use a Coincident joint on one hole pair and a Tangent or Parallel constraint to
   lock rotation.
4. Leave the frame bolt slots free for now. You will set the Y offset in Step 9.

### Step 3: Place the motor pulley (first STEP instance)

1. Import `GT2_20T.STEP` as Component 1 of 2.
2. Apply a Cylindrical joint: pulley bore axis coincident with motor shaft axis.
3. Set axial position so the pulley sits on the frame side of the bracket
   (motor pulley must not be between motor face and bracket).
4. Leave rotational degree of freedom free (belt will determine this).

Reference: `docs/motor_mount_bracket.md` - motor attachment section.

### Step 4: Set up the driven shaft

1. Insert > Create > New Component named "driven_shaft".
2. Sketch a cylinder: 5mm diameter, 200mm long.
3. Position the shaft axis parallel to the motor shaft axis (both along X).
4. Set shaft center 80mm from motor shaft center, measured along Y
   (the belt travel direction). This is the nominal center distance.
5. Set Z height equal to motor shaft Z unless your frame intentionally offsets the motor
   lower for packaging. Keep both shafts coplanar in Z for a first-pass layout.

### Step 5: Place the bearing blocks (x2)

1. Import `bearing_block_v1.stl` twice, as separate components.
2. For each block: mate the 5.2mm bore axis to the driven shaft axis (Cylindrical joint).
3. Space the blocks symmetrically about the drive roller position.
   From `docs/bearing_block.md` layout table:
   - Bearing block centers at approximately +/-21.5mm from roller center
   - Adjust to clear the drive roller flanges (roller edge at +/-14.5mm)
4. Orient each block so its bearing pocket (8.15mm bore side) faces outward,
   away from the roller center.
5. The frame bolt holes run through the Y faces. Confirm these faces point toward
   your frame wall geometry.

### Step 6: Place the drive roller

1. Import `drive_roller_proto_v1.stl`.
2. Apply a Cylindrical joint: roller bore axis coincident with driven shaft axis.
3. Center the roller on the shaft (roller center = shaft center in X).
   The 25mm contact face spans the belt channel width.
4. Leave the roller free to spin (rotational DOF free).

Reference: `docs/drive_roller.md` for bore and flange geometry.

### Step 7: Place the driven pulley (second STEP instance)

1. Import `GT2_20T.STEP` again as Component 2 of 2.
2. Apply a Cylindrical joint: pulley bore axis coincident with driven shaft axis.
3. Position axially beyond the outer bearing block on the motor side (+X or -X,
   whichever faces the motor). This keeps the pulley in the belt plane.
4. Set the pulley axial position so its belt plane (center of the tooth section)
   aligns with the motor pulley belt plane. This is the single most critical
   geometric constraint in this assembly.

### Step 8: Check belt plane alignment

Before proceeding: verify motor pulley belt plane Z height equals driven pulley belt
plane Z height. Misalignment here causes belt tracking failure.

How to check:
- Measure from the motor shaft centerline to the center of the motor pulley tooth section.
- Measure from the driven shaft centerline to the center of the driven pulley tooth section.
- These two values must match (within approximately 0.5mm for layout; 0mm is the target).
- Adjust axial position of either pulley until they match.

If the motor is intentionally lower for packaging, adjust driven pulley axial offset to
compensate. Do not skip this check.

### Step 9: Set motor mount bracket Y offset (tension slot)

1. Re-visit the motor mount bracket from Step 2.
2. Move the entire motor + bracket assembly in +Y (away from driven shaft)
   until the center distance reads your target value:
   - 80mm for 100T belt (200mm belt)
   - 84mm for 104T belt (208mm belt)
3. The bracket slot provides 8.5mm of adjustment range. Keep the motor at
   mid-slot for the initial layout so you have adjustment room in both directions.
4. Confirm: center distance from motor shaft center to driven shaft center in Y
   equals your target (80mm or 84mm).

### Step 10: Place the idler roller

1. Import `idler_roller_proto_v1.stl`.
2. The idler mounts on a separate M5 axle at the opposite end of the belt path
   from the drive roller. This is not on the supported shaft.
3. Set idler axle parallel to the driven shaft (both along X).
4. Position idler so the neoprene belt wraps from the drive roller to the idler
   at the conveyor belt end. Exact axial placement depends on your frame and channel
   geometry.
5. Idler Z height: set so the belt bed (top face of belt) is at the correct channel height
   for brick transport.

Reference: `docs/idler_roller.md` for crown and bearing geometry.

### Step 11: Visualize the timing belt path

Fusion 360 does not have a native belt tool. Approximate the belt path manually:

1. Sketch > create two tangent lines from the outer edge of the motor pulley to the
   outer edge of the driven pulley (one on each side).
2. Add two arcs at each pulley to close the loop (arc radius = pulley OD / 2).
3. This gives a visual check of belt wrap angle and path clearance.
4. Verify the belt path does not intersect any part of the frame, bracket, or bearing blocks.

Minimum wrap angle on the smaller pulley: 120 degrees is acceptable. Less than 90 degrees
is a slip risk. Both pulleys are identical (20T), so wrap angle is symmetric at 80mm center
distance.

---

## 3. Key Constraint Checks

Run these in order after completing placement. Each check must pass before moving on.

### Check 1: Shaft parallelism

- Select motor shaft axis and driven shaft axis.
- Use Inspect > Measure or the Section Analysis tool to confirm the axes are parallel.
- They must be parallel. Any angular offset causes belt skew and tracking failure.
- Tolerance: visually parallel in all views. Zero angular error in layout.

### Check 2: Belt plane alignment

- Motor pulley belt-section center Z = driven pulley belt-section center Z.
- Check from a front view (looking along X axis).
- If misaligned: adjust the axial position of the pulley that is easier to move.
- Tolerance: 0.5mm max for initial layout. Match as closely as possible.

### Check 3: Center distance

- Measure distance between motor shaft axis and driven shaft axis in the Y direction.
- Must match your chosen belt length:
  - 100T belt = 80mm center distance
  - 104T belt = 84mm center distance
- Bracket slot mid-position gives adjustment room in both directions.

### Check 4: No hard interference

- Inspect > Interference between all components.
- Common interference points to check:
  - Motor pulley vs. bracket face (pulley must be on frame side of bracket)
  - Drive roller flanges vs. bearing block bodies
  - Driven pulley vs. outer bearing block face
  - Idler roller body vs. frame channel walls
- Zero hard interference required before any part goes to print.

### Check 5: Belt path clearance

- Trace the timing belt path sketch from Step 11.
- Check clearance to bracket edges, bearing block tops, and any frame walls.
- Minimum 1mm clearance around belt at all points.

---

## 4. Pass / Fail Validation Criteria

| Check | Pass | Fail |
|-------|------|------|
| Shaft parallelism | Axes parallel in all views | Any visible angular offset |
| Belt plane alignment | Z positions match within 0.5mm | Offset visible in front view |
| Center distance | Matches chosen belt length +/-0.5mm | Outside range, no room for adjustment |
| Hard interference | Zero red interference volumes | Any overlapping solid volumes |
| Belt path clearance | 1mm+ all around belt path | Belt sketch intersects any part |
| Bracket slot position | Motor at mid-slot | Motor at hard end of slot |
| Pulley axial position | Pulley on frame side of bracket | Pulley between motor face and bracket |
| Idler height | Belt bed at correct channel height | Idler too high or low for brick transport |

The assembly is ready for physical build when all checks pass.

---

## 5. Common Pitfalls

**Pulley between motor and bracket**: the purchased pulley goes on the motor shaft
on the frame side of the bracket only. If it sits between the motor face and the bracket,
it will be inaccessible and will contact the bracket under load.

**Skipping belt plane check**: shafts can be parallel but pulleys offset axially. These
are independent checks. A parallel shaft with misaligned pulleys still causes belt walk.

**Motor at end of slot**: if you set the motor at the maximum slot extension during layout,
you have no room to take up belt stretch during physical assembly. Start at mid-slot.

**Using the printed motor_pulley.scad model as the purchased part**: the printed envelope
is a cylinder with no teeth. It is for layout only. The STEP file from the timing_pulley
datasheet is the correct geometry to use for both pulley instances.

**Hub width assumption**: the STEP model hub width may differ slightly from your received
WINSINN pulleys. Verify the received pulley hub width before finalizing axial positions.
See `docs/datasheet/motion/timing_pulley/README.md` for the caveat on this STEP file.

**Two bearing blocks, one for each end**: the bearing block STL is a single unit. You
need two instances. Both pocket faces point outward from the roller center.
