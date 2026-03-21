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

**PASS:** 2x3 brick blocks the beam (firmware reads: is_large = true) 10/10 times.
2x2 brick clears the beam (firmware reads: is_large = false) 10/10 times.

**FAIL:** Any incorrect read. Any inconsistent read (some block, some clear) with the
same brick type. Any read that requires repositioning the brick within normal tolerances.

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

**Setup:** Belt running with no bricks. Hall sensor or motor PWM feedback active.

**PASS:** Belt holds 200mm/s +/-10% (180-220mm/s) for 30 seconds at full duty.
No belt slipping on pulleys. No belt jumping teeth.

**FAIL:** Speed drifts outside 180-220mm/s range. Belt slips or jumps during operation.

**Note:** If no speed sensor: verify target speed by measuring time for a mark on the
belt to travel a known distance (10 measurements, compute mean).

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
