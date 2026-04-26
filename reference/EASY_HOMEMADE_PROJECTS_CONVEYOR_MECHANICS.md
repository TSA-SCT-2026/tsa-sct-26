# Easy HomeMade Projects Conveyor Mechanics Reference

Use this as a mechanical reference for the tutorial-style conveyor only. Do not copy exact dimensions from this file into the active sorter. Active geometry must come from source-backed values in `cad/DIMENSIONS.md`, real dry-fit measurements, or the final CAD export after physical confirmation.

Source video: [How to Make 2-Way Conveyor Belt System Using Stepper Motor & Arduino](https://www.youtube.com/watch?v=qoejrZhn5XI) by Easy HomeMade Projects.

## What To Preserve

The tutorial is valuable because the conveyor is simple to build, inspect, and repair:

- Wood or sheet side structure rather than an aluminum extrusion frame
- Printed bearing holders at the four roller ends
- Adjustable bearing holders for conveyor belt tension
- Printed rollers on through shafts
- Printed shaft spacers to keep the rollers centered
- NEMA17 stepper motor with an adjustable timing-belt mount
- Timing pulley drive outside one side plate
- Screw-together assembly that can be dry-fit before final drilling
- Belt cut from sheet material and joined after wrapping around the rollers

Preserve the construction style and order. Do not preserve the tutorial's dimensions as final sorter truth.

## Tutorial Mechanical Layout

The tutorial build uses:

- NEMA17 stepper motor
- Two roller shafts
- Four 608 bearings
- Two printed rollers
- Four printed bearing holders
- Four printed shaft spacers
- Printed or wood NEMA17 motor board with adjustment slots
- Timing pulleys and timing belt
- Wood, hardboard, or laminated sheet structure
- Conveyor belt made from sheet material with a taped joint

Observed tutorial material dimensions from the transcript:

- Top sheet or bed piece: about 270mm x 85mm
- Belt strip length: about 600mm
- Hardboard piece: about 280mm x 85mm
- Two long side pieces: about 300mm x 80mm
- Shaft path: 8mm shaft
- Bearing path: 608 bearings

These are reference observations, not active states dimensions.

## Team Substitutions

Use the same general shape, but change the hardware path for the states sorter:

- Use the current 25mm neoprene belt strip, not the tutorial belt width.
- Use the shorter active conveyor geometry from the build guide, with about 300mm to 400mm usable top-run length.
- Use 5mm shafting.
- Use matching 5mm-ID bearings, currently the MR85ZZ path, unless real hardware forces a change.
- Use printed rollers sized around the 25mm belt, with simple flanges.
- Use a wood block support under the top belt run instead of 2020 extrusion.
- Keep the NEMA17 motor, but mount it on a grounded outside board with the shaft pointing inward
  toward the conveyor. This keeps the motor serviceable, avoids a large side-plate cutout, and
  creates a rear/right service bay for the GT2 timing path.
- Use GT2 timing pulleys and belt only where they make the drive easier to build and tension.
- Keep the belt surface and final frame height tied to the chute and bin handoff, not the tutorial's frame height.

## Simplified CAD Interpretation

Model only the parts needed for drilling, printing, and fit checks:

- Side plates with external bearing-holder mounting holes and idler adjustment slots
- Fixed and slotted bearing holders
- Idler roller
- Drive roller with a simple hub
- Motor mount plate with NEMA17 holes and adjustment slots
- Wood block support placeholder
- Crossmember spacers only where the side plates need stiffness
- Simple belt path envelope, not belt tooth detail

Skip or simplify these in first-pass CAD:

- Detailed flexible belt geometry
- GT2 tooth profiles
- Exact screw threads
- Exact tape or belt splice geometry
- Final motor board drill position before real pulley fit
- Set screw hole if it slows Fusion work

For the drive roller hub, it is acceptable to print a solid hub, drill the radial set screw hole by hand after printing, tap it for M3, and file a small flat on the shaft. Mark the hub in CAD if useful, but the drilled hole is enough for the first build.

## Bench Build Sequence To Copy

Follow the tutorial's bench logic with the team dimensions:

1. Attach the bearing holders to the side plates.
2. Press the bearings into the holders.
3. Mount the NEMA17 board so the motor can slide for timing belt tension.
4. Install shafts through the rollers.
5. Add spacers so rollers stay centered between the side plates.
6. Dry-fit rollers, shafts, bearings, and side plates.
7. Attach the support bed or wood block below the top belt run.
8. Install timing pulleys and timing belt.
9. Wrap and splice the conveyor belt.
10. Tension the conveyor belt with the adjustable idler holders.
11. Tension the timing belt with the adjustable motor board slots.
12. Run the conveyor alone before adding the sensing shroud or chute.

## First Build Bias

Prefer features that can be adjusted at the bench:

- Slotted idler bearing holders
- Slotted motor board
- Clamp and mark motor board holes from the real pulley path
- Use washers on slots
- Leave access to the drive roller set screw
- Keep the top belt support smooth and low friction
- Keep the side plates easy to drill from a paper template

Do not spend CAD time on precision that the first dry-fit will replace. The conveyor wins by being straight, adjustable, and easy to debug.
