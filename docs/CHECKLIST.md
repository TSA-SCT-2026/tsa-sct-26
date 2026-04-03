# Pre-Run Checklist

## Mechanical

- Chute transition validated with real bricks
- Platform level at rest with lever tip under tab
- Lever pivots freely with no binding
- Springs attached and seated correctly
- Selector disc rotates without edge contact

## Electrical

- LiPo connected with inline fuse installed
- Solenoid flyback diode present and polarity correct
- Stepper VM bulk capacitor installed
- All grounds common and secure
- No loose wiring near moving parts

## Sensors and control

- Home switch responds
- Stop switch responds
- Beam sensors report expected idle state
- Color sensor detected on I2C
- Display state transitions visible

## Functional quick checks

- Self-test passes
- Manual release test: one clean drop and re-latch
- Index test to all 4 bins succeeds
- Bin confirm sensors trigger as expected

## CAD gate signoff

Use this table for phase-gate acceptance evidence.

| Gate | Pass criteria summary | Date | Operator | Result (PASS or FAIL) | Evidence path |
|------|------------------------|------|----------|------------------------|---------------|
| Gate 1: trapdoor mechanism | 50 loaded drop and re-latch cycles, zero failures | | | | |
| Gate 2: chamber and sensing fit | Sensor mounts fit, no obstructed motion, no shroud light leak | | | | |
| Gate 3: selector alignment | Clean drop path for all 4 selector positions | | | | |
| Gate 4: chute transition | No jams and no double-feed events in trial set | | | | |
| Gate 5: packaging and footprint | Under 610mm x 610mm with labels and control placement verified | | | | |

## Single-point failure watchlist

Track high-risk points and readiness before full integration.

| Item | Current risk | Mitigation in place | Spare or fallback ready | Verified date |
|------|--------------|---------------------|--------------------------|---------------|
| Chute transition piece | High | Geometry validated with real bricks | | |
| Lever tip and tab interface | Medium | Chamfer and spring tuning complete | | |
| Solenoid driver path | Medium | Flyback diode and MOSFET checks complete | | |
| Selector disc hub interface | Medium | Aluminum hub mounted and anti-slip verified | | |
| Stop-wall sensing path | Medium | Switch response and seating repeatability verified | | |

## Final readiness

- Start control labeled
- Bin labels installed
- Operator instructions present and readable
