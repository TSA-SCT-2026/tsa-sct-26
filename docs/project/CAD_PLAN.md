# TSA LEGO Sorter CAD - Remaining Work

Deadline: May 2, 2026
Tool: Fusion 360

The conveyor core, frame structure, motor mount, and pulley path are provisionally complete in Fusion.
Remaining items are listed below in priority order.

## Remaining Conveyor Items

These are blocking the physical wood assembly or the first print set.

1. **Screw holes between side plate and center board.** The center support now contacts both inner side-plate faces. Add through-holes in the side plates and matching hole geometry in the board ends. Size to match M3 or wood screw stock.

2. **Final pulley alignment and belt center distance.** Motor pulley is at the motor shaft tip. Driven pulley is against the inner bearing holder face with a 2mm spacer. Final center distance and Y coplanarity check are gated on confirmed conveyor height from the teammate's servo chute. Do not tighten set screws or export the motor board drilling template until height is confirmed.

3. **Exit lip.** Wedge ramp at the drive end, 5-6mm above belt surface, redirects brick downward into chute entry. Defer detailed geometry until conveyor height is confirmed, since the lip's Z position depends on the final belt surface height. Design intent: short printed or wood wedge, about 45 degrees from vertical, spanning the full inner gap in Y, no active sensor in the base design.

## Subassembly 3: Sensing Station

What it is: a single printed tunnel shroud near the feed end that integrates break-beam size sensing and overhead color sensing in one controlled-lighting enclosure. Mount to the wood conveyor bed or a rigid wood frame member, not to the belt or servo chute.

Design intent for another agent: if given the final conveyor measurements and the conveyor CAD, an agent should be able to design this. The required constraints are:

- Single printed tunnel. Front and back open for brick passage. Bottom open for belt. No internal ledges.
- Inner width must clear the 15.8mm as-fed brick width plus yaw and lateral positioning margin. Nominal inner width target is 30mm, giving about 7mm per side. The belt runs through the open bottom, not through the horizontal channel, so the 25mm belt width does not compete with the brick width in the Y direction.
- Inner height must clear brick body (11.4mm) plus studs (1.7mm) with margin. Nominal inner height target is 32mm above belt surface.
- Two break-beam pairs through the side walls. Pair A is 15mm from the shroud front face. Pair B is 55mm from the shroud front face. Hole diameter 3.3mm. Height: 3mm above belt surface.
- TCS3200/GY-31 color sensor pocket in the shroud roof. Sensor face flush with inner ceiling, approximately 19mm above brick stud tops.
- 3mm chamfer on inner edges of front opening to guide brick entry.
- Removable: screw tabs or mounting flanges on the shroud body that land on the wood conveyor bed or a rigid frame member. Final screw positions depend on the assembled wood frame.
- Wall thickness approximately 4mm. Length approximately 65mm in X.
- Entry from the rail guide: inner width should match the rail guide exit gap so bricks flow smoothly from rails into shroud.

Side rails are separate screw-on pieces. Do not merge into the shroud until brick centering is proven with real bricks.

Side rail design intent:
- Low printed or wood guides from the feed end to the shroud entry.
- Taper from a wider entry gap (approximately 38mm) down to the shroud inner width (approximately 30mm) over roughly 120mm of travel.
- Height about 6mm above belt surface, wall thickness about 4mm.
- First build: temporary or clamped, not bonded, so the gap can be adjusted.

CAD work:
- Build the shroud as a single printed part. Estimate 40-50 minutes.
- Build the side rails as separate simple pieces. Estimate 15-20 minutes.
- Both can be drafted this weekend as visual references even if not printed yet. Do not print until real brick clearance is verified on the physical conveyor.

## Subassembly 4: Servo Rotary Chute Selector

Teammate is completing this subassembly. Do not edit the servo chute or bin arc guide docs.

When the teammate's geometry is confirmed, the following items become unblocked:
- Conveyor height (determines cleat/foot height and motor board Z)
- Exit lip final position
- Final pulley center distance

## Subassembly 5: Bins

What it is: four open-top boxes arranged under the chute arc.

Geometry:
- Internal target about 80mm x 80mm x 60mm
- Wall thickness about 3mm
- Funnel entry at the top as part of the bin print, not a separate part
- Labels on the front face: 2x2 RED, 2x2 BLUE, 2x3 RED, 2x3 BLUE

CAD steps:
1. Model one bin with funnel entry
2. Copy to four positions under the chute arc (positions gated on teammate's sweep radius)
3. Add label flats
4. Check that each bin can be removed without hitting the chute

Target time: 30-45 minutes.

## Subassembly 6: Manual Feed Guide

What it is: a small guide that helps the evaluator place one brick consistently.

Keep it simple:
- Printed orientation cue near the feed end
- Low side rails as described in the sensing station section above
- Label: studs up, long side along travel

Target time: 15-20 minutes after side rails are modeled.

## Remaining Execution Steps

### Sensing And Servo Mount

- Model the removable sensing shroud after the conveyor core assembly passes its checks
- Add two break-beam pairs in the shroud side walls
- Add TCS3200 color sensor pocket in shroud roof
- Verify brick clearance through shroud with a real-brick envelope
- Place servo below belt exit (gated on teammate confirming geometry)
- Model or import servo mount after chute geometry is confirmed

### Chute Selector And Bins

- Gated on teammate's servo chute. Add bins and bin guides after chute sweep is confirmed.
- Verify chute exit falls inside each bin funnel at all four positions

### Cleanup And Exports

- Add missing mounting holes across all parts
- Run Fusion interference check
- Export first validation STLs in print priority order (see First Validation Prints below)
- Capture isometric, top, side, and chute alignment screenshots for the inventor's log

## First Validation Prints

Print only what retires the biggest risks. In priority order:

1. Short chute angle coupon (teammate's print)
2. Motor mount L bracket and feet - after conveyor height is confirmed
3. Sensing shroud - after brick clearance is verified in CAD
4. Side rails - after shroud position is confirmed
5. Conveyor-to-chute handoff test piece
6. One bin guide and one bin (after chute sweep is confirmed)
7. Exit lip - after conveyor height and handoff are proven

Do not print the large frame until chute angle, handoff, and servo alignment are proven with real bricks.

## Risk Notes

Conveyor height uncertainty:
- Do not finalize or print motor mount feet, motor board drill template, or exit lip until teammate confirms chute entry height requirement.

Sensing shroud geometry:
- Shroud inner width (30mm) needs to clear the 15.8mm brick plus yaw margin. The belt runs through the open bottom, not the side walls, so belt width is not additive here. 30mm gives about 7mm per side on the brick, which is adequate.
- Break-beam holes must be at the right Z height to cross the brick body, not the stud tops or belt surface.

Pulley alignment:
- Both 20T pulleys must be coplanar in Y before tightening. Misalignment causes belt walk and wears the belt edge. Verify in CAD and again on the physical assembly.
