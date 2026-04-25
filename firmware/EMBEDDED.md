# Embedded Design

## Terminology

- `sensing station`: the conveyor-mounted sensor area before the belt exit
- `servo chute`: the MG995/MG996-class rotary chute that points at one of four bins
- `route ready`: the target servo angle has been reached or the current stub has accepted the route command
- `handoff window`: the bounded time where the brick leaves the belt and enters the chute
- `manual feed`: one brick at a time from the evaluator or tester
- `timed confirmation`: states-build confirmation after the handoff window completes

## State Machine

```text
IDLE -> FEED -> SENSING -> ROUTING -> HANDOFF -> CONFIRM -> FEED
                                                               |
                                                        COMPLETE after brick 24
                                             ERROR_HALT from any state on fault
```

## Physical Event Contract

Required events:
- `START_BUTTON`
- `BRICK_DETECTED`
- `SENSING_DONE`
- `ROUTE_READY`
- `HANDOFF_DONE`
- `RESET`

Optional future events reserved for tighter instrumentation:
- `ENCODER_PULSE`
- `SIZE_SENSOR_READY`
- `COLOR_SENSOR_READY`
- `BIN_CONFIRMED`

## State Definitions

**IDLE**
- Display: READY
- Wait for `START_BUTTON`

**FEED**
- Conveyor moves the manually placed brick through the sensing station
- Wait for `BRICK_DETECTED`
- Timeout: `FEED_TIMEOUT_MS` gives `ERROR_HALT`

**SENSING**
- Firmware reads the current size and color result
- If category is `UNCERTAIN`: `ERROR_HALT`
- Map category to target bin and servo angle

**ROUTING**
- Command servo chute to target bin angle
- Wait for `ROUTE_READY`
- If servo route fails: `ERROR_HALT`

**HANDOFF**
- Conveyor continues long enough for the brick to leave the belt and enter the chute
- Wait for `HANDOFF_DONE`

**CONFIRM**
- States build: auto-confirm after the timed handoff window
- Future instrumented mode may wait for `BIN_CONFIRMED`
- Wrong bin detection is deferred unless optional bin beams are added later

**COMPLETE**
- All 24 bricks processed
- Count totals checked against expected bin counts

## Control Philosophy

- Keep state names and run logs aligned with the states machine
- Keep size sensing aligned with two-pair break-beam timing in the shrouded sensing station
- Keep the color sensor shroud as a calibration requirement
- Keep the servo chute deterministic with four recorded target angles
- Skip bin confirmation beams for states unless the base sorter is already reliable
- Do not add feed automation until manual one-at-a-time sorting is reliable

## Config Focus

`config.h` groups:
- conveyor timing and timeout assumptions
- sensing sample counts and thresholds
- servo target angles
- expected bin counts
- display and serial settings

## Validation Target

Before performance tuning, firmware should satisfy these source-level conditions:
- event names, docs, and code match
- test harness can simulate a full 24-brick run
- CSV logging includes per-brick classification, route, handoff, and bin result
- no chamber, release-gate, or NEMA11 selector assumptions remain in active interfaces
