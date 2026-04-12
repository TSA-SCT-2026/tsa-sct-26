# Conveyor Build Guide

## Before You Start: Time Budget Warning

This guide covers only the conveyor subassembly. Full CAD also includes the servo rotary chute selector,
sensing station, bins, and frame. Those take another 5 to 8 hours, and the servo chute is the mechanism
that earns the most scoring points.

Note: downloaded conveyor assemblies found online use aluminum extrusion frames or require custom
endless belts from manufacturers. Neither is available in the build window. Build this guide from scratch.

Rough time budget for this guide:
- Fusion setup and parameters: 20 to 30 minutes
- Side plate: 30 to 45 minutes
- Two bearing holders: 30 minutes
- Two rollers: 30 to 45 minutes
- Motor mount: 20 minutes
- Belt deck and crossmember: 20 minutes
- Legs: 15 minutes
- Side rail guide: 20 minutes
- Exit lip: 15 minutes
- Break beam brackets: 20 minutes
- Assembly: 45 to 60 minutes

Total: about 5 to 6 hours for a first-time Fusion user working carefully.

## Goal

Build the conveyor in the same simple style as the reference video: two flat wood side plates, printed
bearing holders bolted to the outside faces, shafts through the bearings, printed rollers between the
plates, a NEMA17 on an adjustable side bracket, and a flat deck under the top belt run.

This guide is written for a fast Fusion session. Model the parts well enough to make drilling templates
and print files. Do not model every screw, belt tooth, shaft collar, or final belt splice.

## Coordinate Convention

Use this convention for the whole Fusion file:

- X axis: belt travel, feed end at X=0 and chute end at X=410
- Y axis: across the conveyor, left to right
- Z axis: vertical, plate bottom at Z=0
- Side plate size: 410mm long in X, 80mm tall in Z, 8mm thick in Y
- Side plate inner gap: 45mm
- Drive roller center: X=380, Z=50
- Idler roller center: X=30, Z=50

The drive end is the chute end. The motor and GT2 pulleys go outside the right side plate at the drive end.

## Critical Numbers

- Side plates: 410mm x 80mm x 8mm, two pieces
- Side plate inner gap: 45mm
- Side plate outside width: 61mm total, from two 8mm plates plus 45mm gap
- Drive roller total width: 36mm, from 31mm roller body plus 5mm set screw hub
- Idler roller total width: 31mm
- Belt contact width: 25mm
- Flange width: 3mm each side
- Roller diameter at belt surface: 20mm
- Roller diameter at flange: 28mm
- Roller bore: 5mm
- Drive shaft hole in side plate: 5.5mm round
- Idler shaft slot in side plate: 5.5mm wide x 20mm long
- MR85ZZ bearing: 8mm OD, 5mm ID, 2.5mm wide
- Bearing pocket: 8.15mm diameter x 2.6mm deep
- Bearing holder: 32mm x 32mm x 10mm
- Bearing holder mount holes: two M3 holes at Z=+10mm and Z=-10mm from bearing center
- M3 clearance hole: 3.3mm
- M3 tap pilot in PLA: 2.5mm
- NEMA17 bolt pattern: four M3 holes on a 31mm x 31mm square
- NEMA17 center clearance: 22mm
- 20T to 60T GT2 center distance with 200mm belt: about 58.6mm
- Roller center distance: 350mm
- Leg height: 180mm
- Belt surface height: about 240mm
- Footprint limit: 610mm x 610mm

## Fusion Setup

1. Open Fusion 360.
2. Create a new design.
3. Save as `conveyor_assembly`.
4. Rename the top component to `conveyor_assembly`.
5. Open Modify, Change Parameters.
6. Add these user parameters:

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

Use typed numbers if parameters slow you down. The finished shape matters more than parameter purity.

## Component 1: Side Plate

Make one side plate first. Later you will copy it to create the second side plate.

1. Right-click `conveyor_assembly`, New Component.
2. Name it `side_plate_left`.
3. Create Sketch on the XZ plane.
4. Draw a rectangle from the origin to X=`plate_length`, Z=`plate_height`.
5. Finish Sketch.
6. Extrude the rectangle by `plate_thick` in Y.

For this single part, the extrusion direction does not matter. In the assembly, the wood thickness must
point outward from the 45mm inner gap.

Add shaft holes:

1. Click the large XZ face of the side plate.
2. Create Sketch.
3. Add a 5.5mm circle at X=`drive_x`, Z=`shaft_z`. This is the drive shaft hole.
4. Add the idler shaft slot. In Sketch, choose Slot, then Center to Center Slot.
   - First click: X=`idler_x` minus 10mm, Z=`shaft_z`
   - Second click: X=`idler_x` plus 10mm, Z=`shaft_z`
   - Set the slot width (the distance across) to 5.5mm.
   - This gives a 20mm long, 5.5mm wide slot centered on the idler shaft position.
5. Finish Sketch.
6. Extrude Cut both shapes through the plate.

Add bearing holder mounting holes:

1. Click the same XZ face again.
2. Create Sketch.
3. At the drive shaft center, add two 3.3mm circles:
   - X=`drive_x`, Z=40
   - X=`drive_x`, Z=60
4. At the idler shaft center, add two horizontal mounting slots using Center to Center Slot:
   - Slot centered at X=`idler_x`, Z=40, length 18mm, width 3.3mm
   - Slot centered at X=`idler_x`, Z=60, length 18mm, width 3.3mm
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
5. Add two 3.3mm mounting holes at:
   - X=0, Z=10
   - X=0, Z=-10
6. Finish Sketch.
7. Extrude the square body to 10mm in Y. Do not include the mounting hole profiles in the extrusion.
8. Cut the two M3 holes through all.

Add the bearing pocket:

1. Click the front face of the holder.
2. Create Sketch.
3. Draw an 8.15mm circle centered on the origin.
4. Finish Sketch.
5. Extrude Cut 2.6mm deep.

Add shaft clearance:

1. Click the back face of the holder.
2. Create Sketch.
3. Draw a 5.5mm circle centered on the origin.
4. Finish Sketch.
5. Extrude Cut through all.

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
   - Slot centered at X=0, Z=10, length 18mm, width 3.3mm
   - Slot centered at X=0, Z=-10, length 18mm, width 3.3mm
6. Cut the slots through all.
7. Keep the same bearing pocket and 5.5mm shaft clearance hole.

Use washers under the screw heads when this part is assembled.

## Component 4: Idler Roller

The idler roller is the simple roller. Start with it because it has no set screw hub.

1. New Component.
2. Name it `roller_idler`.
3. Create Sketch on the XZ plane.
4. Draw this side profile above the X axis.

Note on coordinates: in this revolve sketch, Z values are radii from the X axis. When Fusion revolves
the profile 360 degrees around the X axis, each Z value becomes a diameter twice that size. Z=10 gives
a 20mm diameter. Z=14 gives a 28mm diameter. Z=0 is on the axis and stays as the centerline.

   - line from X=0, Z=0 to X=0, Z=14
   - line from X=0, Z=14 to X=3, Z=10
   - line from X=3, Z=10 to X=28, Z=10
   - line from X=28, Z=10 to X=31, Z=14
   - line from X=31, Z=14 to X=31, Z=0
   - line from X=31, Z=0 back to X=0, Z=0

5. Finish Sketch.
6. Revolve the closed profile 360 degrees around the X axis.
   In the Revolve dialog, set the Axis to the X axis. The profile must be a closed loop. If Fusion does
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
4. Create a sketch on the motor-side end face.
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

## Component 6: Motor Mount

The motor mount is a flat adjustable plate outside the right side plate.

1. New Component.
2. Name it `motor_mount`.
3. Create Sketch on the XZ plane.
4. Draw a 90mm x 70mm rectangle.
5. Finish Sketch.
6. Extrude 6mm.

Add NEMA17 holes:

1. Click the large face.
2. Create Sketch.
3. Pick a motor shaft center near the middle of the bracket.
4. Draw a 22mm center clearance circle.
5. Draw four 3.3mm holes around it at:
   - X=+15.5, Z=+15.5 from motor center
   - X=+15.5, Z=-15.5 from motor center
   - X=-15.5, Z=+15.5 from motor center
   - X=-15.5, Z=-15.5 from motor center
6. Draw two 3.3mm x 20mm horizontal slots using Center to Center Slot for mounting the bracket to the
   side plate. Put one near the top of the bracket and one near the bottom.
7. Finish Sketch.
8. Extrude Cut all holes and slots through all.

The motor shaft center should start about 58.6mm from the drive shaft center when placed in the
assembly. This is not a fixed offset in a single direction; it is the center-to-center distance for
the 200mm GT2 belt with 20T and 60T pulleys. The bracket adjustment slots let you shift the motor
on the bench until the belt runs at the right tension. In Fusion, place the motor shaft close enough
to check clearance, then let real tensioning handle the final position.

## Component 7: Belt Deck

Use hardboard if you can. It is faster than printing.

For Fusion reference:

1. New Component.
2. Name it `belt_deck`.
3. Create Sketch on the XY plane.
4. Draw a rectangle 315mm long in X and 22mm wide in Y.
5. Finish Sketch.
6. Extrude 4mm in Z.

Assembly position:

- X position: between the rollers, not touching either roller
- Y position: centered between side plates
- Z position: set the top surface of the deck at Z=58. The roller centers are at Z=50, rollers have a
  10mm radius, so the belt surface sits at Z=60. The deck top at Z=58 puts it 2mm below the belt
  surface, letting the belt ride on the rollers without dragging hard on the deck.

Do not overdesign deck tabs. A ledge, a small printed support, or a dab of removable tape is enough
for the first conveyor test.

## Component 8: Crossmember Spacer

The crossmembers hold the two side plates at the 45mm inner gap.

Simple printed spacer:

1. New Component.
2. Name it `crossmember_spacer`.
3. Sketch a 45mm x 20mm rectangle.
4. Extrude 20mm.
5. Add a 3.3mm hole through each end if you want it bolted.

Use two spacers at minimum: one near the feed end and one near the drive end. Add a third in the
middle if the side plates flex.

## Component 9: Legs

The legs lift the conveyor to the target belt height of about 240mm. They are simple printed or wood
posts. Model them as blocks so the assembly shows the correct height.

1. New Component.
2. Name it `leg`.
3. Create Sketch on the XZ plane.
4. Draw a 20mm x 20mm square.
5. Finish Sketch.
6. Extrude 180mm in Y.

You will place four copies of this leg in the assembly: one near each corner of the conveyor frame.
The legs sit on the ground plane (Z=0) and support the bottom of the side plates.

If you plan to cut the legs from wood, the 20mm x 20mm cross section is a good starting point. Adjust
to match whatever wood stock is available.

## Component 10: Side Rails (Feed Guide)

Side rails center the brick on the belt before it reaches the break beams. Without them, a brick placed
slightly off-center can drift and fall off the side or arrive at the sensor at an inconsistent position.

The rails start wide at the feed end and narrow to just wider than the largest brick, so any brick
placed anywhere in the entry zone ends up centered by the time it reaches the sensors.

Key dimensions:
- Entry gap (at X=0): 38mm between rail inner faces (fills most of the 45mm inner gap)
- Exit gap (at X=120): 26mm between rail inner faces (just wider than the 23.7mm 2x3 brick)
- Transition length: 120mm of tapered wall from entry to exit
- Rail height: 6mm above belt surface (engages the brick body, clear of studs)
- Rail thickness: 4mm

The rails stop at X=120 and leave the rest of the belt clear.

Model this as a standalone part at Z=0, then position it in assembly at belt surface height.

1. New Component.
2. Name it `side_rail_guide`.
3. Create Sketch on the XY plane.
4. Draw the left rail as a trapezoid (top-down view):
   - Left edge: X=0 to X=120, Y=0 (outer face, flush with left plate inner face)
   - Bottom edge at X=0: Y=0 to Y=3.5 (4mm thickness at entry end)
   - Bottom edge at X=120: Y=0 to Y=9.5 (4mm thickness at exit end, inner face shifted inward)
   - Connect (X=0, Y=3.5) to (X=120, Y=9.5) to close the trapezoid inner face
5. Finish Sketch.
6. Extrude 6mm in Z.
7. Repeat for the right rail as a mirrored trapezoid:
   - Left edge at X=0: Y=41.5 to Y=45 (flush with right plate inner face at Y=45)
   - Inner face at X=0: Y=41.5 (giving 38mm gap from Y=3.5 to Y=41.5)
   - Inner face at X=120: Y=35.5 (giving 26mm gap from Y=9.5 to Y=35.5)
   - Or use Sketch Mirror about the centerline Y=22.5 to copy the left rail shape.
8. Both rails are in the same component. Extrude the right rail 6mm in Z with Join.

In assembly, position the bottom face of this component at Z=60 (belt surface level).

Add two 3.3mm mounting holes through each end for M3 bolts into the side plates if needed.
Friction-fit or hot-glue is fine for the first test.

Check in assembly that the narrow exit end at X=120 leaves room for the break beam emitter and
receiver on opposite side plate faces without the rail body blocking the beam path.

## Component 11: Exit Lip

The exit lip is a curved ramp at the drive end that converts the brick's horizontal momentum into
downward motion. It replaces the flat stop wall concept. A flat wall risks the brick tumbling or
bouncing back. The curved lip guides the brick over the edge and down into the chute entry below.

Geometry:
- Spans between the inner faces of the side plates (45mm wide)
- Lip height: 5-6mm above belt surface level (enough to engage the brick body without launching it)
- Curve: a quarter-arc with about 25mm radius, transitioning from horizontal (belt level) to angled
  downward toward the chute entry
- Wall thickness: 4mm
- The bottom of the lip piece sits at belt surface height (Z=60) and the curved face is on the
  downstream side (X=410 face)

The lip is a wedge-shaped block that sits at the drive end of the belt. The face pointing toward the
incoming brick (the -X face) is angled or curved so it deflects the brick upward and over the edge
rather than stopping it dead. Think of a small speed bump at the end of the belt.

Model it as a standalone part at Z=0, then position in assembly:

1. New Component.
2. Name it `exit_lip`.
3. Create Sketch on the XZ plane.
4. Draw this cross-section profile (side view of the lip):
   - Bottom-left corner: X=0, Z=0
   - Bottom-right corner: X=15, Z=0
   - Top-right corner: X=15, Z=6 (6mm tall, the flat top of the lip)
   - Top-left corner: X=4, Z=6 (the top steps back 4mm to create the ramp face)
   - Angled line from (X=4, Z=6) back to (X=0, Z=0) - this is the ramp face
   - The angled face is what the brick hits. It deflects the brick upward over the lip.
5. Finish Sketch.
6. Extrude 45mm in Y (spanning the inner gap between the side plates).
7. Optional: select the top upstream edge (where the angled face meets the flat top) and apply a
   3mm fillet to soften the corner. This is the "curved" version. Skip the fillet if it causes
   trouble in Fusion.

In assembly, position the bottom face at Z=54 so the flat top of the lip sits at Z=60, flush with
the belt surface. The ramp face should point toward X=0 (toward the incoming brick).

Mount holes: two 3.3mm holes through the bottom face for M3 bolts into a crossmember below.

The chute entry funnel for the servo rotary chute selector sits directly below this lip. Position
the chute entry when modeling the chute selector subassembly.

## Component 12: Sensing Shroud

The sensing shroud is a single printed tunnel that covers the sensing zone on the belt. It handles
three things in one part: it blocks ambient light for the color sensor, holds both break beam pairs
at the correct height, and mounts the TCS3200 color sensor in its roof looking straight down at the
brick.

Placing everything in one enclosed part gives the color sensor a controlled, repeatable lighting
environment. Calibration done with this shroud installed is valid. Calibration without it is not.

The shroud sits on top of the belt between X=135 and X=200. The brick enters from the feed-end
opening, passes through, and exits toward the drive end. The side rail guide ends at X=120, so
the brick is already centered before it enters.

Outer dimensions:
- Length: 65mm in X (X=135 to X=200 in assembly)
- Width: 34mm in Y (4mm walls on each side of the 26mm inner width)
- Height: 36mm in Z above belt surface (4mm roof above the 32mm inner height)

Inner dimensions:
- Width: 26mm in Y (matches side rail exit gap, brick fits with a few mm clearance)
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
4. Draw the outer cross-section as a rectangle: 34mm wide in Y, 36mm tall in Z.
   Position it centered on Y=22.5 (the belt centerline): from Y=5.5 to Y=39.5, Z=0 to Z=36.
5. Finish Sketch.
6. Extrude 65mm in X.
7. Create a second sketch on the same YZ face.
8. Draw the inner cutout: 26mm wide in Y, 32mm tall in Z.
   From Y=9.5 to Y=35.5, Z=0 to Z=32. (Bottom is open - do not draw the bottom line.)
   Draw three sides only: left, top, right. Leave the bottom open.
9. Finish Sketch.
10. Extrude Cut 65mm in X through the body. This hollows out the tunnel.

Add break beam holes in the side walls:

11. Click the left side wall outer face (the Y=5.5 face).
12. Create Sketch.
13. Add two 3.3mm circles:
    - X=15, Z=3 from the bottom of the shroud body (this puts the beam at 3mm above belt surface)
    - X=55, Z=3
    (These positions are relative to the shroud body: X=15 corresponds to 15mm into the 65mm length,
    X=55 is 55mm in. In assembly coordinates that will be about X=150 and X=190.)
14. Finish Sketch.
15. Extrude Cut through the left wall (4mm).
16. Repeat on the right side wall outer face (Y=39.5) with the same two hole positions.
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

## Assembly Steps In Fusion

Keep assembly simple. You only need enough CAD to confirm the parts fit.

1. Place `side_plate_left` so its inner face is at Y=0 and the plate material points outward.
2. Right-click `side_plate_left` in the browser, Copy, then right-click the assembly and Paste New to
   create `side_plate_right`. (Paste New creates an independent copy, not a linked one.)
3. Move `side_plate_right` so its inner face is at Y=45mm and the plate material points outward.
4. Place fixed bearing holders at X=380, Z=50 on the outside faces of both side plates.
5. Place slotted bearing holders at X=30, Z=50 on the outside faces of both side plates.
6. Place the idler roller centered between the plates at X=30, Z=50.
7. Place the belt-contact section of the drive roller centered between the plates at X=380, Z=50,
   with the hub on the motor side.
8. Place the belt deck centered between the rollers and between the side plates, top surface at Z=58.
9. Place two crossmember spacers: one near X=50, one near X=360. Orient them so the 45mm length
   runs in Y between the inner faces of the side plates.
10. Place four leg copies near the corners, sitting at Z=0 under the side plates.
11. Import the NEMA17 STEP from `docs/datasheet/motion/nema17/cad/stepper_17hs4401s.step`.
12. Import the 20T pulley STEP from `docs/datasheet/motion/timing_pulley/cad/gt2_20t_5mm.step`.
13. Make a simple 60T pulley placeholder as a 40mm diameter, 11mm wide cylinder.
14. Place the motor mount outside the right side plate near X=380.
15. Place the motor so the shaft center is approximately 58.6mm from the drive shaft center.
    Move it by eye until the pulleys look roughly aligned and the motor clears the side plate.
    The bracket adjustment slots handle final tensioning on the bench.
16. Check that the 20T and 60T pulley midplanes line up in Y.
17. Place the side rail guide with its bottom face at Z=60, centered in Y, starting at X=0.
18. Place the exit lip with its bottom face at Z=54, centered in Y, ramp face pointing toward X=0.
    The flat top sits at Z=60, flush with belt surface. Position it near X=395 to X=410.
19. Place the sensing shroud with its bottom face at Z=60 and its front face at X=135.
    Confirm the shroud body sits between the side plates without touching them.

Do not fight Fusion joints for hours. Move bodies into place, measure, and use simple align tools if
joints slow you down.

## Fusion Checks Before Printing

Run these checks before sending prints:

- Bearing holder bolt holes leave at least 2mm material to the bearing pocket
- Idler holder and idler shaft can slide without the shaft hitting the side plate
- Drive roller hub is on the motor side
- Drive roller hub does not touch the side plate
- Rollers are centered in the 45mm inner gap
- Belt deck top surface is at Z=58, does not touch either roller
- Motor and timing belt path clear the side plate and frame legs
- Motor stays inside the 610mm x 610mm footprint
- Set screw can be reached with the belt installed or by rotating the roller to an access position
- Side rail guide bottom face is at Z=60, narrows from 38mm to 26mm, stops before X=130
- Sensing shroud bottom face is at Z=60, front face at X=135, does not touch either side plate
- Shroud break beam holes are opposite each other across the left and right walls
- Shroud color sensor pocket is in the roof, sensor face near inner roof surface at Z=92
- Shroud exit at X=200 leaves clear belt run to the drive end
- Exit lip ramp face points toward X=0, flat top at Z=60 flush with belt surface
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
17. Cut or print the belt deck and install it just under the top belt run.
18. Wrap the neoprene strip with the idler near the middle of its slot travel.
19. Mark and cut the neoprene belt.
20. Splice with contact cement as a butt joint. Reinforce by gluing a 30mm strip of the same
    neoprene on the inside surface overlapping the joint by 15mm each side. Let cure fully before
    tensioning.
21. Tension the belt by sliding the idler holders.
22. Run the conveyor alone before adding sensors or the chute.

## First Test

The conveyor passes first test when:

- belt runs without walking off the flanges
- drive roller does not slip on the shaft
- GT2 belt does not skip
- neoprene belt has no visible sag over the deck
- one LEGO brick rides through without catching
- idler tension can still be adjusted after assembly

After that, move on to conveyor-to-chute handoff, color sensor shroud placement, and the servo rotary
chute selector.
