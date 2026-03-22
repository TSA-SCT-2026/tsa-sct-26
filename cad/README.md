# CAD

All printable parts for the sorting system. Everything structural is PLA printed on the school printer.

## Directory structure

```
cad/
  chute/          feed chute tube, flared top, cam housing, exit ramp (integrated)
  chute_sensing/  color sensor shroud, IR break-beam mounts
  escapement/     stepper cam disk
  frame/          belt frame rails, end plates, channel walls, rollers, tensioner
  pushers/        solenoid U-bracket mounts, face plates (x3)
  bins/           four bin guides with angled floor and entrance beam slot
```

## Print order

Print the chute transition piece first. Test it with real bricks before printing anything else. This is the highest mechanical risk in the project and the gate for all downstream CAD decisions.

Second: stepper cam disk. Simple geometry, worth iterating early before the housing is printed around it.

Everything else: after the chute transition is validated.

## Conventions

- Export STL files alongside source files (Fusion 360 / FreeCAD / whatever you use)
- Name files descriptively: `cam_disk_v2.stl`, `chute_transition_v1.f3d`, etc.
- Keep version numbers in filenames during iteration so old versions are not lost
- Critical dimensions are in MECHANICAL.md. Match them, and note any deviations

## Tolerance notes

FDM variance is 0.2-0.5mm. All channels that interface with bricks or the belt need explicit tolerances in CAD. Undersized channels jam. Oversized channels let bricks yaw past the taper. See MECHANICAL.md for target dimensions and acceptable ranges.
