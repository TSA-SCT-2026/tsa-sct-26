# Short TODO

Updated April 6, 2026 after the start-gate and release-gate architecture freeze.
Current priority: follow `docs/project/CAD_ORCHESTRATION.md`, place the final order, and start CAD without more system-level drift.

## This session's next tasks

1. Follow the orchestration doc
- Use `docs/project/CAD_ORCHESTRATION.md` as the active session plan
- Keep `OPEN_DECISIONS.md` limited to true runtime-tuning questions
- Do not reopen frozen architecture during CAD unless new physical evidence forces it

2. Finalize the buy list
- Order the remaining timing-stage parts, selector hub, required switch kit, operator controls, UHMW tape, and IO expander
- Make the hardware-store run for M3, M5, and the chamber stop pad
- Keep owned start-gate servos and release solenoids out of the buy-now list

3. Review the refreshed timing-stage STL outputs
- `cad/frame/rollers/scripts/build_rollers.sh` now runs end to end
- Check the generated pulley, roller, and coupon STLs before printing
- Keep only the current `drive_roller` and `coupon_shaft_fit` output names in future artifact updates

4. Validate the highest-risk physical part
- Print the chute transition prototype first
- Test real-brick feed under full queue load
- Record jam and double-feed results

5. Keep only real unresolved issues in `OPEN_DECISIONS.md`
- Do not let solved switch or strip-width decisions leak back into the open list
- Reserve easy mounting provision in CAD only where it costs very little
- Compare any new measurements against `cad/DIMENSIONS.md`

6. Run conveyor fit coupons
- Print shaft-fit, bearing-fit, and crown coupons from the roller subsystem
- Check supported-shaft fit, bearing fit, and tracking behavior
- Adjust roller and pulley parameters only from measured results

7. Prepare deferred validation work
- Keep color-sensor calibration deferred until the real sensor is mounted with the shroud installed
- Keep the selector evidence gate deferred until real selector timing data exists
- Bring both topics up automatically when work touches sensing, routing, or throughput claims
