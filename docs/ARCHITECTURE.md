# SCT 2026: States Architecture

## Mission

Build a reliable LEGO brick sorter for TSA System Control Technology states on May 2, 2026.

Success criteria:
- Sort 24 bricks into 4 bins correctly
- Let a first-time evaluator run the system from written instructions
- Prioritize reliability and repeatability before speed
- Use simple mechanisms that can be designed, built, tested, and documented before competition

The previous chamber and release-gate architecture is preserved under `_archive/previous-chamber-release-design-2026-04/` for later nationals work. It is prior art, not the active states build.

## Active Production Architecture

The states build uses:
- Manual one-at-a-time brick feed onto a conveyor
- NEMA17-driven conveyor
- Wood-frame-mounted sensing station near the conveyor start
- Break-beam timing size sensing with two pairs in the sensing shroud
- TCS3200/GY-31 color sensor with a light-blocking shroud
- MG995/MG996/MG996R-class heavy servo rotary chute selector
- Four bins arranged under the chute sweep
- Wood frame with 3D printed brackets where useful
- Operator-facing labels, display states, and start control

The active build intentionally removes:
- 24-brick compressed queue
- Start gate
- Isolation chamber
- Solenoid release gate
- Retracting support
- NEMA11 selector
- Dual-ToF chamber sizing as a frozen architecture
- 2020 extrusion frame assumption

This trade keeps enough scoring strength to win states while reducing the chance that a complex mechanism fails before evaluation.

## Operating Flow

```text
[operator places one brick on conveyor]
    -> [brick passes size sensor]
    -> [brick passes shrouded color sensor]
    -> [classification locks]
    -> [servo chute points at target bin]
    -> [conveyor carries brick off belt into chute]
    -> [brick slides into labeled bin]
    -> [system returns to READY for the next brick]
```

If time remains after the manual-feed system is reliable, a simple feed chute can be added. It must not be allowed to delay the base sorter.

## Core Design Principle

Correct sorting beats mechanism impressiveness.

The states machine should be judged as a complete, understandable, repeatable system. A simple conveyor and servo chute that works every time is stronger than a sophisticated queue and release mechanism that is not fully validated.

Control decisions should still be event-aware:
- Do not move the chute after the brick has already committed to the belt exit
- Do not classify color without the shroud installed
- Validate break-beam timing with real bricks before tuning speed
- Do not optimize speed until a full 24-brick run is correct

## Brick Set

The system sorts:
- 6 red 2x2 bricks
- 6 blue 2x2 bricks
- 4 red 2x3 bricks
- 8 blue 2x3 bricks

Bin labels:
- 2x2 RED
- 2x2 BLUE
- 2x3 RED
- 2x3 BLUE

Bin order:
1. 2x2 RED
2. 2x2 BLUE
3. 2x3 RED
4. 2x3 BLUE

Manual feed orientation target:
- studs up
- long side along the conveyor travel direction

As-fed 2x3 brick envelope:
- X travel length: 23.7mm
- Y cross-belt width: 15.8mm

The orientation cue should be printed or labeled near the feed area so the evaluator does not have to infer it.

## Conveyor

The conveyor should follow the simple reference-video structure as closely as practical while keeping the team's modified measurements: wood block support instead of the previous 2020 extrusion support, thin side plates, printed bearing holders, shafts and rollers between the plates, and a NEMA17 motor on an adjustable side bracket.

Baseline intent:
- Usable top-run length around 300mm to 400mm
- Belt width around the current 25mm neoprene strip
- Belt surface height around 200mm to 300mm from base, adjusted to fit chute angle and bin removal
- NEMA17 motor retained
- Wood block support replacing the previous 2020 aluminum extrusion support idea
- Belt path and motor clearance verified inside the 610mm x 610mm footprint

The downloaded conveyor STEP path is no longer the default for states. It may still be used as visual or dimensional reference, but the active build path prioritizes the tutorial-style conveyor with modified measurements because it is easier to build, inspect, and debug in the remaining sprint.

## Sensing Station

All sensing is integrated into one printed shroud tunnel near the feed end of the belt, mounted to the wood conveyor bed or a rigid wood frame member. It is upstream of the belt exit so the firmware has time to classify the brick and move the servo chute before handoff.

Layout along belt travel:

```text
[feed end X=0]
  -> sensing shroud (X=135 to X=200, single printed tunnel)
       break beam pair A at X=150  (size timing, leading edge detection)
       break beam pair B at X=190  (size timing, trailing edge / belt speed cross-check)
       TCS3200 in shroud roof       (color sensing under controlled lighting)
  -> open belt run (X=200 to X=395)
  -> exit lip (X=395 to X=410, redirects brick downward into chute entry)
```

Size:
- Two break beam pairs through the side walls of the sensing shroud
- Firmware measures blocked duration at known belt speed to distinguish 15.8mm (2x2) from 23.7mm (2x3)
- Two pairs provide redundancy and a speed cross-check without assuming step rate accuracy
- Decision locked: break-beam timing with two pairs

Color:
- TCS3200/GY-31 module in the roof of the sensing shroud, looking straight down
- Shroud walls block ambient light on all four sides; the only valid calibration is with the shroud installed
- Sensor face is approximately 19mm from the top of the brick studs

Exit lip:
- Curved ramp wedge at the drive end, 5 to 6mm above belt surface
- Converts horizontal momentum to downward motion, guides brick into the chute entry below
- No exit sensor in the base design; firmware uses belt transit time as the handoff timer
- Add an exit sensor only if transit timing proves unreliable during testing

## Servo Rotary Chute Selector

The selector is the main custom mechanism for the states build.

It uses the heavy MG995/MG996/MG996R-class servo from `docs/datasheet/motion/heavy_servo/` to rotate an angled chute toward one of four bins.

Baseline geometry:
- chute internal width around 30mm
- chute internal height around 15mm
- chute arm as short as physically possible - shorter arm means less flex and less position error
- chute angle around 35 to 40 degrees from horizontal; exact value matters less with UHMW lining
- four servo positions spread across roughly 105 degrees total sweep
- target position examples around 37, 72, 107, and 142 degrees in bin order: 2x2 RED, 2x2 BLUE, 2x3 RED, 2x3 BLUE

Reliability approach - three layers that together remove the need for a perfect angle or precise positioning:
1. UHMW tape lining on the chute interior - reduces friction to the point where bricks slide at 30 degrees reliably. Apply from the first build, not as a fix after testing.
2. Wide bin entry funnels - each bin mouth is significantly wider than the brick with angled walls that guide the brick in. Servo only needs to be accurate to within about 15 degrees, not a narrow slot.
3. Short arm - keep the chute arm under 100mm if possible. Every extra mm of arm amplifies pivot flex and servo position error at the exit.

Before final printing:
- Test a short UHMW-lined chute with real bricks to confirm slide at chosen angle
- Verify chute entry height and chute exit height separately. Do not reuse the exit height as the entry height.
- Verify each servo position drops the brick into the funnel, not the bin itself
- Verify the servo mount keeps the output shaft aligned with the chute pivot
- Keep wiring clear of the servo horn and chute sweep

## Frame And Bins

Frame:
- Use wood frame structure with 3D printed brackets where useful
- Keep the build stiff enough that sensor and chute alignment do not drift
- Keep bin access clear from the front
- Keep cable routing clean and visible as engineered work

Bins:
- Four open-top bins arranged under the chute arc
- Internal target around 80mm x 80mm x 60mm
- Label each bin with its category
- Reserve clearance so bins can be removed without hitting the chute
- Each bin has a printed funnel entry at the verified catch height. Wide angled walls catch bricks even with several degrees of chute position error. The funnel is part of the bin print, not a separate part.

Footprint:
- Hard limit is 610mm x 610mm
- Keep the boundary visible in CAD while placing the frame, conveyor, chute, and bins

## Operator UX

The evaluator operates alone. The physical machine must explain itself.

Required:
- Labeled start button
- Display states: READY, SORTING, SORT COMPLETE, ERROR
- Feed orientation cue
- Bin labels
- Clean wire routing
- Written operating instructions

The system should make the sequence obvious: place brick, press or confirm start if required, let the sorter route it, repeat until all 24 bricks are sorted.

## Documentation And Evidence

Judged documentation should show:
- Why the team simplified from the archived architecture
- Why the tutorial-style conveyor path with modified measurements replaced downloaded conveyor geometry as the first states path
- Why the final size sensor was chosen
- Chute angle test results
- Servo position and bin alignment evidence
- Color calibration data with the shroud installed
- Full 24-brick run logs

Lower-priority firmware TODO after CAD and mechanical bringup:
- Replace sensing stubs with calibrated two-pair break-beam timing and shrouded TCS3200 classification.
- Align category-to-bin mapping with the bin order in this document.

Run logs belong in `docs/runs/`.
Notebook planning belongs in `docs/notebook/`.
Open decisions belong in `docs/project/OPEN_DECISIONS.md`.

## Reopen Rules

Only reopen the archived chamber and release-gate architecture if the user explicitly chooses to target nationals work again.

For states, reopen only:
- exact conveyor CAD source
- exact chute angle and servo positions
- frame construction details
- whether a simple feed chute is worth adding after the manual-feed sorter works
