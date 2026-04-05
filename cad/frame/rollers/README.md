# Rollers OpenSCAD Package

This folder contains the production conveyor timing-stage models for the off-axis NEMA17 conveyor, plus fit coupons for the same subsystem.

System level assembly flow is in `cad/MECHANICAL.md`.

Files:
- `src/rollers_params.scad`: shared parameters, truth tags, asserts, and dimension report
- `src/motor_pulley.scad`: provisional timing pulley envelope for the NEMA17 motor shaft
- `src/drive_roller.scad`: smooth drive roller on a supported shaft
- `src/idler_roller.scad`: crowned idler with dual MR115 pockets
- `src/roller_coupons.scad`: bearing fit, shaft-fit, and crown test coupons
- `scripts/build_rollers.sh`: validation and STL export script
- `docs/`: part cards and parameter notes for the roller subsystem
- `ASSEMBLY.md`: beginner friendly in depth assembly and fit guide

Run:
```bash
cd cad/frame/rollers
./scripts/build_rollers.sh
```

Output:
- STL files are written to `cad/frame/rollers/stl/`

Prototype status:
- LEGO geometry and widthwise transport constraint are treated as `LOCKED_TRUTH`
- Purchased hardware fit values are treated as `PROVISIONAL_ESTIMATE` until parts are measured
- Direct motor-shaft drive is retired from the active conveyor path
