# Rollers OpenSCAD Package

This folder contains prototype OpenSCAD models for the belt drive pulley and idler roller.

System level assembly flow is in `docs/ASSEMBLY.md`.

Files:
- `src/rollers_params.scad`: shared parameters, truth tags, asserts, and dimension report
- `src/drive_pulley.scad`: 25mm OD drive pulley with split clamp shaft-fit hub
- `src/idler_roller.scad`: 25mm OD crowned idler with dual MR115 pockets
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
