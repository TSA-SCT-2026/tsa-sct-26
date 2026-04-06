# Optimization Plan Log

Timestamp: 2026-04-06 11:31:54 EDT

Purpose:
Create one optimization program that maxes accuracy first, then maxes speed from first principles instead of inherited codebase assumptions.

Primary rule:
No speed gain is accepted if it reduces accuracy, reliability, reset truth, or evaluator-safe operation.

Scope:
- Release gate geometry and timing
- Selector chute geometry and timing
- Conveyor timing stage and belt speed
- Firmware event contract and state machine
- Simulation model
- Logging, tests, validation, and notebook evidence
- Deferred runtime decisions captured in `docs/project/OPEN_DECISIONS.md`

Working assumptions:
- The release gate is still under refactor and must not be frozen around stale trapdoor wording.
- Feed restart should be gated by `RESET_CONFIRMED`, not by full bin arrival.
- Selector motion and the next release should be gated by `CHUTE_CLEAR` or equivalent release-path-clear truth.
- The requested `~16mm` figure is treated here as the current target for effective support coverage or lever-tip travel envelope. The true minimum clean-withdrawal distance still has to be measured on hardware.
- Timing-stage tooth count is not frozen. Start runtime work from `20T/20T`. Keep `16T/20T` as a later experiment only if measured data says the conveyor is still the limiter after release and selector optimization.

Desired end state:
- Chamber refill begins as soon as reset truth is satisfied.
- The machine does not wait for full bin arrival before starting the next feed.
- Selector and release travel are minimized so belt speed is not wasted below the real bottleneck.
- Every speed claim is backed by measured timestamps in logs.

Section 1: Control truth that must be frozen before geometry tuning

Step 1.1:
Freeze event meanings across docs, firmware, and simulation.

Required meanings:
- `RESET_CONFIRMED`: release support is back in the safe supporting position and the chamber may accept the next brick
- `CHUTE_CLEAR`: the previous brick has cleared the selector and release volume enough that selector motion and the next release are safe
- `BIN_CONFIRMED`: scoring and fault-detection event only. It must not be the feed restart gate

Files affected later:
- `docs/ARCHITECTURE.md`
- `firmware/README.md`
- `firmware/EMBEDDED.md`
- `firmware/src/events.h`
- `firmware/src/state_machine.cpp`
- `simulation/src/simulation.js`

Step 1.2:
Refactor the control model into separate resource gates.

Required resource gates:
- Chamber and feed resource: restart on `RESET_CONFIRMED`
- Selector and release resource: block next selector move and next release until `CHUTE_CLEAR`
- Bin scoring resource: accept `BIN_CONFIRMED` later without blocking feed

Required overlap sequence:
1. Brick N releases
2. Release gate resets
3. Feed for brick N+1 begins
4. Brick N+1 seats and is sensed
5. Selector for brick N+1 moves only when release path is clear
6. Brick N+1 releases only when selector-ready and chute-clear are both true

Section 2: Release gate optimization

Objective:
Get the release gate short-travel, repeatable, and fast before chasing belt speed.

Accuracy gate:
- One seated brick only
- No partial support during release
- No false release under queue load
- 50 loaded cycles with zero missed drops
- 50 loaded cycles with zero failed resets

Step 2.1:
Define the closed support span from brick geometry.

Geometry basis:
- The brick floor that must stay supported along travel is based on the `15.8mm` short dimension.
- Closed support width should cover the brick floor with real-print margin.

First prototype target:
- Closed support span: `16.5mm` to `18.0mm`

Reason:
- `15.8mm` exact is too tight for print tolerance and seat variation.
- Support that is much longer than needed increases travel demand and reset time.

Step 2.2:
Separate two different release distances.

Distance A:
- Closed support span: the length needed to fully support the brick floor when closed

Distance B:
- Clean withdrawal distance: the much smaller distance needed to stop touching the brick during release

Rule:
- Do not assume Distance B equals Distance A.
- Measure minimum clean withdrawal on a bench prototype.

Measurement procedure:
1. Seat a real brick in the chamber
2. Mark the closed support edge
3. Retract the support in `0.5mm` steps
4. Record the first distance at which the brick drops cleanly 10 times in a row
5. Add `1mm` engineering margin

Output to record:
- `release_clean_withdrawal_mm`

Step 2.3:
Use the solenoid stroke only as needed.

Known fact:
- The current solenoid stroke is `10mm`

Working target from the request:
- Provide about `16mm` effective lever-tip travel envelope if that full motion is truly required by the support geometry

Important rule:
- If measured clean withdrawal is smaller, reduce the motion ratio and keep more force margin.

Candidate class 3 lever ratio study:
- Prototype ratio band: `1.4:1` to `2.0:1`
- Center test point if `16mm` tip travel is truly needed: `1.6:1`

Candidate sets:
- Effort point `10mm`, load point `14mm`
- Effort point `10mm`, load point `16mm`
- Effort point `10mm`, load point `18mm`

Selection rule:
- Choose the shortest load arm that still guarantees clean release at low battery and worst friction.
- More amplification is not automatically better because it costs force margin and stiffness.

Step 2.4:
Optimize pivot placement and support shape.

Required studies:
- Support edge chamfer shape
- Pivot height relative to support plane
- Return bias direction
- Passive re-latch versus positively driven re-latch

Required outcomes:
- Support withdraws fully from the brick path
- Support returns without bounce
- Support does not re-enter the path late
- Reset truth is physical and repeatable

Step 2.5:
Measure release timing as three separate truths.

Required timestamps:
- `ts_release_start_ms`
- `ts_reset_confirm_ms`
- `ts_chute_clear_ms`

Derived values:
- `release_to_reset_ms`
- `release_to_chute_clear_ms`
- `reset_to_next_entry_margin_ms`

Required pass criteria before belt-speed tuning:
- `release_to_reset_ms` is repeatable with small spread
- `release_to_chute_clear_ms` is known from real trials
- `ts_reset_confirm_ms` can occur before `ts_bin_confirm_ms` without error

Section 3: Selector chute optimization

Objective:
Minimize selector move time and stop error without hurting routing accuracy.

Accuracy gate:
- Clean path in all 4 positions
- No edge catch in repeated drops
- No wrong-bin routing in normal operation
- Repeatable home truth

Step 3.1:
Challenge the current selector radius.

Current risk:
- The current selector outlet offset may be larger than necessary and may be adding inertia and move time with no routing benefit.

Required study:
1. Use the real funnel opening size and required outlet spacing
2. Derive the minimum rotor radius that still gives clean outlet separation and wall thickness
3. Compare that result to the current offset

Decision rule:
- If a smaller radius preserves clean routing, reduce it.

Design priority:
- Minimize radius first
- Remove mass far from the axis second
- Preserve hub stiffness third

Step 3.2:
Define the selector body around timing.

Required outputs:
- Minimum outlet offset that still keeps bins separated
- Minimum wall thickness that still prints cleanly
- Rotor pocketing pattern
- Hub clamp or coupler geometry
- Home flag geometry

Mass reduction rule:
- Remove mass far from the axis first
- Keep the outer ring only where it improves guidance or strength

Step 3.3:
Measure the selector speed numbers that matter.

Required measured values:
- Adjacent move time
- Worst-case move time
- Weighted-average move time using the real 24-brick distribution
- Re-home penalty
- Position repeatability
- Missed-step or overshoot rate

Step 3.4:
Apply the selector evidence gate to optimization.

Selector stays active only if measured selector timing plus real release timing still supports a winning system with margin.

If not:
- Reopen downstream routing alternatives in the notebook
- Do not keep the selector active just because it is already in the repo

Section 4: Conveyor timing stage and belt speed

Objective:
Use the belt only as fast as the rest of the machine can actually benefit from.

Accuracy gate:
- No belt walk
- No seat overshoot
- No double-entry
- Chamber seat repeatable under full 24-brick queue

Step 4.1:
Treat tooth count as runtime-deferred.

Start point:
- `20T/20T` runtime baseline

Deferred experiment:
- `16T/20T` later, only if data says the system still needs timing-stage experimentation after release and selector optimization

Decision rule:
- Do not freeze a ratio change into architecture before runtime logs show that conveyor refill still limits cycle time after release and selector optimization.

Step 4.2:
Tune belt speed from reset-safe time, not bin-arrival time.

Required formula:
- `useful belt speed = chamber_pitch_mm / release_to_reset_s`

Secondary limit:
- If selector and chute-clear remain slower than reset-safe, further belt increases have weak value.

Step 4.3:
Measure chamber pitch honestly.

Required measured values:
- Cold-start travel to first seat
- Steady-state pitch advance distance
- Entry-to-seat time at each belt setting
- Overshoot rate
- Double-entry risk

Step 4.4:
Run belt-speed tests only after release and selector data exist.

Runtime belt-speed sweep:
1. Establish `20T/20T`
2. Test low, medium, and high belt speeds with fixed release and selector settings
3. Log `ts_entry_ms`, `ts_seated_ms`, `ts_reset_confirm_ms`, `ts_chute_clear_ms`
4. Check whether faster belt changes per-brick cycle time
5. If not, belt is no longer the bottleneck

Step 4.5:
Only then try later timing-stage experiments.

Condition to unlock `16T/20T` testing:
- Chamber refill time still dominates total cycle time after release and selector are already tuned

If false:
- Keep the simpler baseline
- Bank the reliability margin

Section 5: Firmware and control refactor

Objective:
Make the code match the optimized hardware truth.

Step 5.1:
Rename and split events.

Required event set:
- `START_BUTTON`
- `ENTRY_DETECTED`
- `CHAMBER_SEATED`
- `SENSING_DONE`
- `SELECTOR_READY`
- `RELEASE_STARTED`
- `RESET_CONFIRMED`
- `CHUTE_CLEAR`
- `BIN_CONFIRMED`
- `RESET`

Step 5.2:
Refactor the state machine.

Current blocker:
- The state machine still serializes one brick as `release -> bin confirm -> reset -> next feed`

Required behavior:
- Feed for brick N+1 starts after `RESET_CONFIRMED`
- Brick N+1 may seat and be sensed while brick N is still falling
- Selector motion for brick N+1 waits for `CHUTE_CLEAR`
- Release for brick N+1 waits for both `SELECTOR_READY` and `CHUTE_CLEAR`

Step 5.3:
Split timing constants.

Replace single serial assumptions with:
- `RELEASE_PULSE_MS`
- `RESET_CONFIRM_TIMEOUT_MS`
- `CHUTE_CLEAR_TIMEOUT_MS`
- `BIN_CONFIRM_TIMEOUT_MS`

Step 5.4:
Log overlap truth directly.

Required new log rows or fields:
- `reset_confirmed`
- `chute_clear`
- `feed_restart`
- `selector_index_ms`
- `release_to_reset_ms`
- `release_to_chute_clear_ms`

Required property:
- Logs must support `ts_reset_confirm_ms < ts_bin_confirm_ms`

Section 6: Simulator and runtime model

Objective:
Make the simulator stop baking in the wrong bottleneck.

Current blocker:
- The simulator still advances each brick as one serial chunk ending at reset after confirm.

Required model split:
- Chamber and feed timeline
- Selector and release timeline
- Bin-confirm timeline

Required simulator controls:
- `reset_confirm_ms`
- `chute_clear_ms`
- `bin_confirm_latency_ms`
- `selector_block_until_clear`

Required simulator tests:
- Feed may restart before bin confirm
- Selector move waits for chute clear
- Wrong-bin confirm still faults correctly even if the next brick has already started

Section 7: Validation and scoring evidence

Objective:
Turn optimization into notebook-grade proof.

Required test additions:
- Overlap test where `ts_reset_confirm_ms < ts_bin_confirm_ms`
- Overlap test where brick N+1 seats before brick N bin confirmation
- Selector safety test showing no selector move before chute clear
- Release safety test showing no second release before chute clear

Required notebook outputs:
- Release-gate decision matrix
- Selector geometry decision matrix
- Timing-stage ratio decision matrix
- Equations and measured bottleneck model
- Failure-mode update for overlap risks

Cross-codebase TODO by file area

Docs and project memory:
- `docs/ARCHITECTURE.md`: rewrite the active pipeline around reset-gated feed overlap
- `docs/build/CALIBRATION.md`: add measured `reset_confirm` and `chute_clear` procedures
- `docs/build/TEST_PROTOCOL.md`: add overlap pass/fail tests
- `docs/build/CHECKLIST.md`: add overlap readiness checks
- `docs/runs/README.md`: require overlap-capable timestamps
- `docs/project/OPEN_DECISIONS.md`: keep non-mechanical speed-max choices deferred
- `docs/notebook/README.md`: require decision matrices and overlap evidence

CAD and mechanical:
- `cad/DIMENSIONS.md`: add release travel targets, selector radius targets, and measured truth when available
- `cad/MECHANICAL.md`: lock release and selector optimization rules after bench proof
- `cad/frame/rollers/src/rollers_params.scad`: keep pulley ratio baseline provisional

Firmware:
- `firmware/src/events.h`
- `firmware/src/state_machine.h`
- `firmware/src/state_machine.cpp`
- `firmware/src/config.h`
- `firmware/src/logger.h`
- `firmware/src/logger.cpp`
- `firmware/src/test_harness.cpp`
- `firmware/README.md`
- `firmware/EMBEDDED.md`
- `firmware/FAILURE_MODES.md`

Simulation:
- `simulation/src/simulation.js`
- `simulation/src/defaults.js`
- `simulation/src/replay.js`
- `simulation/tests/simulator.test.js`
- `simulation/README.md`
- `simulation/TODO.md`

Open decisions that must stay open until runtime data exists

These belong in `docs/project/OPEN_DECISIONS.md`, not in frozen architecture docs:
- Timing-stage pulley tooth count after baseline runtime
- Final conveyor target speed
- Whether selector motion can begin before full bin confirmation
- Whether `CHUTE_CLEAR` comes from a dedicated sensor, chamber-clear logic, or another truth path
- Whether a smaller-radius selector still preserves routing margin under real drops

Mechanical decisions that should be optimized directly, not deferred:
- Release support closed span
- Release clean-withdrawal distance
- Lever ratio and pivot placement
- Return bias direction and strength
- Selector outlet offset
- Selector pocketing and mass placement

Implementation order:
1. Freeze event meanings
2. Prototype release gate and measure clean-withdrawal truth
3. Measure `release_to_reset_ms`
4. Measure `release_to_chute_clear_ms`
5. Reduce selector radius and mass if routing margin allows
6. Measure adjacent, worst-case, and weighted selector timing
7. Refactor firmware event contract and state machine
8. Refactor logging and test harness
9. Refactor simulator
10. Run `20T/20T` runtime baseline
11. Decide whether belt speed is still the bottleneck
12. If yes, try later timing-stage experiments from `docs/project/OPEN_DECISIONS.md`
13. Record all decisions in notebook matrices and run logs

Stop conditions:
- Do not speed-max further if accuracy drops
- Do not speed-max further if reset truth becomes ambiguous
- Do not speed-max further if selector routing margin shrinks below reliable repeatability
- Do not speed-max further if belt speed increases no longer reduce total cycle time
- Do not speed-max further if added complexity hurts evaluator-facing reliability

Success criteria:
- Accuracy is maxed and repeatable
- Release gate and selector have measured timing truth
- Belt is only as fast as the real bottleneck can use
- Every remaining speed-max option that is not yet proven is captured in `docs/project/OPEN_DECISIONS.md`
- The notebook can defend both the chosen hardware and the rejected faster-looking alternatives
