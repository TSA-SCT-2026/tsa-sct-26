# Conveyor Build Guide

## Build Status (as of 2026-04-18)

Steps 1-7 of "Do This First" are provisionally complete in Fusion. Remaining conveyor CAD:
- Screw holes between side plate and center board
- Final pulley center distance and Y coplanarity check (gated on confirmed conveyor height from teammate)
- Exit lip geometry (shape is designed, position in frame is gated on height)
- Sensing shroud and side rails (separate subassembly, not height-dependent, next to model)

Modifications made during the build sprint that deviate from the original guide steps:
- Motor mount: replaced "grounded and braced with standoffs" with a printed 2-stage L bracket with gussets on motor and frame sides, M3 mounting feet with short slots on each stage
- Center belt support: extended to contact both inner side-plate faces so screws can be driven from outside through the side plates into the support ends
- Motor pulley Y: set flush with motor shaft tip. Driven pulley Y: seated against inner bearing holder face with a 2mm printed spacer

## Before You Start: Build Scope

This guide covers only the conveyor subassembly. Full CAD also includes the servo rotary chute selector,
sensing station, bins, and frame. Treat those as separate subassemblies, not as reasons to stall the
conveyor core.

Note: downloaded conveyor assemblies found online use aluminum extrusion frames or require custom
endless belts from manufacturers. Neither is available in the build window. Build this guide from scratch.

## Goal

Build the conveyor in the same simple style as the reference video: a wood block support that
parts can screw into, thin wood side plates, printed bearing holders, shafts through the bearings,
printed rollers between the plates, and a grounded outside NEMA17 motor board with the motor facing
inward toward the right conveyor side plate.

Reference tutorial structure to preserve:
- NEMA17 motor, printed rollers, printed bearing holders, motor adjustment slots, shaft spacers, and pulleys
- Wood or hardboard support piece connected into the frame, not an aluminum extrusion frame
- Adjustable bearing holders for belt tension
- Movable motor board slots for timing belt tension
- Grounded motor structure through a 2-stage L bracket with gussets on motor and frame sides
- Overall visual build order and simple screw-together construction

Do not copy the tutorial dimensions or hardware as law. This build is intentionally modified for the
team's materials and sorter geometry: 5mm shafts and matching 5mm-ID bearings, the current 25mm
neoprene belt width, the current top-run length target, the NEMA17 motor, and the clearance needed
for LEGO bricks.

This guide is written to get usable conveyor CAD and build files without wasting time on fake
precision. Model the parts well enough to make drilling templates and print files. Do not model every
screw, belt tooth, shaft collar, or final belt splice.

Numeric values are sketch anchors and derived checks, not proof that the real assembly works. Use
exact values for axis direction, hole patterns, clearances, and rough placement inside part sketches
or assembly skeleton sketches. In assembly, create visible construction points, axes, or planes for
the few important anchors, then place parts relative to that construction geometry: mate faces, align
shaft axes, center parts between inner side-plate faces, and check belt keep-out envelopes. Do not
use typed Move coordinates as the source of truth. If a sketch anchor and the real assembly conflict,
trust the real assembly and update the dimension.

## Do This First

Ignore the detailed reference numbers until a step asks for them. Build the CAD in this order:

1. Side plates with shaft holes
2. Bearing holders
3. Rollers
4. Core conveyor assembly: side plates, bearings, shafts, and rollers
5. Wood block support and crossmembers
6. Low base plate with short conveyor mounting feet or standoffs
7. Outside NEMA17 motor board, fitted after the core conveyor and low frame exist
8. Extras after the conveyor core fits: side rails, sensing shroud, and exit lip

Do not start by perfecting the motor, shroud, exit lip, or frame. The conveyor core proves whether the
rest of the CAD has somewhere real to attach.

## Sketch Anchor Rules

Only sketch anchors you should define directly:
- Drive shaft center: X=340, Z=50
- Idler shaft center: X=30, Z=50
- Bearing holder mounting holes: 10mm above and 10mm below each shaft center

Everything else is fit-later or adjustable:
- Motor board: start from the 20T-to-20T skeleton point, then fit to the real pulley and belt path
- Exit lip: first version is a handoff test coupon and will probably move
- Sensing shroud: use the listed dimensions, then verify belt and brick clearance
- Crossmembers and frame panels: place where they support the frame and do not block moving parts

Do not try to be a CNC machine. Use sketch anchors to stop CAD from drifting, not to replace dry-fit
checks. For assembly-level placement, make a simple skeleton sketch with the conveyor datum, shaft
axes, belt-surface plane, base plane, and provisional motor-shaft point. Turn that sketch on while
placing components, then constrain parts to it with joints, alignments, offsets, and measured
clearance checks.

## Do Not Overthink

- Motor board: model the slot and through-bolt pattern, but confirm pulley alignment in assembly before drilling wood.
- Exit lip: the first version is allowed to be wrong. It exists to make handoff testing fast.
- Sensing shroud: approximate placement is fine for v1 if the belt clears and the brick does not rub.
- Crossmembers: place them where they stiffen the side plates and stay out of the belt path. If the
  wood block support holds the spacing well, fewer printed crossmembers may be needed.
- Fusion parameters: skip them if they slow you down. Type the numbers directly.

## Hole Strategy

Model holes in Fusion when the hole pattern is part of a printed part or drilling template:
- side-plate shaft holes and bearing-holder mounting holes
- bearing-holder holes, pockets, and slots
- NEMA17 motor slots and motor-board through holes in the outside grounded motor board
- shroud tabs or flanges, preferably as short adjustment slots where useful
- break-beam and color sensor mounting geometry in the sensing shroud

Mark or drill holes from the fitted real assembly when the exact position depends on wood stock,
pulley alignment, or clearance:
- final motor-board holes through the real wood motor board and base brackets
- wood block support screws
- crossmember screws if a crossmember is fitted after pulley clearance is known
- temporary side rail screws
- exit lip screws unless the lip lands on a known printed bracket

For printed threaded features, use pilot holes in Fusion only if they help. Tapping in the real part
is normal. For the drive roller set screw, the first-build default is to print the hub solid, drill a
2.5mm pilot toward the shaft bore, tap M3 by hand, and file a small flat on the shaft.

## Coordinate Convention

Use this convention for the conveyor subassembly:

- X axis: belt travel, feed end at X=0 and chute end at X=370
- Y axis: across the conveyor, left to right
- Z axis: vertical, side plate bottom at local Z=0
- Side plate size: 370mm long in X, 80mm tall in Z, 8mm thick in Y as a CAD placeholder
- Side plate inner gap: 50mm
- Drive roller center: X=340, Z=50
- Idler roller center: X=30, Z=50

If the Home Depot side-plate stock is not 8mm thick, change the side plate thickness after measuring
the real material. Do not change shaft centers, bearing-holder hole spacing, or the 50mm inner gap
just because the outer plate thickness changes.

This keeps the pulley frame simple while modeling. The active frame path is now a low base plate with
short feet, standoffs, or small brackets that locate the existing conveyor side plates, not a tall
four-leg conveyor and not duplicate full-length side panels by default.

The 12mm base offset is a provisional starting support height, not an unsupported floating gap. It comes from two
first-pass checks:
- Handoff check: local belt surface is about Z=60, so a 12mm base offset puts the belt surface near
  72mm from the base. With a 5mm to 6mm exit lip, the first handoff target is around 77mm to 78mm,
  close to the provisional chute entry height.
- Motor-board check: the NEMA17 body sits outside the conveyor on a grounded board, so the conveyor
  base offset is now chosen for chute handoff, lower belt return clearance, and motor-board brace
  geometry rather than for a motor body under the belt.

If either check changes, change the offset. A 6mm, 10mm, or direct-to-base conveyor is acceptable if
the motor-board braces clear the base and the belt exit still feeds the chute. Whatever offset is
used, support it with fixed feet, standoffs, or short brackets under the conveyor side plates.

The drive end is the chute end. The NEMA17 body sits outside the right conveyor side plate on a
grounded motor board, the shaft points inward toward the conveyor, and the 20T GT2 pulleys remain in
the service bay between the motor board and the right conveyor side plate.

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
4. Draw a 370mm x 80mm rectangle from the origin.
5. Finish Sketch.
6. Extrude the rectangle by 8mm in Y.

For this single part, the extrusion direction does not matter. In the assembly, the wood thickness must
point outward from the 50mm inner gap.

Add shaft holes:

1. Click the large XZ face of the side plate.
2. Create Sketch.
3. Place a construction point for the drive shaft at X=340, Z=50.
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

## Component 6: Grounded Outside NEMA17 Motor Board

The active NEMA17 layout uses a separate vertical motor board parallel to the right conveyor side
plate. The motor body sits outside the conveyor, the shaft points inward toward the conveyor, and the
20T GT2 motor pulley sits between the motor board and the right conveyor side plate. This avoids a
large motor cutout in the conveyor side plate and creates a rear/right service bay that can later
carry wiring or electronics after the conveyor and chute geometry stop moving.

The motor board must not float. Its lower through-bolt holes tie into base brackets or short standoffs. Its
upper through-bolt holes tie back toward the conveyor through rigid standoffs or a brace that clears
the GT2 belt, pulley set screws, and bearing holders.
Use fixed gusseted feet as the default once chute-entry height is confirmed. Use short slot holes in
those feet for one-time setup adjustment before final tightening. Keep shims or washers as fallback
for minor correction after first fit.

Create the motor point in the conveyor side-view skeleton sketch before modeling the board:

1. Create or show the side-view skeleton sketch on the conveyor XZ reference plane.
2. Add the drive shaft point at X=340, Z=50.
3. Draw an 80mm construction circle centered on the drive shaft point. This is the 20T-to-20T,
   200mm GT2 center-distance path.
4. Add a horizontal construction line at Z=14.
5. Use the lower-left intersection of the 80mm circle and the Z=14 line as the motor shaft point.
   This point is X=268.6, Z=14 in conveyor-local coordinates.
6. With the conveyor side-plate bottom 12mm above the base, this same point appears in the motor
   board sketch at base-relative X=268.6, Z=26.
7. Keep the sketch visible so the motor board sketch can project this point. Do not place the motor
   by typing a free 3D coordinate.

Create the motor board:

1. New Component.
2. Name it `outside_motor_board`.
3. Create a vertical construction plane parallel to the right conveyor side plate and about 25mm
   outboard from the right side plate outside face.
4. Create Sketch on that plane.
5. Project the motor shaft point from the side-view skeleton sketch into this board sketch. It should
   land at base-relative X=268.6, Z=26.
6. Draw the board as a rectangle from X=220 to X=330 and base-relative Z=0 to Z=80. If the base
   brackets need more screw landing area, extend the board downward or rearward only after checking
   the 610mm footprint.
7. Finish Sketch.
8. Extrude the board to the measured wood thickness. Use 6mm to 8mm as the placeholder.

Add motor and board holes:

1. Click the board face that the NEMA17 mounts against.
2. Create Sketch.
3. Project the motor shaft point again at base-relative X=268.6, Z=26.
4. Draw a 22mm center clearance circle on the motor shaft point.
5. Add four NEMA17 slot center points before rotation:
   - top-left: X=253.1, Z=41.5
   - top-right: X=284.1, Z=41.5
   - bottom-left: X=253.1, Z=10.5
   - bottom-right: X=284.1, Z=10.5
6. Create four center-to-center slots centered on those points:
   - slot length: 12mm
   - slot width: 3.5mm
   - slot direction: 26.8 degrees upward toward the drive shaft point
7. Add four board through-bolt holes:
   - lower-left: X=230, Z=8
   - lower-right: X=320, Z=8
   - upper-left: X=230, Z=72
   - upper-right: X=320, Z=72
   - hole diameter: 4.3mm for M4 preferred, or 3.3mm for M3 fallback
8. Finish Sketch.
9. Extrude Cut the center clearance hole, NEMA17 slots, and board through-bolt holes through all.

Motor board drill guide (model after motor board CAD is final, blocked on conveyor height):

The NEMA17 slot pattern at 26.8 degrees is nearly impossible to mark accurately by hand. After the
motor board is finalized, create a thin printed drill guide from the same sketch:

1. New Component. Name it `motor_board_drill_guide`.
2. Create Sketch on the same vertical plane as the motor board.
3. Copy in (or re-draw) only the hole and slot geometry: 22mm center clearance circle, four slot
   end-point pairs, and four corner through-bolt holes. Do not include the outer board rectangle.
4. Add a short registration lip (2mm tall, along the bottom edge of the board) so the guide sits
   flush on the wood board bottom edge and cannot slide while drilling.
5. Finish Sketch. Extrude 4mm. This is the full guide thickness.
6. Print in PLA. Lay it on the wood board, clamp, and drill through it.

For the NEMA17 slots: slots cannot be drilled in one pass. Drill 3.5mm round holes at both end
points of each slot (eight holes total), then join the holes with a jigsaw, rotary tool, or file.
The drill guide marks all eight end points and the center clearance hole center.

Do not model or print this guide until the conveyor height is confirmed and the motor-board sketch
is finalized. The Z position of every hole changes with the final support offset.

Assembly placement:

1. Mate the NEMA17 face to the outside face of the motor board.
2. Align the NEMA17 shaft axis to the projected motor shaft point.
3. Place the motor body outside the board so the shaft points inward toward the conveyor.
4. Place one 20T pulley on the motor shaft and one 20T pulley on the drive shaft.
5. Align the two pulley midplanes in Y before adding the GT2 belt envelope.
6. Confirm pulley set screws and board bolts remain reachable after the GT2 belt is installed.

Y placement target:

```text
left inner side plate face: Y=0
right inner side plate face: Y=50
right side plate outside face: Y=58
bearing holder outer face: about Y=68
driven pulley midplane target: about Y=74.5
motor board inner face: about Y=83
motor board outer face: about Y=89 to Y=91
```

Cable routing:
- Rotate the motor so the connector and wires point toward the rear/right service bay, away from the
  belt, chute, bins, and operator-facing side.
- Reserve the service bay for later electronics layout, but do not lock ESP32, driver, or power
  placement until the conveyor, chute, and bins are stable.
- Add printed clips or strain relief later if they improve presentation and keep wires clear.

## Component 7: Wood Block Support

Use one straight wood block cut from the real conveyor dry-fit, usually about 255mm to 260mm with the
current roller geometry. This replaces the earlier 2020 aluminum extrusion support idea and keeps the
conveyor closer to the reference tutorial. The block is both the top-belt support surface and a simple
structural spine that thin side plates and small brackets can screw into.

Choose the straightest available wood stock. Sand the top face smooth and cover it with smooth packing
tape, UHMW tape, or another low-friction strip if the neoprene belt drags during testing. Do not use a
warped block just because it matches the CAD number. Since wood stock will be picked up locally, do
not lock the thickness until the actual board or block is in hand.

Important clearance rules:
- End clearance: do not let the wood support run close to either roller flange, belt wrap, or pulley
  hardware. Use the actual roller flange envelope as the keep-out reference. With the current shaft
  centers at X=30 and X=340 and a 28mm flange diameter, a centered 255mm to 260mm block leaves about
  11mm to 13.5mm clearance to each flange envelope before belt and wood tolerance. The older 295mm to
  300mm block length belongs to the longer 410mm side-plate layout and is now too long.
- Lower return clearance: with the current 20mm roller diameter, the lower belt return runs near
  Z=40. If the wood support top is at Z=58, the support must be thin enough that its bottom stays
  above the lower return path. Use about 10mm to 12mm support thickness for this roller size. If the
  wood in hand is closer to 19mm thick, either rip or plane it thinner, notch it for the lower return
  path, or revise the roller diameter and shaft height before cutting parts.

For Fusion part modeling:

1. New Component.
2. Name it `wood_block_support`.
3. Create Sketch on the XY plane.
4. Draw a rectangle about 255mm to 260mm long in X and about 20mm to 50mm wide in Y. Keep it narrow
   enough that it does not hit the rollers, belt return path, side plates, or bearing hardware.
5. Finish Sketch.
6. Extrude to the measured wood block thickness in Z. If no stock is measured yet, use 10mm as the
   placeholder.

Assembly placement:

Do not place the wood block by typing a global coordinate. Place it from the parts already in the
assembly:

1. Look from the side of the conveyor so both rollers are visible as circles.
2. Center the wood block between the idler roller axis and the drive roller axis.
3. Center the wood block between the two inner side-plate faces.
4. Treat the top of the roller belt-contact surface as the upper belt surface.
5. Align the wood block top face just below that upper belt surface, about 2mm lower.
6. Check both wood block ends against the roller flanges and belt-wrap area. Shorten or shift the
   block until each end has about 10mm or more of visible clearance.
7. Check the bottom of the block against the lower belt return. Thin, notch, or move the block if the
   lower belt would rub it.

Reference math only: in the starter conveyor sketch, the roller center is at Z=50 and the roller
radius is 10mm, so the upper belt surface is around Z=60 and the wood block top starts around Z=58.
That value is a reminder of the 2mm offset, not an assembly coordinate to type into Fusion.

Use the wood block to help tie the side plates together. Fasten through the thin side plates into
the block, or use small printed angle brackets if that is easier to drill. Keep fastener heads below
the belt path. No deck tabs needed for v1.

## Component 8: Crossmember Spacer

The crossmembers and wood block hold the two side plates at the 50mm inner gap.

Simple printed spacer:

1. New Component.
2. Name it `crossmember_spacer`.
3. Sketch a 49.5mm to 50mm x 20mm rectangle.
4. Extrude 20mm.
5. Add a 3.3mm hole through each end if you want it bolted.

Use crossmembers only where they fit cleanly. A feed-end spacer is usually useful. A drive-end spacer
is optional because the drive side also has the roller hub, driven pulley, motor pulley, timing belt,
exit lip, and future chute handoff. Do not widen the conveyor frame just to fit a spacer. Move it,
lower it, shorten it in X, replace it with a small bracket, or omit it if it interferes with the
pulley path, belt-wrap area, set screw access, exit lip, or chute entry.

The spacer does not need a zero-clearance fit. A 50mm spacer in a 50mm inner gap is a CAD-perfect
fit, but real wood and printed plastic may bind. Printing around 49.5mm, or printing 50mm and sanding
until it slips in without spreading the side plates, is acceptable. If the wood block is wide and
screwed in well, it may provide most of the spacing stiffness. Add another spacer only if the side
plates flex.

## Component 9: Low Base Plate And Conveyor Mounting Cleats

1. New Component.
2. Name it `base_plate`.
3. Create Sketch on the XY plane.
4. Draw a rectangle inside the 610mm x 610mm competition footprint. Start around 590mm x 590mm unless
   the bin layout needs the full space.
5. Finish Sketch.
6. Extrude to the measured wood thickness after the Home Depot stock is chosen. Use 6mm to 12mm as a
   placeholder if stock is not measured yet.

Add only the support needed to hold the conveyor low and square:

1. New Component.
2. Name it `conveyor_mount_feet`.
3. Sketch short gusseted feet, angle brackets, or standoffs that touch the lower outside edges of the
   conveyor side plates. Start with four small supports, one near each conveyor corner.
   Use short slot holes for setup adjustment. Keep slot travel short so final lock-down remains rigid.
4. In the assembly skeleton sketch, define the base top plane and a provisional conveyor-side-plate
   bottom plane about 12mm above it. Place the feet or standoffs between those planes.
5. Add screw access from the outside where possible so the conveyor can be removed and re-aligned if needed.
6. Add the grounded outside motor board and its lower base brackets after the conveyor core is
   square. Do not let the motor board act as an unbraced cantilever.

The mounting feet are not a second frame wall. They locate and fasten the conveyor side plates to the base.
Do not add full-length duplicate side panels unless real dry-fit testing proves the side plate flexes
enough to affect belt tracking, motor alignment, or sensor alignment.
Default permanent method: once pulley alignment and chute handoff are correct, lock fixed feet in
place at the chosen slot position. If first fit is slightly off, add a thin shim or washer as a
correction and re-tighten.

Do not use tall 180mm posts unless a later chute and bin layout proves the conveyor must be high.

## Component 10: Exit Lip

The exit lip is a first handoff test coupon at the drive end. It should guide the brick off the belt
and into the chute entry below, but do not treat its first CAD geometry as final. Conveyor-to-chute
handoff is one of the highest-risk parts of the machine and must be tested with real bricks.

Geometry:
- Spans between the inner faces of the side plates (50mm wide)
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
6. Extrude 50mm in Y (spanning the inner gap between the side plates).
7. Optional: select the top upstream edge (where the angled face meets the flat top) and apply a
   3mm fillet to soften the corner. This is the "curved" version. Skip the fillet if it causes
   trouble in Fusion.

In assembly, place the lip near the drive end with the ramp face pointing toward the incoming brick.
Align the lip bottom to the belt surface for the first test so the flat top sits about 5mm to 6mm
above the belt. The reference design puts the lip near X=355 to X=370. Keep it clear of the drive
roller and belt-wrap area.

Mount holes: if the lip lands on a fitted crossmember or printed bracket, add two 3.3mm holes through
the bottom face for M3 bolts. If it lands on wood or the drive-end spacer is omitted, mark the holes
from the fitted lip position after the pulley and belt-wrap clearance is known. If you have time,
make those holes short slots in X so the lip can move a few mm during testing.

The chute entry funnel for the servo rotary chute selector sits directly below this lip. Position
the chute entry when modeling the chute selector subassembly. With the 370mm side plate, the
reference design places the lip near X=355 to X=370.

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
7. Taper to a 30mm inner gap at X=120, centered on Y=25.
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
   Position it centered on Y=25 (the belt centerline): from Y=6 to Y=44, Z=0 to Z=36.
5. Finish Sketch.
6. Extrude 65mm in X.
7. Create a second sketch on the same YZ face.
8. Draw the inner cutout: 30mm wide in Y, 32mm tall in Z.
   From Y=10 to Y=40, Z=0 to Z=32. (Bottom is open - do not draw the bottom line.)
   Draw three sides only: left, top, right. Leave the bottom open.
9. Finish Sketch.
10. Extrude Cut 65mm in X through the body. This hollows out the tunnel.

Add break beam holes in the side walls:

11. Click the left side wall outer face (the Y=6 face).
12. Create Sketch.
13. Add two 3.3mm circles:
    - X=15, Z=3 from the bottom of the shroud body (this puts the beam at 3mm above belt surface)
    - X=55, Z=3
    (These positions are relative to the shroud body: X=15 corresponds to 15mm into the 65mm length,
    X=55 is 55mm in. In assembly coordinates that will be about X=150 and X=190.)
14. Finish Sketch.
15. Extrude Cut through the left wall (4mm).
16. Repeat on the right side wall outer face (Y=44) with the same two hole positions.
    These holes must be exactly opposite the left wall holes so the beams cross straight.

Add color sensor mount in the roof:

The GY-31 PCB is 34mm x 34mm x 1.5mm. The LED assembly hangs 15.5mm below the PCB bottom face.
The PCB cannot be recessed into the 4mm roof and reach the inner ceiling: the PCB must sit on top
of the roof with the LED assembly passing through a clearance hole. The sensor face lands about
7mm above brick stud tops, which is within operating range. Calibration corrects for the distance.

17. Click the top face of the shroud (the Z=36 face).
18. Create Sketch.
19. Draw a 35mm x 35mm rectangle centered at X=32.5, Y=25 (center of the shroud in both axes).
    This is the PCB footprint with 0.5mm clearance per side.
20. Inside that rectangle, draw a 26mm x 26mm rectangle centered at X=32.5, Y=25.
    This is the LED assembly clearance hole. Verify against the real PCB before printing.
21. Finish Sketch.
22. Extrude Cut the inner 26mm x 26mm rectangle Through All (punches through the full 4mm roof).
    Do not extrude the outer 35mm x 35mm rectangle - it is reference geometry only.
23. Add four 3.3mm holes for M3 bolts at the PCB corner mounting hole positions (2.75mm from
    each edge of the 35mm footprint). The PCB rests flat on the roof top surface and bolts down
    with M3 x 10mm bolts, nuts accessible through the front or back shroud opening.

Add entry chamfer:

22. Select the four inner edges of the front opening (the X=0 face of this component, which will be
    at X=135 in assembly).
23. Apply a 3mm chamfer.

In assembly, align the shroud bottom opening with the belt surface and place the front opening after
the feed rails. The reference design puts the front face at X=135.

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

Assembly translation:
- If the guide says center between side plates, use the two inner side-plate faces as the references.
- If the guide says align to a shaft, bearing, or roller, select the cylindrical axis in Fusion.
- If the guide says belt surface, use the top of the roller belt-contact surface or a simple belt
  envelope as the reference.
- If the guide gives X, Y, or Z values, treat them as the original design math. Use them to check a
  measurement, not as the main way to place a component.

1. Ground or fix `side_plate_left` so its inner face is the reference face for the conveyor gap.
2. Right-click `side_plate_left` in the browser, Copy, then right-click the assembly and Paste New to
   create `side_plate_right`. (Paste New creates an independent copy, not a linked one.)
3. Align the two inner side-plate faces with a 50mm offset and point both plate thicknesses outward.
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
10. Fit crossmember spacers only after the roller, pulley, belt, wood block, and exit-lip keep-outs
    are visible. Start with one feed-end spacer, then add a drive-end spacer only if it clears the
    hub, pulley path, belt-wrap area, set screw access, exit lip, and future chute entry. Do not let a
    spacer stick outside the side plates or force the frame wider. Orient any spacer so its length
    runs between the inner faces of the side plates, with slip clearance if needed.
11. Add the base plate and short conveyor mounting feet or standoffs. In the assembly skeleton
    sketch, put the conveyor side-plate bottom plane about 12mm above the base as a first placement.
    The local belt surface at Z=60 will then be about 72mm above the base.
12. Create the 20T-to-20T motor construction point in the side-view skeleton sketch: drive point at
    X=340, Z=50, 80mm construction circle around that point, and horizontal construction line at
    Z=14. Use the lower-left intersection at about X=268.6, Z=14.
13. Add the outside motor board on a vertical plane about 25mm outboard from the right conveyor side
    plate outside face. Project the skeleton motor point into the board sketch before cutting slots.
    It should land at base-relative X=268.6, Z=26.
14. Add lower base brackets and upper standoffs or a brace so the motor board is grounded and cannot
    flex as a cantilever.
15. Import the NEMA17 STEP from `docs/datasheet/motion/nema17/cad/stepper_17hs4401s.step`.
16. Import the 20T pulley STEP from `docs/datasheet/motion/timing_pulley/cad/gt2_20t_5mm.step`.
17. Place the NEMA17 on the outside motor board with the motor body outside the board and the shaft
    pointing inward toward the conveyor.
18. Place matching 20T pulleys on the motor shaft and drive shaft, then align the pulley midplanes in
    Y. Confirm the motor can slide a few millimeters along the 26.8 degree slot direction.
19. Add a simple GT2 belt envelope and check clearance to the motor board, side plate, bearing
    holders, brace hardware, bolt heads, and pulley set screws.
20. Add a simple conveyor belt envelope and check top run and lower return clearance.
21. Add placeholder feed side rails near the feed end, centered around the belt path. The reference
    design runs them from X=0 to X=120 and centers them around Y=25. Keep them separate from the
    shroud.
22. Add a placeholder shroud volume downstream of the feed rails, centered over the belt path. The
    reference design places it from X=135 to X=200, with the 30mm inner width and 32mm inner height
    reserved. Do not model the detailed sensor pocket or break-beam holes until this clears the
    assembled conveyor.
23. Place the exit lip near the drive end, centered between the side plates, with its ramp face toward
    the incoming brick. The lip bottom should start at the belt surface and the flat top should sit
    about 5mm to 6mm above the belt surface for the first handoff test. The reference design places it
    near X=355 to X=370.
24. After the conveyor core and placeholder shroud clear, replace the placeholder with the removable
    sensing shroud. Align its bottom opening with the belt surface and its front face with the
    placeholder shroud location. Confirm the shroud body clears the belt and sits between the side
    plates without touching them.
25. Add screw tabs or flanges for removable shroud mounting only where the Fusion assembly shows a
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
- Rollers are centered in the 50mm inner gap
- Wood block support top surface is about 2mm below the upper belt surface and does not touch either
  roller flange, belt wrap, pulley, or fastener
- Wood block support bottom face and any screws clear the lower belt return path
- Crossmembers do not force the frame wider, protrude outside the side plates, block pulley hardware,
  or limit set screw and exit-lip access
- Bearing pockets face the side plates and bearings are captured after holders are bolted down
- Conveyor mounting feet are accessible after assembly and can be loosened and corrected if first fit
  needs a minor shim or washer
- Motor board is bolted to base brackets and braced back toward the conveyor
- Motor shaft point is projected from the side-view skeleton sketch, not hand-placed in 3D space
- Motor shaft points inward from the outside board toward the conveyor
- Matching 20T pulleys are coplanar in Y and the GT2 belt envelope clears the motor board, side
  plate, bearing holders, brace hardware, bolt heads, and pulley set screws
- Motor wire exits into the rear/right service bay without crossing belt, chute, bin, or operator-facing paths
- Motor stays inside the 610mm x 610mm footprint
- Set screw can be reached with the belt installed or by rotating the roller to an access position
- Sensing shroud bottom opening aligns with the belt surface and the shroud body does not touch either
  side plate
- Sensing shroud is removable with screws and can be adjusted or reprinted without rebuilding the
  conveyor core
- Sensing shroud inner width is 30mm and clears the 25mm belt during belt tracking tests
- Shroud break beam holes are opposite each other across the left and right walls
- Shroud color sensor pocket is in the roof, sensor face near inner roof surface at conveyor-local Z=92
- Shroud exit leaves clear belt run to the drive end
- Feed side rails are separate for the first build, end before the shroud, and do not pinch the brick
  at the shroud entry
- Exit lip ramp face points toward the incoming brick and the first-test flat top sits about 5mm to
  6mm above the belt surface
- Exit lip does not overlap the drive roller or belt-wrap area
- Space below the exit lip and drive end is clear for the chute entry funnel
- Belt exit, exit lip, chute entry, chute exit, and bin catch height are checked together before
  cutting final frame panels

## Build Order At The Bench

1. Print the bearing pocket test coupon.
2. Test one MR85ZZ bearing in the coupon.
3. Adjust the pocket diameter if needed.
4. Print two fixed bearing holders and two slotted bearing holders.
5. Print the idler roller.
6. Cut one side plate to 370mm x 80mm.
7. Print the DXF drilling template at 100% scale. Tape it to the first side plate and drill it.
8. Clamp the first plate to the second blank and drill through both.
9. Make the idler shaft and idler bolt slots in both plates.
10. Dry-fit the idler roller, shaft, bearings, and holders.
11. Print the drive roller. Cut the outside motor board from the fitted CAD template or printed
    drilling template, including the 22mm center clearance hole, four NEMA17 slots, and four board
    through-bolt holes.
12. Dry-fit the full conveyor with side plates, holders, rollers, the wood block, any fitted
    crossmembers, base plate, and conveyor mounting feet.
13. Measure shaft lengths from the real dry-fit. Cut the shafts with a Dremel cutoff wheel.
14. Install the drive shaft, 20T driven pulley, drive roller, and set screw.
15. Bolt the outside motor board to lower base brackets and upper braces or standoffs.
16. Install the NEMA17 on the outside motor board with the shaft facing inward, then install the 20T
    motor pulley and GT2 belt.
17. Align pulley midplanes by eye and measurement.
18. Cut the wood block support to about 255mm to 260mm, smooth the top face, and install it just under
    the top belt run. Dry-fit before drilling final holes and trim the block shorter if either end is
    close to a roller flange, belt wrap, pulley, or fastener.
19. Wrap the neoprene strip with the idler near the middle of its slot travel.
20. Mark and cut the neoprene belt.
21. Splice with contact cement as a butt joint. Reinforce by gluing a 30mm strip of the same
    neoprene on the inside surface overlapping the joint by 15mm each side. Let cure fully before
    tensioning.
22. Tension the belt by sliding the idler holders.
23. Keep the conveyor belt and GT2 belt only tight enough to avoid slipping. Do not over-tension the
    small bearings and printed holders.
24. Run the conveyor alone before adding sensors or the chute.
25. Watch the belt splice over both rollers. If the splice bumps, thumps, or pulls the belt sideways,
    fix that before adding the sensing shroud.
26. Feed real bricks over the bare conveyor and confirm they ride straight enough for sensing.
27. Add separate temporary or printed feed side rails and confirm a brick enters the shroud zone
    without rubbing.
28. Add the removable sensing shroud and confirm belt clearance before calibration.

## Reference Dimensions

Use this only when a step asks for a number. Do not read this section top to bottom before building.

Conveyor:
- Side plates: 370mm x 80mm x 8mm, two pieces
- Side plate inner gap: 50mm
- Side plate outside width: 66mm total, from two 8mm plates plus 50mm gap
- Roller center distance: 310mm
- Active low-frame belt surface height: about 72mm from base as the first CAD placement, then adjust
  from chute entry and bin catch geometry
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
- 20T to 20T GT2 center distance with 200mm belt: about 80mm
- Motor shaft construction point: local X=268.6mm, Z=14mm for the 20T-to-20T path
- Motor board shaft point: base-relative X=268.6mm, with Z derived from the conveyor support height. At 12mm support height, Z=26mm. Recompute if support height changes.
- Motor board inner face: about 25mm outboard from the right side plate outside face
- Motor orientation: body outside the motor board, shaft pointing inward toward the conveyor
- Motor board through holes: 4.3mm for M4 preferred, or 3.3mm for M3 fallback
- NEMA17 slot length: 12mm
- NEMA17 slot width: 3.5mm
- NEMA17 slot direction: 26.8 degrees upward toward the drive shaft
- Base plate: start around 590mm x 590mm, adjust only after chute and bin layout

Optional Fusion parameters:
- `plate_length`: 370mm
- `plate_height`: 80mm
- `plate_thick`: 8mm
- `side_gap`: 50mm
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
- `roller_center_dist`: 310mm
- `shaft_z`: 50mm
- `idler_x`: 30mm
- `drive_x`: 340mm
- `base_plate_w`: 590mm
- `base_plate_d`: 590mm
- `side_panel_h`: 80mm
- `motor_x_20t`: 268.6mm
- `motor_z_20t`: 14mm
- `motor_board_inner_y`: about 83mm if the left inner side plate face is Y=0
- `gt2_center_dist`: 80mm

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
