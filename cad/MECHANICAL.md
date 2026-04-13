# Mechanical Design

All dimensions are in `cad/DIMENSIONS.md`. This document covers design rationale, construction method, and assembly notes for the simplified states build.

## Strategy

The active mechanical plan is to follow the simple conveyor tutorial where practical, modify only what matters, and spend custom CAD time on the servo chute, sensor mounts, bins, and frame.

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

Use the tutorial-style conveyor as the active states path, with the team's modified measurements and
5mm shaft and bearing path. The tutorial is a structural guide, not a hardware spec.

Build around:
- One wood block support replacing the earlier 2020 extrusion support
- Two thin wood side plates
- Printed bearing holders bolted to the outside faces
- Shafts and printed rollers between the plates
- NEMA17 motor on an adjustable side bracket
- 5mm shaft and matching 5mm-ID bearing path
- 25mm neoprene belt sized from the real dry-fit

Adjust as needed:
- Set belt path around the current 25mm neoprene strip
- Set usable top-run length to roughly 300mm to 400mm
- Keep motor and tensioner serviceable
- Mount to the wood frame without exceeding 610mm x 610mm
- Keep the belt exit aligned with the chute entry

Downloaded conveyor models may be used as visual reference only. Do not spend the first CAD window cleaning imported geometry when the tutorial-style conveyor can be modeled and built directly.

The existing NEMA17, fan, timing belt, timing pulley, bearings, and neoprene stock remain useful where they fit the chosen conveyor path.

## Frame

The active frame is wood, with printed brackets where they save time or improve fit. Wood stock will
be picked up locally, so exact board and block thicknesses stay provisional until purchase and
measurement.

The frame needs to:
- Lift the belt high enough for the chute and bins
- Keep the servo pivot stable
- Keep sensing brackets from moving during operation
- Leave the bins removable
- Leave wiring visible enough to look deliberate and clean

Do not design around 2020 extrusion unless the user confirms a material change. The conveyor support
block is wood, not 2020 extrusion.

## Sensing Station

The sensing station mounts near the conveyor exit.

Order along travel:
1. Size sensing
2. Color sensing with shroud
3. Belt exit into servo chute

Size sensing is frozen as break-beam timing with two pairs through the sensing shroud side walls. Keep hole positions easy to adjust after real-brick belt tests, but do not reopen ToF or distance sensing unless the user explicitly changes the size sensor decision.

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
- Keep the pivot-to-exit radius as short as the bin layout allows. Shorter means less flex and less exit position error, but wide funnels may require a larger radius or staggered bins.
- Top open or partly open at the entry
- UHMW tape applied to interior channel surface before first use - not optional, not deferred
- Pivot boss or adapter tied to the servo horn

Servo mount:
- Servo output shaft points upward
- Servo body sits in a pocket or cradle
- Mount plate ties into the frame
- Chute pivot axis stays vertical and aligned with the horn
- Wires leave away from the horn and chute sweep

Chute angle:
- Start CAD at 40 degrees from horizontal
- With UHMW lining, bricks slide at 30 degrees or less. 40 degrees is conservative and reliable.
- Print a short UHMW-lined test segment before committing to the full chute geometry
- The angle is not the sensitivity point. Arm flex and bin funnel width are the sensitivity points.

Bin alignment:
- Use four construction rays from the pivot
- Start with about 35 degrees between positions
- Each bin has a wide funnel entry built into the bin print at the verified catch height
- Funnel walls angle inward over 15 to 20mm, narrowing from a wide catch opening to the bin interior
- Target funnel catch width: 50mm or wider if the sweep radius can support it. Check adjacent funnel overlap before locking the radius.
- Verify chute exit falls inside each funnel at all four positions
- Use a revolute joint or equivalent Fusion position check before printing

## Bins

Use four simple open-top bins under the chute arc.

Each bin should:
- Hold its expected brick count with margin
- Have a clear label on the front
- Be removable without bumping the chute
- Have a printed funnel entry at the verified catch height. Top-entry is preferred, but a side-entry catch is acceptable if it is intentional and tested.

The funnel entry is not optional. It is what allows the servo to land a brick reliably without needing exact positioning. Design the funnel as part of the bin body, not a separate part.

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
2. Model the tutorial-style conveyor and import the NEMA17 reference.
3. Place conveyor on a provisional wood frame.
4. Import the heavy servo reference from `docs/datasheet/motion/heavy_servo/`.
5. Model servo mount, chute body, pivot interface, and first-pass bin rays.
6. Model four bins and bin entry guides.
7. Model the color shroud with two break-beam pairs through the side walls.
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
2. Servo horn or pivot slop under load
3. Size sensor geometry after the sensor choice is made
4. Frame stiffness around the servo and sensing station

Note: chute slide friction and servo positioning accuracy are mitigated by design (UHMW lining and wide bin funnels). They remain items to verify but are no longer the top risks.

Solve these with small prototypes and real-brick tests before making larger frame parts.
