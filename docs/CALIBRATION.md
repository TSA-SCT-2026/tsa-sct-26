# Calibration Procedures

Calibration is valid only with final mechanical geometry installed, including the color shroud and lever trapdoor mechanism.

## Preconditions

- 3S LiPo power, charged in normal operating range
- Chamber, platform, lever, and springs installed
- Selector disc mounted on aluminum hub and homing functional
- Color shroud installed and light leaks closed

## 1. Lever release calibration

Goal: guarantee reliable tab clearance and re-latch.

Procedure:
1. Run 50 release cycles with no brick.
2. Confirm each cycle clears tab, drops platform, and re-latches.
3. Run 50 release cycles with real bricks in chamber.
4. Record failures by type: no-drop, no-return, no-re-latch.

Acceptance:
- 0 failures in 50 consecutive loaded cycles
- Return-to-level within 200ms target

If failures occur:
- Tune chamfer geometry first
- Tune return spring force second
- Re-run full loaded cycle test after each change

## 2. Color threshold calibration

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

## 3. Belt and approach timing calibration

Procedure:
1. Confirm consistent seat detect timing from entry to stop switch.
2. Tune belt target and settle window for stable static sensing.
3. Validate no bounce-induced false transitions.

Acceptance:
- No approach timeout during 24-brick runs
- No seated-state oscillation

## 4. Full-system verification

Run at least 10 full 24-brick runs and log CSV data for notebook use.
