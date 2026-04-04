# CAD

All printable parts for the chamber-drop sorter. Current architecture uses a class 3 lever trapdoor and selector disc routing.

Brick orientation requirement: widthwise across conveyor, with brick length along travel.
Why: this keeps across-channel fit constant for both sizes and makes size sensing deterministic from along-travel beam spacing.

## Directory structure

```
cad/
  chute/          feed chute tube, transition, chamber body, stop-wall interfaces
  chute_sensing/  color sensor shroud, IR break-beam mounts
  frame/          belt frame rails, end plates, channel walls, rollers, tensioner
  frame/rollers/  OpenSCAD pulley, idler, and fit coupon models
  bins/           four bin guides with angled floor and entrance beam slot
```

Non-CAD references:
- `cad/DIMENSIONS.md`: critical geometry source of truth
- `cad/MECHANICAL.md`: mechanism rationale and tradeoffs
- `docs/ARCHITECTURE.md`: full pipeline and active CAD gates

OpenSCAD roller workflow:
- `cad/frame/rollers/scripts/build_rollers.sh`
- `cad/frame/rollers/stl/` output directory
- `cad/frame/rollers/ASSEMBLY.md` in depth roller assembly guide

Roller subsystem layout:
- `cad/frame/rollers/src/`: OpenSCAD source files
- `cad/frame/rollers/docs/`: part cards and parameter notes
- `cad/frame/rollers/scripts/`: export and validation scripts
- `cad/frame/rollers/stl/`: exported printable meshes

## Print order

1. Chute transition piece first
- Validate with real bricks before any major print run
- Gate: no jams and no double-feed behavior

2. Trapdoor mechanism parts
- Platform
- Hinge bracket
- Lever arm
- Lever pivot bracket
- Solenoid mount bracket

Gate: 50 reliable drop and re-latch cycles before printing larger assemblies.

3. Chamber and sensing fit prints
- Beam hole fit and alignment
- Color sensor window and shroud fit
- Stop-wall switch fit

4. Selector disc prototype
- Validate concentric alignment and clean drop path to all 4 bins

5. Remaining structural parts
- Frame, channel walls, bin shells, covers, cable guides

## Conventions

- Export STL files alongside source files
- Use descriptive versioned names: `platform_v3.stl`, `selector_disc_v2.stl`
- Record any critical dimension deviation in commit context
- Do not freeze geometry from simulation alone: validate with real bricks

## Tolerance notes

FDM variance is 0.2-0.5mm. All brick-facing channels and interfaces need explicit tolerance.

- Undersized channels cause jams
- Oversized channels allow yaw and mis-seat risk
- Sensor mount tolerance must preserve beam alignment and shroud seal
