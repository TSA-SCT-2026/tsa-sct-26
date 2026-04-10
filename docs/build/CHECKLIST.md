# Pre-Run Checklist

## Mechanical

- Chute transition validated with real bricks
- Timing-belt stage mounted, aligned, tensioned, and guarded
- Supported drive roller spins freely with no belt walk
- Conveyor pitch advance reaches the chamber without overshoot or slip
- Release mechanism at rest in a safe state
- Release path returns cleanly without binding
- Return elements or reset elements attached and seated correctly
- Selector chute indexes without edge contact

## Electrical

- LiPo connected with inline fuse installed
- Release actuator protection present and polarity correct if coil-based
- Stepper VM bulk capacitor installed
- All grounds common and secure
- No loose wiring near moving parts

## Sensors and control

- Home switch responds
- Stop switch responds
- Chamber clear condition is detectable
- Dual ToF sensors report expected idle and seated ranges
- TCS3200 color sensor output responds in the expected test mode
- Selector-ready event appears before release
- Next-feed restart waits for physical reset confirmation
- Display state transitions visible

## Functional quick checks

- Self-test passes
- Queue-fed start with 24 bricks loads and begins from READY
- Manual release test: one clean drop and safe reset
- Reset-to-next-seat cycle repeats without double-entry
- Index test to all 4 bins succeeds
- Bin confirm sensors trigger as expected

## CAD gate signoff

Use this table for phase-gate acceptance evidence.

| Gate | Pass criteria summary | Date | Operator | Result (PASS or FAIL) | Evidence path |
|------|------------------------|------|----------|------------------------|---------------|
| Gate 1: release mechanism | 50 loaded drop and reset cycles, zero failures | | | | |
| Gate 2: chamber and sensing fit | Sensor mounts fit, no obstructed motion, no shroud light leak | | | | |
| Gate 3: selector alignment | Clean drop path for all 4 selector positions | | | | |
| Gate 4: conveyor packaging and pitch | Timing-belt stage aligned, tensioned, and repeatable | | | | |
| Gate 5: chute transition | No jams and no double-feed events in trial set | | | | |
| Gate 6: packaging and footprint | Under 610mm x 610mm with labels and control placement verified | | | | |

## Single-point failure watchlist

Track high-risk points and readiness before full integration.

| Item | Current risk | Mitigation in place | Spare or fallback ready | Verified date |
|------|--------------|---------------------|--------------------------|---------------|
| Chute transition piece | High | Geometry validated with real bricks | | |
| Release interface and reset path | Medium | Geometry and reset path complete | | |
| Release actuator driver path | Medium | Protection and drive checks complete | | |
| Selector chute indexer interface | Medium | Mount hardware locked and anti-slip verified | | |
| Conveyor timing-belt stage | Medium | Pulley ratio, tension, and guarded packaging verified | | |
| Stop-wall sensing path | Medium | Switch response and seating repeatability verified | | |

## Final readiness

- Start control labeled
- Bin labels installed
- Operator instructions present and readable
