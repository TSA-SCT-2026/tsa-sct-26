# Short TODO

Updated April 5, 2026 after the selector-chute architecture reset and firmware build repair.
Current priority: move from architecture cleanup into hardware-truth validation.

## This session's next tasks

1. Unblock CAD output generation
- Install OpenSCAD CLI
- Re-run `cad/frame/rollers/scripts/build_rollers.sh`
- Confirm the timing-stage STL outputs generate cleanly

2. Validate the highest-risk physical part
- Print the chute transition prototype first
- Test real-brick feed under full queue load
- Record jam and double-feed results

3. Lock real hardware truth
- Measure the received NEMA17 shaft, selector hardware, color sensor, and solenoid
- Compare those measurements against `cad/DIMENSIONS.md`
- Update only the dimensions that are now known from real parts

4. Run conveyor fit coupons
- Print shaft-fit, bearing-fit, and crown coupons from the roller subsystem
- Check supported-shaft fit, bearing fit, and tracking behavior
- Adjust roller and pulley parameters only from measured results

5. Prepare deferred validation work
- Keep color-sensor calibration deferred until the real sensor is mounted with the shroud installed
- Keep the selector evidence gate deferred until real selector timing data exists
- Bring both topics up automatically when work touches sensing, routing, or throughput claims
