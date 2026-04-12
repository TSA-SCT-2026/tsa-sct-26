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
- Belt-mounted sensing station near the conveyor exit
- Size sensing still undecided
- TCS3200/GY-31 color sensor with a light-blocking shroud
- MG995/MG996/MG996R-class heavy servo rotary chute selector
- Four bins arranged under the chute sweep
- Wood or 3D printed frame from available materials
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
- Do not claim a size sensor method is frozen until it is selected and tested
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

Manual feed orientation target:
- studs up
- long side along the conveyor travel direction

The orientation cue should be printed or labeled near the feed area so the evaluator does not have to infer it.

## Conveyor

The conveyor should start from a proven downloaded CAD assembly if possible, then be adapted to fit the sorter.

Baseline intent:
- Usable belt length around 300mm to 400mm
- Belt width around 40mm to 50mm
- Belt surface height around 200mm to 300mm from base, adjusted to fit chute angle and bin removal
- NEMA17 motor retained
- Belt path and motor clearance verified inside the 610mm x 610mm footprint

The prior custom roller and timing-stage work is archived and may be used as fallback reference. It is not the default first CAD path if a proven conveyor assembly imports cleanly.

## Sensing Station

All sensing is integrated into one printed shroud tunnel near the feed end of the belt.

Layout along belt travel:

```text
[feed end X=0]
  -> side rail guide (X=0 to X=120, centers brick on belt)
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
- Brick is centered by the side rail guide before it enters the shroud
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
- chute length around 100mm to 150mm
- chute angle around 35 degrees from horizontal as the first test point
- four servo positions spread across roughly 105 degrees total sweep
- target position examples around 37, 72, 107, and 142 degrees

Before final printing:
- Test a short chute with real bricks at 30, 35, 40, and 45 degrees
- Verify the chute exit overlaps each bin guide at all four positions
- Verify the servo mount keeps the output shaft aligned with the chute pivot
- Keep wiring clear of the servo horn and chute sweep

If bricks do not slide reliably, increase angle or improve the chute surface before changing the whole architecture.

## Frame And Bins

Frame:
- Use wood or 3D printed structure from available materials
- Keep the build stiff enough that sensor and chute alignment do not drift
- Keep bin access clear from the front
- Keep cable routing clean and visible as engineered work

Bins:
- Four open-top bins arranged under the chute arc
- Internal target around 80mm x 80mm x 60mm
- Label each bin with its category
- Reserve clearance so bins can be removed without hitting the chute

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
- How the downloaded conveyor geometry was selected or rejected
- Why the final size sensor was chosen
- Chute angle test results
- Servo position and bin alignment evidence
- Color calibration data with the shroud installed
- Full 24-brick run logs

Run logs belong in `docs/runs/`.
Notebook planning belongs in `docs/notebook/`.
Open decisions belong in `docs/project/OPEN_DECISIONS.md`.

## Reopen Rules

Only reopen the archived chamber and release-gate architecture if the user explicitly chooses to target nationals work again.

For states, reopen only:
- size sensor family
- exact conveyor CAD source
- exact chute angle and servo positions
- frame construction details
- whether a simple feed chute is worth adding after the manual-feed sorter works
