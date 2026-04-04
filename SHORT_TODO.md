# Short TODO

Updated April 4, 2026 after architecture sync.
Current phase: CAD first, with hardware truth lock only where it changes interfaces.

Details live in `docs/ARCHITECTURE.md` under `Dedicated TODO: CAD phase only`.

## This session

1. Lock real hardware truth
- Measure and photograph the received NEMA 17, color sensor, and solenoid
- Update any repo dimensions or assumptions that still differ from the real parts

2. Rebuild and lock roller outputs
- Regenerate all roller STL files from `cad/frame/rollers/scripts/build_rollers.sh`
- Run shaft-fit, bearing-fit, and crown coupons, then lock final values in `cad/frame/rollers/src/rollers_params.scad`

3. Build the Fusion conveyor module
- Place the NEMA 17, pulley, idler, belt envelope, axle stack, and tensioner travel
- Verify shaft fit, roller coplanarity, belt tracking envelope, and motor clearance

4. De-risk the highest-risk mechanisms
- Prototype the chute transition first
- Bench-validate the trapdoor release and re-latch cycle with repeated runs

5. Freeze control and packaging interfaces
- Freeze the conveyor stepper driver interface and GPIO plan after the driver choice is final
- Confirm footprint, labels, display visibility, and operator-facing layout
