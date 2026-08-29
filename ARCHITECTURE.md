# Architecture

The sorter is a sequential, event-driven ESP32 system. It accepts one brick,
measures it inside a shrouded station, positions a rotary chute, and waits for
the timed handoff before accepting the next brick.

## Physical path

```text
feed guide
  -> GT2 conveyor
  -> beam A
  -> TCS3200 color sampling region
  -> beam B
  -> belt exit
  -> servo rotary chute
  -> four bins
```

The configured event set contains red and blue 2x2 and 2x3 bricks. Bricks are
fed studs up, with their long axis aligned to conveyor travel. The current
firmware owns one brick record at a time; physical spacing must prevent a second
brick from entering the sensing window before the controller returns to `FEED`.

## Firmware layers

| Area | Responsibility |
| --- | --- |
| `main.cpp` | Safe startup pin states, subsystem initialization, event dispatch |
| `sensors.cpp` | Interrupt-backed beam timing, color sampling, classification, calibration state |
| `state_machine.cpp` | Sequential feed, sense, route, handoff, confirm, complete, and error states |
| `actuators.cpp` | Stepper PWM, conveyor speed profiles, servo positioning, buzzer, status logging |
| `events.cpp` | Fixed-capacity event queue shared by hardware and the serial harness |
| `logger.cpp` | Human-readable and CSV serial evidence |
| `test_harness.cpp` | Serial commands for calibration, component checks, and simulated events |
| `tuning_profile.h` | ESP32 NVS persistence for operator-selected calibration values |

## State flow

```text
IDLE -> FEED -> SENSING -> ROUTING -> HANDOFF -> CONFIRM
          ^                                      |
          |                                      v
          +------------- next brick --------- FEED

COMPLETE is declared but is not reached automatically in the current path
on protected failure: ERROR_HALT
```

After a startup delay, the firmware starts a run automatically. There is no
physical start-button path in the current implementation. Beam A normally
opens the sensing cycle; beam B can provide a fallback entry trigger if beam A
is missed. Interrupt service routines retain the edge timestamps while the
64-entry event queue decouples input capture from state transitions.

## Size measurement

Let the active center-to-center beam spacing be $d$ (40.436 mm by default),
and define entry/exit timestamps from the active-low beam transitions. The
implemented estimates are:

$$
\Delta t_{lead}=t_{B,in}-t_{A,in},\quad
\Delta t_{trail}=t_{B,out}-t_{A,out}
$$

$$
v_{lead}=\frac{d}{\Delta t_{lead}},\quad
v_{trail}=\frac{d}{\Delta t_{trail}},\quad
v_{avg}=\frac{v_{lead}+v_{trail}}{2}
$$

When only a positive leading-edge delta is available, `v_avg` uses `v_lead`.
If no dynamic estimate is available, length calculation uses the configured
230 mm/s fallback. Beam occlusion durations
$\tau_A=t_{A,out}-t_{A,in}$ and
$\tau_B=t_{B,out}-t_{B,in}$ produce:

$$
L_A=v\tau_A,\qquad L_B=v\tau_B
$$

With both valid measurements, firmware averages the lengths when Beam A has at
most one extra edge, Beam B has at most two, and both speed and length skew are
at most 40%. Otherwise it prefers the beam with fewer extra transitions; on an
equal-edge high-skew result it conservatively chooses the larger length. A
single valid occlusion can supply a provisional length, but size classification
is marked valid only when Beam B entry and exit timing are complete. This keeps
the millimeter threshold responsive to measured belt motion while rejecting an
incomplete two-beam pass as a confident result.

## Color measurement

The TCS3200 is switched through red, blue, and clear filters during a pass. Each
filter frequency is derived from up to four valid pulse periods. During beam
transit the controller attempts a color sample every 70 ms; invalid pulse
periods are discarded. If the operator has captured a belt baseline, its
average red, blue, and clear frequencies are subtracted channel-by-channel and
clamped at zero before the sample is retained. The kept net channels are then
averaged across the pass and produce:

```text
red_ratio = red_net / (red_net + blue_net)
```

At least three belt-separated pass samples are required for a normal result.
Low-signal samples are rejected; if too few remain, the highest-signal rejected
sample is retained as an explicitly best-effort reading. The ratio is compared
with the operator-selected threshold. Current comments record repeated moving
known-brick clusters near 0.467 for blue and 0.512 for red, with a configured
0.490 threshold; those are calibration references, not an accuracy claim.

## Operator-assisted calibration

The software does not derive new thresholds automatically. An operator:

1. captures a belt baseline,
2. records representative red/blue and 2x2/2x3 passes,
3. chooses separating color and size thresholds,
4. enters those values over serial, and
5. saves the profile to NVS.

The saved profile contains the color threshold, size threshold, beam spacing,
and averaged belt readings. Defaults in `config.h` remain the recovery path.

## Routing and confirmation

The category-to-bin mapping selects one of four calibrated servo angles. The
controller estimates travel from the sensing station to the chute and tries to
position the chute before the brick commits to the exit. Conveyor speed can be
reduced when the available routing window is tight.

The current path is deliberately sequential: `_token` guards one active
`BrickRecord`, and the next record is created only after timed confirmation.
Events that do not match the current state are ignored; therefore the firmware
does not safely track a second physical brick entering the sensing window while
the first is routing. The serial `in_flight` diagnostic is retained for log
compatibility and is always `0`.

Confirmation is time-based. After the estimated handoff and confirmation
windows, firmware records the commanded bin. This is not independent physical
proof that the brick landed correctly; a recorded run or per-bin sensing is
required for that claim.

## Active protection and failure behavior

- Startup clamps the active-low conveyor driver off before rail-settling delays.
- Servo and stepper PWM use isolated ESP32 timer/channel allocation.
- Event-queue overflow, an invalid servo route, or an internal token re-entry
  violation stops the conveyor, holds the selector, sounds the buzzer, logs the
  error, and enters `ERROR_HALT` until reset.
- Beam capture has a 3.5-second deadline. Incomplete timing becomes an uncertain
  result rather than an infinite wait; the state machine marks any forced route
  as low-confidence/guessed.
- Extra beam transitions plus speed/length skew determine whether two length
  estimates are averaged, one beam is preferred, or the larger estimate is
  used.
- Thresholds, pin assignments, timing constants, speed profiles, and servo
  angles are hardware-sensitive and should not change without a physical test.

Declared error names for feed timeout, sensor fault, position drift, and missed
bin remain in the interface, but the current physical path does not emit all of
them. Feed timeout presently logs and continues waiting, sensing retry is
compiled with a limit of zero, and no independent bin beam is installed.

## Known gaps

- Status-display functions currently log over serial rather than driving a
  connected display.
- Timed confirmation records the commanded bin; it is not physical landing
  verification.
- `S_COMPLETE` exists, but the current confirmation path starts another brick
  instead of automatically ending after `TOTAL_BRICKS`; runs require operator
  control to stop at the intended set size.
- Low-confidence `chooseBestGuessBin()` routing currently maps 2x3 red to bin 4
  and 2x2 blue to bin 2; the normal confident category mapping is correct.
- The repository has pre-build wiring sketches, but no validated as-built wiring
  diagram or physical acceptance log.
- Final mechanical geometry and conveyor CAD provenance remain reconstruction
  checks rather than verified as-built measurements.
- Power, ambient-light, and routing failure lessons should only be documented
  from verified physical evidence rather than reconstructed from memory.
