# Embedded Design Architecture

## State machine

```
IDLE -> FEEDING -> SIZE_DETECT -> COLOR_DETECT -> ROUTING -> CONFIRM -> FEEDING
                                                                  |
                                                             ERROR_HALT
```

- IDLE: ready screen on display, button waiting, thermal model cooling down between runs
- FEEDING: stepper fires one full rotation, one brick released onto belt
- SIZE_DETECT: hardware timer running, waiting for second beam break or timeout
- COLOR_DETECT: color sensor averaging samples over brick dwell time
- ROUTING: correct plow fires (or does not fire for default path), display animation triggers
- CONFIRM: waiting for the expected bin beam to break within 500ms
- ERROR_HALT: belt stops, stepper stops, all solenoids de-energize, display shows error with bin and brick number, system waits for reset

Transitions happen on hardware interrupt or timer expiry, not polling loops. The only exception is color sampling during COLOR_DETECT, which samples on a tight loop during the brick's dwell window.

## Size detection

Two IR break-beam pairs in the sensing zone, 19mm apart along the belt axis.

Use hardware timer interrupts -- not software polling, not delay-based measurement. The timer must have at least 1 microsecond resolution. This gives thousands of counts of margin on the 21ms signal window, making misclassification effectively impossible from timing alone.

```
Beam 1 breaks:                timer starts
Beam 2 breaks within timeout: brick is 2x3 (23.7mm long)
Beam 2 times out:             brick is 2x2 (15.8mm long)
```

How the geometry works:
- Beams are 19mm apart. Belt moves at 200mm/s.
- A 2x2 brick is 15.8mm long -- shorter than the 19mm beam gap. Its trailing edge clears beam 1 before its leading edge reaches beam 2. Beam 2 never breaks.
- A 2x3 brick is 23.7mm long -- longer than the gap. Its leading edge reaches beam 2 while the brick is still in beam 1. Time from beam 1 break to beam 2 break: 19mm / 200mm/s = 95ms.
- Timeout should be set above 95ms (use ~150ms). The classification threshold sits anywhere between 0 and 95ms -- 50ms is a reasonable starting point. Validate empirically at actual belt speed during calibration. The margin is large enough that the exact value does not matter much.

## Color detection

Color sensor faces down from the channel roof through a printed shroud, 5-10mm above the belt surface.

I2C bus must be explicitly configured to 400kHz. The default 100kHz is insufficient for the polling rate needed. Set this in initialization code before any sensor communication.

```
Brick dwell time at 200mm/s over 15.8mm: 79ms
Minimum integration time per sample: 2.4ms
Maximum samples per brick: 79 / 2.4 = ~32 samples
```

Average all samples taken during the dwell window. Do not rely on a single read. Classification is based on the ratio of red channel to total light (R divided by R+G+B). Red bricks will score well above 0.45 on this ratio; blue bricks will score well below it. The exact threshold must be calibrated empirically with the shroud installed and demo bricks on the actual belt. Do not assume 0.45 is the right value.

## Routing logic

Routing is a direct lookup from classification result to plow assignment. Optimized by brick frequency so the most common brick type fires no solenoid.

```
2x3 blue (8/24): no solenoid fires -- default path, brick reaches end bin
2x2 blue (6/24): plow 1 fires
2x2 red  (6/24): plow 2 fires
2x3 red  (4/24): plow 3 fires
```

One plow fires per brick, maximum. Total solenoid fires per full run: 16.

Solenoid fires immediately when classification completes. The brick is still traveling from the sensing zone to the first plow -- roughly 150ms of travel time at 200mm/s over 30mm. Solenoid actuation takes 10ms. The plow is pre-set before the brick arrives.

PWM hold timing per solenoid:
- Full power for the first 20ms (full extension reached)
- Drop to ~40% PWM duty cycle (maintains position, reduced heat)
- De-energize at approximately 280ms after trigger (brick has cleared the plow)

This is implemented as a timer-based state transition in firmware. The de-energize time can be calculated at the start of each routing event.

## Bin confirmation

Four break-beams, one at each bin entrance. After a brick is routed, start a 500ms countdown for the expected bin beam.

On beam break within window: log success, increment bin counter, trigger display update, transition back to FEEDING.
On timeout: transition to ERROR_HALT.

GPIO interrupt priority: size detection beams are highest priority. Bin confirmation beams are lower priority. Do not allow a bin confirmation interrupt to delay a size detection measurement.

## Thermal model

Runs continuously. Tracks accumulated heat for each actuator independently. Heat increases when an actuator fires. Heat decays exponentially over time when idle.

```
On actuator fire:    heat += duration * heat_per_unit_time
Each update cycle:   heat *= cooling_factor ^ elapsed_seconds
```

Two thresholds:
- WARNING: reduce stepper release rate slightly (increase gap between bricks)
- DANGER: reduce stepper release rate significantly

The model protects hardware during back-to-back demo runs, not just a single run. Between runs, while in IDLE, the model continues to update and heat values decay. The display shows the thermal state as a small bar throughout.

Apply the model to all solenoids and the stepper motor. The belt motor does not need it.

## Display

Color TFT over SPI. All display updates happen asynchronously relative to the sensing and routing pipeline -- they must not block or delay state transitions.

During sort (triggered on ROUTING state entry, not on physical bin arrival):
- Brick silhouette animates down the chute graphic in the correct classification color (red or blue) and correct aspect ratio (2x2 or 2x3)
- Bin counter for the target bin flashes then increments
- Sidebar shows brick number, size, color, destination bin
- Thermal bar updates

Render bricks as small pre-rendered bitmaps stored in flash memory. Clear the background rectangle, redraw the bitmap -- no full-screen refresh needed. Animation should complete within 150ms to avoid queuing at 5 bricks per second.

RGB565 color format: red is 0xF800, blue is 0x001F. These are exact matches for LEGO red and blue under normal conditions.

End screen (triggered when all 24 bricks confirmed):
- SORT COMPLETE header
- Total run time
- Per-bin totals vs expected (6, 6, 4, 8)
- Count of missed confirmations if any
- Green indicator if all counts match, yellow if discrepancy

## Serial output

Log every event with a millisecond timestamp:
- Each brick: number, size classification, color classification, destination bin
- Each bin confirmation: timestamp, which bin, success or timeout
- End of run: all totals, total time
- Thermal model state periodically

Serial output is the engineering notebook. Pipe it to a laptop during calibration and save to CSV. Performance test data, belt speed validation (compare expected vs measured transit times), and accuracy rates all come from this log. Do not manually record runs.

The time between ROUTING timestamp and CONFIRM timestamp gives measured transit time, which divided into the sensor-to-bin distance gives measured belt speed. This is a sanity check on the belt speed calculation. Log it. Judges appreciate a system that validates its own performance claims.

## Button and start/stop

Momentary button, wired to a GPIO with internal pullup. Press starts a run from IDLE. The competition rules require a manual start/stop -- do not auto-run on power-up.

Optional: a second button for slow demo mode (half belt speed, exaggerated animation timing). Costs nothing in firmware. Useful during judging explanations.

## Firmware checklist

State machine:
- [ ] All states and transitions implemented
- [ ] ERROR_HALT stops belt, stepper, and all solenoids
- [ ] Manual button triggers IDLE -> FEEDING
- [ ] End-of-run count verification against expected (6, 6, 4, 8)

Sensing:
- [ ] Hardware timer interrupt for size detection (1us or better resolution)
- [ ] I2C clock explicitly set to 400kHz before first sensor read
- [ ] Color sensor: N-sample averaging over dwell window
- [ ] Color ratio classifier with empirical threshold
- [ ] Size threshold validated empirically during calibration
- [ ] GPIO interrupt priority: size beams > bin confirmation beams

Actuation:
- [ ] Stepper: step and direction control, full rotation per release
- [ ] Stepper: reduced hold current between releases
- [ ] Solenoid: PWM drop to 40% duty cycle after 20ms full extension
- [ ] Solenoid: de-energize at calculated brick-cleared time (~280ms)
- [ ] Solenoid: all flyback diodes confirmed in hardware before first energization
- [ ] Bin confirmation: 500ms timeout, ERROR_HALT on miss

Thermal:
- [ ] Exponential decay model for all solenoids and stepper
- [ ] WARNING threshold: reduce release rate
- [ ] DANGER threshold: reduce release rate further
- [ ] Thermal state displayed on screen

Display:
- [ ] Color TFT driver initialized over SPI
- [ ] Ready screen on IDLE entry
- [ ] Brick animation: correct color, correct aspect ratio, triggers on ROUTING
- [ ] Bin counter flash and increment
- [ ] Thermal bar on sidebar
- [ ] End screen: run time, per-bin totals, accuracy

Logging:
- [ ] Serial timestamp on every significant event
- [ ] Per-brick log: number, size, color, bin, confirmation result
- [ ] End-of-run summary log
- [ ] Belt speed validation from transit time logs
