# Pre-Run Checklist

## Fusion Assembly Before Shroud CAD

- Conveyor core assembled in Fusion with side plates, holders, shafts, rollers, wood block support, grounded outside motor board, NEMA17, and pulley path
- Simple belt envelope clears rollers, side plates, wood support, likely screw heads, and lower return path
- Wood block ends clear roller flange and belt-wrap envelopes by about 10mm or more
- Crossmembers, if used, fit inside the side plates and do not block drive hardware, set screw access, exit lip, or chute entry
- Low base plate and short conveyor mounting cleats or standoffs are modeled instead of duplicate full-length side panels or tall 180mm conveyor legs
- Outside motor board is grounded through a 2-stage L bracket with gussets on motor side and frame side, feet with M3 screw holes on each stage
- NEMA17 body sits outside the motor board, shaft points inward toward the conveyor, and GT2 pulleys are accessible in the rear/right service bay
- Matching 20T pulleys are coplanar in Y
- GT2 belt envelope clears motor board, side plate, bearing holders, brace hardware, bolt heads, and pulley set screws
- Motor wiring exits into the rear/right service bay without crossing belt, chute, bin, or operator-facing paths
- Drive roller hub is on the motor side
- Drive roller set screw access is visible or reachable after hand-drilling
- Motor board slots have adjustment travel for timing belt tension
- Belt exit, exit lip, chute entry, chute exit, and bin catch height are checked together before final frame cuts
- Sensing shroud is still a placeholder volume, not detailed final CAD
- Side rails are separate placeholder strips
- Removable shroud screw tabs or flanges have a plausible landing surface

## Mechanical

- Conveyor mounted, aligned, and tensioned
- Matched side-plate drilling plan verified before cutting or drilling both plates
- Belt tracks without walking into guides
- Conveyor belt and GT2 belt are not over-tensioned
- Belt splice runs over both rollers without bumping or pulling the belt sideways
- Drive roller set screw is tight against a small filed shaft flat
- Conveyor-to-chute handoff gap checked
- Servo chute sweeps to all four positions
- Servo horn and pivot screws tight
- Chute clears frame and wiring
- Bins installed and removable
- Bin labels installed
- Feed orientation cue installed
- Footprint under 610mm x 610mm

## Electrical

- LiPo connected with inline fuse installed
- Stepper VM bulk capacitor installed
- Servo power polarity verified
- Servo power rail can handle load
- Fan connected if used for driver cooling
- All grounds common and secure
- No loose wiring near moving parts
- Rear/right motor bay reserved for later ESP32, driver, power distribution, and strain relief evaluation after conveyor and chute CAD are stable

## Sensors And Control

- Break-beam size sensor pair A and pair B installed
- Size sensor reports expected values in test mode
- TCS3200 color sensor responds in the expected test mode
- Color shroud installed
- Color shroud removable with screws for adjustment, cleaning, or reprint
- First-build feed side rails verified separately from the shroud
- Servo target angles loaded
- Display state transitions visible
- Start control responds

## Functional Quick Checks

- Self-test passes
- Conveyor moves in the correct direction
- Forced route to all 4 bins succeeds
- One red and one blue sample classify correctly in shrouded test
- One 2x2 and one 2x3 sample classify correctly with break-beam timing
- Full 24-brick run instructions are present

## CAD Gate Signoff

Use this table for phase-gate acceptance evidence.

| Gate | Pass criteria summary | Date | Operator | Result (PASS or FAIL) | Evidence path |
|------|------------------------|------|----------|------------------------|---------------|
| Gate 1: chute angle | Real bricks slide at chosen angle | | | | |
| Gate 2: conveyor handoff | No missed chute entries in 25 trials | | | | |
| Gate 3: servo alignment | Clean route to all 4 bins | | | | |
| Gate 4: sensing station | No bracket catches, removable color shroud works | | | | |
| Gate 5: packaging | Under 610mm x 610mm with labels verified | | | | |

## Single-Point Failure Watchlist

| Item | Current risk | Mitigation in place | Spare or fallback ready | Verified date |
|------|--------------|---------------------|--------------------------|---------------|
| Conveyor-to-chute handoff | High | Small handoff prototype | | |
| Conveyor belt tracking | High | Matched side-plate drilling and gentle tension | | |
| Belt splice bump | Medium | Inspect over both rollers before adding shroud | | |
| Drive roller slip | Medium | M3 set screw and filed shaft flat | | |
| Chute slide angle | High | Angle coupon test | | |
| Servo pivot slop | Medium | Horn adapter check | | |
| Break-beam size timing | Medium | Two-pair shroud layout | | |
| Color shroud | Medium | Removable screw-mounted shroud and shroud-installed calibration | | |
| Frame stiffness | Medium | Wood frame with printed braces where useful | | |

## Final Readiness

- Start control labeled
- Bin labels installed
- Feed cue installed
- Operator instructions present and readable
- Full 24-brick run logs captured
