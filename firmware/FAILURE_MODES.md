# Failure Mode Analysis

## Mechanical failure modes

### FM-01: Brick jams before chamber seating

Cause: bad transition geometry or crooked entry
Detection: `FEED_TIMEOUT_MS` or `APPROACH_TIMEOUT_MS`
Response: `ERROR_HALT`
Mitigation: keep channel geometry tight, smooth contact surfaces, validate with full queue load

### FM-02: Double-entry condition

Cause: feed permission restored before platform level or chamber truth
Detection: token false while a new feed attempt starts
Response: `ERROR_HALT (DOUBLE_ENTRY)`
Mitigation: restore feed only on `PLATFORM_LEVEL`

### FM-03: Release occurs before selector-ready truth

Cause: control bug or false ready signal
Detection: wrong-bin confirm or internal state violation
Response: `ERROR_HALT`
Mitigation: release allowed only after `SELECTOR_READY`

### FM-04: Selector chute fails to reach indexed position

Cause: jam, stall, or drift
Detection: selector-ready failure or re-home mismatch
Response: `ERROR_HALT (SELECTOR_JAM or POSITION_DRIFT)`
Mitigation: deterministic home reference, periodic re-home checks, conservative motion profile

### FM-05: Platform does not return to level

Cause: spring, geometry, or latch issue
Detection: missing `PLATFORM_LEVEL` before timeout
Response: `ERROR_HALT (PLATFORM_STUCK)`
Mitigation: bench-cycle the release and re-latch path before integration

## Electrical failure modes

### FM-06: Solenoid driver failure

Cause: missing flyback protection or MOSFET fault
Detection: platform reset failure on subsequent cycle
Response: `ERROR_HALT`
Mitigation: diode, gate resistor, and pre-power verification remain non-negotiable

### FM-07: Sensor bus or optical fault

Cause: wiring issue, noise, or color sensor integration fault
Detection: `SENSING_DONE` reports `UNCERTAIN`
Response: `ERROR_HALT (SENSOR_FAULT)`
Mitigation: static sensing only, shroud installed, shielded routing where possible

## Firmware failure modes

### FM-08: Event contract drift

Cause: docs, harness, and state machine naming diverge
Detection: compile failures or harness mismatch
Response: block optimization work until interfaces agree again
Mitigation: keep `events.*`, `state_machine.*`, `test_harness.*`, and `EMBEDDED.md` synchronized

### FM-09: False performance claim from logs

Cause: count totals reported as accuracy without per-brick truth
Detection: review of CSV schema and summaries
Response: treat old summaries as invalid
Mitigation: CSV-first per-event logs, counts-match summary instead of fake 24/24 accuracy
