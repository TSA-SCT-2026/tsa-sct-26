# Critical Dimensions

Truth model:
- `LOCKED_TRUTH`: stable, physically invariant constraints
- `DECIDED_FOR_STATES`: active states-build choice
- `PROVISIONAL_ESTIMATE`: best estimate before CAD import or physical testing
- `OPEN_DECISION`: unresolved and tracked in `docs/project/OPEN_DECISIONS.md`

## Bricks

Status for this section: `LOCKED_TRUTH`

| Dimension | 2x2 | 2x3 |
|-----------|-----|-----|
| Width | 15.8mm | 15.8mm |
| Length | 15.8mm | 23.7mm |
| Height without studs | 11.4mm | 11.4mm |
| Stud height | 1.7mm | 1.7mm |
| Weight | about 2g | about 3g |

Manual feed orientation target: studs up, long side along conveyor travel.

Reason: this is easiest for an evaluator to place repeatably and gives the sensing station a predictable in-flight profile. If physical testing shows a different orientation is more reliable, update this file and the feed label together.

## Footprint

Status for this section: `LOCKED_TRUTH`

| Feature | Dimension |
|---------|-----------|
| Maximum footprint | 610mm x 610mm |
| CAD boundary | 610mm x 610mm construction rectangle |
| Bin access | Front or front-side removal preferred |
| Cable routing | Outside conveyor, chute, servo horn, and bin paths |

## Frame

Status for this section: `DECIDED_FOR_STATES` for material family, `PROVISIONAL_ESTIMATE` for exact dimensions

| Feature | Dimension or guidance |
|---------|-----------------------|
| Material | Wood or 3D printed structure from available stock |
| 2020 extrusion | Not active for states |
| Belt surface height | 200mm to 300mm target |
| Minimum height check | Bin height plus chute vertical drop plus 20mm clearance |
| Base plate | Fit within 610mm x 610mm |
| Stiffness target | No visible sensor or chute drift during servo motion |

## Conveyor

Status for this section: `PROVISIONAL_ESTIMATE`

| Feature | Dimension or guidance |
|---------|-----------------------|
| CAD source | Downloaded NEMA17 mini conveyor preferred, custom roller work is fallback |
| Motor | NEMA17 17HS4401S reference |
| Usable belt length | 300mm to 400mm target |
| Belt width | 40mm to 50mm target |
| Minimum clear brick width | 23.7mm brick plus side clearance |
| Belt speed phase 1 | Start slow enough for reliable sensing, then tune |
| Belt exit | Direct handoff into servo chute entry |
| Tension method | Use downloaded assembly tensioner if available, otherwise simple slotted idler or motor mount |
| Timing belt and pulley | Keep as active if the imported conveyor uses or benefits from the existing GT2 parts |
| 25mm neoprene strip | Available material, use only if it fits the chosen conveyor path |
| 15mm neoprene strip | Spare stock unless testing shows a use |

## Sensing Station

Status for this section: `OPEN_DECISION` for size sensor, `DECIDED_FOR_STATES` for color sensor

| Feature | Dimension or guidance |
|---------|-----------------------|
| Sensing location | Near belt exit, before chute handoff |
| Size sensor family | Open: break-beam timing, ToF or distance sensor, or another simple option |
| Size sensor spacing | Leave adjustable mounting room until selected |
| Color sensor | TCS3200/GY-31 module |
| Color shroud inner width | Belt width plus clearance |
| Color shroud inner length | About 30mm first pass |
| Color shroud height | About 20mm first pass |
| Brick clearance under shroud | More than 13.5mm including studs if studs-up feed is used |
| Sensor order | Size first, color second, belt exit last |

Color calibration is valid only with the shroud installed in final geometry.

## Servo Rotary Chute Selector

Status for this section: `PROVISIONAL_ESTIMATE`

| Feature | Dimension or guidance |
|---------|-----------------------|
| Servo family | MG995/MG996/MG996R-class heavy servo |
| Servo reference path | `docs/datasheet/motion/heavy_servo/` |
| Chute internal width | 30mm first pass |
| Chute internal height | 15mm first pass |
| Chute wall thickness | 3mm first pass |
| Chute length | 100mm to 150mm |
| Chute angle | Test 30, 35, 40, and 45 degrees |
| First CAD angle | 35 degrees from horizontal |
| Total servo sweep | About 105 degrees first pass |
| Example servo positions | 37, 72, 107, 142 degrees |
| Position spacing | About 35 degrees first pass |
| Pivot axis | Vertical servo output shaft, aligned to chute pivot |
| Servo mount | Pocket or cradle matching the real servo body |
| Chute connection | Servo horn adapter or horn screwed into pivot boss |
| Wire keepout | Clear of horn, chute sweep, and brick path |

Before final printing, verify chute exit overlap with all four bin guides in Fusion using a revolute joint or equivalent position checks.

## Bins

Status for this section: `PROVISIONAL_ESTIMATE`

| Bin label | Count target | Internal target |
|-----------|--------------|-----------------|
| 2x2 RED | 6 bricks | 80mm x 80mm x 60mm |
| 2x2 BLUE | 6 bricks | 80mm x 80mm x 60mm |
| 2x3 RED | 4 bricks | 80mm x 80mm x 60mm |
| 2x3 BLUE | 8 bricks | 80mm x 80mm x 60mm |

Arrangement: four bins under the chute arc. Exact positions are derived from the final pivot point, chute length, and tested servo angles.

## Manual Feed Guide

Status for this section: `DECIDED_FOR_STATES`

| Feature | Dimension or guidance |
|---------|-----------------------|
| Base feed mode | One brick at a time |
| Optional later feature | Simple feed chute only after the base sorter works |
| Orientation label | Studs up, long side along travel |
| Guide width | Belt width or narrower guide that still clears 23.7mm brick length as oriented |
| Anti-wander guide | Low side rails if belt wandering hurts sensing repeatability |

## Operator-Facing Packaging

Status for this section: `DECIDED_FOR_STATES`

| Feature | Requirement |
|---------|-------------|
| Start button label | Printed or engraved, visible at arm's length |
| Display viewing distance | Arm's length readable from front of machine |
| Display state words | READY, SORTING, SORT COMPLETE, ERROR |
| Bin labels | 2x2 RED, 2x2 BLUE, 2x3 RED, 2x3 BLUE |
| Label flats | Reserved on bin faces and control area |
| Cable routing | Clean and strain-relieved |
| Fan | Keep if needed for electronics cooling |

## Validation Checklists

These checks apply to `PROVISIONAL_ESTIMATE` and `OPEN_DECISION` values above.

### Conveyor

- Import or model the chosen conveyor
- Verify belt width and brick clearance
- Verify motor and pulley clearance
- Verify handoff from belt exit to chute entry
- Verify footprint with frame and bins visible

### Sensing

- Choose size sensor family
- Fit the size sensor bracket without blocking brick travel
- Fit the color sensor shroud and verify no light leaks
- Verify shroud clearance with real bricks
- Record calibration with the final shroud installed

### Servo Chute

- Measure or import the actual servo model
- Test chute angle with real bricks
- Verify four servo angles in CAD
- Verify bin guide overlap at each position
- Verify servo power and wiring clearance

### Frame And UX

- Verify 610mm x 610mm footprint
- Verify bin removal clearance
- Verify labels are visible
- Verify the start control and display are readable from arm's length
