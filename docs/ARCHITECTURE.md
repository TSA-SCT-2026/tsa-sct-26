# SCT 2026: System Architecture

## Mission

Build a fully automated LEGO brick sorter for TSA System Control Technology nationals on May 1, 2026.

Success criteria:
- Sort 24 bricks into 4 bins correctly
- Prioritize reliability and repeatability first
- Reach under 10 seconds only after 24/24 correctness is stable
- Make operation obvious for a first-time evaluator using only written instructions

## Current phase

Current phase is CAD completion before other work.

Scope in this phase:
- Lock core mechanical geometry
- Print and validate the highest risk mechanism pieces
- Freeze assembly interfaces before wiring and firmware integration

Out of scope in this phase:
- Throughput tuning
- Firmware optimization
- Final wiring harness finish work

## Core design principle

Every correctness guarantee is enforced by geometry and binary state, not timing.

- One brick occupies the isolation chamber at a time: physical geometry prevents two
- Sensing only happens while the chamber is static
- Chute selector position is committed before release
- Release is support removal, then gravity

The belt carries transport only. Classification and routing correctness do not depend on belt timing.

Brick orientation rule:
- Bricks are widthwise across the conveyor: 15.8mm width across the belt channel, brick length along travel
- Why: fixed across-channel footprint limits yaw and keeps size detection dependent on along-travel length at fixed beam positions

## End-to-end pipeline

```
[feed chute: 24 bricks loaded]
    -> [narrow belt: 20mm channel, 100-120mm transport]
    -> [isolation chamber: stop wall + micro-switch confirm]
    -> [STATIC: dual IR size beams + TCS34725 color]
    -> [classification lock]
    -> [stepper indexes 4-position chute selector]
    -> [solenoid actuates class 3 lever]
    -> [lever clears platform tab]
    -> [platform drops under gravity]
    -> [brick falls through selector opening into target bin]
    -> [bin entry beam confirms arrival]
    -> [platform and lever return]
    -> [token restored, belt feeds next brick]
```

## Mechanism summary

The chamber floor is a hinged platform. A class 3 lever tip supports a tab on the platform far edge.

- Solenoid applies effort near the fulcrum
- Lever tip sweeps outward and clears the tab
- Platform loses support and drops under gravity
- Brick falls through the selector opening
- Springs return platform and lever to latched state

Design intent:
- Solenoid does not carry platform weight
- Actuation work is primarily spring preload
- Re-latch is geometric with chamfered lead-in

## Sensing summary

Size:
- Two IR break-beams at X=5mm and X=21mm
- Both blocked means 2x3
- Any other valid pattern means 2x2

Color:
- TCS34725 through a 12mm x 12mm chamber window
- Black PLA shroud installed during all calibration and runs
- Use normalized red ratio `R/(R+G+B)` with static sampling window

Sampling constraints:
- Belt must be off
- Chamber seated switch must confirm brick presence

## Chute selector summary

- NEMA 11 stepper with TMC2209
- 100mm selector disc with 4 openings at 90-degree intervals
- Disc mounted to 5mm aluminum shaft hub, not printed bore
- Periodic re-home cadence to catch drift

Rule:
- Selector indexing completes before trapdoor release

## Bin mapping

| Bin | Direction | Category | Count | Selector angle |
|-----|-----------|----------|-------|----------------|
| 1 | NW | 2x2 red | 6 | 315 deg |
| 2 | NE | 2x2 blue | 6 | 45 deg |
| 3 | SE | 2x3 blue | 8 | 135 deg |
| 4 | SW default | 2x3 red | 4 | 225 deg |

Default bin is category 2x3 red because it is the rarest bucket.

## Compliance coverage

| Requirement | Implementation |
|-------------|----------------|
| Two or more sensors | Dual IR size beams, color sensor, chamber micro-switch, bin beams |
| Manual start and stop | Momentary control button |
| Programmable controller | ESP32 DevKit |
| Feedback loop | Chamber and bin confirmations with halt on miss |
| Motorized conveyor | JGB37-520 belt drive |
| Automated sorting mechanism | Stepper selector plus trapdoor release |

## Timing budget

Conservative phase estimate:

| Phase | Duration |
|-------|----------|
| Belt transport | 1200ms |
| Settle and sensor read | 247ms |
| Selector index worst case | 2000ms |
| Drop and return | 408ms |
| Confirm and restart | 70ms |
| Total per brick | about 3925ms |
| Total for 24 bricks | about 94s |

Optimization belongs to a later phase after reliability is proven.

## Locked decisions

- Gravity trapdoor with class 3 lever
- Lever tip sweeps outward away from belt
- Static-only sensing
- Routing pre-commit before release
- Occupancy token as source of truth
- 5mm aluminum hub interface for selector disc
- Default bin is the rarest category

## Dedicated TODO: CAD phase only

Status date: April 3, 2026

### A. Gate 1: isolate and validate trapdoor mechanism

Goal: prove repeatable drop and re-latch before printing larger assemblies.

Tasks:
- Print platform, hinge bracket, lever arm, lever pivot bracket, and solenoid bracket
- Assemble and verify free motion at hinge and lever pivots
- Validate lever-tip under-tab engagement at rest
- Validate full tab clearance on solenoid actuation
- Validate spring-assisted re-latch via chamfer geometry
- Run 50-cycle reliability test with real bricks

Exit criteria:
- 0 failed drops in 50 cycles
- 0 failed re-latches in 50 cycles
- Return-to-level under 200ms

### B. Gate 2: chamber and sensing geometry lock

Goal: lock interfaces before any broad part print run.

Tasks:
- Freeze chamber wall geometry and stop-wall switch mounting
- Freeze dual beam hole placements at X=5mm and X=21mm
- Freeze color window and shroud mount geometry
- Confirm sensor hardware clearances with fast fit prints

Exit criteria:
- Sensors mount without forcing
- Brick motion is unobstructed
- No light leak paths around shroud interface

### C. Gate 3: selector disc and drop alignment

Goal: guarantee clean gravitational path from platform to bins.

Tasks:
- Print selector disc prototype with target opening geometry
- Validate disc-to-platform concentric alignment
- Validate drop path to each bin quadrant
- Tune funnel edges if any corner contact appears

Exit criteria:
- Brick clears drop path in all 4 selector positions
- No edge catch across at least 25 drops per bin

### D. Gate 4: chute transition and belt interface

Goal: de-risk highest-risk geometry before full mechanical print set.

Tasks:
- Prototype chute transition piece first and test with real bricks
- Validate exit height and transition smoothness into belt channel
- Validate single-brick feed behavior at target clearances

Exit criteria:
- No double-feed events in 50-feed trial
- No jam at transition under full-height stack load

### E. Gate 5: mechanical packaging and footprint check

Goal: guarantee rules compliance and operator clarity.

Tasks:
- Confirm full assembly envelope stays within 610mm x 610mm
- Place and verify bin labels and orientation cues in CAD
- Confirm cable routing channels and anchor points
- Confirm clear mounting surface for labeled start control

Exit criteria:
- Footprint hard limit met with margin documented
- All operator-facing labels physically placeable and visible

## Secondary TODOs after CAD lock

These are not active until CAD gates are complete:
- Firmware CSV event logging for replay integration
- Simulation compare mode for expected versus replayed timing
- Optional simulation import from `firmware/src/config.h`

## Open questions

- OQ-05: Selector funnel surface finish. Test plain PLA first. Add PTFE tape only if catch appears.
- OQ-06: Belt transport length final value in 100-120mm window.
- OQ-07: Lever chamfer angle tuning. Start at 30 degrees and validate empirically.

## Document map

Primary references:
- Mechanical design: `cad/MECHANICAL.md`
- Critical dimensions: `cad/DIMENSIONS.md`
- Electrical design: `wiring/ELECTRICAL.md`
- Firmware architecture: `firmware/EMBEDDED.md`
- Parts list: `docs/BOM.md`
- Calibration: `docs/CALIBRATION.md`
- Test protocol: `docs/TEST_PROTOCOL.md`
- Build checklist: `docs/CHECKLIST.md`
- Notebook guide: `docs/notebook/README.md`
- Competition info: `docs/COMPETITION_INFO.md`
