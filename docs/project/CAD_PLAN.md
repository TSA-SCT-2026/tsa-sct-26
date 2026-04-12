# TSA LEGO Sorter CAD Execution Plan

Deadline: May 2, 2026
CAD window: April 11 to April 12, 2026, with spillover days if needed
Tool: Fusion 360
Approach: steal proven conveyor geometry, design the custom servo chute selector, assemble on a simple wood or printed frame

## Strategy

Do not CAD a conveyor belt from scratch first. Start from a proven NEMA17 mini conveyor assembly if the STEP import is usable, then resize or simplify arund it.

The custom CAD work is:
1. Wood or 3D printed elevated frame
2. MG995/MG996-class servo rotary chute selector
3. Sensor mounting brackets and TCS3200 color sensor shroud
4. Four bins and bin entry guides
5. Manual feed guide, only as a simple operator aid

Manual one-at-a-time feed is the active states build. A hopper or automatic feed chute is optional only after the base sorter works.

## Priority Asset Checklist

Download or prepare these before modeling:

| Component | Source | Format | Notes |
|-----------|--------|--------|-------|
| Mini conveyor belt assembly, NEMA17 | GrabCAD or equivalent proven public model | STEP preferred | Start here. Target 40mm to 50mm belt width and 300mm to 400mm usable length. |
| Backup conveyor reference | Thingiverse stepper conveyor or any model with clear dimensions | STL or reference dimensions | Use only if the STEP conveyor import is unusable. |
| NEMA17 stepper motor | Already in `docs/datasheet/motion/nema17/` | STEP | Active conveyor motor reference. |
| GT2 20T timing pulley | Already in `docs/datasheet/motion/timing_pulley/` | STEP | Use only if the chosen conveyor needs the GT2 timing path. |
| GT2 belt reference | Already in `docs/datasheet/motion/timing_belt/` | STEP | Use only if the chosen conveyor needs the GT2 timing path. |
| TCS3200/GY-31 color sensor | Already in `docs/datasheet/sensing/color_sensor/` | STEP or STL | Active color sensor. Do not switch to TCS34725 unless BOM truth changes. |
| MG995/MG996-class servo | `docs/datasheet/motion/heavy_servo/` plus a downloaded STEP if needed | STEP preferred | Folder currently has specs and images. Download a usable MG996R STEP or model a dimensioned block from the datasheet. |
| Size sensor candidate | `docs/datasheet/sensing/break_beam/` or `docs/datasheet/sensing/distance_sensor/` | STEP or simple block | Size sensing is undecided. Leave this bracket adjustable until the decision is made. |
| 608ZZ bearings | `docs/datasheet/fasteners/bearing/` and BOM inventory | STEP if needed | Use only if the chosen conveyor uses 608 bearings. |

Do not create new ordered-item claims in the BOM from this checklist. `docs/project/BOM.xlsx` remains purchase truth.

## Assembly Architecture

The machine has four vertical zones:

```text
Zone 1: manual feed area and optional guide
Zone 2: conveyor belt plus sensing station
Zone 3: servo rotary chute selector at belt exit
Zone 4: four bins arranged below chute sweep
```

The frame lifts Zones 1 through 3 high enough that Zone 4 fits underneath with clearance for bin removal.

## Critical Dimensions To Lock Early

- Footprint: 610mm x 610mm maximum
- Belt usable length: about 300mm to 400mm
- Belt width: about 40mm to 50mm
- Belt surface height from base: about 200mm to 300mm
- Chute length: about 100mm to 150mm
- Chute angle: test 30, 35, 40, and 45 degrees with real bricks
- First CAD chute angle: 35 degrees from horizontal
- Chute sweep: about 105 degrees total for four bins
- First servo positions: about 37, 72, 107, and 142 degrees
- Bin internal dimensions: about 80mm x 80mm x 60mm

Keep these numbers in `cad/DIMENSIONS.md` when they change.

## Subassembly 1: Frame

What it is: an elevated table or bridge that holds the conveyor and chute above the bins.

Active material options:
- Wood frame or base plate with printed brackets
- Fully 3D printed brackets and rails if fast enough to print
- Hybrid wood base plus printed servo and sensor mounts

Do not design around 2020 extrusion unless the material situation changes.

CAD steps:
1. New component: `Frame`
2. Sketch a 610mm x 610mm construction boundary on the ground plane
3. Place a simple base plate or four leg locations
4. Place conveyor support rails or side panels
5. Add a stiff servo mount area near the belt exit
6. Keep the front open enough for bin removal
7. Add wire routing paths that stay clear of the belt, chute, and servo horn

Target time: 45 minutes to 1 hour.

## Subassembly 2: Conveyor

What it is: belt, rollers, motor, side plates, and tensioning.

Preferred path:
1. Import the downloaded NEMA17 conveyor STEP assembly
2. Widen belt path to about 40mm to 50mm
3. Adjust length to fit the frame and leave room for the chute
4. Confirm motor clearance and service access
5. Add a simple placeholder belt surface if the model does not include an editable belt

If the downloaded conveyor fails:
1. Model two simple rollers
2. Add side plates with slotted idler adjustment
3. Add NEMA17 motor mount
4. Use GT2 timing pulley and belt only if that makes the conveyor easier to build
5. Keep the design simple enough to print and test quickly

What not to model:
- Do not model a flexible belt in detail
- Do not spend hours on tooth geometry
- Do not build a perfect custom conveyor if the downloaded assembly is workable

Target time: 1.5 to 2 hours with a downloaded base, 3 to 4 hours from scratch.

## Subassembly 3: Sensing Station

What it is: a single printed tunnel shroud near the feed end that integrates break beam size sensing
and overhead color sensing in one controlled-lighting enclosure.

Layout:

```text
[feed end X=0]
  -> side rail guide (X=0 to X=120, centers brick)
  -> sensing shroud (X=135 to X=200)
       break beam pair A at X=150
       break beam pair B at X=190
       TCS3200 in shroud roof
  -> open belt run
  -> exit lip (X=395 to X=410)
```

Sensing shroud:
- Single printed tunnel, 65mm long, 34mm wide, 36mm tall above belt surface
- Inner tunnel 26mm wide, 32mm tall; front and back open for brick passage; bottom open for belt
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

Side rail guide:
- Separate component, tapers from 38mm to 26mm over 120mm starting at X=0
- Centers brick before shroud entry

Exit lip:
- Wedge ramp at X=395-410, 5-6mm above belt surface, ramp face toward incoming brick
- Redirects brick downward into chute entry below
- No exit sensor in base design; firmware uses transit time

CAD work:
- Side rail guide: 20 minutes
- Sensing shroud: 40 to 50 minutes (most complex single piece on the conveyor)
- Exit lip: 15 minutes
- Total: about 1.5 hours after base conveyor is positioned

Target time: 1.5 hours.

## Subassembly 4: Servo Rotary Chute Selector

What it is: an angled chute attached to the heavy servo. The servo rotates the chute to point at one of four bins.

Geometry:

```text
        belt exit
           |
           v
      chute entry and pivot
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
- First CAD angle table: 37, 72, 107, and 142 degrees
- Adjust after bin guides are placed
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
- Add low side rails only if brick wandering hurts sensing repeatability
- Do not add a hopper until the base sorter works

Label:
- studs up
- long side along travel

Target time: 30 minutes.

## Weekend Schedule

### Friday Night

- Create or open the Fusion 360 project
- Download conveyor STEP options
- Download MG996R STEP if the local servo folder is not directly importable
- Import NEMA17, servo, color sensor, and first conveyor option
- Create top-level assembly
- Sketch 610mm x 610mm boundary

### Saturday Morning

Goal: frame and conveyor in place.

- Place conveyor inside the boundary
- Resize conveyor width and length if needed
- Model simple wood or printed frame
- Verify motor clearance
- Verify bin and chute space
- Screenshot for inventor's log

### Saturday Afternoon

Goal: sensing station and servo mount started.

- Reserve size sensor area without freezing the choice
- Model TCS3200 color shroud
- Verify brick clearance through shroud
- Place servo below belt exit
- Model servo mount plate or cradle

### Saturday Evening

Goal: chute selector geometry complete enough to test.

- Model chute body
- Model pivot boss or horn adapter
- Create four angular positions
- Rotate chute to each position and check alignment
- Model first-pass bin entry guides

### Sunday Morning

Goal: bins, feed guide, and full assembly.

- Model one bin and copy to four positions
- Add bin labels
- Add manual feed cue
- Check frame stiffness around servo
- Check bin removal clearance
- Check chute sweep clearance

### Sunday Afternoon

Goal: cleanup and export first validation parts.

- Add missing mounting holes
- Run Fusion interference check
- Export first validation STLs
- Capture isometric, top, side, and chute alignment screenshots
- Update `docs/project/OPEN_DECISIONS.md` if size sensing or frame decisions change

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
| Size sensor | Open decision |
| Frame material | Wood, 3D printed, or hybrid from available stock |
| Color sensor | TCS3200/GY-31 from BOM |
| Conveyor source | Downloaded NEMA17 conveyor preferred |
| BOM change | None until `docs/project/BOM.xlsx` gets real rows |

## Risk Mitigation

Downloaded conveyor import fails:
- Use a second public model as reference
- Fall back to a simple roller and side-plate conveyor
- Do not revive the archived full trough unless it is clearly faster than a simple path

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

Size sensor uncertainty:
- Keep brackets adjustable
- Run a simple proof test before final shroud and bracket exports

## What To Preserve For Later Nationals

The archived chamber and release-gate design is still valuable for a later higher-complexity build. For states, do not let it delay the simple conveyor plus servo chute sorter.
