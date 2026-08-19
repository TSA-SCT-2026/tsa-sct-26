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
fed one at a time, studs up, with their long axis aligned to conveyor travel.

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

after configured count: COMPLETE
on protected failure:   ERROR_HALT
```

After a startup delay, the firmware starts a run automatically. There is no
physical start-button path in the current implementation. Beam A normally
opens the sensing cycle; beam B can provide a fallback entry trigger if beam A
is missed. The event queue decouples input capture from state transitions.

## Size measurement

Both break-beams record entry and exit timestamps. The controller uses the
known center-to-center spacing and lead/trail transit times to estimate belt
speed. Each beam's blocked duration then yields a length estimate. The two
length estimates are averaged and compared with the active millimeter
threshold.

The code rejects implausible length values and tracks speed/length skew for
diagnostics. If transit timing is unavailable, it uses the configured fallback
belt speed; that value is a fallback, not a new measurement.

## Color measurement

The TCS3200 is switched through red, blue, and clear filters during a pass.
Samples with invalid pulse periods are discarded. If the operator has captured
a belt baseline, the average belt frequencies are subtracted from the pass
before classification. The remaining red and blue values produce:

```text
red_ratio = red_net / (red_net + blue_net)
```

The ratio is compared with the active color threshold. Low-signal or
implausible readings are marked uncertain and handled by the configured
best-effort path.

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

The current path is deliberately sequential: no second brick is queued while
one is being routed. The serial `in_flight` diagnostic is retained for log
compatibility and is always `0`.

Confirmation is time-based. After the estimated handoff and confirmation
windows, firmware records the commanded bin. This is not independent physical
proof that the brick landed correctly; a recorded run or per-bin sensing is
required for that claim.

## Safety and failure behavior

- Startup clamps the active-low conveyor driver off before rail-settling delays.
- Servo and stepper PWM use isolated ESP32 timer/channel allocation.
- Conveyor output is stopped on protected errors.
- Event-queue overflow, invalid routing, missed-bin signals, and double-entry
  conditions can halt the controller.
- Thresholds, pin assignments, timing constants, speed profiles, and servo
  angles are hardware-sensitive and should not change without a physical test.

## Known gaps

- Status-display functions currently log over serial rather than driving a
  connected display.
- The repository has no validated wiring image or physical acceptance log.
- TODO: confirm the final mechanical geometry and conveyor CAD provenance.
- TODO: document any verified power, ambient-light, or routing failure lessons
  from the physical build rather than reconstructing them from memory.
