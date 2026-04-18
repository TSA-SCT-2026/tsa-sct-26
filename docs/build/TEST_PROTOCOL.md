# Test Protocol

## Objective

Demonstrate reliable sorting under evaluator-style one-at-a-time operation.

## Test Set

- 24-brick competition set: 6 red 2x2, 6 blue 2x2, 8 blue 2x3, 4 red 2x3
- Bricks fed in the marked orientation

## Test Phases

### Phase 0: Conveyor Core Fusion Check

Run this phase before detailed sensing shroud CAD.

Pass criteria:
- Conveyor core is assembled in Fusion with side plates, holders, shafts, rollers, wood block support, grounded outside motor board, motor, pulley path, and simple belt envelope
- Conveyor placement is checked with assembly relationships, belt envelopes, and interference checks, not fixed global coordinate placement
- Wood block support is set from belt surface and roller flange clearance
- Any crossmember used fits inside the side plates without forcing the frame wider or blocking drive hardware
- Low base plate and short conveyor mounting cleats or standoffs are modeled instead of duplicate full-length side panels or tall 180mm conveyor legs
- Outside motor board is grounded through a 2-stage L bracket with gussets on motor side and frame side
- NEMA17 body sits outside the motor board, shaft points inward toward the conveyor, and GT2 pulleys remain accessible in the rear/right service bay
- Matching 20T pulleys are coplanar in Y
- GT2 belt envelope clears motor board, side plate, bearing holders, brace hardware, bolt heads, and pulley set screws
- Motor board does not visibly flex when timing belt tension is applied by hand
- Motor wiring exits into the rear/right service bay without crossing belt, chute, bin, or operator-facing paths
- Belt exit, exit lip, chute entry, chute exit, and bin catch height are checked as one stack
- Placeholder shroud volume clears the belt and side plates
- Side rails are still separate from the shroud
- Removable shroud mounting tabs or flanges have a plausible screw location
- Drive roller set screw access remains reachable
- Motor board slots and idler holder adjustment remain serviceable
- Rear/right service bay space is reserved for later electronics placement without locking ESP32 or driver location yet

### Phase 1: Conveyor And Handoff

Run 25 one-brick trials through the conveyor and into the chute.

Pass criteria:
- No missed chute entries
- No belt stalls
- No belt walking, splice bump, or roller slip that affects brick travel
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
