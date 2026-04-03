# Rollers OpenSCAD Package

This folder contains prototype OpenSCAD models for the belt drive pulley and idler roller.

Files:
- `rollers_params.scad`: shared parameters, truth tags, asserts, and dimension report
- `drive_pulley.scad`: 25mm OD drive pulley with split clamp D bore hub
- `idler_roller.scad`: 25mm OD crowned idler with dual MR115 pockets
- `roller_coupons.scad`: bearing fit, D bore fit, and crown test coupons
- `build_rollers.sh`: validation and STL export script

Run:
```bash
cd cad/frame/rollers
./build_rollers.sh
```

Output:
- STL files are written to `cad/frame/rollers/stl/`

Prototype status:
- LEGO geometry and widthwise transport constraint are treated as `LOCKED_TRUTH`
- Purchased hardware fit values are treated as `PROVISIONAL_ESTIMATE` until parts are measured
