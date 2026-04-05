# CAD

All printable parts for the chamber-drop sorter. Current production architecture uses a class 3 lever trapdoor, 4-index selector chute routing, and an off-axis timing-belt conveyor stage with a supported drive shaft.

Brick orientation requirement: widthwise across conveyor, with brick length along travel.
Why: this keeps across-channel fit constant for both sizes and makes size sensing deterministic from along-travel beam spacing.

## Directory structure

```
cad/
  chute/          feed chute tube, transition, chamber body, stop-wall interfaces
  chute_sensing/  color sensor shroud, IR break-beam mounts
  frame/          belt frame rails, end plates, channel walls, rollers, tensioner
  frame/rollers/  OpenSCAD timing stage, drive roller, idler, and fit coupon models
  bins/           four bin guides with angled floor and entrance beam slot
  escapement/     retired placeholder only, not part of the active design
```

Non-CAD references:
- `cad/DIMENSIONS.md`: critical geometry source of truth
- `cad/MECHANICAL.md`: mechanism rationale and tradeoffs
- `docs/ARCHITECTURE.md`: full pipeline and active CAD gates

OpenSCAD roller workflow:
- `cad/frame/rollers/scripts/build_rollers.sh`
- `cad/frame/rollers/stl/` output directory
- `cad/frame/rollers/ASSEMBLY.md` in depth roller assembly guide

## Print order

1. Chute transition piece first
- Validate with real bricks before any major print run
- Gate: no jams and no double-feed behavior

2. Conveyor timing stage and fit lock
- Validate the supported shaft, timing pulley, drive roller, idler, and tensioner travel before larger frame prints
- Gate: pulley alignment, shaft support, and belt tracking envelope verified

3. Trapdoor mechanism parts
- Platform
- Hinge bracket
- Lever arm
- Lever pivot bracket
- Solenoid mount bracket

Gate: 50 reliable drop and re-latch cycles before printing larger assemblies.

4. Chamber and sensing fit prints
- Beam hole fit and alignment
- Color sensor window and shroud fit
- Stop-wall switch fit

5. Selector chute prototype
- Validate repeatable indexing and clean drop path to all 4 bins

6. Remaining structural parts
- Frame, channel walls, bin shells, covers, cable guides

## Conventions

- Export STL files alongside source files
- Use descriptive versioned names: `platform_v3.stl`, `selector_chute_v2.stl`, `motor_pulley_v1.stl`, `drive_roller_v1.stl`
- Record any critical dimension deviation in commit context
- Do not freeze geometry from simulation alone: validate with real bricks

## Tolerance notes

FDM variance is 0.2-0.5mm. All brick-facing channels and interfaces need explicit tolerance.

- Undersized channels cause jams
- Oversized channels allow yaw and mis-seat risk
- Sensor mount tolerance must preserve beam alignment and shroud seal
