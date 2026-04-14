# TSA LEGO Sorter CAD Execution Plan

Deadline: May 2, 2026
CAD target: finish the complete working states CAD as quickly as practical, with no fixed session count
Tool: Fusion 360
Approach: build a tutorial-style conveyor with modified measurements, design the custom servo chute selector, assemble on a simple wood frame

## Strategy

Do not spend the CAD window adapting an unknown downloaded conveyor first. Build the conveyor in the same simple structural style as the visual tutorial, but keep the team's modified belt width, belt length, 5mm shaft and bearing path, and sorter clearances. The main material change from the earlier guide is replacing the 2020 extrusion support with a wood block.

Numeric values in this plan are sketch anchors, offsets, and derived checks, not final truth. Use
them to move quickly in part sketches or a visible assembly skeleton sketch, then use assembly
relationships for fit: face alignment, shaft-axis alignment, center-plane alignment, belt keep-out
envelopes, interference checks, slotted mounts, real hardware, and real-brick tests decide final
placement. Do not use typed Move coordinates as the source of truth in Fusion.

The custom CAD work is:
1. Low wood base plate, short conveyor mounting cleats or standoffs, and wood block conveyor support
2. MG995/MG996-class servo rotary chute selector
3. Sensor mounting brackets and TCS3200 color sensor shroud
4. Four bins and bin entry guides
5. Manual feed guide, only as a simple operator aid

Manual one-at-a-time feed is the active states build. A hopper or automatic feed chute is optional only after the base sorter works.

## Priority Asset Checklist

Download or prepare these before modeling:

| Component | Source | Format | Notes |
|-----------|--------|--------|-------|
| Visual conveyor tutorial | User-provided transcript or video | Video or transcript | Structural build reference only. Match the construction style while changing belt width, belt length, shaft size, bearing size, and motor path for this sorter. |
| Simple tutorial-style conveyor CAD | Build from `docs/project/CONVEYOR_BUILD_GUIDE.md` | Fusion design | Primary conveyor path for states. Use the current 25mm neoprene strip and 300mm to 400mm usable top-run length. |
| Downloaded conveyor assembly | GrabCAD or equivalent public model | STEP if useful | Optional visual or dimensional reference only. Do not let import cleanup slow the tutorial-style build. |
| NEMA17 stepper motor | Already in `docs/datasheet/motion/nema17/` | STEP | Active conveyor motor reference. |
| GT2 20T timing pulley | Already in `docs/datasheet/motion/timing_pulley/` | STEP | Use only if the chosen conveyor needs the GT2 timing path. |
| GT2 belt reference | Already in `docs/datasheet/motion/timing_belt/` | STEP | Use only if the chosen conveyor needs the GT2 timing path. |
| TCS3200/GY-31 color sensor | Already in `docs/datasheet/sensing/color_sensor/` | STEP or STL | Active color sensor. Do not switch to TCS34725 unless BOM truth changes. |
| MG995/MG996-class servo | `docs/datasheet/motion/heavy_servo/` plus a downloaded STEP if needed | STEP preferred | Folder currently has specs and images. Download a usable MG996R STEP or model a dimensioned block from the datasheet. |
| Break-beam size sensors | `docs/datasheet/sensing/break_beam/` | STEP or simple block | Frozen states size path: two pairs in the sensing shroud. |
| MR85ZZ 5mm-ID bearings | BOM inventory | Measured part or simple block | Active bearing family for the current 5mm shaft conveyor unless the user changes hardware. |
| 608ZZ bearings | `docs/datasheet/fasteners/bearing/` and BOM inventory | STEP if needed | Spare optional inventory. Do not switch to the tutorial's larger bearing path unless the user changes hardware. |

Do not create new ordered-item claims in the BOM from this checklist. `docs/project/BOM.xlsx` remains purchase truth.

## Assembly Architecture

The machine has four vertical zones:

```text
Zone 1: manual feed area and optional guide
Zone 2: conveyor belt plus sensing station
Zone 3: servo rotary chute selector at belt exit
Zone 4: four bins arranged below chute sweep
```

The frame places Zones 1 through 3 at the height that makes the conveyor-to-chute handoff work. The
current states path is a low frame with the chute and bins arranged around the belt exit, not a tall
raised conveyor unless a later chute and bin layout proves that height is needed.

## Critical Dimensions To Lock Early

- Footprint: 610mm x 610mm maximum
- Belt usable top-run length: about 300mm to 400mm
- Belt width: current 25mm neoprene strip
- Belt surface height from base: start around 68mm for the low-frame CAD path, then adjust from chute entry and bin catch geometry
- Chute length: about 100mm to 150mm
- Chute angle: test 30, 35, 40, and 45 degrees with real bricks
- First CAD chute angle: 35 degrees from horizontal
- Chute sweep: about 105 degrees total for four bins
- First servo positions in bin label order: about 37, 72, 107, and 142 degrees
- Bin internal dimensions: about 80mm x 80mm x 60mm

Keep these numbers in `cad/DIMENSIONS.md` when they change.

## Subassembly 1: Frame

What it is: a low base plate that holds the conveyor, motor, servo chute, and bins in one stiff
machine reference without adding duplicate full-length side panels unless the dry-fit proves they
solve a real stiffness problem.

Active material options:
- Wood frame or base plate with printed brackets
- Short wood cleats, standoffs, or angle brackets that fasten the existing conveyor side plates to the base
- Local right-side motor doubler or printed adjustment plate only if the conveyor side plate needs more thickness around the NEMA17 mount
- Wood block support under the top belt run
- 3D printed brackets and rails only where they save time or improve fit
- Hybrid wood base plus printed servo and sensor mounts

Do not design around 2020 extrusion unless the material situation changes.
Do not add full-length duplicate side panels just because the conveyor is low. If the added panel is
the same thin stock as the conveyor side plate and does not improve stiffness, skip it.

CAD steps:
1. New component: `Frame`
2. Sketch a 610mm x 610mm construction boundary on the ground plane
3. Place a simple base plate
4. Add only the short cleats, standoffs, or brackets needed to locate the conveyor side plates
5. Add a stiff servo mount area near the belt exit
6. Keep the front open enough for bin removal
7. Add wire routing paths that stay clear of the belt, chute, and servo horn

Target time: 45 minutes to 1 hour.

## Subassembly 2: Conveyor

What it is: belt, rollers, motor, wood block support, thin side plates, and tensioning.

Preferred path:
1. Model the wood block support and two thin wood side plates with drive and idler shaft holes
2. Add fixed and slotted printed bearing holders
3. Model two printed rollers around the current 5mm shaft and matching bearing choice
4. Add the low NEMA17 mount after the core roller assembly and low frame exist
5. Set the wood block top face about 2mm below belt surface and keep the bottom clear of the lower belt return
6. Use the 200mm GT2 timing belt with the motor body low inside the machine envelope, shaft outward through the right conveyor side plate or local motor doubler, and pulleys accessible outside
7. Keep the design simple enough to cut, drill, print, and test quickly

What not to model:
- Do not model a flexible belt in detail
- Do not spend hours on tooth geometry
- Do not spend hours cleaning downloaded STEP files

Target time: 3 to 4 hours for the tutorial-style conveyor with modified measurements.

## Subassembly 3: Sensing Station

What it is: a single printed tunnel shroud near the feed end that integrates break beam size sensing
and overhead color sensing in one controlled-lighting enclosure. Mount it to the wood conveyor bed
or a rigid wood frame member, not to the belt or servo chute.

Layout:

```text
[feed end X=0]
  -> sensing shroud (X=135 to X=200)
       break beam pair A at X=150
       break beam pair B at X=190
       TCS3200 in shroud roof
  -> open belt run
  -> exit lip (X=395 to X=410)
```

Sensing shroud:
- Single printed tunnel, 65mm long, 38mm wide, 36mm tall above belt surface
- Inner tunnel 30mm wide, 32mm tall; front and back open for brick passage; bottom open for belt
- The 30mm width is for the 25mm belt, belt tracking, and brick yaw tolerance. The as-fed 2x3 brick is 15.8mm wide in Y and 23.7mm long in X.
- Break beam holes at Z=63 through left and right walls at X=150 and X=190
- Color sensor pocket in roof, sensor face flush with inner ceiling (~19mm above brick studs)
- 3mm chamfer on inner edges of front opening to guide brick entry
- This is the only location that needs controlled lighting; everywhere else is open belt

Size sensing:
- Firmware measures blocked duration at known belt speed
- 15.8mm (2x2) vs 23.7mm (2x3) brick length
- Two pairs give redundancy and a speed cross-check

Color sensing:
- TCS3200/GY-31 in shroud roof
- Only valid calibration is with this shroud installed in final assembly position

Exit lip:
- Wedge ramp at X=395-410, 5-6mm above belt surface, ramp face toward incoming brick
- Redirects brick downward into chute entry below
- No exit sensor in base design; firmware uses transit time

CAD work:
- Sensing shroud: 40 to 50 minutes (most complex single piece on the conveyor)
- Exit lip: 15 minutes
- Total: about 1 hour after base conveyor is positioned

Target time: 1 hour.

## Subassembly 4: Servo Rotary Chute Selector

What it is: an angled chute attached to the heavy servo. The servo rotates the chute to point at one of four bins.

Geometry:

```text
        belt exit
           |
           v
      chute catch and pivot
       /    |    \
    bin 1 bin 2 bin 3 bin 4
```

Chute body:
- Internal width about 30mm
- Internal height about 15mm
- Wall thickness about 3mm
- Length about 100mm to 150mm
- Top open or partly open near the entry
- Smooth path with no ledges for studs to catch
- Pivot boss or horn adapter at the servo axis

Servo mount:
- Servo output shaft points up
- Servo body sits in a pocket or cradle
- Mount locks to the frame, not just a thin flexing plate
- Wires exit away from horn and chute sweep
- Use actual MG996R dimensions: about 40mm x 19mm x 43mm, then verify against the real part

Servo positions:
- Start with four construction rays around 35 degrees apart
- First CAD angle table in bin label order: 37 degrees for 2x2 RED, 72 degrees for 2x2 BLUE, 107 degrees for 2x3 RED, and 142 degrees for 2x3 BLUE
- Adjust after bin guides are placed and checked for funnel overlap
- Verify each position with a revolute joint or equivalent Fusion motion check

Critical test:
- Print a short chute segment
- Test real bricks at 30, 35, 40, and 45 degrees
- Choose the lowest angle that slides reliably
- If bricks stick, increase angle before adding liners

Target time: 3 to 4 hours. This is the main custom design work.

## Subassembly 5: Bins

What it is: four open-top boxes arranged under the chute arc.

Geometry:
- Internal target about 80mm x 80mm x 60mm
- Wall thickness about 3mm
- Bin guides at the top if chute alignment needs a wider capture zone
- Labels on the front face

Labels:
- 2x2 RED
- 2x2 BLUE
- 2x3 RED
- 2x3 BLUE

CAD steps:
1. Model one bin
2. Copy to four positions under the chute arc
3. Add or reserve label flats
4. Add a static guide above each bin if needed
5. Check that each bin can be removed without hitting the chute

Target time: 30 to 45 minutes.

## Subassembly 6: Manual Feed Guide

What it is: a small guide that helps the evaluator place one brick consistently.

Keep it simple:
- Feed is one at a time
- Add a printed or labeled orientation cue
- Add low side rails from X=0 to X=120 to guide the brick into the sensing shroud
- Do not add a hopper until the base sorter works

Label:
- studs up
- long side along travel

Target time: 30 minutes.

## Execution Sequence

### Setup

- Create or open the Fusion 360 project
- Open the visual conveyor tutorial and `docs/project/CONVEYOR_BUILD_GUIDE.md`
- Download MG996R STEP if the local servo folder is not directly importable
- Import NEMA17, servo, and color sensor references
- Create top-level assembly
- Sketch 610mm x 610mm boundary

### Current Conveyor Assembly Checkpoint

Use this checkpoint if the conveyor parts are modeled but the sensing shroud is not.

Goal: assemble the conveyor core in Fusion before committing to detailed shroud CAD.

- Place the two side plates, bearing holders, shafts, rollers, wood block support, motor mount, motor, and pulley path using assembly relationships where practical
- Add a simple belt envelope instead of detailed belt geometry
- Confirm roller centers, side plate gap, motor clearance, lower belt return clearance, wood block end clearance, and set screw access
- Treat the wood block top as about 2mm below the upper belt surface, not as a global Z coordinate
- Start the wood block around 295mm to 300mm long with current rollers, then adjust from flange and belt-wrap clearance
- Add the low base plate and short conveyor mounting cleats before final motor placement. In an assembly skeleton sketch, start with the conveyor side-plate bottom plane about 8mm above the base top plane, making the belt surface about 68mm from the base. This is a supported shim height, not a floating gap
- Place the NEMA17 body low inside the machine envelope with its shaft outward through the right conveyor side plate or local motor doubler. Define a provisional motor-shaft construction point near conveyor-local X=331mm and Z=18mm to 20mm for the 20T to 60T, 200mm GT2 path, then adjust from actual pulley fit
- Confirm the motor body clears the lower belt return and the motor wire can exit rearward or downward cleanly
- Fit crossmembers only where they clear the pulley path, belt wrap, set screw access, exit lip, and future chute entry. Do not widen the frame for a crossmember. Use one feed-end spacer first, then add a drive-end spacer only if it fits cleanly.
- Add only a placeholder shroud box over the belt. Use the X=135 to X=200 reference span from `cad/DIMENSIONS.md`, with the 30mm inner width and 32mm inner height reserved
- Keep feed side rails as separate placeholder strips, not part of the shroud
- Add shroud mounting surfaces or tabs only after the conveyor core assembly shows where screws can actually land
- Do not model the final color sensor pocket or break-beam holes until the shroud location is confirmed over the assembled conveyor
- Model holes when they belong to printed parts or drilling templates. Mark and drill holes from the fitted real assembly when placement depends on wood stock, pulley alignment, or adjustable brackets.

Pass criteria:

- Belt path is believable and clears the wood support, rollers, screw heads, and side plates
- Wood block ends clear the roller flange envelopes and belt wrap by about 10mm or more
- Any crossmember used fits inside the side plates without forcing the frame wider or blocking drive hardware
- Drive roller hub and set screw access remain on the motor side
- Motor can still slide a few millimeters to tension the timing belt
- GT2 pulleys remain accessible outside the right conveyor side plate
- Belt exit, exit lip, chute entry, chute exit, and bin catch height have a plausible shared Z stack
- Shroud placeholder clears the belt and does not touch the side plates
- There is a plausible screw-on mounting path for a removable shroud

### Frame And Conveyor

Goal: frame and conveyor in place.

- Place conveyor inside the boundary
- Resize conveyor width and length if needed
- Model simple wood frame with printed brackets where useful
- Verify motor clearance
- Verify bin and chute space
- Screenshot for inventor's log

### Sensing And Servo Mount

Goal: sensing station and servo mount started.

- Model the removable screw-mounted sensing shroud after the conveyor core assembly passes its placeholder check
- Model two break-beam pairs in the sensing shroud
- Model TCS3200 color shroud pocket
- Keep first-build feed rails separate until brick centering is proven
- Verify brick clearance through shroud
- Place servo below belt exit
- Model servo mount plate or cradle

### Chute Selector

Goal: chute selector geometry complete enough to test.

- Model chute body
- Model pivot boss or horn adapter
- Create four angular positions
- Rotate chute to each position and check alignment
- Model first-pass bin entry guides

### Bins And Feed

Goal: bins, feed guide, and full assembly.

- Model one bin and copy to four positions
- Add bin labels
- Add manual feed cue
- Check frame stiffness around servo
- Check bin removal clearance
- Check chute sweep clearance

### Cleanup And Exports

Goal: cleanup and export first validation parts.

- Add missing mounting holes
- Run Fusion interference check
- Export first validation STLs
- Capture isometric, top, side, and chute alignment screenshots
- Update `docs/project/OPEN_DECISIONS.md` if frame or tuning decisions change

## First Validation Prints

Print only the pieces that retire the biggest risks:

1. Short chute angle coupon
2. Servo mount and pivot coupon
3. Conveyor-to-chute handoff piece
4. Color shroud fit part
5. One bin guide and one bin

Do not print a large frame until chute angle, handoff, and servo alignment are proven with real bricks.

## Unknowns To Resolve

| Question | Current answer |
|----------|----------------|
| Feed mode | Manual one-at-a-time for states |
| Servo | MG995/MG996/MG996R-class heavy servo in `docs/datasheet/motion/heavy_servo/` |
| Size sensor | Break-beam timing with two pairs in the sensing shroud |
| Frame material | Wood frame with printed brackets where useful |
| Color sensor | TCS3200/GY-31 from BOM |
| Conveyor source | Tutorial-style conveyor with modified measurements |
| BOM change | None until `docs/project/BOM.xlsx` gets real rows |

## Risk Mitigation

Tutorial geometry does not fit current hardware:
- Keep the tutorial construction sequence, then change dimensions for the real belt, shaft, bearing, and pulley path
- Use downloaded conveyor files only as visual reference
- Do not revive the archived full trough unless it is clearly faster than the simple side-plate path

Chute angle too shallow:
- Test 30, 35, 40, and 45 degrees
- Increase angle before adding liners

Servo horn slop:
- Use a larger horn face if available
- Add a printed adapter plate
- Keep the chute light and balanced

Bins do not align:
- Use construction rays from the pivot
- Adjust bin guides before changing the servo mount
- Make guides slightly forgiving instead of demanding perfect drops

Footprint risk:
- Keep the 610mm x 610mm boundary visible
- Check after every major component placement

Break-beam size timing:
- Keep pair A and pair B holes easy to adjust after real-brick belt tests
- Run a simple proof test before final shroud export

## What To Preserve For Later Nationals

The archived chamber and release-gate design is still valuable for a later higher-complexity build. For states, do not let it delay the simple conveyor plus servo chute sorter.
