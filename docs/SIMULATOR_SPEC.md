# Brick Sorter System Simulator - Build Spec

A single-file browser app (HTML + JS, no build step) that simulates the full
sorting pipeline with tunable parameters and animated brick visualization.
The goal is systems-level understanding: see exactly how tuning decisions
interact before touching hardware.

---

## What to build

A single `simulator.html` file. Open in any browser, no server needed.
All defaults come directly from `firmware/src/config.h` - the simulator
and firmware must stay in sync.

---

## Layout (three columns)

```
[ CONTROLS panel ]  [ BELT VISUALIZATION ]  [ METRICS panel ]
     ~280px               flexible               ~280px
```

Controls on the left, animated belt in the center, live metrics on the right.
Below the belt: a tabbed section with Timeline, Per-Brick Log, and Warnings.

---

## Controls panel (left)

Group sliders and inputs into collapsible sections. Each parameter has:
- A label in plain English (not the #define name)
- The current value readout next to the slider
- A one-line plain English explanation of what it does
- A "reset to default" button per section (not per slider)

Sections and parameters (defaults from config.h):

### Escapement
| Parameter | config.h name | Default | Range | Unit | Plain English |
|-----------|--------------|---------|-------|------|---------------|
| Release speed | STEPPER_SPS_NORMAL | 800 | 200-2000 | sps | How fast the stepper spins to release one brick. Higher = more bricks per second but fewer color sensor samples. |
| Steps per rev | STEPPER_STEPS_PER_REV | 200 | 200 (locked) | steps | Full-step NEMA 11. Not tunable in practice, shown for reference. |

### Belt
| Parameter | config.h name | Default | Range | Unit | Plain English |
|-----------|--------------|---------|-------|------|---------------|
| Belt speed | BELT_TARGET_MM_S | 200 | 50-400 | mm/s | How fast bricks travel from sensor to bins. Must stay ahead of escapement rate. |
| Motor load factor | (derived) | 65 | 40-95 | % | How much the motor slows under real load vs its no-load spec. 65% is a realistic starting estimate. You measure the real value during calibration. |
| Motor no-load RPM | (derived) | 600 | 100-1000 | RPM | Your JGB37-520 at 6V is rated 600 RPM no-load. Used to compute whether belt can keep up with escapement. |

### Sensing zone
| Parameter | config.h name | Default | Range | Unit | Plain English |
|-----------|--------------|---------|-------|------|---------------|
| Beam spacing | BEAM_GAP_MM | 19 | 10-30 | mm | Physical distance between the two size detection beams. Fixed in CAD - changing this in the sim shows what would happen with a different mechanical layout. |
| Size timeout | SIZE_TIMEOUT_MS | 150 | 50-300 | ms | If beam 2 doesn't break within this window, the brick is classified 2x2. Must be above 95ms (the 2x3 signal time) but below any noise floor. |
| Color integration | COLOR_INTEGRATION_MS | 2 | 2-10 | ms | How long the color sensor takes per reading. Minimum is 2.4ms for the TCS34725. Longer = more accurate single reading, fewer total samples. |

### Solenoid timing
| Parameter | config.h name | Default | Range | Unit | Plain English |
|-----------|--------------|---------|-------|------|---------------|
| Full power duration | SOL_FULL_MS | 20 | 5-50 | ms | How long the solenoid gets full voltage to reach full extension. Too short = doesn't fully extend. Too long = unnecessary heat. |
| Hold duty cycle | SOL_HOLD_PWM / 255 * 100 | 40 | 20-100 | % | PWM level to hold the plow in position after full extension. 40% is enough to hold against spring return. Lower = less heat. |
| De-energize time | SOL_DEENERGIZE_MS | 280 | 100-500 | ms | Total time solenoid stays on before turning off. Must be long enough for the brick to fully clear the plow. |
| Sensor-to-plow travel | SOL_LEAD_MS | 150 | 50-300 | ms | How long it takes a brick to travel from the sensing zone to plow 1. Determines how early the solenoid must fire. Calibrate from actual CAD measurements. |

### Thermal model
| Parameter | config.h name | Default | Range | Plain English |
|-----------|--------------|---------|-------|---------------|
| Heat per solenoid fire | THERMAL_HEAT_PER_SOL | 0.15 | 0.01-0.5 | How much heat accumulates each time a plow fires. Tune this after measuring real solenoid temperature during calibration. |
| Heat per stepper release | THERMAL_HEAT_PER_STEP | 0.05 | 0.01-0.2 | Heat added each time the escapement releases a brick. |
| Cooling rate | THERMAL_DECAY_RATE | 0.5 | 0.1-2.0 | How fast heat dissipates when actuators are idle. Higher = faster cooling. |
| Warning threshold | THERMAL_WARN_LEVEL | 0.60 | 0.3-0.9 | Heat level at which escapement slows to WARNING rate. |
| Danger threshold | THERMAL_DANGER_LEVEL | 0.85 | 0.5-1.0 | Heat level at which escapement slows to DANGER rate. |

### Run settings
| Parameter | Default | Plain English |
|-----------|---------|---------------|
| Number of runs to simulate | 1 | Simulate back-to-back runs to see thermal accumulation. |
| Brick sequence | interleaved | Order bricks are released: interleaved (alternating types), worst-case (all plow-firing bricks first), or default-last (default-path bricks last). |

---

## Belt visualization (center)

Animated top-down view of the belt from above. Horizontal layout,
left = chute end, right = bin end. Width fills available space, height ~220px.

### What to draw

Elements (left to right along belt):
1. Chute exit / escapement zone (hatched box, labeled)
2. Taper channel (narrowing lines, labeled)
3. Sensing zone (highlighted rectangle, labeled "Size + Color")
   - Show beam 1 and beam 2 as vertical lines with gap labeled "19mm"
   - Show color sensor as a small circle above the zone
4. Sensor-to-plow buffer (empty space, labeled with SOL_LEAD_MS value)
5. Plow 1 (labeled "2x2 BLUE"), Plow 2 ("2x2 RED"), Plow 3 ("2x3 RED")
   - Each plow draws as a small diagonal arm, animated when firing
   - Arm extends visually when solenoid fires, retracts after de-energize
6. Belt end -> default bin ("2x3 BLUE")
7. Bins drawn off to the sides where plows divert to

### Brick animation

Bricks are colored rectangles moving left to right:
- 2x2: square, ~10px side
- 2x3: wider rectangle, ~15px wide x 10px tall
- Red bricks: #E24B4A
- Blue bricks: #185FA5

Brick lifecycle:
1. Spawns at chute exit when escapement releases it
2. Moves right at BELT_TARGET_MM_S (scaled to pixels)
3. Changes visual state in the sensing zone (flash white border when classified)
4. Deflected by the correct plow with a short animation
5. Disappears into the correct bin with a flash on that bin counter

Speed is real-time by default with a 2x/5x/10x speed multiplier toggle.

Each brick shows a small tooltip on hover: brick number, type, classification
result, samples taken, whether it was correctly classified.

### Plow animation

When a solenoid fires: the plow arm rotates from ~0 to ~35 degrees over 10ms
(scaled to sim time). Holds for the hold duration. Snaps back over ~20ms.
Color the plow arm orange during FULL_POWER phase, yellow during HOLD phase,
gray when idle.

---

## Metrics panel (right)

Live updating as simulation runs.

### Throughput section
- Escapement rate: X bricks/sec
- Belt speed: X mm/s
- Belt capacity: X bricks/sec (belt speed / 28mm spacing)
- Bottleneck: "ESCAPEMENT" (green) or "BELT" (red)
- Belt margin: +X% over escapement rate
- Estimated run time: Xs for 24 bricks

### Sensing section
- Color dwell time: Xms (2x2 brick at current belt speed)
- Color samples available: N (dwell / COLOR_INTEGRATION_MS)
- Sample floor warning: green if >=8, yellow if 4-7, red if <4
- Size detection window: 0 to 95ms for 2x3, timeout at SIZE_TIMEOUT_MS

### Thermal section (live during simulation)
- Heat bars for each solenoid (1, 2, 3) and stepper - one row each
- Bar fills left to right from 0 to 1.0
- Color: green below WARNING, yellow between WARNING and DANGER, red above DANGER
- Current state label: NORMAL / WARNING / DANGER
- Current escapement rate (may be reduced by thermal throttle)

### Bin counters
- Four bin boxes with current count and expected count
- Green when count matches expected at end of run
- Red if mismatch
- Resets between runs

---

## Bottom section (tabs)

### Timeline tab
Scrollable event log, one row per event, timestamped in ms:
- Brick released, brick enters sensing zone
- Size classified (show gap_us or TIMEOUT)
- Color classified (show ratio, sample count)
- Plow fired / held / released
- Bin confirmed
- Thermal state changes
- Run complete

### Per-brick log tab
One row per brick. Columns:
brick # | type | size result | color result | samples | correct? | transit ms | thermal state at release

### Warnings tab
Any condition that would cause problems on real hardware:
- Belt is bottleneck (belt speed < escapement rate x 28mm)
- Insufficient color samples (<8)
- Size timeout too close to 2x3 signal time (within 20ms of 95ms signal)
- Solenoid de-energize time too short for brick to clear plow
- Thermal danger reached during run
- Plow overlap (two plows need to fire within SOL_DEENERGIZE_MS of each other)

---

## Simulation logic (not animation)

The simulation is a discrete event model, not real-time physics.

Each brick:
1. Released at time T. T increments by (STEPPER_STEPS_PER_REV / STEPPER_SPS) seconds
   per brick, adjusted for thermal throttle if heat is above WARNING.
2. Travels belt. Arrives at sensing zone at T + (dist_to_sensor / belt_speed).
3. Size detection: 2x3 bricks produce a gap_us of (BEAM_GAP_MM / belt_speed * 1e6).
   2x2 bricks produce gap_us = 0 (timeout).
4. Color detection: samples = floor(dwell_time / COLOR_INTEGRATION_MS).
   Dwell time = (brick_length / belt_speed * 1000). Valid samples = samples (sim
   assumes clean readings - real noise is a calibration concern, not a sim concern).
5. Classification: always correct in sim (validates routing logic, not sensor accuracy).
6. Solenoid fires at T_classified. Brick arrives at plow at T_classified + SOL_LEAD_MS.
   Solenoid must have fired before brick arrives (check: actuation 10ms << 150ms lead).
7. Bin confirmation at T + (dist_to_bin / belt_speed). Each bin at a different distance.
   Use approximate distances: bin1=250mm, bin2=280mm, bin3=310mm, bin4=360mm from sensor.
   (Update these when actual CAD measurements are available.)
8. Thermal model: update on every solenoid fire and stepper release. Decay per elapsed time.

Multi-run mode: after 24 bricks, thermal model continues decaying during a configurable
inter-run gap (default 5 seconds for reload time). Start next run from whatever heat level
remains. Show cumulative thermal chart across runs.

---

## Tech stack

Single HTML file. Vanilla JS only. No frameworks, no build step.
Canvas API for the belt animation. CSS grid for layout.
All state in a plain JS object. Simulation runs synchronously,
animation driven by requestAnimationFrame.

Use CSS variables for colors so it works in both light and dark mode.
The file should open by double-clicking - no localhost required.

---

## Defaults object (copy from config.h exactly)

```javascript
const DEFAULTS = {
  // Escapement
  stepper_sps: 800,
  stepper_steps_per_rev: 200,

  // Belt
  belt_target_mm_s: 200,
  motor_noload_rpm: 600,
  motor_load_factor: 0.65,

  // Sensing
  beam_gap_mm: 19,
  size_timeout_ms: 150,
  color_integration_ms: 2,

  // Solenoid
  sol_full_ms: 20,
  sol_hold_pct: 40,
  sol_deenergize_ms: 280,
  sol_lead_ms: 150,

  // Thermal
  thermal_heat_per_sol: 0.15,
  thermal_heat_per_step: 0.05,
  thermal_decay_rate: 0.5,
  thermal_warn_level: 0.60,
  thermal_danger_level: 0.85,

  // Geometry (from MECHANICAL.md)
  brick_2x2_mm: 15.8,
  brick_2x3_mm: 23.7,
  brick_spacing_mm: 28,
  total_bricks: 24,

  // Bin distances from sensing zone (estimate, update from CAD)
  bin_distances_mm: [250, 280, 310, 360],

  // Brick counts by type
  counts: { '2x2_blue': 6, '2x2_red': 6, '2x3_red': 4, '2x3_blue': 8 },

  // Plow assignment (0 = default/no fire)
  plow_map: { '2x2_blue': 1, '2x2_red': 2, '2x3_red': 3, '2x3_blue': 0 }
};
```

---

## File location

Save as `simulator.html` in the repo root. Add to .gitignore is NOT needed -
this is a useful dev tool that should be committed.

Add to README.md under a "Development tools" section:
"Open `simulator.html` in a browser for an interactive system simulator.
No server needed. Defaults match `firmware/src/config.h`."

---

## What NOT to build (keep it simple)

- No color sensor noise simulation (that's a calibration concern, not a design concern)
- No mechanical physics (belt slip, brick tumbling)
- No network requests
- No saved state / localStorage
- No multiple browser tabs sync
- No mobile layout (desktop only, this is a dev tool)

The goal is understanding, not accuracy. If a parameter change causes a warning
to appear, the builder understands why. That is the entire value of this tool.
