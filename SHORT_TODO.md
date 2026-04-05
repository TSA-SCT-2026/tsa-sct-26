# Short TODO

Updated April 5, 2026 after the selector-chute architecture reset, firmware build repair, and conveyor timing-stage cleanup.
Current priority: order the remaining timing-stage parts, validate the chute transition with bricks already on hand, and wait for hardware arrival on April 10 before any measurement work.

## This session's next tasks

1. Order the timing-belt conveyor additions
- Order only the remaining timing-stage items that are not already in `docs/BOM.xlsx`
- Buy the `100T` and `104T` belts, `16T` and `32T` pulleys, `5mm` shaft stock, extra `MR115ZZ` bearings, and `5mm` shaft collars
- Use the slotted `NEMA17` mount as the active tension plan instead of buying a separate tensioner first

2. Review the refreshed timing-stage STL outputs
- `cad/frame/rollers/scripts/build_rollers.sh` now runs end to end
- Check the generated pulley, roller, and coupon STLs before printing
- Keep only the current `drive_roller` and `coupon_shaft_fit` output names in future artifact updates

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
