# SCT 2026: System Architecture

## What this builds

A fully automated LEGO brick sorting conveyor that classifies 24 bricks by size and color and routes them into 4 bins. Built for TSA System Control Technology nationals, May 1 2026.

Target: 24 bricks sorted in under 10 seconds, repeatedly, without operator intervention between runs.

The guiding principle: keep actuation off the critical path. Sense early, pre-set actuators while bricks are still in transit, use passive return mechanisms. Default path (no actuator fires) handles the most common brick type, minimizing duty cycle.

## Pipeline

```
[chute] -> [escapement] -> [taper channel] -> [sensing zone] -> [routing] -> [bin confirmation] -> [display]
```

### Feed: near-vertical gravity chute

Bricks stack at roughly 75 degrees in a printed rectangular channel. Gravity does the feeding. The stepper escapement at the base meters release rate precisely. No active feed mechanism is needed above the escapement. All 24 bricks load before the run starts.

The chute must maintain brick orientation (studs up, length along travel axis) through to the escapement. A round pipe cannot do this. The channel must be rectangular. This is why PVC pipe is not a viable substitute despite its smooth interior.

### Escapement: stepper cam

A stepper motor with a printed single-lobe cam disk releases exactly one brick per full rotation. Step counting makes it self-indexing. It cannot release a partial brick or double-feed. Current reduction between releases handles thermal management without additional hardware. No solenoids at the escapement means no heat accumulation at sustained 5 bricks per second.

### Taper channel

A printed channel that narrows from 30mm to 17.3mm over 60mm, positioned between the chute exit and the sensing zone. Forces bricks square before they reach sensors or plows. A severely yawed brick jams here rather than deep in the system: visible, immediately recoverable, not catastrophic.

### Sensing zone

Two sensor systems operate in the same zone:

Size detection: two IR break-beams spaced 19mm apart. A hardware timer measures the interval between beam breaks. A 2x2 brick (15.8mm) never reaches the second beam, triggering a timeout. A 2x3 brick (23.7mm) breaks both beams with a measurable gap. The length difference between bricks is only 7.9mm. Ultrasonic sensing was rejected because noise margins at close range exceed this difference. Break-beams give a binary digital result with no noise floor.

Color detection: a color sensor facing down through a printed shroud. It samples repeatedly over the full brick dwell time and averages all readings. The shroud blocks ambient light, which is the primary failure mode for this sensor. Calibration must happen with the shroud installed and the sensor's integrated LED as the only light source.

### Routing: plow diverters

Three solenoid-actuated plows with a lever arm geometry and spring return. Solenoids pre-set before bricks arrive. There is roughly 150ms of brick travel time between sensor classification and plow position, and actuation takes about 10ms. Actuation is entirely off the critical path. Servos were rejected because their sweep and return time would cut throughput to 2-3 bricks per second.

PWM hold after full extension reduces heat by roughly 60% per actuation. De-energize once the brick has cleared the plow. Passive spring return is fast and requires no firmware timing.

### Bin confirmation

An IR break-beam at each bin entrance. After routing, the controller waits up to 500ms for the expected bin beam to break. On miss: halt belt, stop escapement, de-energize all solenoids, display error state. On success: increment bin counter, proceed. End of run: verify all four bin totals against expected counts (6, 6, 4, 8).

This detects jams, solenoid misfires, bricks falling off the belt, and double-feeds. It does not detect sensor misclassification (brick routed to wrong bin but physically arrived somewhere). Misclassification is mitigated by averaging color readings and empirical threshold calibration.

### Display

Color TFT. During sort: brick silhouette animates in actual classification color, counters update live, actuator thermal state shown as a small bar. End screen: full run stats, per-bin totals vs expected, any missed confirmations. The display is a live window into internal system state. This is what judges look for, not decoration.

## Feedback loop

The feedback loop is real and bidirectional. Sensors at each bin confirm every brick individually within a hard timeout. A mismatch halts the system rather than continuing. End-of-run count verification gives a second pass. The thermal model reduces release rate under heat load rather than failing silently. Serial output timestamps every event so the engineering notebook data writes itself during calibration.

## Key risks

### Chute transition geometry: highest fabrication risk

The curve from near-vertical to horizontal at the chute base is the hardest piece to print correctly. FDM tolerances compound here. This piece must be printed and tested with real bricks before any other structural CAD work proceeds. Budget at least 3 print iterations. If bricks do not flow reliably through this transition, the entire feed concept fails and needs a design change before the frame exists. Everything else in the build schedule gates on this.

### Stepper driver wiring

If nobody on the team has wired a stepper driver before, budget a dedicated session for this before any integration work. The most common failure point is a missing or incorrectly placed bulk capacitor on the motor power input. Verify this physically before applying power.

### Calibration time

Size and color thresholds must be set empirically with all hardware installed and the shroud in place. This takes time and real bricks. The schedule has dedicated calibration windows. Do not allow earlier phases to slip into them.

### LiPo vs bench supply

The system must be tested on battery before the competition. A bench supply that current-limits during solenoid firing will cause a controller brownout. This is unrecoverable in front of judges. Test on LiPo before April 20.

## Plow routing by brick frequency

Three plows on a single belt. All bricks travel the full belt length. Each plow fires independently based on classification. Only one plow fires per brick. The default path (no plow fires) handles the most frequent brick type to minimize total solenoid actuation.

Brick frequencies:
```
2x3 blue: 8 bricks (33%) - DEFAULT, zero solenoids fire
2x2 blue: 6 bricks (25%) - plow 1
2x2 red:  6 bricks (25%) - plow 2
2x3 red:  4 bricks (17%) - plow 3
```

Total solenoid fires per run: 16 (6 + 6 + 4). Per solenoid: roughly 5 fires each, evenly distributed. Routing is a simple lookup. Classify both attributes, fire the corresponding plow or nothing.

The physical plow order along the belt is flexible since they fire independently. Place them to optimize the sensor-to-plow travel distances for pre-set timing. See MECHANICAL.md for lever geometry and EMBEDDED.md for timing details.

## Build schedule

```
March 16-18    Order everything. AliExpress first (10-15 day lead time).
               Adafruit same day. Display decision and order same day.

March 17-21    Firmware skeleton (no hardware needed).
               State machine scaffold, hardware timer handler stub,
               color sensor driver stub, display driver, stepper control stub,
               solenoid PWM hold logic.

March 17-21    CAD in parallel with firmware.
               Chute transition piece only. Nothing else.
               Stepper cam disk second (simple geometry, iterate early).
               Do not CAD the full frame until transition piece validates.

March 21-24    Chute transition iteration.
               Print, test with real bricks, no electronics needed.
               Iterate until bricks flow smoothly every time.
               This is the gate. Do not proceed until it passes.

March 28 - April 2    Parts arrive (AliExpress ordered March 18, 10-15 day window).
               Validate everything on breadboard immediately on arrival.
               IR break-beams: 2x2 vs 2x3 at 19mm spacing, multiple speeds.
               Color sensor: red vs blue under shroud LED, log raw R/G/B values.
               Belt black filter threshold: log bare belt vs brick surface totals.
               Solenoid: stroke force vs spring candidates from assortment.
               Stepper + driver: get motor spinning, verify step counting.
               Encoder disk: verify H206 pulse output at motor speed.
               Display: driver working, test animation loop.

April 2-9     Full frame CAD and print.
               Use validated chute geometry from earlier iteration.
               All channel sections, plow arms, escapement housing,
               sensor mounts, bin guides.
               Dry assembly: no wiring, verify bricks flow end to end.
               Fix print tolerance issues before wiring anything.

April 9-14     Wiring and firmware integration.
               All solenoids with flyback diodes.
               Stepper driver with 100uF cap on motor power input.
               Both power rails, verify isolation.
               Belt motor via L298N.
               All sensors: color sensor on I2C, break-beams, display on SPI.
               First end-to-end test with real bricks.

April 14-20    Calibration.
               Set size threshold empirically from break-beam data.
               Set color threshold empirically under shroud with demo bricks.
               Tune belt speed, brick spacing, stepper release timing.
               Full 24-brick runs, target 95%+ accuracy.
               Validate feedback loop halts on induced jam.
               Validate thermal model triggers correctly.
               Log everything to serial, save CSV files.

April 20-24    Reliability runs.
               10 full 24-brick runs minimum, all logged.
               Back-to-back runs to stress-test thermal management.
               Fix anything that fails more than once.
               Test on LiPo, not bench supply.

April 24-27    Documentation.
               Engineering notebook from logged run data.
               Decision matrices, equations, failure mode table.
               Calibration procedures, performance test data.

April 27-29    Buffer.
               Something will break during documentation runs.
               This time is not optional. Fix it here, not at competition.

April 29-30    Final prep.
               Full run on LiPo.
               Pack: system, laptop for serial monitor, spare bricks, charger.
               Sleep.

May 1          Competition.
```

## Subsystem decision summaries

Every major design choice (sensor type, actuator type, escapement mechanism) has a documented rationale including rejected alternatives with specific failure reasons. These are in the relevant architecture files and must appear in competition documentation. Judges score decision-making, not just final choices.

