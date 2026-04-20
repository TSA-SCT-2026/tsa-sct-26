# Pre-Run Checklist

## Mechanical

- Conveyor mounted, aligned, and tensioned.
- Belt tracks without walking into guides.
- Conveyor belt and GT2 belt are not over-tensioned.
- Belt splice runs over both rollers without bumping or pulling the belt sideways.
- Drive hardware remains accessible for adjustment.
- Conveyor-to-chute handoff checked with real bricks.
- Servo chute sweeps to all four positions.
- Servo horn and pivot screws tight.
- Chute clears frame and wiring.
- Bins installed and removable.
- Bin labels installed.
- Feed orientation cue installed.
- Footprint under 610mm x 610mm.
- No fixed-height feet, exit lip, or wood cuts depend on unresolved bin geometry.

## Electrical

- LiPo connected with inline fuse installed.
- Stepper VM bulk capacitor installed.
- Servo power polarity verified.
- Servo power rail can handle load.
- Fan connected if used for driver cooling.
- All grounds common and secure.
- No loose wiring near moving parts.
- Electronics placement does not conflict with final conveyor, chute, or bin geometry.

## Sensors And Control

- Break-beam size sensor pair A and pair B installed.
- Size sensor reports expected values in test mode.
- TCS3200 color sensor responds in the expected test mode.
- Color shroud installed.
- Color shroud removable with screws for adjustment, cleaning, or reprint.
- First-build feed side rails verified separately from the shroud.
- Servo target angles loaded from calibration.
- Display state transitions visible.
- Start control responds.

## Functional Quick Checks

- Self-test passes.
- Conveyor moves in the correct direction.
- Forced route to all 4 bins succeeds.
- One red and one blue sample classify correctly in shrouded test.
- One 2x2 and one 2x3 sample classify correctly with break-beam timing.
- Full 24-brick run instructions are present.

## CAD Gate Signoff

Use this table for phase-gate acceptance evidence.

| Gate | Pass criteria summary | Date | Operator | Result (PASS or FAIL) | Evidence path |
|------|------------------------|------|----------|------------------------|---------------|
| Gate 1: chute slide | Real bricks slide with UHMW lining | | | | |
| Gate 2: conveyor handoff | No missed chute entries in 25 trials | | | | |
| Gate 3: servo alignment | Clean route to all 4 bins | | | | |
| Gate 4: sensing station | No bracket catches, removable color shroud works | | | | |
| Gate 5: packaging | Under 610mm x 610mm with labels verified | | | | |

## Single-Point Failure Watchlist

| Item | Current risk | Mitigation in place | Spare or fallback ready | Verified date |
|------|--------------|---------------------|--------------------------|---------------|
| Conveyor-to-chute handoff | High | Small handoff prototype | | |
| Conveyor belt tracking | High | Matched alignment and gentle tension | | |
| Belt splice bump | Medium | Inspect over both rollers before adding shroud | | |
| Servo chute alignment | High | Wide funnels and calibrated positions | | |
| Color calibration drift | Medium | Shroud-installed calibration only | | |
