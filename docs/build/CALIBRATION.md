# Calibration Procedures

Calibration is valid only with final mechanical geometry installed, including the color shroud, release mechanism, selector chute, and timing-belt conveyor stage.

## Preconditions

- 3S LiPo power, charged in normal operating range
- Chamber and release mechanism installed
- Timing-belt stage aligned and tensioned
- Selector chute mounted on its final indexer hardware and homing functional
- Color shroud installed and light leaks closed
- Display, start control, and bin labels installed

## 1. Conveyor pitch and queue cadence calibration

Goal: confirm the steady-state feed advance that supports the compressed queue.

Procedure:
1. Load a full 24-brick queue.
2. Run repeated seat and reset cycles while logging entry, seated, selector-ready, release, reset-confirmed, and next-entry times.
3. Measure the distance the belt advances between one brick clearing the chamber and the next brick reaching the stop wall.
4. Compare the measured advance to the chamber pitch and the safe restart condition.

Acceptance:
- No double-entry during 24-brick runs
- Chamber seats repeatably at the same stop wall position
- Restart distance stays within the measured chamber pitch window across repeated runs
- No seated-state oscillation

If failures occur:
- Tune belt ratio or pulley size first
- Tune approach speed second
- Recheck stop wall contact and chamber alignment after each change

## 2. Release mechanism calibration

Goal: guarantee reliable support removal and safe reset.

Procedure:
1. Run 50 release cycles with no brick.
2. Confirm each cycle removes support and returns to safe state.
3. Run 50 release cycles with real bricks in chamber.
4. Record failures by type: no-drop, incomplete release, no-reset.

Acceptance:
- 0 failures in 50 consecutive loaded cycles
- Return to safe state within 200ms target

If failures occur:
- Tune release geometry first
- Tune return bias second
- Re-run full loaded cycle test after each change

## 3. Color threshold calibration

Procedure:
1. Place seated brick in chamber with belt off.
2. Collect repeated sample windows for red and blue bricks.
3. Compute normalized red ratio clusters.
4. Set threshold with safety margin between clusters.

Acceptance:
- Minimum 0.15 separation between calibrated cluster centers
- Classification stability across repeated trials

Calibration evidence template:

| Trial set | Lighting condition | Shroud installed (yes or no) | Red mean ratio | Blue mean ratio | Separation | Threshold chosen | Operator | Date |
|-----------|--------------------|-------------------------------|----------------|-----------------|------------|------------------|----------|------|
| | | | | | | | | |

## 4. Selector and reset calibration

Procedure:
1. Home the selector chute and verify the selector home micro-switch repeatability.
2. Index to each bin position and measure adjacent move time and worst-case move time.
3. Confirm the selector-ready event occurs before release is allowed.
4. Confirm the reset event occurs before the next feed is allowed.

Acceptance:
- Selector moves cleanly to all four positions
- Re-home penalty is repeatable and within the modeled allowance
- Release does not start before selector-ready
- Next feed does not start before reset confirmation

## 5. Full-system verification

Run at least 10 full 24-brick runs and log CSV data for notebook use.
