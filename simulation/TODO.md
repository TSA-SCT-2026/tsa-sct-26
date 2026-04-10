# Simulator TODO

## Pending

**[TODO] Add firmware CSV event output for direct replay**

The simulator replay parser now accepts practical timestamp formats, but firmware currently logs in human-readable mode only. Add a true per-brick CSV event stream in firmware logger output so replay can consume real hardware runs directly with no manual transformation.

Target fields per brick event:

`brick, ts_enter_ms, ts_confirm_ms, category, size, color, bin, ok, samples, thermal_state`

Required additions that do not currently exist in firmware output:

- `run_id`: identifies run in multi-run calibration sessions.
- `state`: exact state or phase label (`FEED`, `APPROACH`, `SEATED`, `SENSING_DONE`, `INDEXED`, `RELEASED`, `BIN_CONFIRM`, `RESET`, `ERROR_HALT`).
- `ts_state_ms`: timestamp for each state transition row, not only entry and confirm summaries.
- `target_bin`: controller intent at classification time.
- `actual_bin`: confirmed bin from bin-confirm feedback.
- `index_steps`: selector movement in steps for that brick.
- `index_ms`: selector movement duration.
- `solenoid_on_ms`: applied pulse width for that release event.
- `error_code`: explicit error string on halt rows.
- `disc_steps`: absolute selector position after index.

CSV format proposal:

- Event stream row: `run_id,brick,state,ts_state_ms,category,size,color,target_bin,actual_bin,samples,index_steps,index_ms,solenoid_on_ms,disc_steps,thermal_state,error_code`
- Optional summary row: `run_id,summary,total_ms,accuracy,bin1,bin2,bin3,bin4,peak_heat,total_errors`

Why this matters:

- Replay can become exact, not reconstructed.
- Compare mode can compute precise per-phase deltas.
- Troubleshooting becomes deterministic and repeatable across runs.
- TODO: enforce run log filename convention in replay examples and docs: `run_YYYYMMDD_HHMM_session_label.csv`.

---

**[TODO] Add simulation compare mode for replay diagnostics**

Build a compare panel in simulation that:

- Joins expected simulation events and replayed events by `run_id + brick + state`.
- Displays delta per state in ms and highlights drift thresholds.
- Summarizes by phase: median delta, p95 delta, max absolute delta.
- Flags likely root causes:
  - `FEED` drift: belt speed inconsistency.
  - `SENSING_DONE` drift: color integration or I2C delay.
  - `INDEXED` drift: stepper acceleration or load change.
  - `BIN_CONFIRM` drift: release or drop timing changes.

---

**[TODO] Add optional firmware config import mode**

Add a build-time option that reads `firmware/src/config.h` and generates `simulation/src/generated_defaults.js`, then allows selecting:

- Manual defaults (current behavior)
- Use firmware config (generated defaults)

Minimum approach:

- Add parser script in simulation build step for `#define` values used by the simulator.
- Keep `defaults.js` as fallback when parser fields are missing.
- Print a warning for unmapped constants.

---

## Completed

**[DONE] Replace legacy model with chamber simulation**

- Replaced belt and plow model with chamber state flow and release-mechanism timing.
- Added selector chute indexing timing from step count and SPS.
- Added deterministic category-to-bin routing aligned with docs.

**[DONE] Update visualization for current mechanisms**

- Canvas now shows chamber, release mechanism, lever, selector, and bin layout.
- Timeline shows state transitions and key actuator events.
- Per-brick table now reports target vs actual bin and index timing.

**[DONE] Keep replay as troubleshooting workflow**

- Replay accepts full or partial timestamp CSV formats.
- Missing phase timestamps are reconstructed from current parameters.
- Replay uses same UI and metrics pipeline as simulated runs.

**[DONE] Add simulator test suite**

- Added Node-based tests for thermal math, sequence generation, and core simulation behavior.
