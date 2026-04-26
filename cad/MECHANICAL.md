# Mechanical Design

This document records mechanical intent, risks, and validation order for the simplified states build. Exact dimensions live in `cad/DIMENSIONS.md` only when they are source-backed or clearly marked as variables.

## Strategy

The active mechanical plan is a simple conveyor, sensing station, and servo rotary chute selector. Custom CAD should support real fitting, not replace it.

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

Use the tutorial-style conveyor as the active states path. The tutorial is a structural idea, not a source for final dimensions.

Build around:
- Thin side plates and a real dry-fit belt path
- Wood support under the top belt run
- External printed bearing holders for the 5mm shafts
- NEMA17 motor on a serviceable outside motor board
- Accessible pulley and belt tension path
- 5mm shaft and matching bearing path from the active hardware plan

Current side-plate note:
- The active design does not drill 5mm shaft holes through the wood side plates.
- Shaft alignment comes from the external printed bearing holders and their mounting holes.
- Treat older shaft-through-wood instructions as stale sprint notes unless the user explicitly reopens that approach.

Do not lock conveyor height, feet, motor board Z, or exit handoff placement until the bin and chute geometry exists. The belt exit, chute entry, chute exit, and bin catch height must be checked together.

Assembly method:
- Use visible sketch anchors, axes, and construction planes for CAD layout.
- Prefer slots, shims, removable tabs, and drill-from-fit methods for hardware-facing interfaces.
- Mark holes from the fitted real assembly when hole positions depend on wood stock, pulley alignment, or adjustable brackets.
- Delete or generalize stale draft dimensions after physical fitting changes them.

Downloaded conveyor models may be used as visual reference only. Do not spend the sprint cleaning imported geometry when the tutorial-style conveyor can be modeled, inspected, and repaired directly.

The rear or right motor bay may later hold electronics and cable strain relief. Reserve space, but do not create exact electronics mounts until conveyor, chute, and bin geometry are stable.

## Frame

The active frame is wood, with printed brackets where they save time or improve fit. Wood stock will be picked up locally, so board and block thicknesses stay provisional until purchase and measurement.

The frame needs to:
- Place the belt at a height that feeds the chute entry cleanly
- Keep the servo pivot stable
- Keep sensing brackets from moving during operation
- Leave bins removable
- Leave wiring visible enough to look deliberate and clean
- Stay inside the 610mm x 610mm footprint

Woodworking is currently downstream of final bin and chute layout. Do not cut or permanently drill around conveyor feet, servo feet, or exit handoff placement until that geometry is known.

Do not design around 2020 extrusion unless the user confirms a material change.

## Sensing Station

The sensing station mounts near the start of the conveyor on the wood conveyor bed or a rigid frame member. It is upstream of the belt exit so classification can finish before the brick reaches the servo chute handoff.

Order along travel:
1. Manual feed and orientation cue
2. Optional low guide rails
3. Size sensing
4. Color sensing with shroud
5. Belt exit into servo chute

Size sensing is frozen as break-beam timing with two pairs through the sensing shroud side walls. Keep the shroud removable and adjustable until real-brick belt tests confirm fit. Do not reopen ToF or distance sensing unless the user explicitly changes the size sensor decision.

Color sensing uses the TCS3200/GY-31 module. The shroud is required. Open-air color calibration is not valid for this build.

Shroud design goals:
- Block side light
- Clear the brick including studs
- Avoid catching bricks
- Mount rigidly enough that calibration does not drift
- Leave wiring exit away from the belt and servo sweep

## Servo Rotary Chute Selector

This is the main custom mechanical subsystem.

The MG995/MG996/MG996R-class heavy servo rotates an angled chute so its exit points into one of four arc-defined bin catch windows.

Chute body:
- Rectangular or gently guided channel
- Brick clearance derived from the as-fed brick envelope plus yaw tolerance
- Pivot-to-exit radius as short and stiff as the bin layout allows
- UHMW tape applied to sliding surfaces before meaningful tests
- Pivot boss or adapter tied securely to the servo horn

Servo mount:
- Servo output shaft points upward
- Servo body sits in a pocket or cradle verified against the real part
- Mount plate ties into the base plate or a stiff frame member, not the thin conveyor side plate
- Servo feet should use slots for small physical alignment corrections after belt, chute, and bin arc placement
- Chute pivot axis stays vertical and aligned with the horn
- Wires leave away from the horn and chute sweep

Chute angle:
- Choose from UHMW-lined real-brick testing
- Do not treat bare-plastic or open-air slide behavior as final
- The sensitive variables are handoff, arm stiffness, and swept catch margin more than theoretical angle

Bin alignment:
- Use construction rays from the pivot
- Treat servo angles as calibration outputs
- Each bin needs an arc-defined catch window at the verified catch height
- Verify the swept 2x3 brick envelope clears dividers and falls inside the matching bin at all four positions
- Use CAD motion checks, then real-brick tests, before printing large parts

## Bins

Use four labeled receiving bins under the tested chute arc.

Each bin should:
- Hold its expected brick count with margin
- Have a clear label on the front
- Be removable without bumping the chute
- Have an arc-defined catch entry at the verified chute exit height
- Keep category interiors separated

The bin arc and fixed locator platform are reliability features, not decoration. They let the removable bin shell return to the same position after dumping and give the chute a repeatable catch target.

Final bin geometry is now the first active CAD priority. Do not back-fill this file with old bin dimensions as if they are current.

## Operator-Facing Packaging

These features directly affect score because the evaluator operates alone:
- Feed orientation cue
- Labeled start button
- Display states: READY, SORTING, SORT COMPLETE, ERROR
- Bin labels
- Clean cable routing
- Stable frame and controlled footprint

The machine should look like a finished competition device, not a temporary bench test.

## Current CAD Build Status

Critical path:
1. Middle support clearance and servo chute placement.
2. Sweep-defined bin arc and fixed locator platform.
3. Conveyor height, support feet, and exit handoff.
4. Wood base cuts and permanent woodworking.

Still useful in parallel:
- Engineering notebook decision matrices.
- Validation plan.
- Run log schema.
- Operator instructions.
- Logical wiring plan.

## First Physical Tests

Run these before committing to large prints:
1. Short UHMW-lined chute test with real bricks.
2. Conveyor-to-chute handoff test with a real brick.
3. Servo sweep test without a brick.
4. Servo sweep test with chute installed.
5. Bin alignment test for all four categories.
6. Color sensor shroud clearance and calibration test.

## Highest Fabrication Risks

1. Conveyor-to-chute handoff
2. Servo horn or pivot slop under load
3. Break-beam size sensor geometry after shroud fit and timing calibration
4. Frame stiffness around the servo and sensing station
5. Bin divider overlap or poor catch height

Solve these with small prototypes and real-brick tests before making larger frame parts.
