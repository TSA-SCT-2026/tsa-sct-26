# Embedded Design

## Terminology

- `selector chute`: the 4-index routing mechanism under the chamber. It can be rotary or another deterministic indexed mechanism.
- `home reference`: the known selector position used to recover truth and detect drift.
- `selector ready`: the target indexed position is reached and safe for release.
- `drop window`: the bounded release and fall interval before bin confirmation.
- `platform level`: the reset truth that restores feed permission for the next queued brick.
- `chamber pitch`: the queued advance distance needed to bring the next brick into the chamber-ready region.

## State machine

```
IDLE -> FEED -> APPROACH -> SEATED -> SENSING -> INDEXED -> RELEASED -> CONFIRM -> RESET -> FEED
                                                                                    |
                                                                             COMPLETE (after brick 24)
                                                          ERROR_HALT from any state on fault
```

## Physical-truth event contract

Required events:
- `START_BUTTON`
- `ENTRY_DETECTED`
- `CHAMBER_SEATED`
- `SENSING_DONE`
- `SELECTOR_READY`
- `DROP_WINDOW_DONE`
- `BIN_CONFIRMED`
- `PLATFORM_LEVEL`
- `RESET`

Optional events reserved for future instrumentation:
- `CHAMBER_CLEAR`
- `PITCH_ADVANCE_DONE`
- `ENCODER_PULSE`

## State definitions

**IDLE**
- Token true
- Platform level confirmed
- Selector chute homed to reference position
- Display: READY

**FEED**
- Conveyor feed axis advances the queued brick stream toward the chamber
- Wait for `ENTRY_DETECTED`
- Timeout: `FEED_TIMEOUT_MS` -> `ERROR_HALT (JAM_CHUTE)`

**APPROACH**
- Entry detected, chamber is about to seat
- Wait for `CHAMBER_SEATED`
- Timeout: `APPROACH_TIMEOUT_MS` -> `ERROR_HALT (JAM_APPROACH)`

**SEATED**
- Belt off
- Token false
- Wait `SETTLE_MS`
- Then transition to `SENSING`

**SENSING**
- Brick is static in the chamber
- Wait for `SENSING_DONE`
- If category is `UNCERTAIN`: `ERROR_HALT (SENSOR_FAULT)`
- Map category to target bin and selector position

**INDEXED**
- Command selector chute to target indexed position
- Wait for `SELECTOR_READY`
- If selector reports failure: `ERROR_HALT (SELECTOR_JAM)`

**RELEASED**
- Release pulse fires
- Wait for the timed `DROP_WINDOW_DONE` event

**CONFIRM**
- Wait for `BIN_CONFIRMED`
- Wrong bin or timeout: `ERROR_HALT (MISS_BIN)`

**RESET**
- Wait for `PLATFORM_LEVEL`
- Timeout: `PLATFORM_LEVEL_TIMEOUT_MS` -> `ERROR_HALT (PLATFORM_STUCK)`

**COMPLETE**
- All 24 bricks processed
- Count totals checked against expected bin counts

## Control philosophy

- Firmware depends on deterministic indexed positions, not a specific selector geometry
- The selector can be a circular disc or a near-vertical chute if it still provides 4 repeatable positions and selector-ready truth
- Feed timing is modeled around queued chamber pitch, not full conveyor length per brick
- Classification, routing, release, confirm, and reset remain event-gated

## Config focus

`config.h` now groups:
- conveyor kinematics and pitch-advance assumptions
- chamber dwell and timeout windows
- selector indexing constants
- thermal throttling targets

The conveyor model is parameterized around:
- motor steps per rev
- microsteps
- stage ratio
- effective roller travel
- chamber pitch advance
- approach slow-zone behavior

## Validation target

Before performance tuning, firmware should satisfy these source-level conditions:
- event names, docs, and code match
- selector terminology is geometry-agnostic
- no pusher-era or disc-only control assumptions remain in active interfaces
- CSV logging is usable for notebook evidence
