# SCT 2026: System Architecture

## What this builds

A fully automated LEGO brick sorting conveyor that classifies 24 bricks by size and color
and routes them into 4 labeled bins. Built for TSA System Control Technology nationals,
May 1 2026. The evaluator loads all 24 bricks, presses start, and does nothing else.
Target: 24 bricks sorted correctly in under 10 seconds, every time.

The guiding principle: classify early, before the brick ever touches the belt. Sensing
happens while the brick is stationary in the chute. The belt is pure transport.

## Scoring

100 points total: 60 solution, 20 inventor's log, 20 programming structure.
Reliability beats speed. 24/24 at 14 seconds beats 23/24 at 9 seconds. Speed only
matters as a tiebreaker when reliability is equal.

## Pipeline

```
[chute] -> [escapement + sensing] -> [belt] -> [pushers] -> [bins]
```

All 24 bricks load into the chute before the run starts. The stepper escapement
releases one brick per cam rotation. While each brick sits stationary on the cam chord,
both size and color are read. The brick is classified before it touches the belt. The belt
carries it to the correct pusher position, which fires at a pre-calculated delay.

## Sensing (in chute, brick stationary)

**Size:** Two IR break-beams cross the 22mm chute interior at X=5mm and X=21mm from the
reference wall (measured along the 27mm dimension). Beam spacing 16mm exceeds the 2x2
brick width (15.8mm), so a 2x2 can never span both beams simultaneously. A 2x3 brick
(23.7mm) always spans both at any position within the chute. Detection logic: both beams
blocked = 2x3. Anything else = 2x2. Binary result, position-independent, no timing
measurement, no speed dependency.

**Color:** TCS34725 mounted on the chute wall exterior, looking through a 12mm x 12mm
window. A black PLA shroud blocks ambient light. The sensor's onboard LED is the only
light source. At 24ms integration time, the cam dwell window (~260ms) yields 10+ samples
from a stationary brick. Classification uses R/(R+G+B) ratio averaged over all samples.

Both reads complete before the cam releases the brick. The belt speed has zero effect
on classification quality.

## Belt

Flat neoprene belt (19mm wide, 3/4") on smooth printed rollers. Motor D-shaft presses
directly into drive roller bore (rigid coupling, zero slip at that joint). Neoprene strip
(~700mm loop) wraps both rollers via friction. Hall sensor + PI firmware corrects speed drift.
Belt runs 0.5-1mm below the channel wall bottom edges; walls rest on frame rails.

Active transport length: 290mm. Belt speed: 200mm/s nominal. Target is locked empirically
starting at 150mm/s; Hall PID holds it there during the run.

Pusher positions from chute exit:
- Pusher 1 (2x2 red): 75mm - fires at 375ms after chute exit
- Pusher 2 (2x2 blue): 150mm - fires at 750ms after chute exit
- Pusher 3 (2x3 blue): 225mm - fires at 1125ms after chute exit
- Belt end (2x3 red, DEFAULT): 290mm - no pusher fires

## Bin assignment

| Bin | Category | Count | Pusher | Rationale |
|-----|----------|-------|--------|-----------|
| 1 | 2x2 red | 6 | Solenoid 1 | |
| 2 | 2x2 blue | 6 | Solenoid 2 | |
| 3 | 2x3 blue | 8 | Solenoid 3 | |
| 4 | 2x3 red | 4 | DEFAULT | Rarest category as default. Any classifier failure lands in the lowest-count bin, making contamination visible. Minimizes total misroute count if the classifier degrades. |

Total pusher fires per run: 20 (6+6+8). Default path fires 0 times.

## Key decisions locked

Do not revisit without a strong reason. Rationale for each is in the subsystem docs.

- **Stepper escapement:** NEMA 11 + TMC2209 + single-lobe cam. Self-indexing. Step
  counting provides exact brick position. StallGuard detects jams passively.
- **Sensing in chute:** eliminates sensing timing entirely, decouples belt speed from
  classification quality, eliminates belt-mounted sensor bridges. Dual size beams
  (X=5 and X=21mm) give position-independent detection regardless of where the brick
  lands on the cam chord.
- **Direct solenoid pusher:** no lever arm, no pivot. Face plate on rod, spring return.
  Solenoid on-time 40ms, spring returns fast. Lower part count than lever design.
- **Flat neoprene belt + Hall PID:** Motor D-shaft presses into drive roller (zero slip
  at coupling). Neoprene strip wraps rollers via friction. Hall sensor on idler roller
  (2 magnets, 2 pulses/rev) feeds a PI loop that corrects speed drift from voltage sag
  and micro-slip. Transport surface is smooth neoprene - best possible grip on ABS bricks.
  Rollers are simple smooth cylinders with no precision requirements. Belt splice is flat
  overlap with tapered ends and flexible CA glue, no tooth alignment needed.
- **TCS34725 + shroud:** integrated white LED, known I2C interface. Shroud is mandatory
  for calibration and operation. Open-air calibration is meaningless.
- **3S LiPo 11.1V:** LM2596 handles up to 40V input. Runs Buck1 to 6V (belt, solenoids)
  and Buck2 to 5V (ESP32, sensors, display). TMC2209 takes 11.1V direct.
- **Default = rarest:** 2x3 red (4 bricks) as default bin minimizes classifier failure damage.
- **JGB37-520 belt motor:** 6mm D-shaft, 600 RPM at 6V, high gearbox torque. TT motors
  at available ratios are too slow under load. D-shaft press-fits into drive roller bore -
  no pulleys, no intermediate belt, zero slip at the motor-to-roller joint.

## Build schedule

```
March 16-21    Order parts (AliExpress first - 10-15 day lead time).
               Firmware skeleton: state machine, stub modules, config.h.
               CAD: chute transition piece only. Nothing else.

March 21-24    Chute transition iteration.
               Print, test with real bricks, no electronics.
               This is the gate. Do not proceed until reliable.

March 28-Apr 2 Parts arrive. Validate everything on breadboard:
               - IR beam: 2x2 vs 2x3 at 20mm position
               - Color sensor: red vs blue under shroud, log R/G/B
               - Solenoid: stroke force vs spring candidates
               - Stepper + driver: spinning, step counting verified
               - Display: driver working, animation test

Apr 2-9        Full frame CAD and print.
               Use validated chute geometry. Dry assembly first.
               Fix print tolerance issues before wiring anything.

Apr 9-14       Wiring and firmware integration.
               All solenoids with flyback diodes.
               Stepper driver with 100uF cap on motor power input.
               First end-to-end test with real bricks.

Apr 14-20      Calibration.
               Color thresholds empirically from shroud + demo bricks.
               Full 24-brick runs, target 100% accuracy.
               All on LiPo, not bench supply.
               Log everything to serial -> CSV files.

Apr 20-24      Reliability runs.
               10 full runs minimum, all logged.
               Back-to-back stress test for thermal management.
               Fix anything that fails more than once.

Apr 24-27      Documentation.
               Engineering notebook from run data.
               Decision matrices, equations, calibration procedures.

Apr 27-29      Buffer. Something will need fixing here. This is not optional.

Apr 29-30      Final prep. Full run on LiPo. Pack system.

May 1          Competition.
```

## Top risks

| Risk | Mitigation |
|------|------------|
| Chute transition geometry | Print first, test with real bricks before any other CAD. Budget 3 iterations. This gates everything. |
| Color sensor window alignment | Calibrate only with shroud installed. Log raw R/G/B, set thresholds from data. |
| Solenoid spring tension | Order spring assortment. Test tension vs solenoid force before assembly. |
| Neoprene belt friction | Test neoprene on bare PLA roller day-1. If slip: add heat-shrink sleeve to drive roller. If still failing: rough roller with 120-grit. Fallback: GT2 motor-to-roller + neoprene roller-to-belt. |
| Belt tracking | Idler crowned 0.5mm + 2mm flanges on both rollers. If belt walks: increase crown to 1mm. |
| Stepper driver wiring | Bulk capacitor on VM input is non-negotiable. Verify before power-on. |
| LiPo vs bench supply | All calibration and testing on LiPo after April 9. Bench supply current-limits and masks real behavior. |

## Subsystem docs

- Physical design: docs/MECHANICAL.md
- Wiring and power: docs/ELECTRICAL.md
- Firmware architecture: docs/EMBEDDED.md
- Parts list: docs/BOM.md
- All critical dimensions: docs/DIMENSIONS.md
- Color calibration procedure: docs/CALIBRATION.md
- Pass/fail acceptance criteria: docs/TEST_PROTOCOL.md
- Engineering notebook: docs/NOTEBOOK.md
- Competition run notes: docs/competition/
