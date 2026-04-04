# Short TODO

Updated April 4, 2026.
Current phase: CAD first, with hardware truth lock only where it changes interfaces.

Details live in `docs/ARCHITECTURE.md` under `Dedicated TODO: CAD phase only`.

## This session

1. Chute transition gate
- Prototype chute transition first
- Validate no jams and no double-feed behavior

2. Trapdoor mechanism gate
- Print and bench-validate platform, lever, hinge, and solenoid brackets
- Complete 50-cycle drop and re-latch test

3. Conveyor module CAD gate
- Build the Fusion conveyor module around the NEMA 17, pulley, idler, belt envelope, and tensioner travel
- Verify shaft fit, roller coplanarity, belt tracking envelope, and footprint margin

4. Roller STL and coupon lock
- Regenerate all roller STL files from `cad/frame/rollers/scripts/build_rollers.sh`
- Run shaft-fit, bearing-fit, and crown coupons, then lock final params in `cad/frame/rollers/src/rollers_params.scad`

5. Hardware truth lock gate
- Verify the received color sensor module against the `TCS3200 GY-31` purchase record
- Verify the received solenoid against the `0530 Linear Solenoid Electromagnet 12V` purchase record
- Photograph, measure, and update any remaining repo assumptions that do not match the real hardware

6. Conveyor control freeze gate
- Freeze the conveyor stepper driver interface and GPIO plan after the real driver choice is locked
- Keep conservative acceleration and gate feed on entry beam, stop switch, and reset completion

7. Packaging and operator UX gate
- Confirm CAD envelope stays within 610mm x 610mm
- Place and verify operator labels, display readability, and start-control area
