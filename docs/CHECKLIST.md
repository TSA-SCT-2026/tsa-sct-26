# Master Build Checklist

Every phase has a gate. Do not start a phase until its gate is cleared.
Check items off as you go. This file is the source of truth for build status.

---

## Phase 0: Ordering (gate: all parts in transit before March 26)

### AliExpress cart - verify before checkout
- [ ] GT2 belt: confirm 400mm closed loop is the selected variant
- [ ] 0530 solenoid: confirm 5V variant selected (not 12V or 24V)
- [ ] NEMA 11 stepper: confirm 28mm body, 4-wire bipolar, 5mm shaft
- [ ] GT2 20-tooth pulley: confirm 5mm bore
- [ ] LM2596 buck converter: confirm buying 2 units
- [ ] TMC2209 stepper driver: confirm buying 2 units
- [ ] H206 slot optocoupler: confirm buying 2 units
- [ ] TCS34725 color sensor: confirm LED and INT pin are broken out on the board
- [ ] JGB37-520 gearmotor: confirm 6V variant, 600 RPM no-load, 6mm D-shaft (same listing sells 7-960 RPM - select carefully)
- [ ] IR break-beams (AliExpress): 4 pairs, bin confirmation only
- [ ] Torsion springs: assortment with multiple stiffness values
- [ ] Aluminum heatsinks: small enough to fit solenoid body (~20x20mm)
- [ ] Thermal adhesive tape: in cart
- [ ] PCB protoboard: in cart
- [ ] 22AWG stranded wire kit: in cart

### Adafruit - order same session as AliExpress
- [ ] IR break-beam 3mm pairs #2168: 2 packs (4 pairs total, 2 used for size detection, 2 spare)
- [ ] Display ordered: ST7789 color TFT (Pimoroni 1.3in or Waveshare 1.69in)
- [ ] Display is NOT SSD1306 (monochrome, incompatible with animation requirement)

### Amazon
- [ ] 2S LiPo 7.4V 2000mAh: 2 units ordered (Zeee, Ovonic, or Gens Ace)
- [ ] 2S balance charger ordered

### Already owned - verify quantities now
- [ ] ESP32 DevKit: present and functional
- [ ] 1N4007 or equivalent flyback diodes: 3 minimum (SR5150 Schottky is acceptable)
- [ ] 100uF 25V electrolytic capacitors: at least 5 on hand
- [ ] 1000uF 25V electrolytic capacitors: at least 2 on hand
- [ ] 4.7k resistors: at least 2 (I2C pullups if needed)
- [ ] Breadboard and jumper wires: present (bringup only, not final build)
- [ ] Heat shrink tubing: present
- [ ] PLA filament: confirmed available at school printer
- [ ] Isopropyl alcohol: present or sourced
- [ ] LEGO bricks: 24 total (6 red 2x2, 6 blue 2x2, 4 red 2x3, 8 blue 2x3)

---

## Phase 1: Firmware skeleton (March 17-21, no hardware needed)

- [x] State machine scaffold: IDLE/FEEDING/SIZE_DETECT/COLOR_DETECT/ROUTING/CONFIRM/ERROR_HALT
- [x] ERROR_HALT stops belt, stepper, all solenoids
- [x] Button triggers IDLE to FEEDING
- [x] End-of-run count verification against expected (6, 6, 4, 8)
- [x] Color sensor: N-sample averaging with black belt filter
- [x] Color ratio classifier with threshold placeholder in config.h
- [x] Thermal model: exponential decay, WARNING and DANGER thresholds, all solenoids and stepper
- [x] Bin confirmation: 500ms timeout, ERROR_HALT on miss
- [x] Serial logging: timestamp on every event, per-brick log, end-of-run summary, CSV mode
- [x] Test harness: serial command interface, `test fullrun`, `test classify`, `log csv`
- [ ] Firmware compiles clean: run `pio run` from firmware/ with zero errors
- [ ] `test fullrun` routes all 24 bricks correctly and summary matches (6, 6, 4, 8)
- [ ] Hardware timer interrupt stub for size detection (1us resolution)
- [ ] I2C clock set to 400kHz before first sensor read in init code
- [ ] Stepper: step and direction control, full rotation per release
- [ ] Stepper: reduced hold current between releases
- [ ] Solenoid: PWM drop to 40% at 20ms, de-energize at ~280ms
- [ ] Display module: ST7789 SPI driver, READY/SORTING/SORT COMPLETE/ERROR screens
- [ ] Display: brick animation triggers on ROUTING (correct color, correct aspect ratio)
- [ ] Display: bin counter flash and increment
- [ ] Display: thermal bar on sidebar
- [ ] Display: end screen with run time, per-bin totals, accuracy
- [ ] All display updates non-blocking (must not delay sensing or routing pipeline)
- [ ] GPIO interrupt priority set: size beams highest, bin confirmation lower

---

## Phase 2: CAD - chute transition (March 17-24, gate for all other CAD)

- [ ] Top-down footprint layout drawn: verify full system fits within 610mm x 610mm
- [ ] Bin positions sketched: two bins per side alongside belt, confirm clearance
- [ ] Chute transition piece CAD complete
- [ ] Chute transition piece printed (iteration 1)
- [ ] Brick flows through transition without jamming or tumbling (all 24 bricks, 3 passes)
- [ ] Brick orientation maintained (studs up, length along travel axis) through transition
- [ ] Both 2x2 and 2x3 bricks tested through transition
- [ ] Gate cleared: chute transition reliable before any other structural CAD

### Only after gate cleared:
- [ ] Stepper cam disk CAD complete (simple geometry, early iteration)
- [ ] Cam disk printed and tested: pushes brick cleanly, holds queue during rest of rotation
- [ ] Color sensor shroud CAD complete (integral with channel roof section)
- [ ] Shroud printed: seal test with handheld flashlight, ratio must not shift under ambient light

---

## Phase 3: Parts arrival validation (March 28 - April 2)

Do every one of these on breadboard before any integration. Log everything.

### Power rails
- [ ] Both LM2596 outputs measured at exactly 5.0V before connecting any load
- [ ] Rail 1 (solenoids) and Rail 2 (logic) confirmed isolated except common ground
- [ ] 100uF cap placed adjacent to TMC2209 motor power input, orientation verified
- [ ] 1000uF cap placed on logic rail 5V close to ESP32, orientation verified

### Stepper
- [ ] TMC2209 wired in standalone mode (no UART): MS1 and MS2 both LOW for full step
- [ ] EN pulled LOW (TMC2209 enable is active-low, unlike A4988 SLEEP which is active-high)
- [ ] Stepper spins at commanded rate without skipping steps
- [ ] Step counting verified: 200 steps = 1 full rotation

### Belt motor
- [ ] JGB37-520 runs at target speed under load (time a brick over 100mm, calculate mm/s)
- [ ] Direction correct: belt surface moves away from chute
- [ ] L298N PWM tested at 1kHz, 5kHz, 10kHz: pick frequency, log result
- [ ] Actual belt speed at operating PWM measured and recorded

### IR break-beams - size detection (Adafruit pairs)
- [ ] Both beams operational: emitter powered, receiver gives clean HIGH/LOW
- [ ] 2x2 brick at 19mm spacing: second beam never breaks (confirm timeout classification)
- [ ] 2x3 brick at 19mm spacing: second beam breaks within expected window
- [ ] Test at multiple speeds, log timing data

### IR break-beams - bin confirmation (AliExpress pairs)
- [ ] All 4 pairs powered and giving clean digital output
- [ ] Brick passing through breaks beam reliably, no phantom triggers

### Color sensor
- [ ] I2C communication confirmed at 400kHz
- [ ] Shroud installed before any reading is logged
- [ ] Red 2x2 brick: log raw R/G/B values and ratio for 10 passes
- [ ] Blue 2x2 brick: log raw R/G/B values and ratio for 10 passes
- [ ] Red 2x3 brick: log raw R/G/B values and ratio for 10 passes
- [ ] Blue 2x3 brick: log raw R/G/B values and ratio for 10 passes
- [ ] Bare belt: log raw total to set black belt filter threshold
- [ ] Clear separation visible between red and blue ratios
- [ ] Threshold value set in config.h from empirical data

### Solenoids
- [ ] All 3 flyback diodes installed, orientation verified with multimeter (before any energization)
- [ ] Each solenoid energizes and extends stroke on command
- [ ] PWM hold at 40% maintains extension
- [ ] Spring selected: heaviest spring each solenoid can reliably overcome
- [ ] Heatsinks attached to all 3 solenoid bodies with thermal adhesive tape

### Belt encoder
- [ ] H206 mounted next to slotted pulley disk
- [ ] Pulse output confirmed on oscilloscope or serial monitor at belt speed
- [ ] Pulse count per rotation matches slot count on disk
- [ ] ESP32 hardware pulse counter reading speed correctly

### Display
- [ ] SPI communication confirmed
- [ ] READY screen renders correctly
- [ ] Brick animation runs at correct color and aspect ratio
- [ ] No screen update blocks a state transition (verified by timing serial log)

---

## Phase 4: Full frame CAD and print (April 2-9)

- [ ] All channel sections CAD complete using validated chute transition geometry
- [ ] Plow lever arms: pivot-to-solenoid 8mm, pivot-to-tip 24mm, ratio 3.0
- [ ] Plow leading face chamfered at 35 degrees
- [ ] Solenoid connection to lever: pinned (M2 pin), not hook or slot
- [ ] Sensor mounts: color sensor at 5-10mm standoff above belt, break-beams at 19mm spacing
- [ ] Bin guides printed with downward ramp geometry (no foam needed)
- [ ] Chute entrance opening: narrow enough vertically to prevent brick-on-side entry
- [ ] Chute entrance guide: printed indication for studs-up orientation
- [ ] Belt tensioner slot: 5mm of adjustment range
- [ ] All parts printed
- [ ] Dry assembly: no wiring, walk a brick by hand from chute to belt end
- [ ] All 24 bricks pass end-to-end in dry assembly without snagging
- [ ] All print tolerance issues fixed before wiring begins
- [ ] Start button labeled (printed or engraved text)
- [ ] Bin labels on each bin: "2x2 RED", "2x2 BLUE", "2x3 RED", "2x3 BLUE"

---

## Phase 5: Wiring and integration (April 9-14)

Complete the wiring checklist from ELECTRICAL.md verbatim before first power-on:
- [ ] 100uF cap adjacent to TMC2209 motor power input, correctly oriented
- [ ] 1000uF cap on logic rail 5V supply, close to ESP32
- [ ] All 3 solenoid flyback diodes installed, orientation verified with multimeter
- [ ] Rail 1 and Rail 2 separated, common ground only
- [ ] Both LM2596 outputs measured at 5V before connecting anything
- [ ] Stepper EN pulled LOW (active-low on TMC2209)
- [ ] MS1 and MS2 both LOW (full step mode)
- [ ] ESP32 powered from clean Rail 2, not motor rail
- [ ] All I2C devices on same bus: shared SDA, SCL, and ground
- [ ] Display SPI connections correct: MOSI, SCLK, CS, DC, RST
- [ ] Photointerrupter output on GPIO interrupt pin
- [ ] All solenoid and sensor wiring soldered to perfboard (no breadboard jumpers in final build)
- [ ] Cable routing clean: no wires crossing belt path or plow travel zone

### First powered test
- [ ] Power on with no motor or solenoid commands: verify no smoke, no heat
- [ ] Belt motor spins in correct direction at low PWM
- [ ] Stepper rotates one full revolution on command
- [ ] All 3 solenoids actuate and return on command
- [ ] Color sensor reads over I2C
- [ ] All 6 break-beam receivers give correct HIGH/LOW
- [ ] Display renders READY screen
- [ ] First end-to-end brick test with real brick

---

## Phase 6: Calibration (April 14-20)

- [ ] Exact sensor-to-bin distances measured in assembled system and logged
- [ ] Solenoid de-energize timing updated from 280ms estimate to calculated value
- [ ] Bin confirmation timeout updated from 500ms estimate to calculated value
- [ ] Size threshold validated empirically at actual belt speed, logged, set in config.h
- [ ] Color threshold validated empirically under shroud, logged, set in config.h
- [ ] Belt speed tuned: run bricks at 100, 150, 200, 250mm/s, log accuracy at each
- [ ] Operational belt speed set to fastest speed holding 95%+ accuracy
- [ ] PI controller gains tuned, documented
- [ ] Thermal model thresholds validated: WARNING and DANGER trigger at correct heat levels
- [ ] Feedback loop validated: induced jam causes ERROR_HALT within 500ms
- [ ] All calibration data saved to CSV in docs/runs/
- [ ] Threshold and timing values in config.h match empirical results (no guessed values)

---

## Phase 7: Reliability runs (April 20-24)

- [ ] All runs on LiPo, not bench supply
- [ ] 10 full 24-brick runs completed, all logged to CSV
- [ ] Accuracy 95%+ across all runs
- [ ] Back-to-back runs: thermal model triggers correctly, system does not overheat
- [ ] LiPo voltage logged start and end: confirm capacity covers full judging session
- [ ] Reload drill: 24 bricks from bins back to chute in under 30 seconds, practiced 3 times
- [ ] Error recovery procedure practiced: jam, halt, clear, reset, reload - looks deliberate
- [ ] Zero hardware failures across reliability run set
- [ ] Any failure that occurred more than once: root cause found and fixed

---

## Phase 8: Documentation (April 24-27)

- [ ] Engineering notebook covers full project timeline with dates
- [ ] Decision matrices in docs/engineering/decision_matrices/ for all major choices:
  - Escapement type (cam vs other)
  - Sensor type for size detection (break-beam vs ultrasonic)
  - Sensor type for color (TCS34725 vs alternatives)
  - Actuator type (solenoid vs servo)
  - Motor type (JGB37-520 vs TT gearmotor and other alternatives)
- [ ] Each matrix includes: alternatives considered, criteria, selection rationale
- [ ] Calibration procedures documented in docs/engineering/calibration/
- [ ] Performance test data in docs/runs/ with analysis
- [ ] Accuracy rate documented with run logs as evidence
- [ ] Thermal model equations documented
- [ ] Sizing and timing calculations documented (belt speed, brick spacing, dwell time)
- [ ] Operator instructions written for a stranger: how to load, start, interpret display, reset

---

## Phase 9: Buffer and final prep (April 27-30)

- [ ] Full run on LiPo passes with no issues
- [ ] AC outlet availability at judging table confirmed (determines LiPo vs bench supply at competition)
- [ ] Pack list verified:
  - [ ] System
  - [ ] Laptop with serial monitor configured
  - [ ] 2x LiPo (both charged)
  - [ ] Balance charger
  - [ ] 24 LEGO bricks (correct counts: 6 red 2x2, 6 blue 2x2, 4 red 2x3, 8 blue 2x3)
  - [ ] Spare display (if budget allows)
  - [ ] Spare flyback diodes
  - [ ] Small screwdriver set
  - [ ] IPA wipes
- [ ] Someone other than you operates the system from written instructions only and succeeds
- [ ] Sleep.
