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
- No failed re-latches
- No platform-stuck events

### Phase 2: Routing and confirmation

Run 25 drops per bin target using forced category/test mode.

Pass criteria:
- Correct selector index and bin confirmation for each drop
- No disc jam events

### Phase 3: Full-run reliability

Run 10 full 24-brick sessions on LiPo power.

Pass criteria:
- 24 processed per run
- Correct bin totals each run
- No unrecovered ERROR_HALT events

## Data logging

Capture serial logs for every run as CSV and store in `docs/runs/` once that directory is active.

## Failure handling

For each failed run:
- Record failure code
- Record brick number/state at failure
- Record corrective action
- Repeat run after fix to confirm resolution
