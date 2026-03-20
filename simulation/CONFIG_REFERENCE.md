# Simulator Config Reference

Open `simulator.html` in any browser. No server, no install, no build.

All defaults in the simulator match `firmware/src/config.h` exactly.
When you change a value in config.h, update the matching `DEFAULTS` object
in simulator.html and they stay in sync.

---

## How the simulation works

The simulator is a discrete event model, not physics. It assumes:
- The belt moves at exactly `BELT_TARGET_MM_S` (no slip, no wobble)
- Every brick is classified correctly (sensor noise is a calibration problem, not a design problem)
- Solenoids fire and retract on schedule

What it does model accurately:
- Timing: release intervals, sensing windows, solenoid phases, bin transit times
- Throughput math: escapement rate vs belt capacity
- Color sample count at any speed/integration combination
- Thermal heat accumulation and exponential decay across back-to-back runs
- Warnings when parameter combinations would cause real hardware problems

Use the simulator to understand tradeoffs. Use the serial log during calibration
to measure what actually happens.

---

## Parameters

### Escapement

**Release speed** - `STEPPER_SPS_NORMAL` - default 800 sps

Steps per second the stepper motor runs to release one brick. One full rotation
(200 steps) releases one brick. So:

```
bricks/sec = sps / steps_per_rev = 800 / 200 = 4 bricks/sec
release interval = 1000 / 4 = 250ms between bricks
run time (24 bricks) ≈ 24 / 4 = 6s + transit time ≈ 8-9s total
```

Tuning strategy: start at 800. Run 3 full sets. If accuracy holds at 95%+, go
to 1000. Increment by 200 until accuracy degrades or step-skipping is audible.
Document each level. The TMC2209 handles 2000+ sps. The practical ceiling is
color sensor sample count (8 samples minimum) and NEMA 11 torque under load.

When thermal WARNING or DANGER kicks in, the firmware automatically drops to
533 sps or 267 sps respectively. The simulator shows this in the thermal section.

**Steps per revolution** - `STEPPER_STEPS_PER_REV` - 200 (locked)

Full-step NEMA 11, 1.8 degrees per step. Not tunable. Shown for reference.

---

### Belt

**Belt speed** - `BELT_TARGET_MM_S` - default 200 mm/s

How fast the belt surface moves. The belt must move faster than bricks arrive
or they pile up at the escapement:

```
belt capacity (bricks/sec) = belt_speed / brick_spacing_mm
                            = 200 / 28 = 7.14 bricks/sec

margin = (7.14 - 4.0) / 4.0 = +78% headroom at defaults
```

78% headroom is comfortable. The simulator warns if this goes negative.

Belt speed also drives color sample count and size detection timing. Faster belt
means fewer samples and tighter size margins. The simulator shows both.

The JGB37-520 at 6V is rated 600 RPM no-load. Under real motor load and with
belt friction it will run slower. The motor load factor (65% default) estimates
this. Actual speed is measured empirically during calibration.

**Motor load factor** - derived - default 65%

Estimate of how much the motor slows under real belt and brick load vs no-load
spec. 65% is a reasonable starting point for a gearmotor with belt tension.
Measure actual speed during calibration and update this value.

```
estimated loaded RPM = 600 RPM * 0.65 = 390 RPM
belt speed = (390 / 60) * (pulley circumference)
pulley: GT2, 20 teeth, 2mm pitch -> circumference = 40mm
belt speed = 6.5 * 40 = 260mm/s at load (65% of no-load)
```

The actual belt speed is set by the PWM duty cycle on the L298N. The PI
controller (when implemented) closes the loop using the encoder. This parameter
is informational for the simulator's capacity calculation.

**Motor no-load RPM** - derived - default 600 RPM

JGB37-520 at 6V is rated 600 RPM no-load. Used with the load factor above.

---

### Sensing Zone

**Beam spacing** - `BEAM_GAP_MM` - default 19 mm

Physical distance between the two IR break-beam pairs in the sensing zone.
This is fixed in CAD. Changing it in the simulator shows what would happen
with a different mechanical layout.

The geometry:
```
2x2 brick: 15.8mm long < 19mm gap -> never breaks beam 2 -> classified 2x2
2x3 brick: 23.7mm long > 19mm gap -> breaks beam 2 -> classified 2x3

2x3 signal window = 19mm / 200mm/s = 95ms (time from beam1 break to beam2 break)
```

The 7.9mm length difference between bricks is the entire classification signal.
19mm gap is the key that makes it work: larger than 2x2 (15.8mm), smaller than 2x3 (23.7mm).
Any gap between 15.8mm and 23.7mm works. 19mm is the midpoint.

**Size timeout** - `SIZE_TIMEOUT_MS` - default 150 ms

If beam 2 does not break within this window, the brick is classified 2x2.
Must be above the 2x3 signal window (95ms at 200mm/s) with enough margin
to handle belt speed variation:

```
at 200mm/s: 2x3 signal = 95ms. timeout = 150ms. margin = 55ms. Good.
at 240mm/s: 2x3 signal = 79ms. timeout = 150ms. margin = 71ms. Better.
at 150mm/s: 2x3 signal = 127ms. timeout = 150ms. margin = 23ms. Tighter.
```

The simulator warns if margin falls below 20ms.

**Color integration time** - `COLOR_INTEGRATION_MS` - default 2 ms

How long the TCS34725 takes per reading. The hardware minimum is ~2.4ms.
The firmware uses 2ms as a target; actual integration is rounded up by the driver.

Samples per brick at defaults:
```
dwell time = brick_length / belt_speed = 15.8mm / 200mm/s = 79ms
samples = floor(79ms / 2ms) = 39 samples
```

39 samples gives very high confidence in the averaged R/(R+G+B) ratio.
The minimum for reliable classification is 8 samples. The simulator
shows sample count in the sensing metrics and warns below 8.

---

### Solenoid Timing

Three solenoids, one per plow. Each fires independently. The most frequent
brick type (2x3 blue, 8/24) takes the default path with no solenoid fire.
Total fires per run: 16 (6 + 6 + 4).

**Full power duration** - `SOL_FULL_MS` - default 20 ms

How long the solenoid gets 100% PWM to reach full stroke extension.
Too short: plow does not fully extend and brick deflection is incomplete.
Too long: unnecessary heat with no benefit once stroke is complete.
20ms is conservative for a 0530 push-pull solenoid with ~10mm stroke.

**Hold duty cycle** - `SOL_HOLD_PWM` (40%) - default 40%

After full extension, PWM drops to 40% to hold position against the spring.
40% maintains extension while reducing heat by ~60% versus continuous full power.
Lower hold = less heat, but if too low the spring overcomes the hold and the
plow retracts mid-brick. 40% is the starting point; confirm against your spring
selection during calibration.

`SOL_HOLD_PWM = 102` in config.h (102/255 = 40%).

**De-energize time** - `SOL_DEENERGIZE_MS` - default 280 ms

Total time solenoid stays energized (full power phase + hold phase) before
turning off and letting the spring return the plow.

The brick must fully clear the plow before de-energize:
```
brick arrives at plow: T_classified + sol_lead_ms = T_classified + 150ms
2x3 brick clears plow: +23.7mm / 200mm/s = +118.5ms after arrival
total from T_classified: 150 + 118.5 = 268.5ms

de-energize at 280ms -> 11.5ms margin after brick clears. Fine.
```

The simulator warns if de-energize happens before the brick clears.

After de-energize: spring return takes ~20ms. Plow is reset and ready.

**Sensor-to-plow travel** - `SOL_LEAD_MS` - default 150 ms

How long it takes a brick to travel from the sensing zone to plow 1.
The solenoid fires the instant classification completes. The brick is still
traveling toward the plow. This is the pre-set window.

```
150ms lead at 200mm/s = 30mm from sensor to plow 1
solenoid actuation: ~10ms
pre-set margin: 150 - 10 = 140ms before brick arrives
```

140ms is enormous margin. The solenoid is fully extended and settled
long before the brick arrives. Update from actual CAD measurements
of sensor-to-plow-1 distance once the frame is assembled.

Plows 2 and 3 are further along the belt, so their pre-set margins are
even larger. The `SOL_LEAD_MS` value only needs to cover plow 1 (closest to sensor).

---

### Thermal Model

The thermal model protects hardware during back-to-back runs. It is not
a real temperature model - it is a dimensionless heat accumulator that
triggers escalating slowdowns when actuators are stressed.

Each actuator (3 solenoids + stepper) tracks heat independently.
Heat accumulates on every fire and decays exponentially when idle.

```
on fire:         heat += heat_per_fire (clamped at 1.0)
each second idle: heat = heat * decay_rate^1 = heat * 0.5
```

With `decay_rate = 0.5`, heat halves every second. A solenoid firing
at 4 bricks/sec (4 fires per 6 seconds, interleaved) reaches roughly:

```
steady-state heat ≈ heat_per_fire / (1 - decay_rate^(interval_sec))
with interval ~1sec between same-plow fires:
≈ 0.15 / (1 - 0.5) = 0.30
```

0.30 is well below WARNING (0.60). A single run at defaults never
triggers thermal throttling.

Thermal throttling becomes visible during back-to-back runs. Run the
simulator with `num_runs = 3` and `sequence = worst_case` to see it.

**Heat per solenoid fire** - `THERMAL_HEAT_PER_SOL` - default 0.15

**Heat per stepper release** - `THERMAL_HEAT_PER_STEP` - default 0.05

**Cooling rate** - `THERMAL_DECAY_RATE` - default 0.5 per second

Heat halves every second at 0.5. Increase this if real hardware runs
cooler than expected; decrease if it runs hotter.

**Warning threshold** - `THERMAL_WARN_LEVEL` - default 0.60

Above this: escapement slows to `STEPPER_SPS_WARNING` (533 sps, 2.7 bricks/sec).

**Danger threshold** - `THERMAL_DANGER_LEVEL` - default 0.85

Above this: escapement slows to `STEPPER_SPS_DANGER` (267 sps, 1.3 bricks/sec).

---

### Bin distances (geometry - update from CAD)

```
bin_distances_mm: [250, 280, 310, 360]
```

Distance from the sensing zone center to each bin entrance, in mm.
These are estimates. Actual values depend on frame layout. Update them
after dry assembly by measuring the physical sensor-to-bin-entrance path.

Bin assignment:
```
bin 1 (2x2 BLUE): 250mm from sensor, plow 1
bin 2 (2x2 RED):  280mm from sensor, plow 2
bin 3 (2x3 RED):  310mm from sensor, plow 3
bin 4 (2x3 BLUE): 360mm from sensor, default (no plow)
```

The bin confirmation timeout (`CONFIRM_TIMEOUT_MS = 500ms`) must be
larger than the longest transit time:

```
longest transit: 360mm / 200mm/s = 1800ms -> well within 500ms? No.
```

Wait - the bin distance is measured from the sensor. At 200mm/s:
`360mm / 200mm/s = 1800ms`. The `CONFIRM_TIMEOUT_MS = 500ms` is
measured from when the routing command fires, not from when the brick
enters the sensor. The brick is already traveling when routing fires.
Actual timeout is from T_classified (which is ~150ms after T_sense).
Remaining travel = (bin_dist - sol_lead_dist) / speed = (360 - 30) / 200 = 1.65s.
500ms is too short for bin 4. This is an open calibration item - verify
sensor-to-bin distances in dry assembly and recalculate confirm timeout.

This is tracked in docs/ISSUES.md.

---

### Brick counts and routing

```
2x3 BLUE: 8 bricks -> plow 0 (no fire, default path) -> bin 4
2x2 BLUE: 6 bricks -> plow 1 -> bin 1
2x2 RED:  6 bricks -> plow 2 -> bin 2
2x3 RED:  4 bricks -> plow 3 -> bin 3
```

Default path carries the most common type. 16 total solenoid fires per run.
Per solenoid: 6, 6, 4. Heat distribution is uneven but within thermal budget.

Expected end-of-run bin counts: **6, 6, 4, 8**.

---

## Warnings explained

**Belt is bottleneck** - error
Belt capacity (belt_speed / brick_spacing) is less than escapement rate
(sps / steps_per_rev). Bricks arrive faster than the belt can carry them
away. Jam at the escapement. Fix: increase belt speed or reduce sps.

**Insufficient color samples** - warning below 8, error below 4
Fewer than 8 samples puts the averaged ratio at risk from single-reading
noise. The TCS34725 at 2ms integration gives 39 samples at default speed.
Samples drop if belt is fast or integration time is long.

**Size timeout too close to 2x3 signal** - warning
The 2x3 brick produces a signal at `beam_gap / belt_speed` ms. The timeout
must be above this with at least 20ms margin for belt speed variation.
If timeout < signal window: 2x3 bricks are misclassified as 2x2.

**Solenoid de-energize too short** - error
The solenoid turns off before the brick has fully passed the plow. The spring
yanks the plow back under the brick and jams it. Fix: increase de-energize
time or reduce sol_lead_ms (move plow closer to sensor).

**Plow overlap** - warning
Same plow needs to fire again before it has finished de-energizing from the
last fire. This happens when same-type bricks are consecutive and sps is high.
Different plows can fire simultaneously without any issue. Only the same plow
firing twice within sol_deenergize_ms matters. The simulator checks this
against the actual sequence order.

**Thermal danger reached** - info
Appears in warnings tab after a simulation where heat exceeded the danger
threshold. Not necessarily a problem - the system throttles and recovers.
If it happens in a single run at competition speed, consider adjusting
thermal parameters or increasing decay rate.
