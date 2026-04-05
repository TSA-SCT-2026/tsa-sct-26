# SCT 2026: System Architecture

## Mission

Build a fully automated LEGO brick sorter for TSA System Control Technology nationals on May 1, 2026.

Success criteria:
- Sort 24 bricks into 4 bins correctly
- Make the evaluator flow obvious: load 24 bricks, press start, read clear status, remove sorted bins
- Prioritize reliability and repeatability before chasing final cycle time
- Reach sub-10-second total sorting only through measured, state-safe optimization

If a high-level term is unclear, read `docs/project/GLOSSARY.md` before guessing from context.

## Numeric source of truth

All mechanism geometry and derived transport values used by documentation must trace back to `cad/DIMENSIONS.md`.

If another doc needs a number, either:
- reference `cad/DIMENSIONS.md` directly, or
- mark it as derived and identify the source dimensions

## Active production architecture

This repo is centered on one active architecture:
- Preloaded 24-brick compressed queue
- Long-side-across brick orientation
- One-brick isolation chamber
- Static size and color sensing in the chamber
- 4-index selector chute under the trapdoor, active for now
- Gravity release by support removal
- NEMA17 conveyor feed axis
- Off-axis toothed timing-belt stage to a supported smooth drive roller
- Event-gated restart only after physical reset truth is satisfied

Rejected upstream singulation families such as cam, Geneva, star wheel, and dual-pin are documented in the notebook alternatives.

## Core design principle

Correctness is closed by geometry and binary state, not by optimistic timing guesses.

- One brick occupies the chamber at a time because geometry prevents two
- Sensing happens only while the chamber is static
- Selector position is committed before release
- Release is support removal, then gravity
- Restart happens only after physical reset truth is satisfied

Timing matters for speed, but timing does not replace chamber truth, selector truth, bin confirmation, or platform-level confirmation.

## Brick orientation rule

Bricks are long-side-across the conveyor:
- 2x3 bricks present 23.7mm across the conveyor/channel and 15.8mm along travel
- 2x2 bricks remain 15.8mm by 15.8mm

Why:
- The chamber footprint is keyed to the long side so only one brick can occupy the trapdoor region at a time
- Chamber, chute, and sensor geometry must be re-derived from this orientation before freeze

## Terminology

Use these terms consistently:
- `selector`: the routing mechanism below the trapdoor
- `selector chute`: the active 4-index gravity-routing mechanism with four deterministic target positions
- `index position`: one of the four valid routed positions of the selector chute
- `trapdoor platform`: the chamber floor that drops when support is removed
- `chamber pitch`: effective steady-state distance from one seated brick to the next seated brick in the compressed queue
- `cold start`: first-brick feed from the initial queue rest state
- `steady state`: repeating cycle after the queue is already compressed against the chamber workflow
- `safe restart condition`: the physical truth required before the next pitch advance begins

Older repo text may say `disc`. Treat that as stale wording unless a specific circular geometry is being discussed historically.

## End-to-end pipeline

```text
[preloaded chute: 24 bricks compressed]
    -> [narrow conveyor channel]
    -> [one-brick isolation chamber]
    -> [STATIC: size beams + color sensor]
    -> [classification lock]
    -> [selector chute indexes to target]
    -> [platform support removed]
    -> [brick falls through aligned selector chute opening]
    -> [bin entry confirmation]
    -> [platform returns level]
    -> [next brick advances by chamber pitch]
```

## Throughput model

Do not model this machine as one full conveyor traverse per brick.

The active model uses chamber pitch:
- Cold-start phase: first brick travels from initial queue rest state to chamber seat
- Steady-state phase: each following brick advances only far enough to refill the chamber from the compressed queue

Top-level timing terms:
- `cold_start_feed_time`: time for the first brick to reach chamber seat
- `chamber_pitch_mm`: effective distance from one seated brick to the next seated brick in steady state
- `pitch_advance_time`: time to move the next brick from queue-ready position into the seated chamber state
- `settle_and_sense_time`: belt-off sensing window after seating
- `selector_ready_time`: time from classification lock to selector-ready confirmation
- `drop_window_time`: release pulse plus fall window until bin confirmation is expected
- `reset_confirm_time`: time from release to confirmed platform-level reset
- `safe_restart_condition`: the physical event combination required before pitch advance begins again

### Cold-start model

Cold-start is relevant for the first brick only and for recovery from large gaps.

Cold-start sequence:
1. Feed from queue rest state
2. Chamber seat confirmation
3. Static sensing
4. Selector-ready confirmation
5. Release and bin confirmation
6. Platform-level reset

### Steady-state model

Steady-state is the performance model that matters for winning.

Steady-state sequence:
1. Chamber is free and reset-confirmed
2. Next brick advances by chamber pitch
3. Chamber seat confirmation
4. Static sensing
5. Selector-ready confirmation
6. Release and bin confirmation
7. Platform-level reset
8. Next pitch advance begins when the safe restart condition is satisfied

Safe overlap is allowed only when physical truth permits it. Examples:
- Selector can start moving as soon as classification is locked
- Feed can restart only after reset truth is confirmed
- No release occurs until selector-ready is confirmed

## Conveyor architecture

The active conveyor path is:

`NEMA17 stepper -> toothed timing-belt stage -> supported roller shaft -> smooth drive roller -> neoprene belt`

Why this is the active production path:
- Ratio flexibility without replacing the motor
- Supported roller shaft instead of loading the motor shaft directly
- Easier packaging inside the 610mm x 610mm footprint
- Easier tuning of speed versus torque after real hardware arrives
- Cleaner service and replacement path

The conveyor uses an off-axis timing-belt stage to a supported smooth drive roller.

## Sensing summary

Size:
- Two IR break-beams at chamber positions defined in `cad/DIMENSIONS.md`
- Size classification depends on static beam state only

Color:
- Purchased module in the current hardware log is the TCS3200 GY-31 family
- Final interface, calibration, and thresholds must be based on received hardware
- Color calibration is valid only with the installed shroud and final geometry

Sampling constraints:
- Belt off
- Chamber seated
- No release motion active

## Routing summary

The active routing mechanism is a 4-index selector chute under the trapdoor.

The selector chute is active, not permanently frozen.

Optimization target:
- Weighted selector transition cost
- Reset latency
- Total steady-state routing cost

Do not treat “home equals rarest bin” as a sufficient speed argument. The relevant metric is weighted angular travel across the real brick distribution and real motion profile.

## Selector evidence gate

The selector chute remains active unless evidence shows it cannot support a first-place system.

Required gate study before production freeze:
- Adjacent-move time
- Worst-case move time
- Weighted-average move time across the real 24-brick distribution
- Re-home penalty
- Selector-inclusive steady-state per-brick routing cost
- Comparison against at least one downstream fixed-diverter alternative documented in the notebook

Use official motion references plus bench data:
- MOONS' NEMA11 curves: https://www.moonsindustries.com/series/nema-11-standard-hybrid-stepper-motors-b020102
- Oriental Motor speed-torque guidance: https://www.orientalmotor.com/stepper-motors/technology/speed-torque-curves-for-stepper-motors.html
- TMC2209 datasheet: https://www.analog.com/media/en/technical-documentation/data-sheets/TMC2209_datasheet_rev1.08.pdf

Required interpretation rule:
- Representative NEMA11 data suggests the motor class is not inherently too slow for a 4-position diverter
- Actual selector cost still depends on the real motor, voltage, microstep choice, selector inertia, friction, and motion profile
- If the selector-inclusive model and bench data do not credibly support sub-10-second sorting with margin, reopen downstream routing before freezing production CAD

## Control philosophy

The firmware contract must be built around physical truth events, not doc drift or guessed delays.

Required event families:
- `ENTRY_DETECTED`
- `CHAMBER_SEATED`
- `SENSING_DONE`
- `SELECTOR_READY`
- `DROP_WINDOW_DONE`
- `BIN_CONFIRMED`
- `PLATFORM_LEVEL`
- optional `CHAMBER_CLEAR`
- optional `PITCH_ADVANCE_DONE`

The conveyor restart rule is:
- do not feed the next brick until reset truth is satisfied

## Compliance coverage

| Requirement | Implementation |
|-------------|----------------|
| Two or more sensors | Size beams, color sensor, chamber seat confirm, bin confirm, optional platform-level confirm |
| Manual start and stop | Labeled start control and power control |
| Programmable controller | ESP32 |
| Feedback loop | Chamber, selector, bin, and reset truth in the control loop |
| Motorized conveyor | NEMA17 stepper conveyor with timing-belt stage |
| Automated sorting mechanism | 4-index selector chute plus trapdoor release |

## Current implementation priorities

### Gate 1: chute transition truth

Goal: validate the highest-risk geometry first.

Tasks:
- Prototype chute transition first
- Validate single-brick feed under full queue load
- Validate transition into the narrow conveyor channel

Exit criteria:
- No jams in 50 feed attempts
- No double-feed events in 50 feed attempts

### Gate 2: conveyor packaging and interface lock

Goal: lock the production conveyor path before larger assemblies drift.

Tasks:
- Package motor, timing-belt stage, supported roller shaft, idler, and tension adjustment
- Confirm service clearance, belt guard clearance, and cable-routing paths
- Confirm chamber and frame interfaces based on the packaged conveyor module

Exit criteria:
- No hard interference in assembled CAD
- Ratio, shaft support, and tension-adjust ranges documented
- Conveyor module fits the system footprint with margin
