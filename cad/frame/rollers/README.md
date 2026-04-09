# Rollers OpenSCAD Package

This folder contains the production conveyor timing-stage models for the off-axis NEMA17 conveyor, plus fit coupons for the same subsystem.

System level assembly flow is in `cad/MECHANICAL.md`.

Files:
- `src/rollers_params.scad`: shared parameters, truth tags, asserts, and dimension report
- `src/motor_pulley.scad`: timing pulley envelope for the NEMA17 motor shaft (layout reference; active build uses purchased 20T GT2 pulley)
- `src/drive_roller.scad`: smooth drive roller on a supported shaft
- `src/idler_roller.scad`: crowned idler with dual MR85 pockets
- `src/motor_mount_bracket.scad`: NEMA17 mount plate with 8mm belt tension slots
- `src/bearing_block.scad`: single MR85ZZ bearing support block (two needed per shaft)
- `src/roller_coupons.scad`: bearing fit, shaft-fit, and crown test coupons
- `scripts/build_rollers.sh`: validation and STL export script
- `docs/`: part cards and parameter notes for the roller subsystem
- `ASSEMBLY.md`: beginner-friendly physical assembly guide
- `docs/FUSION360_ASSEMBLY.md`: step-by-step Fusion 360 CAD assembly guide for the timing stage

Run:
```bash
cd cad/frame/rollers
./scripts/build_rollers.sh
```

Output:
- STL files are written to `cad/frame/rollers/stl/`

Status:
- LEGO geometry and long-side-across transport constraint are treated as `LOCKED_TRUTH`
- Purchased hardware fit values are treated as `PROVISIONAL_ESTIMATE` until parts are measured
