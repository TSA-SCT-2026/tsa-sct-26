# Failure Mode Analysis V6

All failure modes, probability, detection, response, mitigation.

---

## Mechanical failure modes

### FM-01: Brick jams at stop wall

Cause: Brick exits chute crooked.
Probability: Low. 20mm channel constrains 15.8mm brick, corrects in 20-30mm of belt travel.
Detection: APPROACH_TIMEOUT.
Response: ERROR_HALT (JAM_APPROACH).
Mitigation: PTFE tape on belt channel interior. 2mm x 45-deg chamfer on chamber entry.

### FM-02: Double-feed

Cause: Exit opening too tall.
Probability: Near zero. 13.5mm opening < 22.8mm two bricks. Geometrically impossible.
Detection: DOUBLE_ENTRY event (entry beam while token = 0).
Response: ERROR_HALT (DOUBLE_ENTRY).
Mitigation: Print exit at 13.5mm. Measure with calipers.

### FM-03: Platform does not drop when lever fires

Cause: Lever tip does not fully clear the platform tab during the solenoid stroke.
Probability: Low given 3x stroke margin (30mm tip travel vs 10mm required).
Even at half solenoid stroke (4mm): tip travels 15mm, still 50% more than required.
Detection: BIN_CONFIRM_TIMEOUT (brick never arrives in bin).
Response: ERROR_HALT (MISS_BIN_N).
Mitigation: Verify full lever travel clears tab during bench test before integration.
If tab is not cleared: shorten lever arm at effort point (move effort point closer to
load, increases tip travel at the cost of requiring more solenoid force).

### FM-04: Platform does not return to level after drop

Cause: Platform return spring too weak.
Probability: Low if spring is correctly selected in bench testing.
Detection: PIN_SHELF_LEVEL (GPIO 39) does not trigger within PLATFORM_RETURN_MS.
Response: ERROR_HALT (PLATFORM_STUCK).
Mitigation: Test 50 consecutive cycles before integration. Increase spring rate if
platform fails to return in 200ms. Spring must not be so strong it resists lever re-engagement.

### FM-05: Lever tip does not re-engage platform tab after platform returns

Cause: Chamfer angle too shallow (lever tip doesn't get pushed aside by returning platform).
Or lever return spring too weak to hold tip in correct position.
Probability: Moderate if chamfer is not tuned. This is the primary iteration point.
Detection: Next brick falls through open platform (no stop switch trigger) -> APPROACH_TIMEOUT.
Response: ERROR_HALT (JAM_APPROACH).
Mitigation: Tune chamfer angle empirically. Start at 30 degrees. Decrease toward 20 if
re-engagement force is too high for the returning platform. Increase toward 45 if false
releases occur during normal operation. Budget 3 lever arm reprints for this tuning.

### FM-06: False release (lever tip pushed aside by brick weight or vibration)

Cause: Brick's weight on platform creates a moment that pushes the lever tip outward.
Or stepper vibration during indexing shakes the lever out.
Analysis: Brick weight (3g) at 20mm from hinge = 0.59 mNm. Lever return spring force
(0.1N) at 30mm from lever fulcrum = 3.0 mNm. Spring holds lever with 5x margin against
brick-weight-induced lateral force at the tab. Vibration is lower amplitude than static load.
Probability: Very low given 5x margin.
Detection: Would appear as APPROACH_TIMEOUT on next brick (platform open, brick falls through).
Mitigation: Do not use the weakest spring in the assortment. Target 0.08-0.12N return spring.
Increase spring force if false releases are observed during stepper operation.

### FM-07: Disc jams mid-rotation

Cause: Disc catches on chamber floor edge.
Detection: TMC2209 StallGuard triggers.
Response: ERROR_HALT (DISC_JAM).
Mitigation: 5mm disc-to-chamber-floor clearance. Bevel disc top edges. Tune SGTHRS.

### FM-08: Brick drops into wrong funnel

Cause: Solenoid fires before disc reaches target.
Why impossible: INDEXED state must complete before RELEASED state begins. Solenoid
does not fire until INDEXED is confirmed by step count. Not timer-based.

### FM-09: Brick bounces out of bin

Cause: Impact velocity too high. Bin walls too short.
KE = 0.0023 J. Bounce height = 20mm. Bin walls 70-110mm. Margin 50-90mm.
Probability: Near zero. Listed for completeness.

---

## Electrical failure modes

### FM-10: Solenoid MOSFET fails (shorts closed)

Cause: Back-EMF without flyback diode.
Effect: Solenoid fires continuously. Lever stays swept out. Platform stays open.
Next brick falls through. Detected as APPROACH_TIMEOUT on following brick.
Mitigation: Flyback diode non-negotiable. 1k gate resistor. Verify polarity before power-on.

### FM-11: I2C lockup

Detection: Wire.endTransmission() non-zero.
Response: ERROR_HALT (SENSOR_FAULT).
Mitigation: 4.7k pull-ups on SDA/SCL. Route away from motor leads.

### FM-12: Hall sensor no pulse

Detection: No pulse for 500ms with belt running.
Response: ERROR_HALT.

---

## Firmware failure modes

### FM-13: Step count drift

Detection: Periodic re-home every RETHOME_PERIOD_BRICKS. Count mismatch.
Response: ERROR_HALT (POSITION_DRIFT).

### FM-14: Color misclassification

Detection: Post-run count verification.
Mitigation: Calibrate with shroud. 0.15 minimum gap between clusters.

---

## Summary

| ID | Failure | Probability | Detected | Response |
|----|---------|-------------|----------|----------|
| FM-01 | Jam at stop wall | Low | APPROACH_TIMEOUT | ERROR_HALT |
| FM-02 | Double-feed | Near zero | DOUBLE_ENTRY | ERROR_HALT |
| FM-03 | Platform doesn't drop | Low (3x margin) | BIN_CONFIRM_TIMEOUT | ERROR_HALT |
| FM-04 | Platform doesn't return | Low | PLATFORM_STUCK | ERROR_HALT |
| FM-05 | Lever doesn't re-engage | Moderate (requires tuning) | APPROACH_TIMEOUT | ERROR_HALT |
| FM-06 | False release | Very low (5x margin) | APPROACH_TIMEOUT | ERROR_HALT |
| FM-07 | Disc jams | Low | StallGuard | ERROR_HALT |
| FM-08 | Wrong funnel | Impossible by design | N/A | N/A |
| FM-09 | Brick bounces out | Near zero | Visual | None |
| FM-10 | MOSFET shorts | Very low | APPROACH_TIMEOUT | ERROR_HALT |
| FM-11 | I2C lockup | Low | Wire return | ERROR_HALT |
| FM-12 | Hall no pulse | Low | Stall detection | ERROR_HALT |
| FM-13 | Step drift | Low | Re-home check | ERROR_HALT |
| FM-14 | Color misclassify | Low | Post-run count | None per-cycle |

FM-05 (lever chamfer re-engagement) is the primary fabrication risk and the one
item that requires empirical iteration. It is not a design flaw — it is a geometry
tuning problem with a clear resolution path (adjust chamfer angle, reprint lever arm).
Budget 3 prints of the lever arm before final assembly.
