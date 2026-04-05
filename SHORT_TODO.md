# Short TODO

Updated April 5, 2026 after the selector-chute architecture reset and firmware build repair.
Current priority: finish ordering the timing-belt conveyor parts, validate CAD outputs, and wait for hardware arrival on April 10 before any measurement work.

## This session's next tasks

1. Order the timing-belt conveyor additions
- Add the timing-belt stage items from `docs/BOM.md` to the actual purchase plan
- Prioritize belt, pulleys, supported shaft hardware, and tension hardware before parts arrival

2. Unblock CAD output generation
- Re-run `cad/frame/rollers/scripts/build_rollers.sh`
- Confirm the timing-stage STL outputs generate cleanly

3. Validate the highest-risk physical part
- Print the chute transition prototype first
- Test real-brick feed under full queue load
- Record jam and double-feed results

4. Lock real hardware truth after parts arrival
- Parts are expected on April 10
- Do not plan any real-hardware measurement before then
- Measure the received NEMA17 shaft, selector hardware, color sensor, and solenoid after arrival
- Compare those measurements against `cad/DIMENSIONS.md`
- Update only the dimensions that are now known from real parts

5. Run conveyor fit coupons
- Print shaft-fit, bearing-fit, and crown coupons from the roller subsystem
- Check supported-shaft fit, bearing fit, and tracking behavior
- Adjust roller and pulley parameters only from measured results

6. Prepare deferred validation work
- Keep color-sensor calibration deferred until the real sensor is mounted with the shroud installed
- Keep the selector evidence gate deferred until real selector timing data exists
- Bring both topics up automatically when work touches sensing, routing, or throughput claims
