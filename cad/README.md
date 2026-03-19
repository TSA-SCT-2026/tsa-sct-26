# CAD

All printable parts for the sorting system. Everything structural is PLA printed on the school printer.

## Directory structure

```
cad/
  chute/       near-vertical feed chute and transition curve at the base
  escapement/  stepper cam disk and motor housing
  frame/       main belt frame, channel walls, base plate, tensioner mounts
  sensing/     IR break-beam mounts and color sensor shroud (part of channel roof)
  plows/       plow arms (x3), pivot mounts, solenoid mounts
  bins/        four bin guides with ramp geometry for energy absorption
```

## Print order

Print the chute transition piece first. Test it with real bricks before printing anything else. This is the highest mechanical risk in the project and the gate for all downstream CAD decisions.

Second: stepper cam disk. Simple geometry, worth iterating early before the housing is printed around it.

Everything else: after the chute transition is validated.

## Conventions

- Export STL files alongside source files (Fusion 360 / FreeCAD / whatever you use)
- Name files descriptively: `plow_arm_v3.stl`, `chute_transition_v1.f3d`, etc.
- Keep version numbers in filenames during iteration so old versions are not lost
- Critical dimensions are in MECHANICAL.md. Match them, and note any deviations

## Tolerance notes

FDM variance is 0.2-0.5mm. All channels that interface with bricks or the belt need explicit tolerances in CAD. Undersized channels jam. Oversized channels let bricks yaw past the taper. See MECHANICAL.md for target dimensions and acceptable ranges.
