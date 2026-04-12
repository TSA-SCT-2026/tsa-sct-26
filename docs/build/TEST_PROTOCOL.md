# Test Protocol

## Objective

Demonstrate reliable sorting under evaluator-style one-at-a-time operation.

## Test Set

- 24-brick competition set: 6 red 2x2, 6 blue 2x2, 8 blue 2x3, 4 red 2x3
- Bricks fed in the marked orientation

## Test Phases

### Phase 1: Conveyor And Handoff

Run 25 one-brick trials through the conveyor and into the chute.

Pass criteria:
- No missed chute entries
- No belt stalls
- No sensor bracket or shroud catches

### Phase 2: Servo Routing

Run 25 forced-route trials per bin.

Pass criteria:
- Correct chute position for each target
- Correct bin receives each brick
- No servo hard-stop contact
- No chute edge catch

### Phase 3: Size Sensing

Run this phase with the two break-beam pairs installed in the sensing shroud.

Trial set:
- 20 trials with 2x2 bricks
- 20 trials with 2x3 bricks
- Mixed 24-brick trial after thresholds are set

Pass criteria:
- Every 2x2 trial classifies as 2x2
- Every 2x3 trial classifies as 2x3
- Raw values or timing windows are logged
- No rule depends on perfect hand placement

### Phase 4: Color Sensing

Run 20 trials each for red and blue with the shroud installed.

Pass criteria:
- Every red trial classifies as red
- Every blue trial classifies as blue
- Calibration data records the shroud as installed
- Threshold is documented

### Phase 5: Full-Run Reliability

Run 10 full 24-brick sessions on LiPo power.

Pass criteria:
- 24 processed per run
- Correct bin totals each run
- No unrecovered ERROR state
- CSV log contains per-brick timestamps for feed, size sense, color sense, route command, route ready, and bin result if available
- Run summary reports correctness truthfully, not just total throughput

## Data Logging

Capture serial logs for every run as CSV and store in `docs/runs/`.
Use one row per brick and include enough timestamps to replay the run.

## Failure Handling

For each failed run:
- Record failure code
- Record brick number and stage
- Record corrective action
- Repeat the relevant phase after the fix
