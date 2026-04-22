# SCT 2026: States Architecture

## Mission

Build a reliable LEGO brick sorter for TSA System Control Technology states on May 2, 2026.

Success criteria:
- Sort 24 bricks into 4 bins correctly
- Let a first-time evaluator run the system from written instructions
- Prioritize reliability and repeatability before speed
- Use simple mechanisms that can be designed, built, tested, and documented before competition

The previous chamber and release-gate architecture is preserved under `_archive/previous-chamber-release-design-2026-04/` for later nationals work. It is prior art, not the active states build.

## Measurement Truth

This architecture document defines system intent, not final CAD geometry.

Exact dimensions belong in active docs only when they trace to a competition rule, BOM row, exact purchased listing, datasheet-backed standard part, real hardware measurement, or finalized CAD export. Draft CAD values must stay as variables, ranges, or test coefficients until physical fitting confirms them.

When a mechanism is still being fit on real hardware, prefer:
- design constraints
- source-backed part dimensions
- measured interfaces
- validation tests
- symbolic equations with variable tables

Avoid copying convenience coordinates or old CAD step values into new instructions.

## Active Production Architecture

The states build uses:
- Manual one-at-a-time brick feed onto a conveyor
- NEMA17-driven conveyor
- Wood-frame-mounted sensing station near the conveyor start
- Break-beam timing size sensing with two pairs in the sensing shroud
- TCS3200/GY-31 color sensor with a light-blocking shroud
- MG995/MG996/MG996R-class heavy servo rotary chute selector
- Four labeled receiving bins arranged by the tested chute sweep
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

Source-backed brick dimensions and derived sensing thresholds belong in `cad/DIMENSIONS.md` and `docs/notebook/EQUATIONS.md`.

The orientation cue should be printed or labeled near the feed area so the evaluator does not have to infer it.

## Conveyor

The conveyor should follow the simple reference-video structure where practical while using source-backed purchased parts and real dry-fit measurements.

Baseline intent:
- NEMA17 motor retained
- Tutorial-style conveyor logic retained
- Wood block support replacing the previous 2020 extrusion support idea
- Side plates, rollers, belt, and bearing holders sized from the actual build path
- Motor and belt service access kept visible
- Conveyor height derived from the chute and bin handoff, not from old CAD coordinates
- Belt path and motor clearance verified inside the 610mm x 610mm footprint

The downloaded conveyor STEP path is no longer the default for states. It may still be used as visual or dimensional reference, but the active build path prioritizes the tutorial-style conveyor with modified measurements because it is easier to build, inspect, and debug in the remaining sprint.

## Sensing Station

All sensing is integrated into one printed shroud tunnel near the feed end of the belt. It mounts to the wood conveyor bed or a rigid wood frame member. It is upstream of the belt exit so the firmware has time to classify the brick and move the servo chute before handoff.

Layout intent along belt travel:

```text
[feed and orientation cue]
  -> [low guide rails, if needed]
  -> [sensing shroud with two break-beam pairs and color sensor]
  -> [open belt run]
  -> [exit lip or handoff guide]
  -> [servo rotary chute selector]
```

Size sensing:
- Two break-beam pairs pass through the sensing shroud side walls
- Firmware measures blocked duration at known belt speed to distinguish 2x2 from 2x3
- Two pairs provide redundancy and a speed cross-check without assuming step rate accuracy
- Decision locked: break-beam timing with two pairs

Color sensing:
- TCS3200/GY-31 module in the roof of the sensing shroud, looking straight down
- Shroud walls block ambient light on all four sides
- The only valid calibration is with the shroud installed in its final position

Shroud and rail geometry must be re-derived from the real conveyor, real bricks, and real sensor modules. Do not use old CAD guide dimensions as final.

Exit handoff:
- The handoff guide or lip is a testable feature at the drive end
- Its height and placement derive from the final belt surface, chute entry, and bin catch window
- No exit sensor in the base design; firmware uses belt transit time as the handoff timer
- Add an exit sensor only if transit timing proves unreliable during testing

## Servo Rotary Chute Selector

The selector is the main custom mechanism for the states build.

It uses the heavy MG995/MG996/MG996R-class servo from `docs/datasheet/motion/heavy_servo/` to rotate an angled chute toward one of four bins.

Baseline constraints:
- Chute must clear the largest as-fed brick with yaw tolerance
- Chute arm should be as short and stiff as the bin layout allows
- Chute slope must be validated with a UHMW-lined real-brick slide test
- Servo positions are calibration outputs, not architecture truth
- Bin catch windows must be checked against the actual swept chute and brick envelope
- Conveyor-to-chute entry and chute-to-bin exit heights must be checked separately

Reliability approach:
1. UHMW tape lining on chute sliding surfaces.
2. Rounded high-wall chute end so overshoot stays contained without a separate static ramp.
3. Sweep-defined bin arc with enough divider clearance for brick width, yaw margin, and servo error.
4. Short, stiff chute arm to reduce flex and exit position error.

Before final printing:
- Test a short UHMW-lined chute with real bricks
- Verify chute entry height and chute exit height separately
- Verify each servo position drops the brick into the matching bin arc catch window
- Verify the servo mount keeps the output shaft aligned with the chute pivot
- Keep wiring clear of the servo horn and chute sweep

## Frame And Bins

Frame:
- Use wood frame structure with 3D printed brackets where useful
- Keep the build stiff enough that sensor and chute alignment do not drift
- Keep bin access clear from the front
- Keep cable routing clean and visible as engineered work

Bins:
- Four receiving bins arranged under the tested chute arc
- Each bin has a clear front label
- Each bin must hold its expected brick count with margin
- Each bin needs an arc-defined catch opening at the verified chute exit height
- Exact bin geometry is now part of the active CAD critical path and should be designed before locking chute, conveyor feet, or woodworking

Footprint:
- Hard limit is 610mm x 610mm
- Keep the boundary visible while placing the frame, conveyor, chute, and bins

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
- Why the tutorial-style conveyor path replaced imported conveyor geometry as the first states path
- Why the final size sensor was chosen
- Chute angle test results
- Servo position and bin alignment evidence
- Color calibration data with the shroud installed
- Full 24-brick run logs

Engineering notebook equations should be symbolic first and list suggested coefficients separately. Do not hard-code draft CAD numbers into derived formulas.

Lower-priority firmware TODO after mechanical bringup:
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
