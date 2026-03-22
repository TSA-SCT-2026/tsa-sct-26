# Acceptance Test Protocol

Pass/fail criteria for each subsystem. Every subsystem must pass its individual
test before integration. Full system test is the final gate before competition.

All tests run on LiPo, not bench supply.

---

## Escapement

**Setup:** Load 5 bricks in chute. Run stepper at target speed (3 rev/sec).

**PASS:** 20 consecutive single releases. No double-release. No brick jam lasting
more than 500ms. Cam rotates smoothly, no step skipping (verify via StallGuard UART read).

**FAIL:** Any double-release. Any jam requiring manual intervention. Any missed release
(cam rotates but brick does not fall to belt).

**Threshold tuning:** If double-releases occur, increase lobe arc by 5 degrees and reprint.
If jams occur, reduce lobe arc or increase chute clearance.

---

## Size Sensing

**Setup:** Hold each brick type at the cam chord position with the escapement stopped.
Test at multiple positions within the brick's range of play (0 to 11.2mm for 2x2,
0 to 3.3mm for 2x3) to verify position independence.

**PASS:** 2x3 brick: both beams blocked (firmware reads: is_large = true) 10/10 times
at every tested position. 2x2 brick: both beams simultaneously blocked never occurs
(firmware reads: is_large = false) 10/10 times at every tested position.

**FAIL:** Any incorrect read. Both beams simultaneously blocked for a 2x2 brick at any
position. Either beam failing to block for a 2x3 brick at any position.

---

## Color Sensing

**Setup:** Follow docs/CALIBRATION.md procedure first. Thresholds must be set before
running this test.

**PASS:** 20 readings per brick type, all classified correctly. Minimum 6 valid samples
per brick (sample_count >= 6). No CAT_UNCERTAIN results.

**FAIL:** Any misclassification. Any UNCERTAIN result. Fewer than 6 valid samples
(indicates sensor, shroud, or LED issue).

---

## Belt

**Pre-test (day-1 friction check):** Before wiring anything, wrap neoprene strip around
one printed roller and pull one end with ~2N force. Must not slide on bare PLA. If slides:
add heat-shrink sleeve to drive roller. Gate for all subsequent assembly.

**Setup:** Belt running with no bricks. Hall sensor active (GPIO 4). Log speed to serial
every 100ms for 60 seconds.

**PASS:** Mean speed within 5% of BELT_TARGET_SPEED. Std dev < 5% of mean. No belt walking
off rollers during 60-second run. No neoprene splice bump detectible in speed trace.

**FAIL:** Speed drifts outside 5% window or shows high variance. Belt walks off rollers
(fix: increase idler crown, check flange clearance). Splice creates visible speed spike.

**Wall clearance check:** Place brick in channel, slide by hand full length. No catching
at wall joints, slot edges, slot-to-wall transitions. Chamfer any sharp edges found.

---

## Pushers (test each independently)

**Setup:** Place one brick at each pusher position manually. Fire each pusher via serial
command: `test pusher 1`, `test pusher 2`, `test pusher 3`.

**PASS:** Brick fully exits the channel and lands in the correct bin opening, 20 consecutive
fires. Spring return completes within 100ms of solenoid de-energizing (verify by re-fire
timing). No brick stuck at channel slot edge.

**FAIL:** Any brick that does not fully exit. Any spring return failure. Any brick that
catches on the slot edge (chamfer the edge if this occurs).

---

## Bin Confirmation Beams

**Setup:** Manually push bricks into each bin from the correct pusher position.

**PASS:** Beam breaks within 500ms of brick entry for 20 consecutive bricks per bin.

**FAIL:** Any timeout. Any false trigger (beam breaks with no brick).

---

## Full System

**Setup:** Load 24 bricks in the competition mix (6 red 2x2, 6 blue 2x2, 8 blue 2x3,
4 red 2x3). Labels on chute guide. All components installed. 3S LiPo connected and charged.

**PASS:**
- 24/24 bricks in correct bins
- All bin confirmation beams triggered in correct sequence
- No ERROR_HALT during run
- Run completes in under 10 seconds
- Display shows SORT COMPLETE with correct per-bin counts
- 10 consecutive runs passing all above criteria

**FAIL:**
- Any single misroute
- Any ERROR_HALT on a correctly loaded run
- Run time exceeds 10 seconds (acceptable for reliability testing; optimize only after 24/24 is stable)

---

## Competition readiness checklist

Before packing for competition, the full system test must have passed 10/10 runs on the
same day. Run all 10 consecutively without resetting the thermal model. If any run fails,
fix the issue and restart the 10-run count.

Log all runs to `docs/runs/` with date and run number.
