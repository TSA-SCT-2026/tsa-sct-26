# Failure Mode Analysis

## Mechanical Failure Modes

### FM-01: Brick misses chute entry

Cause: belt exit gap, chute entry height, or servo position error
Detection: missing or wrong `BIN_CONFIRMED`
Response: `ERROR_HALT`
Mitigation: validate conveyor-to-chute handoff with real bricks before large prints

### FM-02: Brick sticks in chute

Cause: chute angle too shallow, rough surface, or undersized channel
Detection: missing `BIN_CONFIRMED`
Response: `ERROR_HALT`
Mitigation: test 30, 35, 40, and 45 degree chute angles with real bricks

### FM-03: Servo chute fails to reach target

Cause: power sag, horn slop, hard-stop contact, or bad angle table
Detection: route-ready failure or wrong-bin result
Response: `ERROR_HALT`
Mitigation: calibrate four servo angles and verify power rail under load

### FM-04: Belt walking or stall

Cause: tension, roller alignment, or belt rubbing
Detection: brick fails to reach sensing station or chute handoff
Response: `ERROR_HALT`
Mitigation: low-speed conveyor check before full runs

## Electrical Failure Modes

### FM-05: Servo power sag

Cause: servo current draw exceeds the regulator or wiring capacity
Detection: route undershoot, reset, jitter, or serial brownout evidence
Response: stop tuning speed and fix power path
Mitigation: use a power rail appropriate for the MG995/MG996-class servo and share ground with logic

### FM-06: Sensor optical fault

Cause: wiring issue, noise, shroud leak, or unresolved size-sensor geometry
Detection: `SENSING_DONE` reports `UNCERTAIN`
Response: `ERROR_HALT`
Mitigation: shroud-installed color calibration and documented size-sensor thresholds

## Firmware Failure Modes

### FM-07: Event contract drift

Cause: docs, harness, and state machine naming diverge
Detection: compile failures or harness mismatch
Response: block optimization work until interfaces agree again
Mitigation: keep `events.*`, `state_machine.*`, `test_harness.*`, and `EMBEDDED.md` synchronized

### FM-08: False performance claim from logs

Cause: count totals reported as accuracy without per-brick truth
Detection: review of CSV schema and summaries
Response: treat old summaries as invalid
Mitigation: CSV-first per-event logs, counts-match summary instead of fake 24/24 accuracy
