# Test Protocol

## Objective

Demonstrate reliable sorting and mechanism integrity under evaluator-style operation.

## Test set

- 24-brick competition set: 6 red 2x2, 6 blue 2x2, 8 blue 2x3, 4 red 2x3
- Bricks loaded per operating instructions

## Test phases

### Phase 1: Release mechanism validation

Run 50 loaded release cycles.

Pass criteria:
- No missed drops
- No failed resets
- No release-path stuck events

### Phase 2: Queue cadence and chamber pitch

Run 25 loaded seat and reset cycles with the compressed queue loaded.

Pass criteria:
- No double-entry events
- Chamber seats repeatably at the same stop wall position
- Next feed does not begin before reset confirmation
- Measured restart distance stays within the chamber pitch window

### Phase 3: Size sensing validation

Run 20 seated checks for each case:
- empty chamber
- `2x2` left-biased
- `2x2` centered
- `2x2` right-biased
- `2x3`

Pass criteria:
- Empty chamber keeps both ToF lanes clear
- Every `2x2` trial occupies exactly one lane
- Every `2x3` trial occupies both lanes
- Repeated seat and reset cycles do not drift out of the calibrated threshold bands

### Phase 4: Routing and confirmation

Run 25 drops per bin target using forced category/test mode.

Pass criteria:
- Correct selector index and bin confirmation for each drop
- No selector jam events
- Selector-ready latency and worst-case move time stay within the modeled gate
- Re-home penalty remains repeatable
- Wrong-bin confirm events are never observed in normal routing

### Phase 5: Reset and chamber clear

Run 25 release and reload cycles while verifying chamber-clear and reset-confirmation events.

Pass criteria:
- Chamber clear is detected before the next brick enters
- Reset confirmation arrives before feed restart
- No stale chamber occupancy remains after reset

### Phase 6: Full-run reliability

Run 10 full 24-brick sessions on LiPo power.

Pass criteria:
- 24 processed per run
- Correct bin totals each run
- No unrecovered ERROR_HALT events
- CSV log contains per-brick timestamps for entry, seated, sensing, selector ready, release, bin confirm, and reset complete
- Run summary reports correctness truthfully, not just total throughput

## Data logging

Capture serial logs for every run as CSV and store in `docs/runs/`.
Use one row per brick and include the full timestamp chain needed to replay the run.

## Failure handling

For each failed run:
- Record failure code
- Record brick number/state at failure
- Record corrective action
- Repeat run after fix to confirm resolution
