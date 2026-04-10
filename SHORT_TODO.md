# Short TODO

Updated April 9, 2026 after the integrated trough timing-stage update.
Current priority: complete the Fusion 360 assembly of the integrated trough timing stage so conveyor interfaces stop drifting. First physical print priority still belongs to the chute transition prototype.

## This session's next tasks

1. Finish the integrated-trough Fusion 360 assembly
- Use `cad/frame/rollers/docs/FUSION360_ASSEMBLY.md` as the active check list
- Verify shaft parallelism, pulley belt plane, bridge belt window clearance, set-screw access, and idler hardware keep-out
- Lock any required interface notes before printing the full trough

2. Keep the chute transition as the first physical validation print
- Print the chute transition prototype before any full conveyor-body print
- Test real-brick feed under full queue load
- Record jam and double-feed results in run logs

3. Print and validate the conveyor coupon set
- Print `trough_end_cap_coupon_v1.stl` first
- Print shaft-fit, bearing-fit, and crown coupons from the roller subsystem
- Use measured fit and tracking results before changing any roller or trough parameters

4. Review the current timing-stage outputs
- `cad/frame/rollers/scripts/build_rollers.sh` now exports the trough and trough coupon along with the roller parts
- Check the generated timing-stage STLs before sending anything to the printer
- Keep output naming stable unless there is a versioned geometry change

5. Keep architecture drift under control
- Use `docs/project/CAD_ORCHESTRATION.md` as the broader session plan
- Keep `OPEN_DECISIONS.md` limited to true runtime-tuning questions
- Do not reopen frozen architecture during CAD unless new physical evidence forces it

6. Prepare deferred validation work
- Keep color-sensor calibration deferred until the real sensor is mounted with the shroud installed
- Keep the selector evidence gate deferred until real selector timing data exists
- Bring both topics up automatically when work touches sensing, routing, or throughput claims
