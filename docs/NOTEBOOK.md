# Engineering Notebook

Inventor's Log is 20 of 100 points. Evaluated by judges without the team present - it must stand alone. Every entry pattern: what question, what was measured/calculated, what decision it produced, what validated it.

---

## Required sections (checklist)

### Design decisions (derive from equations, not intuition)

- [ ] Size sensor selection: IR break-beam vs ultrasonic vs ToF vs camera
  - Key equation: noise floor of each option vs brick length difference (7.9mm)
  - Include measurement specs for each sensor type considered

- [ ] Color sensor selection: TCS34725 vs camera vs discrete photodiodes
  - Key equation: dwell time at operating speed vs minimum integration time required per sensor type

- [ ] Plow actuation: solenoid vs servo vs paddle wheel vs pop-up gate
  - Key equation: actuation time needed vs inter-brick window at target throughput (140ms at 5 bricks/s)

- [ ] Escapement mechanism: stepper cam vs dual-pin solenoid vs Geneva drive vs rotary servo
  - Key constraint: sustained duty cycle at 5 bricks/s over multiple consecutive runs
  - Thermal analysis for each option

### Equations (write out with all variables and constants labeled)

- [ ] Lever geometry: L1, L2, ratio, solenoid stroke, tip travel derivation
- [ ] Belt sizing: pulley pitch diameter, center-to-center distance, total belt length
- [ ] Brick spacing: maximum brick length + buffer -> center-to-center gap -> inter-brick window
- [ ] Dwell time: belt speed, brick width, samples per brick
- [ ] Size detection timing: beam spacing, belt speed, expected gap for 2x3, timeout for 2x2
- [ ] Solenoid on-time: brick length + face plate width, belt speed -> on-time calculation (40ms nominal)
- [ ] Thermal model: heat accumulation rate, exponential decay constant, threshold derivations
- [ ] Power budget: all components, peak simultaneous draw, LiPo capacity vs run count
- [ ] Throughput: stepper cycle time, belt speed, brick spacing -> bricks per second

### Charts (every major parameter gets a chart)

- [ ] Accuracy vs belt speed (from calibration sweep data)
- [ ] Speed vs time (PI controller behavior during a run)
- [ ] Color ratio distribution: red and blue brick readings plotted, threshold shown
- [ ] Thermal state over time during back-to-back runs
- [ ] Per-bin accuracy over 10+ runs (bar chart, expected vs actual)

### Calibration records

- [ ] Color sensor: raw R/G/B readings for both brick colors under shroud LED, belt running
- [ ] Color sensor: floor threshold for belt surface black filter (bare belt total light reading)
- [ ] Size detection: actual beam-to-beam timing measured for 2x3 bricks at operating speed
- [ ] Size detection: confirmed 2x2 never reaches beam 2 (confirmed timeout behavior)
- [ ] Belt speed: actual speed at target PWM duty cycle vs calculated target (200mm/s)
- [ ] Solenoid spring: final spring weight selected, measured retraction time

### Performance data (minimum 10 runs, all logged)

- [ ] Run log table: run number, date, belt speed, time, per-bin totals, accuracy, notes
- [ ] Longest consecutive correct run count
- [ ] Any failures: what failed, what run number, was it repeatable
- [ ] Back-to-back demo simulation (3+ runs in sequence, thermal state logged)

### Failure mode analysis

- [ ] Table: failure type, probability, how detected, system response
- [ ] Undetected failure type: sensor misclassification (routed to wrong bin but physically arrived)
  - Explain why this is acceptable and how it is mitigated (averaging, threshold calibration)

### Feedback loop documentation

- [ ] Diagram: where sensors are, what they detect, what the controller does with each signal
- [ ] Distinguish between real-time feedback (bin confirmation halts on miss) and end-of-run verification (count check)
- [ ] PI controller: block diagram showing input (speed setpoint), measurement, error, output (PWM)
- [ ] Thermal model: explain as a feedback loop (heat accumulates, system responds, loop closes on temperature)

---

## What data the serial output provides automatically

If serial logging is implemented correctly during calibration, the following are generated without manual recording:
- Per-brick log -> accuracy calculation
- Timestamps -> run time, transit times, measured belt speed
- Thermal state -> thermal model validation
- Bin confirmation results -> failure mode analysis data

Do not manually record any of this. Pipe serial to CSV and analyze from the file.

---

## Timeline for notebook completion

| Section | Data available | Target completion |
|---------|---------------|-------------------|
| Design decisions | Now (documented in architecture files) | April 22 |
| Equations | Now (in architecture files) | April 22 |
| Calibration records | After April 14 integration | April 20 |
| Performance charts | After April 20 reliability runs | April 24 |
| Run log table | After April 20 reliability runs | April 24 |
| Failure mode analysis | After calibration | April 25 |
| Feedback loop documentation | After integration | April 25 |
| Charts | After all data collected | April 26 |
