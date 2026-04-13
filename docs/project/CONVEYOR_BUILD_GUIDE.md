# Conveyor Build Guide

## Before You Start: Build Scope

This guide covers only the conveyor subassembly. Full CAD also includes the servo rotary chute selector,
sensing station, bins, and frame. Treat those as separate subassemblies, not as reasons to stall the
conveyor core.

Note: downloaded conveyor assemblies found online use aluminum extrusion frames or require custom
endless belts from manufacturers. Neither is available in the build window. Build this guide from scratch.

## Goal

Build the conveyor in the same simple style as the reference video: a wood block support that
parts can screw into, thin wood side plates, printed bearing holders, shafts through the bearings,
printed rollers between the plates, and a NEMA17 on an adjustable side bracket.

Reference tutorial structure to preserve:
- NEMA17 motor, printed rollers, printed bearing holders, printed motor bracket, shaft spacers, and pulleys
- Wood or hardboard support piece connected into the frame, not an aluminum extrusion frame
- Adjustable bearing holders for belt tension
- Movable motor bracket for timing belt tension
- Overall visual build order and simple screw-together construction

Do not copy the tutorial dimensions or hardware as law. This build is intentionally modified for the
team's materials and sorter geometry: 5mm shafts and matching 5mm-ID bearings, the current 25mm
neoprene belt width, the current top-run length target, the NEMA17 motor, and the clearance needed
for LEGO bricks.

This guide is written to get usable conveyor CAD and build files without wasting time on fake
precision. Model the parts well enough to make drilling templates and print files. Do not model every
screw, belt tooth, shaft collar, or final belt splice.

Coordinates are starting anchors, not proof that the real assembly works. Use exact coordinates for
axis direction, hole patterns, clearances, and rough placement. Then check the actual imported parts,
slots, belt path, brick clearance, and hardware before drilling or printing anything that depends on
fit. If a coordinate and the real assembly conflict, trust the real assembly and update the dimension.

## Do This First

Ignore the detailed reference numbers until a step asks for them. Build the CAD in this order:

1. Side plates with shaft holes
2. Bearing holders
3. Rollers
4. Core conveyor assembly: side plates, bearings, shafts, and rollers
5. Motor mount, fitted after the core conveyor exists
6. Wood block support and crossmembers
7. Extras after the conveyor core fits: side rails, sensing shroud, and exit lip

Do not start by perfecting the motor, shroud, exit lip, or frame. The conveyor core proves whether the
rest of the CAD has somewhere real to attach.

## Coordinate Rules

Only coordinates you must place directly:
- Drive shaft center: X=380, Z=50
- Idler shaft center: X=30, Z=50
- Bearing holder mounting holes: 10mm above and 10mm below each shaft center

Everything else is fit-later or adjustable:
- Motor mount: start near the suggested position, then fit to the real pulley and belt path
- Exit lip: first version is a handoff test coupon and will probably move
- Sensing shroud: use the listed dimensions, then verify belt and brick clearance
- Crossmembers and legs: place where they support the frame and do not block moving parts

Do not try to be a CNC machine. Use coordinates to stop CAD from drifting, not to replace dry-fit
checks.

## Do Not Overthink

- Motor mount: do not pre-drill final side-plate holes from CAD alone. Fit it in assembly.
- Exit lip: the first version is allowed to be wrong. It exists to make handoff testing fast.
- Sensing shroud: approximate placement is fine for v1 if the belt clears and the brick does not rub.
- Crossmembers: place them where they stiffen the side plates and stay out of the belt path. If the
  wood block support holds the spacing well, fewer printed crossmembers may be needed.
- Fusion parameters: skip them if they slow you down. Type the numbers directly.

## Coordinate Convention

Use this convention for the conveyor subassembly:

- X axis: belt travel, feed end at X=0 and chute end at X=410
- Y axis: across the conveyor, left to right
- Z axis: vertical, side plate bottom at local Z=0
- Side plate size: 410mm long in X, 80mm tall in Z, 8mm thick in Y as a CAD placeholder
- Side plate inner gap: 45mm
- Drive roller center: X=380, Z=50
- Idler roller center: X=30, Z=50

If the Home Depot side-plate stock is not 8mm thick, change the side plate thickness after measuring
the real material. Do not change shaft centers, bearing-holder hole spacing, or the 45mm inner gap
just because the outer plate thickness changes.

This keeps the pulley frame simple while modeling. In the full machine assembly, raise the whole
conveyor subassembly by `leg_h` so the local belt surface at Z=60 becomes about 240mm above the base.
The legs are vertical supports between the base and the raised conveyor, not parts that run across Y.

The drive end is the chute end. The motor and GT2 pulleys go outside the right side plate at the drive end.

## Fusion Setup

1. Open Fusion 360.
2. Create a new design.
3. Save as `conveyor_assembly`.
4. Rename the top component to `conveyor_assembly`.
5. Skip user parameters unless you already like using them.
6. Keep the reference dimensions section open only when a step asks for a number.

## Component 1: Side Plate

Make one side plate first. Later you will copy it to create the second side plate.

1. Right-click `conveyor_assembly`, New Component.
2. Name it `side_plate_left`.
3. Create Sketch on the XZ plane.
4. Draw a 410mm x 80mm rectangle from the origin.
5. Finish Sketch.
6. Extrude the rectangle by 8mm in Y.

For this single part, the extrusion direction does not matter. In the assembly, the wood thickness must
point outward from the 45mm inner gap.

Add shaft holes:

1. Click the large XZ face of the side plate.
2. Create Sketch.
3. Place a construction point for the drive shaft at X=380, Z=50.
4. Add a 5.5mm circle on that point.
5. Place a construction point for the idler shaft at X=30, Z=50.
6. Add a horizontal center-to-center slot through the idler point:
   - slot length: 20mm
   - slot width: 5.5mm
   - slot center: X=30, Z=50
7. Finish Sketch.
8. Extrude Cut both shapes through the plate.

Add bearing holder mounting holes:

1. Click the same XZ face again.
2. Create Sketch.
3. At the drive shaft construction point, add two 3.3mm holes:
   - one 10mm above the shaft center
   - one 10mm below the shaft center
4. At the idler shaft construction point, add two horizontal mounting slots:
   - one 10mm above the shaft center, 18mm long, 3.3mm wide
   - one 10mm below the shaft center, 18mm long, 3.3mm wide
5. Finish Sketch.
6. Extrude Cut through the plate.

Export the drilling template:

After all holes are added, right-click the sketch in the browser panel (left side), choose Save as DXF.
Print the DXF at 100% scale. Tape it to the wood blank and drill one plate. Clamp that drilled plate to
the second wood blank and drill through both so the holes match.

## Component 2: Fixed Bearing Holder

This holder bolts to the drive end. It does not slide.

1. New Component.
2. Name it `bearing_holder_fixed`.
3. Create Sketch on the XZ plane.
4. Draw a 32mm x 32mm rectangle centered on the origin. The bearing center is the origin.
5. Add two 3.3mm mounting holes:
   - one 10mm above the bearing center
   - one 10mm below the bearing center
6. Finish Sketch.
7. Extrude the square body to 10mm in Y. Do not include the mounting hole profiles in the extrusion.
8. Cut the two M3 holes through all.

Add the bearing pocket:

1. Choose the face that will touch the side plate. The bearing pocket faces the side plate so the
   bearing sits close to the plate and is captured when the holder is bolted down.
2. Click that side-plate face of the holder.
3. Create Sketch.
4. Draw an 8.15mm circle centered on the origin.
5. Finish Sketch.
6. Extrude Cut 2.6mm deep.

Add shaft clearance:

1. Click the opposite outside face of the holder.
2. Create Sketch.
3. Draw a 5.5mm circle centered on the origin.
4. Finish Sketch.
5. Extrude Cut through all.

Use the same orientation rule for both sides of the conveyor: bearing pocket toward the wood side
plate, flat outside face visible. Press the bearings into the holders before bolting the holders to
the side plates.

Print one test coupon with only this pocket before printing all holders. If the bearing is too loose,
reduce pocket diameter by 0.05mm. If it is too tight, increase by 0.05mm.

## Component 3: Slotted Bearing Holder

This holder goes at the idler end so belt tension can be adjusted.

Fastest path:

1. Copy `bearing_holder_fixed`.
2. Paste New. (This creates an independent copy that you can edit without affecting the original.)
3. Rename the copy `bearing_holder_slotted`.
4. Edit the mounting hole sketch.
5. Replace the two round M3 holes with horizontal slots using Center to Center Slot:
   - one slot 10mm above the bearing center, 18mm long, 3.3mm wide
   - one slot 10mm below the bearing center, 18mm long, 3.3mm wide
6. Cut the slots through all.
7. Keep the same bearing pocket and 5.5mm shaft clearance hole.

Use washers under the screw heads when this part is assembled.

## Component 4: Idler Roller

The idler roller is the simple roller. Start with it because it has no set screw hub.

1. New Component.
2. Name it `roller_idler`.
3. Create Sketch on the YZ plane.
4. Draw this side profile above the Y axis.

Note on coordinates: in this revolve sketch, Y values are roller width and Z values are radii from
the Y axis. When Fusion revolves the profile 360 degrees around the Y axis, each Z value becomes a
diameter twice that size. Z=10 gives
a 20mm diameter. Z=14 gives a 28mm diameter. Z=0 is on the axis and stays as the centerline.

   - line from Y=0, Z=0 to Y=0, Z=14
   - line from Y=0, Z=14 to Y=3, Z=10
   - line from Y=3, Z=10 to Y=28, Z=10
   - line from Y=28, Z=10 to Y=31, Z=14
   - line from Y=31, Z=14 to Y=31, Z=0
   - line from Y=31, Z=0 back to Y=0, Z=0

5. Finish Sketch.
6. Revolve the closed profile 360 degrees around the Y axis.
   In the Revolve dialog, set the Axis to the Y axis. The profile must be a closed loop. If Fusion does
   not accept it, check that all line endpoints snap to each other without gaps.
7. Click one end face.
8. Create Sketch.
9. Draw a 5mm circle centered on the roller axis.
10. Finish Sketch.
11. Extrude Cut through all.

This makes a 31mm roller with 25mm belt contact and two flanges. Skip crown for v1.

## Component 5: Drive Roller

The drive roller is the idler roller plus a 5mm hub on the motor side.

1. Copy `roller_idler`.
2. Paste New.
3. Rename the copy `roller_drive`.
4. Create a sketch on the +Y end face. This is the motor side in the assembly.
5. Draw a 20mm circle centered on the shaft axis.
6. Finish Sketch.
7. Extrude the circle outward 5mm as Join. This creates the set screw hub.

Add set screw hole:

1. Create a sketch on the outside of the hub or use a construction plane tangent to the hub.
2. Add a 2.5mm hole aimed radially toward the 5mm shaft bore.
3. Cut until it reaches the shaft bore.
4. After printing, tap M3 or install a heat-set insert.
5. File a small flat on the shaft where the set screw lands.

If this step gets annoying in Fusion, use the simpler real-world fallback: print the hub solid, drill
the set screw hole by hand after printing, then tap it.

For the first states build, the hand-drilled set screw hole is acceptable as the default if the radial
Fusion cut slows you down. Keep the hub large enough, drill a 2.5mm pilot toward the shaft bore, tap
for M3, file a small flat on the shaft, and confirm the screw can be reached after the roller and belt
are installed.

## Component 6: Motor Mount

The motor mount is a flat adjustable plate outside the right side plate.

1. New Component.
2. Name it `motor_mount`.
3. Create Sketch on the XZ plane.
4. Draw a 100mm x 70mm rectangle centered on the motor shaft center.
5. Finish Sketch.
6. Extrude 6mm.

Add NEMA17 holes:

1. Click the large face.
2. Create Sketch.
3. Put the motor shaft center at the origin of this bracket sketch.
4. Draw a 22mm center clearance circle.
5. Draw four 3.3mm holes around it at:
   - X=+15.5, Z=+15.5 from motor center
   - X=+15.5, Z=-15.5 from motor center
   - X=-15.5, Z=+15.5 from motor center
   - X=-15.5, Z=-15.5 from motor center
6. Draw two 3.3mm x 24mm horizontal slots using Center to Center Slot for mounting the bracket to the
   side plate:
   - slot centered at X=0, Z=+25 from motor center
   - slot centered at X=0, Z=-25 from motor center
7. Finish Sketch.
8. Extrude Cut all holes and slots through all.

For the 20T to 60T, 200mm GT2 path, start the motor shaft center near:
- X=`drive_x` minus `gt2_center_dist` = 321.4mm
- Z=`shaft_z` = 50mm

This makes the timing belt run mostly horizontal and keeps the motor body within the side plate height.
The bracket slots adjust along X, which is the belt tension direction for this first-pass layout. Do
not treat X=321.4 as a drill coordinate. Place the motor, check the real pulley and belt path, then
mark or drill the side-plate motor-mount holes from the fitted bracket position. If the actual driven
pulley or belt length differs, recompute only the center distance and keep moving.

## Component 7: Wood Block Support

Use one straight wood block cut from the real conveyor dry-fit, usually about 295mm to 300mm with the
current roller geometry. This replaces the earlier 2020 aluminum
extrusion support idea and keeps the conveyor closer to the reference tutorial. The block is both the
top-belt support surface and a simple structural spine that thin side plates and small brackets can
screw into.

Choose the straightest available wood stock. Sand the top face smooth and cover it with smooth packing
tape, UHMW tape, or another low-friction strip if the neoprene belt drags during testing. Do not use a
warped block just because it matches the CAD number. Since wood stock will be picked up locally, do
not lock the thickness until the actual board or block is in hand.

Important clearance rules:
- End clearance: do not let the wood support run close to either roller flange, belt wrap, or pulley
  hardware. Use the actual roller flange envelope as the keep-out reference. With the current shaft
  centers at X=30 and X=380 and a 28mm flange diameter, a centered 295mm to 300mm block leaves about
  11mm to 13.5mm clearance to each flange envelope before belt and wood tolerance. The old 315mm
  starting length left only about 3.5mm per end, which is too little for a hand-built conveyor.
- Lower return clearance: with the current 20mm roller diameter, the lower belt return runs near
  Z=40. If the wood support top is at Z=58, the support must be thin enough that its bottom stays
  above the lower return path. Use about 10mm to 12mm support thickness for this roller size. If the
  wood in hand is closer to 19mm thick, either rip or plane it thinner, notch it for the lower return
  path, or revise the roller diameter and shaft height before cutting parts.

For Fusion reference:

1. New Component.
2. Name it `wood_block_support`.
3. Create Sketch on the XY plane.
4. Draw a rectangle about 295mm to 300mm long in X and about 20mm to 45mm wide in Y. Keep it narrow
   enough that it does not hit the rollers, belt return path, side plates, or bearing hardware.
5. Finish Sketch.
6. Extrude to the measured wood block thickness in Z. If no stock is measured yet, use 10mm as the
   placeholder.

Assembly position:

- X position: centered between the roller axes, then shortened or shifted so both ends clear the
  roller flange and belt-wrap keep-out zones by at least about 10mm.
- Y position: centered between side plates.
- Z position: align the top surface to a support plane about 2mm below the upper belt surface. In the
  current local conveyor datum, the roller centers are at Z=50, rollers have a 10mm radius, and the
  belt surface sits around Z=60, so the support top starts around Z=58. Treat that as an alignment
  relationship, not as a final global machine coordinate. The bottom face depends on the actual wood
  thickness and must clear the lower belt return path.

Use the wood block to help tie the side plates together. Fasten through the thin side plates into
the block, or use small printed angle brackets if that is easier to drill. Keep fastener heads below
the belt path. No deck tabs needed for v1.

## Component 8: Crossmember Spacer

The crossmembers and wood block hold the two side plates at the 45mm inner gap.

Simple printed spacer:

1. New Component.
2. Name it `crossmember_spacer`.
3. Sketch a 45mm x 20mm rectangle.
4. Extrude 20mm.
5. Add a 3.3mm hole through each end if you want it bolted.

Use two spacers as a starting point: one near the feed end and one near the drive end. If the
wood block is wide and screwed in well, it may provide most of the spacing stiffness. Add a third
spacer in the middle only if the side plates flex.

## Component 9: Legs

The legs lift the conveyor to the target belt height of about 240mm. They are simple printed or wood
posts. Model them as blocks so the assembly shows the correct height.

1. New Component.
2. Name it `leg`.
3. Create Sketch on the XY plane.
4. Draw a 20mm x 20mm square.
5. Finish Sketch.
6. Extrude `leg_h` in Z.

You will place four copies of this leg in the full machine assembly: one near each corner of the
conveyor frame. Put the leg bottoms on the machine base plane and the leg tops against the raised
side plate bottoms.

If you plan to cut the legs from wood, the 20mm x 20mm cross section is a good starting point. Adjust
to match whatever wood stock is available.

## Component 10: Exit Lip

The exit lip is a first handoff test coupon at the drive end. It should guide the brick off the belt
and into the chute entry below, but do not treat its first CAD geometry as final. Conveyor-to-chute
handoff is one of the highest-risk parts of the machine and must be tested with real bricks.

Geometry:
- Spans between the inner faces of the side plates (45mm wide)
- Lip height: first test position has the top 5 to 6mm above belt surface level
- Ramp face starts near belt surface and rises toward the downstream edge
- Wall thickness: 4mm

The lip is a wedge-shaped block that sits at the drive end of the belt. The face pointing toward the
incoming brick is angled or curved so the brick rides over the lip instead of hitting a flat stop wall.
Gravity and the chute entry below handle the downward part of the motion after the brick clears the
belt exit.

Model it as a standalone part at Z=0, then position in assembly:

1. New Component.
2. Name it `exit_lip`.
3. Create Sketch on the XZ plane.
4. Draw this cross-section profile (side view of the lip):
   - Bottom-left corner: X=0, Z=0
   - Bottom-right corner: X=15, Z=0
   - Top-right corner: X=15, Z=6
   - Top-left corner: X=4, Z=6
   - Angled line from (X=4, Z=6) back to (X=0, Z=0). This is the ramp face.
   - The angled face points toward X=0, toward the incoming brick.
5. Finish Sketch.
6. Extrude 45mm in Y (spanning the inner gap between the side plates).
7. Optional: select the top upstream edge (where the angled face meets the flat top) and apply a
   3mm fillet to soften the corner. This is the "curved" version. Skip the fillet if it causes
   trouble in Fusion.

In assembly, position the bottom face at Z=60 for the first test so the flat top sits at Z=66. The
ramp face should point toward X=0. Put the lip near X=395 to X=410, clear of the drive roller.

Mount holes: two 3.3mm holes through the bottom face for M3 bolts into a crossmember below.
If you have time, make those holes short slots in X so the lip can move a few mm during testing.

The chute entry funnel for the servo rotary chute selector sits directly below this lip. Position
the chute entry when modeling the chute selector subassembly.

## Component 11: Sensing Shroud

The sensing shroud is a single printed tunnel that covers the sensing zone on the belt. It handles
three things in one part: it blocks ambient light for the color sensor, holds both break beam pairs
at the correct height, and mounts the TCS3200 color sensor in its roof looking straight down at the
brick.

Placing everything in one enclosed part gives the color sensor a controlled, repeatable lighting
environment. Calibration done with this shroud installed is valid. Calibration without it is not.

The shroud sits over the belt between X=135 and X=200 and mounts to the wood conveyor bed or a
rigid wood frame member. The brick enters from the feed-end opening, passes through, and exits
toward the drive end. The side rail guide ends at X=120, so the brick is already centered before
it enters.

Do not glue the shroud in place for the first states build. Make it a removable screw-mounted part
with side tabs or small flanges. Use two or four screws into the wood conveyor bed, a crossmember, or
a rigid frame member. Short slots are better than round holes if they are easy, because they allow a
few millimeters of centering adjustment after the belt tracks under real tension.

Keep the side rail guide separate from the shroud until brick centering is proven. Temporary rails
made with screws, tape, clamps, or simple printed strips are acceptable during first conveyor tests.
After the rail gap is verified with real bricks, the rail design can be reprinted cleanly or merged
with another printed part if that saves assembly time.

Add the side rail guide as a separate first-build part:

1. New Component.
2. Name it `feed_side_rails`.
3. Create Sketch on the XY plane at the belt surface reference.
4. Draw two low rails from X=0 to X=120.
5. Make each rail 4mm thick in Y.
6. Use an entry inner gap around 38mm at X=0.
7. Taper to a 30mm inner gap at X=120, centered on Y=22.5.
8. Finish Sketch.
9. Extrude both rails 6mm in Z.
10. Leave the last 15mm before the shroud open so a slightly off-center brick can self-correct through
   the shroud entry chamfer instead of being pinched.

Outer dimensions:
- Length: 65mm in X (X=135 to X=200 in assembly)
- Width: 38mm in Y (4mm walls on each side of the 30mm inner width)
- Height: 36mm in Z above belt surface (4mm roof above the 32mm inner height)

Inner dimensions:
- Width: 30mm in Y (clears the 25mm neoprene belt with about 2.5mm per side; the as-fed 2x3 brick is 15.8mm wide in Y)
- Height: 32mm in Z from belt surface (clears 13.1mm brick with studs, leaves ~19mm to sensor face)
- Front and back faces are open (brick passes through)
- Bottom is open (belt passes through)

Sensor face is about 19mm from the top of the brick's studs. This is a good distance for the
TCS3200 to read reflected light.

Entry chamfer: add a 3mm chamfer on the inner edges of the front opening (the X=135 face) to help
guide the brick in if it arrives very slightly off-center.

Model it:

1. New Component.
2. Name it `sensing_shroud`.
3. Create Sketch on the YZ plane.
4. Draw the outer cross-section as a rectangle: 38mm wide in Y, 36mm tall in Z.
   Position it centered on Y=22.5 (the belt centerline): from Y=3.5 to Y=41.5, Z=0 to Z=36.
5. Finish Sketch.
6. Extrude 65mm in X.
7. Create a second sketch on the same YZ face.
8. Draw the inner cutout: 30mm wide in Y, 32mm tall in Z.
   From Y=7.5 to Y=37.5, Z=0 to Z=32. (Bottom is open - do not draw the bottom line.)
   Draw three sides only: left, top, right. Leave the bottom open.
9. Finish Sketch.
10. Extrude Cut 65mm in X through the body. This hollows out the tunnel.

Add break beam holes in the side walls:

11. Click the left side wall outer face (the Y=3.5 face).
12. Create Sketch.
13. Add two 3.3mm circles:
    - X=15, Z=3 from the bottom of the shroud body (this puts the beam at 3mm above belt surface)
    - X=55, Z=3
    (These positions are relative to the shroud body: X=15 corresponds to 15mm into the 65mm length,
    X=55 is 55mm in. In assembly coordinates that will be about X=150 and X=190.)
14. Finish Sketch.
15. Extrude Cut through the left wall (4mm).
16. Repeat on the right side wall outer face (Y=41.5) with the same two hole positions.
    These holes must be exactly opposite the left wall holes so the beams cross straight.

Add color sensor pocket in the roof:

17. Click the top face of the shroud (the Z=36 face).
18. Create Sketch.
19. Draw a rectangle sized for your TCS3200/GY-31 module. Measure the actual PCB before drawing.
    Typical GY-31 PCB is about 30mm x 18mm. Center it at X=32, Y=22.5 (middle of the shroud length
    and width). Leave 1mm clearance on each side.
20. Finish Sketch.
21. Extrude Cut deep enough for the module body plus the sensor face, about 8 to 10mm deep.
    The sensor face should end up flush with or just inside the inner roof surface at Z=32.

Add entry chamfer:

22. Select the four inner edges of the front opening (the X=0 face of this component, which will be
    at X=135 in assembly).
23. Apply a 3mm chamfer.

In assembly, position the shroud with its bottom face at Z=60 (belt surface level) and its front
face at X=135.

Before exporting the shroud STL, add the removable mounting tabs or flanges:

1. Add one tab on each side wall near the bottom of the shroud, outside the brick path.
2. Put one 3.3mm clearance hole in each tab for M3 screws, or size the holes for the screws actually
   used in the wood.
3. If it is easy in Fusion, make the holes short slots across Y so the shroud can be centered over
   the belt after assembly.
4. Confirm screw heads, nuts, or washers cannot touch the belt or brick.
5. Confirm the shroud can be unscrewed without removing the conveyor belt or drive roller.

## Assembly Steps In Fusion

Keep assembly simple, but use assembly relationships where they save confusion. Coordinates are only
layout anchors. The real checks are face alignment, axis alignment, center planes, belt keep-out
envelopes, and interference checks.

1. Ground or fix `side_plate_left` so its inner face is the reference face for the conveyor gap.
2. Right-click `side_plate_left` in the browser, Copy, then right-click the assembly and Paste New to
   create `side_plate_right`. (Paste New creates an independent copy, not a linked one.)
3. Align the two inner side-plate faces with a 45mm offset and point both plate thicknesses outward.
   Use an as-built joint, rigid joint, or Align plus measurement, whichever is fastest in Fusion.
4. Place the fixed bearing holders on the outside faces of both side plates. Align each bearing pocket
   axis concentrically with the drive shaft hole axis and mate the pocket face against the plate face.
5. Place the slotted bearing holders on the outside faces of both side plates. Align each bearing
   pocket axis with the idler shaft slot centerline and keep the pocket face against the plate face.
6. Place the idler roller by aligning its shaft axis with the idler bearing axes, then center the
   roller body between the two inner side-plate faces.
7. Place the drive roller by aligning its shaft axis with the drive bearing axes, then center the
   belt-contact section between the side plates. Keep the hub on the +Y motor side.
8. Add transparent keep-out cylinders for the roller flange envelopes, or use the roller flanges
   themselves as the keep-out reference.
9. Place the wood block support between the rollers. Center it between the roller axes and side
   plates, align its top face about 2mm below the upper belt surface plane, and confirm both ends
   clear the roller flange and belt-wrap keep-out zones by at least about 10mm.
10. Place two crossmember spacers: one near X=50, one near X=360. Orient them so the 45mm length
    runs in Y between the inner faces of the side plates.
11. Move the whole conveyor subassembly up by `leg_h` when placing it into the full machine frame.
12. Place four leg copies near the corners, running vertically from the base to the side plate bottoms.
13. Import the NEMA17 STEP from `docs/datasheet/motion/nema17/cad/stepper_17hs4401s.step`.
14. Import the 20T pulley STEP from `docs/datasheet/motion/timing_pulley/cad/gt2_20t_5mm.step`.
15. Make a simple 60T pulley placeholder as a 40mm diameter, 11mm wide cylinder.
16. Place the motor mount outside the right side plate near the drive end.
17. For the 20T to 60T, 200mm GT2 path, start the motor shaft center near X=321.4, Z=50.
    Use that as a layout anchor only. Final motor holes come from the fitted bracket position.
18. Align the 20T and 60T pulley midplanes in Y and confirm the motor mount still slides in the
    timing belt tension direction.
19. Add a simple belt envelope and check top run and lower return clearance.
20. Add placeholder feed side rails from X=0 to X=120, centered around Y=22.5. Keep them separate
    from the shroud.
21. Add a placeholder shroud volume from X=135 to X=200, with the 30mm inner width and 32mm inner
    height reserved. Do not model the detailed sensor pocket or break-beam holes until this clears
    the assembled conveyor.
22. Place the exit lip with its bottom face at Z=60, centered in Y, ramp face pointing toward X=0.
    The flat top sits at Z=66 for the first handoff test. Position it near X=395 to X=410.
23. After the conveyor core and placeholder shroud clear, replace the placeholder with the removable
    sensing shroud. Put its bottom face at Z=60 and its front face at X=135. Confirm the shroud body
    clears the belt and sits between the side plates without touching them.
24. Add screw tabs or flanges for removable shroud mounting only where the Fusion assembly shows a
    real landing surface.

If a formal joint slows the session down, use Align plus measurement for that relationship and keep
going. The goal is an assembly that exposes clearance mistakes before wood is cut or prints are sent.

## Fusion Checks Before Printing

Run these checks before sending prints:

- Bearing holder bolt holes leave at least 2mm material to the bearing pocket
- Idler holder and idler shaft can slide without the shaft hitting the side plate
- Drive roller hub is on the motor side
- Drive roller hub does not touch the side plate
- Drive roller set screw location is reachable, even if the hole will be hand-drilled after printing
- Drive shaft has room for a small filed flat where the set screw lands
- Rollers are centered in the 45mm inner gap
- Wood block support top surface is about 2mm below the upper belt surface and does not touch either
  roller flange, belt wrap, pulley, or fastener
- Wood block support bottom face and any screws clear the lower belt return path near Z=40
- Bearing pockets face the side plates and bearings are captured after holders are bolted down
- Motor and timing belt path clear the side plate and frame legs
- Motor bracket is fitted to the actual GT2 pulley and belt path before drilling
- Motor stays inside the 610mm x 610mm footprint
- Set screw can be reached with the belt installed or by rotating the roller to an access position
- Sensing shroud bottom face is at Z=60, front face at X=135, does not touch either side plate
- Sensing shroud is removable with screws and can be adjusted or reprinted without rebuilding the
  conveyor core
- Sensing shroud inner width is 30mm and clears the 25mm belt during belt tracking tests
- Shroud break beam holes are opposite each other across the left and right walls
- Shroud color sensor pocket is in the roof, sensor face near inner roof surface at Z=92
- Shroud exit at X=200 leaves clear belt run to the drive end
- Feed side rails are separate for the first build, end before the shroud, and do not pinch the brick
  at the shroud entry
- Exit lip ramp face points toward X=0, first-test flat top at Z=66
- Exit lip does not overlap the drive roller (drive roller center is at X=380, lip starts at X=395)
- Space below the exit lip and drive end is clear for the chute entry funnel
- Belt exit is close enough to feed the future servo rotary chute selector

## Build Order At The Bench

1. Print the bearing pocket test coupon.
2. Test one MR85ZZ bearing in the coupon.
3. Adjust the pocket diameter if needed.
4. Print two fixed bearing holders and two slotted bearing holders.
5. Print the idler roller.
6. Cut one side plate to 410mm x 80mm.
7. Print the DXF drilling template at 100% scale. Tape it to the first side plate and drill it.
8. Clamp the first plate to the second blank and drill through both.
9. Make the idler shaft and idler bolt slots in both plates.
10. Dry-fit the idler roller, shaft, bearings, and holders.
11. Print the drive roller and motor mount.
12. Dry-fit the full conveyor with side plates, holders, rollers, crossmembers, and legs.
13. Measure shaft lengths from the real dry-fit. Cut the shafts with a Dremel cutoff wheel.
14. Install the drive shaft, 60T pulley, drive roller, and set screw.
15. Install the NEMA17, motor mount, 20T pulley, and GT2 belt.
16. Align pulley midplanes by eye and measurement.
17. Cut the wood block support to about 295mm to 300mm, smooth the top face, and install it just under
    the top belt run. Dry-fit before drilling final holes and trim the block shorter if either end is
    close to a roller flange, belt wrap, pulley, or fastener.
18. Wrap the neoprene strip with the idler near the middle of its slot travel.
19. Mark and cut the neoprene belt.
20. Splice with contact cement as a butt joint. Reinforce by gluing a 30mm strip of the same
    neoprene on the inside surface overlapping the joint by 15mm each side. Let cure fully before
    tensioning.
21. Tension the belt by sliding the idler holders.
22. Keep the conveyor belt and GT2 belt only tight enough to avoid slipping. Do not over-tension the
    small bearings and printed holders.
23. Run the conveyor alone before adding sensors or the chute.
24. Watch the belt splice over both rollers. If the splice bumps, thumps, or pulls the belt sideways,
    fix that before adding the sensing shroud.
25. Feed real bricks over the bare conveyor and confirm they ride straight enough for sensing.
26. Add separate temporary or printed feed side rails and confirm a brick enters the shroud zone
    without rubbing.
27. Add the removable sensing shroud and confirm belt clearance before calibration.

## Reference Dimensions

Use this only when a step asks for a number. Do not read this section top to bottom before building.

Conveyor:
- Side plates: 410mm x 80mm x 8mm, two pieces
- Side plate inner gap: 45mm
- Side plate outside width: 61mm total, from two 8mm plates plus 45mm gap
- Roller center distance: 350mm
- Belt surface height: about 240mm in the full machine
- Footprint limit: 610mm x 610mm

Rollers:
- Drive roller total width: 36mm, from 31mm roller body plus 5mm set screw hub
- Idler roller total width: 31mm
- Belt contact width: 25mm
- Flange width: 3mm each side
- Roller diameter at belt surface: 20mm
- Roller diameter at flange: 28mm
- Roller bore: 5mm

Bearings and holes:
- Drive shaft hole in side plate: 5.5mm round
- Idler shaft slot in side plate: 5.5mm wide x 20mm long
- MR85ZZ bearing: 8mm OD, 5mm ID, 2.5mm wide
- Bearing pocket: 8.15mm diameter x 2.6mm deep
- Bearing holder: 32mm x 32mm x 10mm
- Bearing holder mount holes: 10mm above and 10mm below the shaft center
- M3 clearance hole: 3.3mm
- M3 tap pilot in PLA: 2.5mm

Motor and belt:
- NEMA17 bolt pattern: four M3 holes on a 31mm x 31mm square
- NEMA17 center clearance: 22mm
- 20T to 60T GT2 center distance with 200mm belt: about 58.6mm after the actual pulley and belt are confirmed
- 20T to 20T GT2 center distance with 200mm belt: about 80mm fallback if the driven pulley changes

Optional Fusion parameters:
- `plate_length`: 410mm
- `plate_height`: 80mm
- `plate_thick`: 8mm
- `side_gap`: 45mm
- `roller_body_w`: 31mm
- `drive_hub_w`: 5mm
- `roller_dia`: 20mm
- `flange_dia`: 28mm
- `shaft_dia`: 5mm
- `shaft_clearance`: 5.5mm
- `bearing_pocket_d`: 8.15mm
- `bearing_pocket_depth`: 2.6mm
- `holder_size`: 32mm
- `holder_thick`: 10mm
- `m3_clearance`: 3.3mm
- `m3_tap`: 2.5mm
- `roller_center_dist`: 350mm
- `shaft_z`: 50mm
- `idler_x`: 30mm
- `drive_x`: 380mm
- `leg_h`: 180mm
- `gt2_center_dist`: 58.6mm

## First Test

The conveyor passes first test when:

- belt runs without walking off the flanges
- drive roller does not slip on the shaft
- GT2 belt does not skip
- neoprene belt has no visible sag over the wood block support
- one LEGO brick rides through without catching
- idler tension can still be adjusted after assembly

After that, move on to conveyor-to-chute handoff, color sensor shroud placement, and the servo rotary
chute selector.
