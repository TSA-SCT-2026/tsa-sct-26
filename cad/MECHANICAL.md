# Mechanical Design

All dimensions are in `cad/DIMENSIONS.md`. This document covers design rationale, construction method, and assembly notes for the simplified states build.

## Strategy

The active mechanical plan is to steal proven conveyor geometry, modify only what matters, and spend custom CAD time on the servo chute, sensor mounts, bins, and frame.

The previous chamber, release-gate, and NEMA11 selector design is archived for later nationals work. It should not drive current states CAD unless the user explicitly reopens that path.

## Brick Handling

Feed mode is manual one-at-a-time.

The evaluator places one brick on the conveyor, studs up and long side along travel. This removes the highest-risk singulation problem from the states build. A simple feed chute can be added later only after the base sorter is reliable.

Why:
- The rules do not require automated feeding
- Manual feed is easier for an evaluator to understand
- It reduces jam risk
- It lets the team focus on sorting accuracy and documentation

## Conveyor

Use a downloaded NEMA17 mini conveyor assembly if it imports cleanly into Fusion 360.

Modify as needed:
- Widen belt path to roughly 40mm to 50mm
- Adjust usable length to roughly 300mm to 400mm
- Keep motor and tensioner serviceable
- Mount to the wood or printed frame without exceeding 610mm x 610mm
- Keep the belt exit aligned with the chute entry

If the downloaded model fails, use the existing custom roller work as fallback prior art. Do not spend the first CAD window re-inventing a conveyor if a proven assembly works.

The existing NEMA17, fan, timing belt, timing pulley, bearings, and neoprene stock remain useful where they fit the chosen conveyor path.

## Frame

The active frame is wood or 3D printed, based on what is actually on hand.

The frame needs to:
- Lift the belt high enough for the chute and bins
- Keep the servo pivot stable
- Keep sensing brackets from moving during operation
- Leave the bins removable
- Leave wiring visible enough to look deliberate and clean

Do not design around 2020 extrusion unless the user confirms a material change.

## Sensing Station

The sensing station mounts near the conveyor exit.

Order along travel:
1. Size sensing
2. Color sensing with shroud
3. Belt exit into servo chute

Size sensing is open. Keep CAD brackets adjustable until the sensor family is chosen. Candidate paths include break-beam timing and distance sensing.

Color sensing uses the TCS3200/GY-31 module. The shroud is required. Open-air color calibration is not valid for this build.

Shroud design goals:
- Block side light
- Clear the brick including studs
- Avoid catching bricks
- Mount rigidly enough that calibration does not drift
- Leave wiring exit away from the belt and servo sweep

## Servo Rotary Chute Selector

This is the main custom mechanical subsystem.

The MG995/MG996/MG996R-class heavy servo rotates an angled chute so its exit points at one of four bin guides.

Chute body:
- Rectangular channel
- Internal width around 30mm
- Internal height around 15mm
- Top open or partly open at the entry
- Smooth interior surface
- Pivot boss or adapter tied to the servo horn

Servo mount:
- Servo output shaft points upward
- Servo body sits in a pocket or cradle
- Mount plate ties into the frame
- Chute pivot axis stays vertical and aligned with the horn
- Wires leave away from the horn and chute sweep

Chute angle:
- Start CAD at 35 degrees from horizontal
- Print a short test chute and test 30, 35, 40, and 45 degrees with real bricks
- Increase angle if bricks hesitate or stick
- Avoid surface treatments until angle and geometry have been tested first

Bin alignment:
- Use four construction rays from the pivot
- Start with about 35 degrees between positions
- Verify chute exit overlaps each fixed bin guide
- Use a revolute joint or equivalent Fusion position check before printing

## Bins

Use four simple open-top bins under the chute arc.

Each bin should:
- Hold its expected brick count with margin
- Have a clear label on the front
- Be removable without bumping the chute
- Accept the brick even if the chute exit has small alignment error

Add static entry guides if testing shows the chute exit needs a wider capture area.

## Operator-Facing Packaging

These features directly affect score because the evaluator operates alone:
- Feed orientation cue
- Labeled start button
- Display states: READY, SORTING, SORT COMPLETE, ERROR
- Bin labels
- Clean cable routing
- Stable frame and controlled footprint

The machine should look like a finished competition device, not a temporary bench test.

## CAD Build Order

1. Create or import top-level assembly and 610mm x 610mm boundary.
2. Import downloaded conveyor and NEMA17 reference.
3. Place conveyor on a provisional wood or printed frame.
4. Import the heavy servo reference from `docs/datasheet/motion/heavy_servo/`.
5. Model servo mount, chute body, pivot interface, and first-pass bin rays.
6. Model four bins and bin entry guides.
7. Model color shroud and provisional size sensor mounting space.
8. Check footprint, bin access, cable keepouts, and chute sweep.
9. Export only the print parts needed for first validation.

## First Physical Tests

Run these before committing to large prints:
1. Short chute angle test with real bricks at 30, 35, 40, and 45 degrees.
2. Conveyor-to-chute handoff test with a real brick.
3. Servo sweep test without a brick.
4. Servo sweep test with chute installed.
5. Bin alignment test for all four categories.
6. Color sensor shroud clearance test.

## Highest Fabrication Risks

1. Conveyor-to-chute handoff
2. Chute slide angle and surface friction
3. Servo horn or pivot slop
4. Size sensor geometry after the sensor choice is made
5. Frame stiffness around the servo and sensing station

Solve these with small prototypes and real-brick tests before making larger frame parts.
