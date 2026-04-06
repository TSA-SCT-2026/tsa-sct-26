# CAD

All printable parts for the sorter. Current production architecture uses a release gate that is still under refactor, 4-index selector chute routing, and an off-axis timing-belt conveyor stage with a supported drive shaft.

Brick orientation requirement: long-side-across conveyor, with the 23.7mm side of a 2x3 spanning the channel and the 15.8mm side running along travel.
Why: this keys the chamber footprint to the long side so only one brick can occupy the release zone at a time. The chamber and sensing geometry stay provisional until re-derived from this rule.

## Directory structure

```
cad/
  chute/          feed chute tube, transition, chamber body, release interfaces
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

3. Release mechanism prototype parts
- Keep the actuation style provisional until the refactor is frozen
- Use only the simplest geometry that proves support removal and reset

Gate: 50 reliable release and reset cycles before printing larger assemblies.

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
