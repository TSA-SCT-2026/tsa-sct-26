# LEGO Brick Sorter -- Master Plan V3 (Final)

**Date:** March 21, 2026
**Competition:** NJ TSA System Control Technology -- Nationals, May 1, 2026
**Build window:** April 1 -- April 27 (4 weeks after parts arrival)
**Team size:** 3 members
**Budget:** $150
**Footprint:** 610mm x 610mm (2ft x 2ft)

This document supersedes MASTER_PLAN.md and ADDENDUM_V2.md. It incorporates all prior analysis, corrects errors found during independent review, and adds missing detail. Everything the team needs to build, wire, program, document, and win is here.

---

## 0. Critical Errors Corrected from V1/V2

Before proceeding, these are the plan-level errors found in the prior documents. Each would have caused real failure or lost points. They are all fixed in the sections that follow.

**0.1 Chute holds only 10 of 24 bricks (CRITICAL)**
V1/V2 spec a 120mm chute. Each brick is 11.4mm tall. 24 bricks stacked loosely = 274mm. The chute must be at least 300mm tall to hold all 24 bricks plus the cam mechanism. The prior design would have forced the judge to hand-feed bricks during the sort, violating the operator instructions ("do not touch during sorting"). Fixed: chute is now 310mm tall.

**0.2 Color sensor collides with bricks (CRITICAL)**
V1/V2 mount the TCS34725 at 8mm above belt surface. Brick height is 11.4mm. The brick physically smashes into the sensor. Fixed: sensor mounted at 15mm above belt surface (3.6mm clearance above brick top, within the 3-10mm optimal sensing range per TCS34725 datasheet guidance).

**0.3 Solenoid voltage mismatch (CRITICAL)**
V1/V2 assume 5V solenoid operation. The JF-0530B is available in 6V, 12V, and 24V variants. The 12V variant run at 5V delivers only ~17% of rated force (force scales with voltage squared). The cart must contain 6V variants specifically, OR the solenoid rail must be 12V. Fixed: order 6V variant explicitly. Run on the 6V buck converter rail (same one powering the belt motor). Peak current at 6V is ~1A per solenoid.

**0.4 Solenoid force curve misunderstood (SIGNIFICANT)**
V1/V2 assume 1N constant force across 10mm stroke. The JF-0530B has 5N HOLDING force (at 0mm air gap, end of stroke) and ~0.5N INITIAL force (at 10mm air gap, start of stroke). Average force across the stroke is roughly 1.5-2N. The brick still clears easily (even 0.5N accelerates a 3g brick at 167 m/s^2), but the physics in the notebook must reflect the real curve, not a flat assumption.

**0.5 ESP32 GPIO 36/39 phantom interrupts (SIGNIFICANT)**
V1/V2 assign GPIO 36 (chute exit beam) and GPIO 39 (bin1 confirmation beam). These pins have a documented hardware errata: built-in 270pF capacitors cause crosstalk with GPIO 37/38, and phantom interrupts fire when ADC1 is active on other pins (which happens during TCS34725 reads on the I2C bus sharing the ADC reference). Since break-beams use interrupts, this is a reliability problem. Fixed: pin assignment revised to avoid GPIO 36 and 39 for interrupt-driven inputs.

**0.6 GPIO 34-39 need external pull-up resistors (MODERATE)**
These pins have no internal pull-ups. Break-beam receivers are open-collector or open-drain and need pull-ups to function. V1/V2 never mention external pull-ups. Fixed: 10k external pull-ups on all break-beam receiver pins, specified in the wiring section.

**0.7 GT2 20mm closed-loop belt is not a standard product (MODERATE)**
V1/V2 assume this is orderable. Standard GT2 closed-loop belts come in 6mm and 10mm widths. 20mm is available only as open-ended belt (Amazon, 5-10m rolls). Fixed: order 20mm open-ended GT2 belt and splice into a loop using a metal crimp plate, OR use the hybrid drive approach (6mm GT2 drive belt + 25mm flat PVC transport belt on crowned rollers).

**0.8 Belt speed too fast for color sensing reliability (MODERATE)**
At 200mm/s, a 2x2 brick (15.8mm long) passes the sensor in 79ms. At 24ms integration time, that is 3 samples. Three samples is marginal for reliable color discrimination under varying conditions. Fixed: target belt speed reduced to 120mm/s. This gives 5-6 samples per brick, doubles all timing margins, and adds only 4 seconds to total sort time (still under 20 seconds). There is no scoring reward proportional to speed -- reliability at 60 points outweighs a few seconds.

**0.9 No power-on self-test (MODERATE)**
The judge connects a battery and sees "READY." If a sensor is disconnected, a solenoid is dead, or the belt motor is jammed, the system discovers this mid-sort and fails. Fixed: power-on self-test sequence added to firmware: test each solenoid (single pulse), verify each break-beam (emitter on / receiver responds), verify color sensor (I2C ACK), verify stepper (single step + StallGuard check), verify belt motor (short spin + confirm beam toggle). Display SELF-TEST PASS/FAIL before allowing START.

**0.10 No stud-interference analysis for channel (MINOR)**
LEGO studs protrude 1.8mm above the brick body. If studs face a channel wall, they can catch on slot edges or sensor mounts. Fixed: channel design requires all internal surfaces to be smooth and flush. Pusher slots use chamfered edges. Sensor shroud recessed above channel ceiling, not protruding into channel.

**0.11 Default bin accumulates all errors (STRATEGIC)**
V1/V2 assign 2x3 blue (33%, 8 bricks) as the default bin (belt end, no pusher). Every missed push of any type also lands in this bin. One failure = two errors (wrong brick in bin 4 + missing brick from correct bin). Fixed: assign the RAREST category as default. 2x3 red (17%, 4 bricks) becomes the default. This minimizes the number of bricks that traverse the full belt (reducing wear/timing exposure) and means the error-accumulation bin has only 4 expected bricks, making contamination more obvious during post-sort verification.

---

## 1. System Architecture (Revised)

### 1.1 Architecture Diagram

```
                    [CHUTE - 310mm tall, vertical]
                    [Judge loads all 24 bricks here]
                              |
                    [ESCAPEMENT - NEMA 11 + cam]
                              |
                              v
[=== 25mm CHANNEL (GT2 belt, flat side up) ========================]
  |         |             |             |              |
  |     [SENSE ZONE]  [PUSHER 1]   [PUSHER 2]    [PUSHER 3]
  |     beams+color   bin1:2x2red  bin2:2x2blue  bin3:2x3blue
  |                                                    |
  |                                              [BELT END]
  |                                           bin4:2x3red (DEFAULT)
  v
[BASE PLATE 610mm x 610mm]
```

### 1.2 Bin Assignment (Revised)

| Bin | Position | Category | Count | Pusher | Rationale |
|-----|----------|----------|-------|--------|-----------|
| 1 | Pusher 1 (closest to chute) | 2x2 red | 6 | Solenoid 1 | Second-most-common, short belt travel |
| 2 | Pusher 2 (middle) | 2x2 blue | 6 | Solenoid 2 | Same count as bin 1 |
| 3 | Pusher 3 (far) | 2x3 blue | 8 | Solenoid 3 | Most common, but needs the push |
| 4 | Belt end (no pusher) | 2x3 red | 4 | None | RAREST category = default. Minimizes error accumulation. |

Why 2x3 red as default instead of 2x3 blue: only 4 bricks traverse the full belt with zero actuation risk. If the system mis-classifies one brick, bin 4 gets 5 instead of 4 -- clearly visible. If the most common brick (8 units) were default, a single error is hidden in the larger count. The rarest-as-default strategy maximizes the signal-to-noise ratio of the error detection system.

### 1.3 Key Design Parameters

| Parameter | Value | Rationale |
|-----------|-------|-----------|
| Belt speed | 120 mm/s | 5-6 color samples per brick, 2x timing margins vs 200mm/s |
| Channel width | 25mm internal | 15.8mm brick + 4.6mm clearance per side. More forgiving than 20mm. |
| Escapement rate | 3 bricks/sec | 333ms between releases. Conservative. Still sorts 24 in ~10s + 4s drain. |
| Solenoid voltage | 6V DC | Match to JF-0530B 6V variant. Peak ~1A, duty < 5%. |
| Solenoid on-time | 40ms | Conservative. Brick clears in <10ms but extra time ensures full stroke. |
| Belt type | 20mm open-ended GT2 (spliced) or 6mm GT2 drive + 25mm flat PVC | See section 3.1 |
| Chute height | 310mm | Holds all 24 bricks (274mm stack) + 36mm for cam + exit ramp |
| Sensor height | 15mm above belt | 3.6mm above brick top. Within TCS34725 optimal range. |
| Total sort time | ~14 seconds | 8s release + 4s drain + 2s margin. Fast enough to impress. |

---

## 2. Scoring Strategy

The rubric awards 100 points across three categories. Every design decision must trace back to maximizing these points.

### 2.1 Solution to Problem: 60 points

This is the working device. It must sort 24/24 correctly when operated by a stranger. The rubric includes accuracy, reliability, sensor integration, feedback loop, and speed.

**Strategy:** Reliability over speed. 24/24 correct at 14 seconds beats 23/24 correct at 8 seconds. Every design choice that trades reliability for speed is wrong. The belt runs at 120mm/s, not 200mm/s. The escapement runs at 3 bricks/sec, not 8. Timing margins are 2x wider than strictly necessary.

**Feedback loop:** The rubric requires "a feedback loop that confirms successful sorting or identifies errors." The system has bin confirmation break-beams at each bin entrance. When a brick is pushed toward bin N, the firmware expects bin N's beam to break within 500ms. If it does: increment sorted count. If it does not: increment error count. The OLED display shows real-time sorted/error counts. This is a genuine closed-loop feedback system, not decorative.

**Exceeding sensor requirements:** The rubric requires "at least two sensors." The system uses: two IR break-beams for size discrimination, one TCS34725 for color classification, one chute-exit break-beam for release confirmation, four bin-entry break-beams for sort confirmation, and optionally a Hall-effect sensor for belt speed feedback. That is 9 sensors minimum, 10 with the Hall sensor. Document each sensor and its purpose in the notebook. The judges notice when you exceed minimums with purpose.

### 2.2 Inventor's Log: 20 points

This is the engineering notebook. It must show process, not just outcome. The rubric rewards "decisions derived from data."

**Strategy:** The notebook IS the differentiator. Every competing team will build a conveyor that mostly works. The team that wins is the one whose notebook shows the deepest engineering thinking. Our notebook has: 7 rejected alternatives with quantitative scoring, force calculations with real solenoid curve data, timing analysis at multiple belt speeds, an omniveyor evaluation with friction-budget physics, and iteration data from testing.

**Specific content plan in Section 10.**

### 2.3 Programming Structure: 20 points

This is code quality. The rubric explicitly scores clean architecture, logical organization, and readable structure.

**Strategy:** The firmware must look professional. Every file has a header comment block. Every function has a doc comment. Constants are in a single config header with units in comments. The state machine is documented with an ASCII diagram in the code. Variable names are descriptive (not `x`, `t`, `flag`). Error handling is explicit, not silent. The code compiles with zero warnings.

**Specific firmware plan in Section 7.**

---

## 3. Mechanical Design

### 3.1 Belt System

**Decision: Use open-ended 20mm GT2 belt, spliced into a closed loop.**

Rationale: 20mm open-ended GT2 belt is readily available on Amazon (ships in 2-3 days). Closed-loop 20mm GT2 is not a standard product. The splice adds one fabrication step but eliminates the positive-drive uncertainty that a flat PVC belt would introduce.

**Splice method:** Cut belt to 950mm. Overlap ends by 20mm. Sandwich the overlap between two small aluminum plates (30mm x 22mm x 1mm, cut from aluminum sheet). Rivet or bolt with M2 hardware through the belt between teeth. The splice rides on the flat (idler) side of the belt path, never engaging the toothed pulleys directly. Test splice tensile strength with a 2kg pull before assembly.

**If splice proves unreliable in testing (fallback):** Switch to hybrid drive: 6mm GT2 belt drives the drive roller, which is covered in a rubber sleeve. A 25mm flat PVC belt wraps around both rollers and is driven by friction. The GT2 belt provides positive motor-to-roller coupling. The PVC belt provides a smooth transport surface. This adds belt-on-roller slip as a variable, mitigated by adequate roller friction (rubber sleeve) and the Hall-effect speed sensor for closed-loop correction.

**Belt bed:** The flat surface between rollers supporting the belt. Must be rigid and flat over 400mm. Do NOT 3D print this as a single piece (FDM warps). Instead: use a piece of 3mm aluminum flat bar (available at hardware stores, ~$5) cut to 400mm x 22mm. Alternatively, use a piece of acrylic or polycarbonate. The belt slides over this surface with low friction. Apply a strip of PTFE (plumber's) tape to the top surface if friction is excessive.

**Rollers/Pulleys:** Two 20-tooth GT2 pulleys, 20mm wide. If 20mm-wide GT2 pulleys are unavailable: 3D print custom pulleys with GT2 tooth profile. There are parametric generators for OpenSCAD that produce accurate GT2 profiles. Print at 0.12mm layer height, 100% infill, with the bore vertical for best concentricity. Press-fit onto 5mm steel shafts (or use set screws).

### 3.2 Channel

**Internal dimensions: 25mm wide x 15mm tall (above belt surface).**

The V1/V2 plan used 20mm width. Revised to 25mm because:

1. At 25mm, clearance per side is (25 - 15.8) / 2 = 4.6mm. This accommodates FDM printing tolerances (+/-0.3mm), slight brick rotation, and any debris.
2. The solenoid 10mm stroke still clears the brick off the belt. At 25mm channel: face plate starts flush with one wall, extends 10mm. Brick near edge is at ~4.6mm from pusher wall. After push, brick near edge at ~14.6mm. Brick far edge at 14.6 + 15.8 = 30.4mm. Channel far wall at 25mm. Brick extends 5.4mm past the wall, through the 30mm bin opening. The brick's exit velocity (1.5+ m/s) carries it fully into the bin.
3. The 25mm channel still self-aligns 2x3 bricks (23.7mm > 25mm - no wait, 23.7mm < 25mm. A 2x3 brick is 23.7 x 15.8. Both dimensions fit in a 25mm channel. Self-alignment fails.

**CRITICAL RE-EVALUATION:** At 25mm channel width, a 2x3 brick (23.7 x 15.8) can enter in EITHER orientation (both dimensions < 25mm). Self-alignment requires the channel to reject the 23.7mm dimension. This requires channel width < 23.7mm. But the 2x2 brick is 15.8 x 15.8, needing width > 15.8mm.

Acceptable channel width range: 16.5mm to 23.0mm.

**Revised channel width: 22mm internal.**

At 22mm:
- 2x2 (15.8 x 15.8): fits with 3.1mm clearance per side. Good.
- 2x3 with 15.8mm across channel: fits with 3.1mm clearance. Correct orientation.
- 2x3 with 23.7mm across channel: 23.7 > 22. BLOCKED. Self-alignment works.

Solenoid push at 22mm channel: face plate extends 10mm. Brick near edge at ~3.1mm. After push, near edge at ~13.1mm. Far edge at 13.1 + 15.8 = 28.9mm. Channel wall at 22mm. Brick extends 6.9mm past wall into 30mm bin opening. Exit velocity carries it in. Works.

**Belt width must also be <= 22mm.** A 20mm GT2 belt fits inside a 22mm channel with 1mm margin per side. The channel walls sit on the belt frame rails, flanking the belt.

**Channel wall construction:**
- Two walls, each 400mm long x 15mm tall x 3mm thick, PLA.
- Pusher side: three slots (24mm wide x 13mm tall) at pusher positions. Slot edges chamfered 1mm to prevent brick studs from catching.
- Bin side: three openings (30mm wide x 13mm tall) aligned with pusher slots. Opening chamfered on entry side to guide brick into bin ramp.
- Top: partially open for sensor access. A sensor bridge spans the channel at the sensing zone, holding the TCS34725 shroud above the bricks.
- Interior surfaces: smooth. Sand after printing. Apply PTFE tape to any areas where bricks slide against the wall (especially near the chute exit where bricks transition from stationary to belt speed).

### 3.3 Chute and Escapement

**Chute:**
- Vertical tube, internal cross-section: 22mm x 27mm.
  - Why 27mm instead of 26mm: the 2x3 brick is 23.7mm. In the chute's 27mm dimension, the brick has 1.65mm clearance per side. The extra millimeter vs V2's 26mm prevents jamming from printing tolerance stack-up.
- Height: 310mm total. Bottom 36mm is the cam/exit zone. Top 274mm holds 24 bricks stacked vertically (24 x 11.4mm).
- Top opening: flared to 35mm x 40mm for easy loading. The flare transitions over 20mm of height (gentle funnel).
- Walls: 3mm PLA, printed as a rectangular tube. If printer bed is too small for 310mm vertical: print in two halves (top half + bottom half) with a tongue-and-groove joint at the seam. Glue with CA (superglue) and reinforce with a printed external sleeve.
- Mounting: chute bolts to a vertical support plate that attaches to the base plate. The support plate is 5mm plywood or printed PLA with gussets. The chute must be rigid -- if it sways, the cam-to-chute alignment changes and bricks jam.

**Escapement (Vertical Chute, Bottom-Exit Cam -- revised from V2):**

Mechanism: single-lobe cam disk on NEMA 11 stepper shaft. The cam rotates on a horizontal axis perpendicular to the belt direction. One full rotation releases one brick.

Cam profile:
- Disk diameter: 30mm.
- Flat chord on the disk body (the "seat") where the bottom brick rests. Chord depth: 2mm. This gives the brick a flat surface to sit on instead of a curved surface. The brick queue weight pushes the bottom brick onto this flat seat.
- Single lobe: extends from the disk body by 8mm (enough to push a brick across the chute exit opening). The lobe arc is 50 degrees (not 90 degrees as V1 suggested, not 60 as V2 suggested). At 3 rev/sec, 50 degrees takes 46ms. The brick drops under gravity in 48ms (sqrt(2 * 0.0114 / 9.81) * 1000). At 46ms the blocking edge has ARRIVED before the next brick fully drops. This eliminates double-release.
- Lobe edge: SHARP transition from lobe to body. No taper. The lobe's trailing edge is a near-vertical wall that acts as the blocking surface for the next brick.
- Print at 0.12mm layer height, 100% infill, for maximum profile accuracy.

Exit geometry: the cam pushes the bottom brick horizontally out one side of the chute, onto a short ramp (10mm long, 5-degree decline) that deposits the brick onto the moving belt surface. The ramp is critical -- it gives the brick a gentle transition from zero forward velocity to belt-speed. Without the ramp, the brick drops 2-3mm onto the belt and may bounce or tumble.

Stepper driver: TMC2209 (in cart). Benefits documented in notebook: silent operation (StealthChop), UART-configurable current limit, StallGuard for jam detection. StallGuard is the most valuable feature: if a brick jams the cam, the stepper stalls, TMC2209 reports it via UART, firmware enters ERROR state. Free jam sensor.

### 3.4 Sensor Mounts

**Break-beam pair (size discrimination):**
- Beam 1: 50mm from chute exit, crossing the channel at 6mm above belt surface.
- Beam 2: 69mm from chute exit (19mm downstream of beam 1).
- Each beam: emitter on one channel wall, receiver on the opposite wall. Mount in small 3D-printed clips that press-fit into slots in the channel walls.
- 10k pull-up resistors on each receiver output, pulled to 3.3V.

**Color sensor (TCS34725):**
- Mounted on a bridge that spans the channel at 60mm from chute exit (between beam 1 and beam 2).
- Sensor faces DOWN, 15mm above belt surface (3.6mm above brick top).
- Enclosed in a light-blocking shroud: 20mm x 20mm x 25mm tall, black PLA, open bottom. The shroud drops below the channel top edge to block ambient light from all sides. The narrow channel already constrains the brick position to +/-3mm cross-belt, so the sensor always reads the center of the brick.
- The onboard white LED provides illumination. In the enclosed shroud, this is the only light source. Consistent illumination = consistent readings.
- Important: the shroud must NOT protrude into the channel opening (below the channel ceiling). It sits ON TOP of the channel, looking down through a hole in the channel ceiling. The hole is 15mm x 15mm, centered over the belt.

**Chute exit beam:**
- One IR break-beam at the chute exit (just after the cam), confirming each brick release.
- Mount in the chute wall at the exit opening.

**Bin confirmation beams (4 total):**
- One break-beam at each bin entrance.
- Mounted in the bin wall just inside the opening, beam crossing the brick's entry path.
- These confirm that a brick actually entered the bin after being pushed (or after falling off the belt end for bin 4).

**Optional: Hall-effect belt speed sensor:**
- Small magnet epoxied to the idler roller rim.
- Hall-effect sensor (e.g., A3144) mounted adjacent to the roller, 2mm air gap.
- Each rotation = one pulse. Pulses/sec x pi x d_roller = belt speed.
- Closed-loop speed control: if measured speed drifts from target, adjust belt motor PWM.
- This is optional because GT2 positive drive already minimizes slip. But it adds a documented feedback loop and impresses judges. Cost: ~$0.50.

### 3.5 Solenoid Mounts and Face Plates

Three identical assemblies, one at each pusher position.

**Solenoid mount bracket:**
- U-shaped 3D-printed bracket clamps the JF-0530B body (13mm x 16mm x 30mm).
- Two M2 screws through the bracket into threaded holes in the solenoid's existing mounting tabs.
- Bracket attaches to the belt frame via two M3 bolts through a 5mm-travel slot (for fine-tuning pusher position along the belt axis).
- The solenoid rod points toward the channel. The rod center is at 6mm above belt surface (aligned with the approximate brick center of mass).

**Face plate:**
- 20mm wide (along belt axis) x 11mm tall x 2mm thick. PLA at 100% infill, or cut from 2mm aluminum.
- Attached to solenoid rod via a press-fit sleeve (3D-printed collar that friction-fits over the rod, secured with a drop of CA glue).
- All edges chamfered 0.5mm to prevent catching on the channel wall slot.
- Face surface flat, perpendicular to push direction.

**Why 20mm wide (not 18mm as in V1/V2):** at 120mm/s belt speed, the timing window for a 2x2 brick (15.8mm) to be in front of the face plate is (15.8 + 20) / 120 * 1000 = 298ms. The solenoid fires for 40ms. Timing margin is 258ms. At 200mm/s belt speed (V1/V2), it would be 179ms margin. The wider face plate and slower belt together give enormous timing tolerance.

### 3.6 Bins

Four bins. Each must hold its expected brick count plus margin for errors.

| Bin | Expected bricks | Internal size (W x L x H) |
|-----|-----------------|---------------------------|
| 1 (2x2 red) | 6 | 30mm x 30mm x 80mm |
| 2 (2x2 blue) | 6 | 30mm x 30mm x 80mm |
| 3 (2x3 blue) | 8 | 30mm x 35mm x 100mm |
| 4 (2x3 red, default) | 4 | 30mm x 35mm x 60mm |

Construction: 3mm PLA walls, floor angled 10 degrees toward the back wall (bricks slide to rest position, cannot bounce out). Open top for brick entry. Break-beam slot at bin entrance.

Bins 1-3 mounted on the bin side of the belt, at base plate level, directly below the channel openings. Bin 4 at belt end, below the belt exit point.

### 3.7 Base Plate and Frame

**Base plate:** 610mm x 610mm x 6mm MDF or plywood. Pre-drill mounting holes for the belt frame, chute support, and bins. Finish with a coat of clear polyurethane or paint (looks professional, prevents warping from humidity).

**Belt frame:** Two side rails (460mm long x 8mm wide x 40mm tall) plus cross-braces. Material: PLA or aluminum angle. The belt sits between the side rails with the channel walls on top.

**Chute support:** Vertical plate (310mm x 50mm x 5mm) bolted to the base plate with two gussets (triangular braces, 50mm x 50mm x 5mm) for rigidity. The chute bolts to this plate. The chute must not sway during escapement operation.

**Electronics bay:** Underneath the belt frame and beside the chute. The ESP32, TMC2209, MOSFET board, buck converters, and wiring terminal strips mount here. Clean wire routing with cable ties.

### 3.8 Assembly Order

1. Print test pieces first (channel section, chute section, cam wheel).
2. Test physical fit with real LEGO bricks before printing full-size parts.
3. Assemble belt frame with rollers, install belt, test belt motion with motor.
4. Attach channel walls to belt frame.
5. Install solenoid mounts and face plates. Test each pusher: manually place a brick, fire solenoid, verify brick exits cleanly.
6. Assemble chute and cam. Test brick release: load 10 bricks, run escapement, count releases, check for double-release.
7. Mount all sensors, wire to ESP32.
8. Install bins with confirmation beams.
9. Full integration: 24-brick sort attempt.
10. Tune timing constants in firmware.
11. Mount everything on base plate. Final test.

---

## 4. Electrical Design

### 4.1 Power Architecture

```
[LiPo 3S 11.1V 1300mAh]
         |
         +---> [Buck converter 1] -> 6V rail
         |        |-> Belt motor (JGB37-520 via L298N)
         |        |-> Solenoids x3 (via MOSFETs)
         |
         +---> [Buck converter 2] -> 5V rail
         |        |-> ESP32 (Vin pin)
         |        |-> TCS34725 (Vdd)
         |        |-> SSD1306 OLED (Vcc)
         |        |-> IR break-beam emitters (via resistors)
         |
         +---> [TMC2209] -> NEMA 11 stepper
                  (TMC2209 has onboard regulator,
                   takes motor voltage directly: 11.1V)
```

**Power budget:**

| Load | Voltage | Current (peak) | Current (avg) | Power (avg) |
|------|---------|----------------|---------------|-------------|
| Belt motor | 6V | 1.2A | 0.5A | 3W |
| Solenoids (1 at a time) | 6V | 1A | 0.05A | 0.3W |
| NEMA 11 stepper | 11.1V | 0.4A | 0.3A | 3.3W |
| ESP32 | 5V | 0.24A | 0.15A | 0.75W |
| Sensors + display | 5V | 0.1A | 0.05A | 0.25W |
| Total | -- | -- | ~1.05A | ~7.6W |

LiPo 3S 1300mAh = 14.4Wh. Runtime at 7.6W = 1.9 hours. More than sufficient.

### 4.2 Pin Assignment (Revised -- avoids GPIO 36/39 for interrupts)

```c
// === STEPPER (Escapement) via TMC2209 ===
#define PIN_STEP       GPIO_NUM_25
#define PIN_DIR        GPIO_NUM_26
#define PIN_ENABLE     GPIO_NUM_27
#define PIN_TMC_UART   GPIO_NUM_23   // TMC2209 UART (single-wire, needs 1k resistor)

// === BELT MOTOR via L298N ===
#define PIN_MOTOR_ENA  GPIO_NUM_14   // PWM speed control
#define PIN_MOTOR_IN1  GPIO_NUM_12
#define PIN_MOTOR_IN2  GPIO_NUM_13

// === SOLENOIDS via N-channel MOSFETs ===
#define PIN_SOL1       GPIO_NUM_32   // Pusher 1 (2x2 red -> bin 1)
#define PIN_SOL2       GPIO_NUM_33   // Pusher 2 (2x2 blue -> bin 2)
#define PIN_SOL3       GPIO_NUM_15   // Pusher 3 (2x3 blue -> bin 3)

// === BREAK-BEAMS (interrupt-capable, with EXTERNAL 10k pull-ups) ===
#define PIN_BEAM1      GPIO_NUM_34   // Size beam upstream (input-only, ext pull-up)
#define PIN_BEAM2      GPIO_NUM_35   // Size beam downstream (input-only, ext pull-up)
#define PIN_CHUTE_BEAM GPIO_NUM_4    // Chute exit confirm (NOT gpio36, avoids errata)
#define PIN_BIN1_BEAM  GPIO_NUM_16   // Bin 1 confirm
#define PIN_BIN2_BEAM  GPIO_NUM_17   // Bin 2 confirm
#define PIN_BIN3_BEAM  GPIO_NUM_5    // Bin 3 confirm
#define PIN_BIN4_BEAM  GPIO_NUM_18   // Bin 4 confirm

// === I2C BUS ===
#define PIN_SDA        GPIO_NUM_21
#define PIN_SCL        GPIO_NUM_22
// TCS34725 at 0x29, SSD1306 OLED at 0x3C -- no conflict

// === OPERATOR INTERFACE ===
#define PIN_START_BTN  GPIO_NUM_19   // External momentary button, pulled high, active low
#define PIN_BUZZER     GPIO_NUM_2    // Piezo buzzer for audio feedback

// === OPTIONAL: Hall-effect belt speed sensor ===
#define PIN_HALL       GPIO_NUM_0    // Hall sensor on idler roller
```

**Notes on pin choices:**
- GPIO 34, 35: input-only, no internal pull-ups. MUST have 10k external pull-ups to 3.3V. Used for the two size-discrimination beams (most timing-critical). These pins are safe for interrupts as long as GPIO 36/39 are not used concurrently with ADC reads.
- GPIO 36, 39: NOT USED. These have documented phantom interrupt issues with the internal ADC hall sensor capacitors (270pF to GPIO 37/38). Errata reference: ESP32 ECO 3.11.
- GPIO 4, 5, 16, 17, 18: general-purpose I/O, all support interrupts, have internal pull-ups. Used for non-timing-critical break-beams (chute exit, bin confirmations).
- GPIO 12: caution at boot (affects flash voltage). The L298N IN1 connection is fine because the motor won't move until firmware sets up the pin.
- GPIO 0: has internal pull-up, boot button on most devkits. Safe to use for Hall sensor (polled, not interrupt-driven, and only after boot completes).

### 4.3 Solenoid Driver Circuit (per solenoid)

```
ESP32 GPIO ---> [1k resistor] ---> IRLZ44N Gate
                                     |
                                     Drain --- [Solenoid -] --- [1N4007 diode cathode]
                                     |                              |
                                     Source --- GND            [Solenoid +] --- 6V rail
                                                                    |
                                                          [1N4007 diode anode]
```

The IRLZ44N is a logic-level MOSFET (Vgs threshold ~1-2V, fully on at 3.3V gate drive). The 1N4007 flyback diode clamps the inductive voltage spike when the solenoid de-energizes. Without the flyback diode, the spike can reach 50-100V and destroy the MOSFET or cause ESP32 resets. This is non-negotiable.

**Build three identical circuits on a small perfboard.** Label each channel. Add a common ground bus and common 6V bus with a 100uF electrolytic capacitor across the 6V rail (absorbs current spikes when solenoids fire).

### 4.4 TMC2209 Wiring

```
TMC2209 Pin  ->  Connection
VM           ->  11.1V (LiPo direct, through main power switch)
GND          ->  Common ground
VCC_IO       ->  3.3V (from ESP32 3.3V pin)
STEP         ->  GPIO 25
DIR          ->  GPIO 26
EN           ->  GPIO 27 (active low: LOW = enabled)
PDN/UART     ->  GPIO 23 via 1k resistor (single-wire UART)
MS1          ->  Tied per desired microstepping (or leave floating for UART config)
MS2          ->  Tied per desired microstepping (or leave floating for UART config)
OA1, OA2     ->  NEMA 11 coil A
OB1, OB2     ->  NEMA 11 coil B
```

**UART configuration in firmware (at startup):**
- Set run current to 400mA (NEMA 11 rated ~0.5A, derate 20% for thermal margin)
- Enable StealthChop (silent operation)
- Enable StallGuard with sensitivity tuned during testing (SGTHRS register)
- Set microstep resolution to 8x (1600 steps/rev for smooth cam rotation)

### 4.5 Complete Wiring Checklist

This is the document the wiring person follows. Every connection is listed.

| From | To | Wire color convention | Notes |
|------|----|----------------------|-------|
| LiPo + | Main power switch | Red | XT60 connector |
| Power switch | Buck 1 input +, Buck 2 input +, TMC2209 VM | Red | Star distribution from switch |
| LiPo - | Buck 1 input -, Buck 2 input -, TMC2209 GND, ESP32 GND | Black | Common ground bus |
| Buck 1 output (6V) | L298N +12V, MOSFET board 6V bus | Orange | Belt motor + solenoid power |
| Buck 2 output (5V) | ESP32 Vin, sensor Vcc bus | Yellow | Logic + sensor power |
| ESP32 3.3V | TMC2209 VCC_IO, pull-up resistor bus (10k to beam pins) | -- | 3.3V reference |
| ESP32 GPIO 25 | TMC2209 STEP | -- | Stepper step |
| ESP32 GPIO 26 | TMC2209 DIR | -- | Stepper direction |
| ESP32 GPIO 27 | TMC2209 EN | -- | Stepper enable |
| ESP32 GPIO 23 | TMC2209 PDN_UART via 1k | -- | UART config |
| ESP32 GPIO 14 | L298N ENA | -- | Belt motor PWM |
| ESP32 GPIO 12 | L298N IN1 | -- | Belt direction |
| ESP32 GPIO 13 | L298N IN2 | -- | Belt direction |
| ESP32 GPIO 32 | MOSFET gate 1 via 1k | -- | Solenoid 1 |
| ESP32 GPIO 33 | MOSFET gate 2 via 1k | -- | Solenoid 2 |
| ESP32 GPIO 15 | MOSFET gate 3 via 1k | -- | Solenoid 3 |
| ESP32 GPIO 21 | TCS34725 SDA, SSD1306 SDA | -- | I2C data |
| ESP32 GPIO 22 | TCS34725 SCL, SSD1306 SCL | -- | I2C clock |
| ESP32 GPIO 34 | Beam 1 receiver + 10k pull-up to 3.3V | -- | Size beam 1 |
| ESP32 GPIO 35 | Beam 2 receiver + 10k pull-up to 3.3V | -- | Size beam 2 |
| ESP32 GPIO 4 | Chute beam receiver + 10k pull-up to 3.3V | -- | Chute exit |
| ESP32 GPIO 16 | Bin 1 beam receiver (internal pull-up enabled) | -- | Bin 1 confirm |
| ESP32 GPIO 17 | Bin 2 beam receiver (internal pull-up enabled) | -- | Bin 2 confirm |
| ESP32 GPIO 5 | Bin 3 beam receiver (internal pull-up enabled) | -- | Bin 3 confirm |
| ESP32 GPIO 18 | Bin 4 beam receiver (internal pull-up enabled) | -- | Bin 4 confirm |
| ESP32 GPIO 19 | Start button + 10k pull-up to 3.3V | -- | Operator start |
| ESP32 GPIO 2 | Piezo buzzer + 100 ohm resistor | -- | Audio feedback |
| Beam emitters | 5V via 220 ohm current-limiting resistors | -- | IR LED drive |

---

## 5. Timing Analysis (at 120 mm/s belt speed)

All timing calculations anchored to the revised belt speed of 120mm/s.

### 5.1 Sensor-to-Pusher Delays

```
Beam 1 position:  50mm from chute exit
Beam 2 position:  69mm from chute exit (19mm gap)
Pusher 1:        160mm from chute exit
Pusher 2:        270mm from chute exit
Pusher 3:        380mm from chute exit
Belt end:        460mm from chute exit

Time from beam 1 to pusher 1: (160 - 50) / 120 * 1000 =  917 ms
Time from beam 1 to pusher 2: (270 - 50) / 120 * 1000 = 1833 ms
Time from beam 1 to pusher 3: (380 - 50) / 120 * 1000 = 2750 ms
```

### 5.2 Size Discrimination Timing

```
Beam gap: 19mm
Belt speed: 120 mm/s

2x3 brick (23.7mm along belt):
  Leading edge breaks beam 1, travels 19mm, breaks beam 2.
  Time between breaks: 19 / 120 * 1000 = 158ms
  Brick continues past beam 1 for: (23.7 - 19) / 120 * 1000 = 39ms after beam 2

2x2 brick (15.8mm along belt):
  Leading edge breaks beam 1, travels 15.8mm, clears beam 1.
  15.8mm < 19mm gap. Beam 2 NEVER breaks.
  Timeout at 200ms confirms 2x2.

Discrimination: BINARY. No overlap. 2x3 triggers beam 2 at 158ms. 2x2 never triggers beam 2.
Margin: 200ms timeout - 158ms 2x3 crossing = 42ms. Any brick that triggers beam 2 before 200ms is 2x3. Any that doesn't is 2x2.
```

### 5.3 Color Sensing Window

```
At 120mm/s, sensor is at position 60mm (centered between beams):

2x2 brick: enters sensor field when leading edge reaches ~52mm (sensor FOV ~8mm radius).
  Time in sensor field: (15.8 + 8) / 120 * 1000 = 198ms
  At 24ms integration: ~8 samples. GOOD.

2x3 brick: time in sensor field: (23.7 + 8) / 120 * 1000 = 264ms
  At 24ms integration: ~11 samples. EXCELLENT.
```

With 8-11 samples, averaging and outlier rejection are possible. Red vs blue LEGO is a huge spectral separation -- the R/(R+G+B) ratio for red LEGO is typically 0.55-0.65, for blue LEGO 0.15-0.25. Even noisy samples will cluster correctly.

### 5.4 Pusher Timing Window

```
Face plate width: 20mm along belt axis
Brick minimum length: 15.8mm (2x2)
Valid window: (20 + 15.8) / 120 * 1000 = 298ms
Solenoid on-time: 40ms
Timing margin: 298 - 40 = 258ms of acceptable trigger jitter.

Even with +/-15% belt speed variation:
  At 138mm/s: (35.8 / 138) * 1000 = 259ms window. 219ms margin.
  At 102mm/s: (35.8 / 102) * 1000 = 351ms window. 311ms margin.
```

### 5.5 Inter-Brick Spacing

```
Release rate: 3 bricks/sec = 333ms between releases
Belt speed: 120mm/s
Spacing on belt: 120 * 0.333 = 40mm

Minimum gap between bricks:
  Worst case: two 2x3 bricks (23.7mm each)
  Gap: 40 - 23.7 = 16.3mm
  This is adequate. No two bricks overlap at any pusher position.

Bricks simultaneously on belt:
  Belt length: 460mm. Brick spacing: 40mm.
  Max bricks on belt: 460 / 40 = 11.5, so ~11 bricks on belt during peak operation.
  Each brick is tracked independently by firmware (timestamp + classification result).
```

### 5.6 Total Sort Time

```
Release phase: 24 bricks / 3 per sec = 8.0 sec
Drain phase: last brick travels 460mm at 120mm/s = 3.8 sec
Total: 11.8 sec + margin = ~14 sec

This is well under 30 seconds. Fast enough to impress, slow enough to be reliable.
```

---

## 6. Escapement Deep-Dive

### 6.1 Double-Release Prevention

This is the highest-risk mechanical failure mode. If the cam releases two bricks at once, both arrive at the sensing zone simultaneously. The sensor reads an ambiguous signal. Both bricks get the same classification. One ends up in the wrong bin.

**Prevention mechanisms (defense in depth):**

1. Cam lobe arc of 50 degrees at 3 rev/sec: the blocking edge arrives in 46ms. Gravity drop time for the next brick to fall one brick-height is 48ms. The blocking edge JUST beats the drop. Physical testing will verify this and may require adjusting the lobe arc by +/-5 degrees.

2. Flat chord on cam body: the bottom brick sits on a defined flat surface, not a curved one. This prevents the brick from rocking or shifting prematurely.

3. Chute exit beam: detects each brick leaving the chute. If the beam detects two edges too close together (< 100ms apart), firmware flags a DOUBLE_RELEASE error and enters a cautious mode (slow the escapement to 1 rev/sec for the remaining bricks).

4. Queue weight friction: bricks in the queue press down on the bottom brick. When the cam pushes the bottom brick sideways, the friction between the bottom brick and the one above it resists the push. But only for the bottom brick -- the cam directly contacts and overcomes this friction. The brick ABOVE the bottom brick has the queue weight above IT, which holds it in place. This is actually a self-correcting system: the heavier the queue, the harder it is for the second brick to follow.

### 6.2 Cam-to-Belt Transition

The cam pushes the bottom brick horizontally out of the chute, onto the belt. The brick has zero forward velocity (parallel to belt travel). The belt is moving at 120mm/s.

What happens:
1. Brick enters the belt surface laterally (perpendicular to belt travel).
2. Belt friction accelerates the brick forward. Time to reach belt speed: depends on friction and brick mass.
   - Friction force: mu * m * g = 0.5 * 0.003 * 9.81 = 0.015N
   - Acceleration: F/m = 0.015 / 0.003 = 5 m/s^2
   - Time to reach 120mm/s: v/a = 0.12 / 5 = 24ms
   - Distance traveled during acceleration: v^2 / (2a) = 0.12^2 / (2*5) = 1.4mm
3. So the brick is at belt speed within 24ms and 1.4mm of entering the belt.

This is negligible. The brick is effectively at belt speed by the time it reaches the sensing zone (50mm downstream, 417ms after entering the belt). There is no timing uncertainty from the belt transition.

**However:** during the transition, the brick enters the belt from the side. It must align with the channel walls. The channel walls extend all the way back to the chute exit, so the brick enters the channel as it enters the belt. The channel walls physically guide the brick into alignment. If the cam push is too aggressive, the brick might hit the far channel wall and bounce. The cam push force should be gentle enough to place the brick in the channel without bouncing. The NEMA 11 stepper rotation speed determines this -- at 3 rev/sec, the cam lobe sweeps through in ~46ms, pushing the brick at a moderate speed.

---

## 7. Firmware Architecture

### 7.1 Module Structure

```
firmware/
  src/
    main.cpp              -- setup(), loop(), state machine top level
    config.h              -- ALL pin defs, timing constants, thresholds, with units in comments
    selftest.h/.cpp       -- power-on self-test sequence
    escapement.h/.cpp     -- TMC2209 UART config, step/dir control, StallGuard, release counting
    sensing.h/.cpp        -- break-beam ISRs, color sampling, size+color classification
    diverter.h/.cpp       -- solenoid firing logic, per-brick tracking, timer-based trigger
    display.h/.cpp        -- OLED state display, brick counter, bin counts, error display
    feedback.h/.cpp       -- bin confirmation beams, error detection, accuracy tracking
    belt.h/.cpp           -- belt motor PWM control, optional speed feedback
    audio.h/.cpp          -- piezo buzzer tones for sort events and errors
  platformio.ini          -- PlatformIO config for ESP32 DevKit
  README.md               -- build instructions, pin assignment, architecture diagram
```

### 7.2 State Machine

```
POWER_ON
  -> run self-test
  -> if pass: -> IDLE (display "READY 0/24")
  -> if fail: -> ERROR (display which subsystem failed)

IDLE
  -> operator presses START -> SORTING

SORTING
  -> start belt motor
  -> start escapement (3 rev/sec continuous)
  -> for each brick released (chute exit beam):
       enter per-brick pipeline:
         DETECT -> CLASSIFY -> TRACK -> DIVERT -> CONFIRM

  Per-brick pipeline (runs concurrently for multiple bricks on belt):
    DETECT:
      beam 1 breaks -> record timestamp, start color sampling
      beam 2 breaks (or timeout) -> size determined
    CLASSIFY:
      color samples averaged -> R_ratio computed
      category = f(size, R_ratio) -> {2x2_RED, 2x2_BLUE, 2x3_BLUE, 2x3_RED}
    TRACK:
      compute arrival time at assigned pusher (or belt end)
      add to active_bricks[] queue with {category, fire_time, pusher_id}
    DIVERT:
      timer fires at computed time -> activate assigned solenoid for 40ms
      (2x3_RED: no solenoid, falls off belt end)
    CONFIRM:
      expected bin beam breaks within 500ms -> sorted_count++
      no break within 500ms -> error_count++

  -> if sorted_count + error_count == 24: -> COMPLETE
  -> if StallGuard triggers: -> ERROR (jam detected)
  -> operator presses STOP: -> IDLE

COMPLETE
  -> stop belt motor
  -> stop escapement
  -> display "COMPLETE 24/24" + per-bin counts
  -> play completion tone
  -> -> IDLE (after STOP pressed)

ERROR
  -> stop belt motor
  -> stop escapement
  -> display error type and which subsystem
  -> operator follows recovery instructions
  -> STOP resets to IDLE
```

### 7.3 Per-Brick Tracking Data Structure

```c
struct BrickEvent {
    uint32_t beam1_time_us;      // micros() when beam 1 broke
    uint32_t beam2_time_us;      // micros() when beam 2 broke (0 if timeout = 2x2)
    uint16_t color_r_sum;        // accumulated red channel
    uint16_t color_g_sum;        // accumulated green channel
    uint16_t color_b_sum;        // accumulated blue channel
    uint8_t  color_sample_count; // number of valid color samples
    uint8_t  category;           // classification result (enum)
    uint8_t  target_pusher;      // 0=pusher1, 1=pusher2, 2=pusher3, 3=belt_end
    uint32_t fire_time_us;       // when to fire the solenoid (micros())
    bool     diverted;           // solenoid fired?
    bool     confirmed;          // bin beam confirmed?
};

// Circular buffer of active bricks (max 12 on belt at once)
BrickEvent active_bricks[16];
uint8_t brick_head = 0;
uint8_t brick_tail = 0;
```

### 7.4 Color Classification Algorithm

```c
enum BrickCategory {
    CAT_2x2_RED  = 0,  // -> pusher 1 -> bin 1
    CAT_2x2_BLUE = 1,  // -> pusher 2 -> bin 2
    CAT_2x3_BLUE = 2,  // -> pusher 3 -> bin 3
    CAT_2x3_RED  = 3,  // -> belt end -> bin 4 (DEFAULT)
    CAT_UNCERTAIN = 4   // -> ERROR: stop and flag
};

BrickCategory classify(bool is_large, uint16_t r, uint16_t g, uint16_t b) {
    float r_ratio = (float)r / (float)(r + g + b);

    // Thresholds calibrated with real bricks. These are starting points.
    // Red LEGO: R_ratio typically 0.55 - 0.65
    // Blue LEGO: R_ratio typically 0.15 - 0.25
    // Midpoint: 0.40. Dead zone: 0.30 - 0.50 flagged uncertain.

    bool is_red;
    if (r_ratio > 0.50f) {
        is_red = true;
    } else if (r_ratio < 0.30f) {
        is_red = false;
    } else {
        return CAT_UNCERTAIN;  // should not happen with clean LEGO
    }

    if (!is_large && is_red)  return CAT_2x2_RED;
    if (!is_large && !is_red) return CAT_2x2_BLUE;
    if (is_large && !is_red)  return CAT_2x3_BLUE;
    if (is_large && is_red)   return CAT_2x3_RED;

    return CAT_UNCERTAIN;  // unreachable
}
```

### 7.5 Self-Test Sequence

```
SELF-TEST (runs once at power-on, takes ~3 seconds):

1. Display "SELF-TEST..."
2. Test I2C devices:
   - Scan for TCS34725 at 0x29. If no ACK: FAIL "COLOR SENSOR".
   - Scan for SSD1306 at 0x3C. If no ACK: FAIL "DISPLAY" (but display this on serial).
3. Test break-beams:
   - For each beam: read pin state. Should be HIGH (beam unbroken, pull-up).
   - If any beam reads LOW at startup: FAIL "BEAM X BLOCKED" (something is blocking it).
4. Test solenoids:
   - Fire each solenoid for 20ms with 200ms between. Listen/feel for click.
   - No electrical verification possible without current sense, but the audible click
     confirms mechanical function. Display "SOL 1 OK" etc.
5. Test belt motor:
   - Run belt for 500ms at 50% PWM. Verify Hall sensor triggers (if installed)
     or just run and let operator visually confirm.
6. Test stepper:
   - Rotate cam 360 degrees (one full revolution).
   - Check TMC2209 StallGuard: if stall detected, FAIL "STEPPER JAM".
7. If all pass: display "SELF-TEST PASS" for 1 second, then "READY 0/24".
8. If any fail: display error, require power cycle to retry.
```

### 7.6 Firmware Development Priority (Revised Schedule)

| Day | Person A (firmware) | What to test |
|-----|-------------------|--------------|
| 1-2 | State machine skeleton + config.h + serial logging | Compiles, runs on bare ESP32 |
| 3-4 | TMC2209 UART config + step/dir + StallGuard | Stepper turns, stall detected when blocked |
| 5-6 | Break-beam ISRs + hardware timer for beam gap | Finger breaks beam, serial logs timestamps |
| 7-8 | TCS34725 I2C driver + continuous sampling + classify() | Red/blue brick held over sensor, classification logged |
| 9-10 | Solenoid firing module + timer-based trigger from beam event | Solenoid fires at correct delay after beam break |
| 11-12 | Belt motor PWM + speed calibration | Belt runs at target speed, measured with stopwatch |
| 13-14 | Self-test sequence + OLED display + buzzer | Full power-on self-test, display shows state |
| 15+ | Integration testing, 24-brick runs, tuning | Real sorting runs, timing refinement |

### 7.7 Code Quality Standards (for 20-point Programming Structure score)

1. Every .h/.cpp file starts with a header block:
```c
/**
 * sensing.h -- Brick size and color classification module
 *
 * Uses two IR break-beams (19mm gap) for binary size discrimination
 * and TCS34725 color sensor for red/blue classification.
 *
 * Part of the LEGO Brick Sorter firmware.
 * NJ TSA System Control Technology 2026.
 */
```

2. Every function has a doc comment explaining what it does, its parameters, and return value.

3. All magic numbers are in config.h with descriptive names and units:
```c
constexpr float BELT_SPEED_MM_PER_S = 120.0f;  // target belt speed
constexpr float BEAM_GAP_MM = 19.0f;            // distance between size beams
constexpr uint32_t SOLENOID_ON_MS = 40;          // solenoid energize duration
```

4. Error handling is explicit: every function that can fail returns a status code. No silent failures.

5. The state machine is documented with an ASCII diagram in main.cpp.

6. Zero compiler warnings with -Wall -Wextra.

7. Consistent code style: camelCase for variables, UPPER_SNAKE for constants, PascalCase for types.

---

## 8. Engineering Notebook Strategy

This section is a content plan, not an outline. Each subsection describes what to write AND why it scores points.

### 8.1 Structure (40-50 pages total)

**Section 1: Problem Definition (3 pages)**
- Restate the problem in the team's own words. Do not copy the design brief verbatim. Interpret it.
- Define success criteria explicitly: 24/24 sorted correctly, operated by a stranger from written instructions, within 2ft x 2ft, all subsystems integrated, feedback loop confirmed.
- Identify constraints: budget ($150), time (4 weeks), team size (3), footprint (610mm x 610mm), no on-site work, no interview.
- Why this matters for scoring: judges look for evidence that the team understood the problem deeply before jumping to solutions.

**Section 2: Research (4 pages)**
- Industrial sorting systems: brief overview of Amazon fulfillment center technology. Identify the key principles: conveyor transport, sensor-based classification, actuator-driven diversion, feedback confirmation.
- Sensor technologies: compare photoelectric, capacitive, color, and vision-based sensing. Justify TCS34725 selection (cost, accuracy, I2C interface, IR filter).
- Actuator technologies: compare solenoids, servos, pneumatics, stepper-driven linkages. Include a comparison table with force, speed, cost, complexity.
- Belt drive technologies: compare flat belt, timing belt, chain drive. Justify GT2 timing belt (positive drive, deterministic speed).

**Section 3: Ideation (5 pages) -- CRITICAL for scoring**
- Present ALL 7 options (A through G) plus the omniveyor (H). Hand-drawn sketches for each. One paragraph description per option.
- This shows breadth of thinking. Judges reward teams that explored the solution space before converging. Even rejected options demonstrate engineering maturity.

**Section 4: Analysis and Decision (8 pages) -- HIGHEST VALUE**
This is where the team differentiates from every other team at nationals.

Include these specific quantitative analyses:

a) Decision matrix (8 options x 8 criteria, weighted). Full table with scores and totals.

b) Force analysis with real solenoid curve:
```
JF-0530B force-stroke characteristic:
  At 10mm stroke (start of push): F_initial ~ 0.5N
  At 0mm stroke (end of push): F_holding = 5.0N
  Average force across stroke: F_avg ~ 1.5N (approximate, nonlinear)

Brick acceleration: a = F_avg / m = 1.5 / 0.003 = 500 m/s^2
Exit velocity: v = sqrt(2 * a * d) = sqrt(2 * 500 * 0.010) = 3.16 m/s
Time to clear channel: t = (channel_width - brick_offset) / v ~ 2ms

Margin: solenoid on-time (40ms) >> clearance time (2ms). 20x margin.
```

c) Timing analysis at 120mm/s belt speed. Show all delays, windows, and margins. Include a timeline diagram for a single brick's journey from chute to bin.

d) Omniveyor rejection with friction-budget physics (copy the key calculation from V2, it is excellent notebook material):
```
Normal force on 3g brick: 0.003 * 9.81 = 0.029N
Max friction force (mu=0.6): 0.018N
Solenoid force: 1.5N (average)
Force ratio: 1.5 / 0.018 = 83x advantage for solenoid
```

e) Break-beam size discrimination geometry. Diagram showing beam gap, brick sizes, and the binary discrimination logic.

f) Power budget calculation. Show that 1300mAh battery lasts >1.5 hours.

g) Belt speed tradeoff analysis. Show timing margins at 100, 120, 150, 200 mm/s. Justify 120mm/s as the reliability-optimal speed.

**Section 5: Detailed Design (6 pages)**
- System architecture diagram (the one from section 1.1, but hand-drawn and annotated).
- Component-level drawings with all critical dimensions.
- Electrical schematic (hand-drawn or KiCad). This is essential. The wiring table from section 4.5, drawn as a proper schematic.
- Pin assignment table.
- BOM with costs, sources, and delivery dates.

**Section 6: Prototype and Testing (6 pages)**
- Photos of test prints (channel section, chute section, cam wheel).
- Photos of first full assembly.
- Data tables from color sensor calibration (10+ readings per brick color, mean, std dev, threshold).
- Data tables from full-system test runs (24-brick run results: which bricks sorted correctly, which didn't, why).
- Break-beam timing logs from first tests.

**Section 7: Iteration (3 pages)**
- What changed after testing. Before/after comparison.
- Examples: "Cam lobe arc reduced from 60 to 48 degrees after observing double-release in run 3. Result: zero double-releases in runs 4-12."
- Timing parameter adjustments with measured vs. expected values.

**Section 8: Final Design (3 pages)**
- Final photos of the completed system from multiple angles.
- Final system specifications table.
- Operator instructions (printed copy included in notebook).

### 8.2 Presentation Quality

- Use a bound composition notebook or a printed/bound document. Not loose pages.
- Date every entry. Sign each page.
- Hand-drawn sketches are valued. They show personal engagement, not just CAD output.
- Include printed photos (tape or glue in, with captions).
- Write in first person ("We chose..." "I calculated..."). The notebook is a narrative of the team's engineering journey.

---

## 9. Bill of Materials (Final, Order Tonight)

### 9.1 Already in Cart (verify before ordering)

| Item | Qty | Verify |
|------|-----|--------|
| JF-0530B solenoid | 4 | MUST be 6V variant. Check listing. If 12V, need to change power rail or order 6V separately. |
| NEMA 11 stepper | 1 | Verify 5mm shaft with D-flat |
| TMC2209 stepper driver | 1 | Verify UART-capable variant (not just step/dir breakout) |
| JGB37-520 gearmotor 6V 600RPM | 1 | Verify 6V variant, 6mm shaft |
| L298N motor driver | 1 | Standard H-bridge module |
| TCS34725 color sensor | 1 | Adafruit or HiLetgo breakout with onboard LED |
| IR break-beam pairs | 6 | Check beam width. 3mm slot beams preferred. |
| LM2596 buck converters | 2 | One set to 6V, one to 5V |
| GT2 timing belt + pulleys | 1 set | Check width. If 6mm or 10mm: need to order 20mm separately. |
| ESP32 DevKit | 1 | 30-pin or 38-pin variant |
| Wire, solder, perfboard | -- | Standard electronics supplies |

### 9.2 Order Tonight (additions/corrections)

| Item | Qty | Est. Cost | Source | Ship Time | Why |
|------|-----|-----------|--------|-----------|-----|
| 20mm GT2 open timing belt, 5m | 1 | $12 | Amazon | 2-3 days | Transport belt. Cut to 950mm, splice into loop. |
| GT2 20-tooth pulleys, 5mm bore | 2 | $6 | Amazon | 2-3 days | Belt drive. Verify 20mm belt width compatibility or plan to print. |
| LiPo 3S 11.1V 1300mAh XT60 | 1 | $15 | Amazon | 2-3 days | Power source. 1300mAh gives 2hr+ runtime. |
| LiPo balance charger | 1 | $10 | Amazon | 2-3 days | Required. Do not charge LiPo without balance charger. |
| Adafruit IR break-beam pairs (3mm) | 4 | $12 | Adafruit | 3-5 days | High-accuracy for size beams (2) + chute exit (1) + spare |
| SSD1306 0.96" OLED I2C | 1 | $4 | Amazon | 2-3 days | State display |
| IRLZ44N N-channel MOSFET | 6 | $4 | Amazon | 2-3 days | Solenoid drivers (3) + spares (3) |
| 1N4007 diodes | 10 | $2 | Amazon | 2-3 days | Flyback protection |
| 10k resistor pack | 1 | $2 | Amazon | 2-3 days | Pull-ups for break-beams |
| Piezo buzzer (passive) | 2 | $2 | Amazon | 2-3 days | Audio feedback + spare |
| 100uF electrolytic capacitors | 5 | $2 | Amazon | 2-3 days | Decoupling on power rails |
| M2 + M3 hardware assortment | 1 | $5 | Amazon | 2-3 days | Fasteners |
| PTFE (plumber's) tape | 1 | $2 | Hardware store | Today | Channel wall low-friction lining |
| 3mm aluminum flat bar, 500mm | 1 | $3 | Hardware store | Today | Belt bed (rigid, flat) |
| Clear acrylic sheet 3mm, 200x50mm | 1 | $4 | Hardware store | Today | Transparent pusher-side channel wall section |
| Small neodymium magnets, 3mm | 2 | $2 | Amazon | 2-3 days | Hall-effect belt speed sensor |
| A3144 Hall-effect sensor | 2 | $2 | Amazon | 2-3 days | Belt speed measurement |
| 5mm steel rod, 100mm | 2 | $3 | Amazon/hardware | 2-3 days | Roller shafts |
| 608ZZ bearings (8mm ID) or 625ZZ (5mm ID) | 4 | $4 | Amazon | 2-3 days | Roller bearings |
| PLA filament 1kg (if low) | 1 | $18 | Amazon | 2-3 days | Printing material |

**Total additions: ~$114**
**Estimated total project cost: ~$70-80 (cart) + $114 (additions) = ~$184-194**

**OVER BUDGET by ~$35-44.** Need to cut:

Items to cut if budget-constrained:
- Hall-effect sensor + magnets ($4): nice-to-have, not essential with GT2 positive drive. Save $4.
- PLA filament: only order if actually low. Save $18.
- Reduce break-beam order: 4 Adafruit pairs instead of 4 + the 6 in cart may be enough. Reassess after inventory. Save $0-12.
- Clear acrylic: can use sanded PLA instead for transparency effect. Save $4.

**Revised additions after cuts: ~$88-96. Total: ~$158-176. Close to budget.**

If the solenoids in the cart are 12V (not 6V), either: run the solenoid rail at 12V (add a third buck converter or tap LiPo directly through the 11.1V rail -- close enough for a 12V solenoid), OR order 6V variants separately ($8 for 4 on Amazon).

---

## 10. Build Schedule (4 Weeks)

### Week 1 (April 1-7): Subsystem Prototyping

| Day | Person A (Firmware) | Person B (CAD/Mechanical) | Person C (Electrical/Notebook) |
|-----|-------------------|--------------------------|-------------------------------|
| 1 | State machine skeleton, config.h, serial logging | Top-down 2D layout sketch. Verify footprint. | Inventory all parts. Verify solenoid voltage. Set buck converters to 6V and 5V. |
| 2 | TMC2209 UART module | TEST PRINT: 50mm channel section with one pusher slot | Solder MOSFET driver board (3 channels). Test with solenoid. |
| 3 | Break-beam ISR module | TEST PRINT: 50mm chute section for brick fit | Wire break-beam circuits with pull-ups. Test with ESP32. |
| 4 | TCS34725 driver + color sampling | Test prints with real bricks. Iterate if needed. | Begin wiring distribution board (perfboard hub). |
| 5 | Solenoid firing module | Belt frame CAD. Roller design. | Color sensor calibration: collect 20+ readings per color. |
| 6 | Belt motor PWM control | Belt frame print (overnight) | Begin notebook Section 1-3 (Problem, Research, Ideation). |
| 7 | Integration: beam ISR -> classify -> solenoid fire (bench test) | Channel walls CAD + print | Continue notebook. Draw electrical schematic. |

**Week 1 milestone:** Belt frame printed. Channel and chute test prints validated with real bricks. Firmware modules compiling and tested individually. MOSFET board working. Notebook sections 1-3 drafted.

### Week 2 (April 8-14): Mechanical Integration

| Day | Person A | Person B | Person C |
|-----|---------|---------|---------|
| 8 | Escapement stepper control + StallGuard | Solenoid mount brackets CAD + print | Wire full system on breadboard. All sensors connected. |
| 9 | Belt speed calibration | Cam wheel CAD + print (multiple iterations expected) | Test full wiring: every sensor reads, every actuator fires. |
| 10 | Self-test sequence | Chute body CAD (full 310mm) + print (overnight) | Notebook Section 4 (Analysis + Decision). This is the big one. |
| 11 | OLED display module | Assemble belt frame + channel walls + belt | Continue Section 4. Include all equations. |
| 12 | Buzzer audio feedback | Install solenoid mounts on belt frame | Solder final wiring board (no more breadboard). |
| 13 | Per-brick tracking data structure + timer-based divert | Test escapement: load 10 bricks, count releases | Transfer all wiring from breadboard to soldered board. |
| 14 | Bin confirmation feedback module | Print bins. Assemble bins with confirmation beams. | Section 5 (Detailed Design) with schematic and drawings. |

**Week 2 milestone:** All mechanical parts printed and assembled. All wiring soldered. Firmware has all modules. Each subsystem tested. Notebook sections 1-5 complete.

### Week 3 (April 15-21): System Integration and Testing

| Day | All Three Together |
|-----|-------------------|
| 15 | Mount everything on base plate. Route and secure all wires. |
| 16 | First power-on. Self-test. Fix any wiring issues. |
| 17 | First 24-brick sort attempt. Record results meticulously. |
| 18 | Analyze failures from day 17. Fix mechanical issues (reprint parts if needed). Fix firmware timing. |
| 19 | Run 5 more 24-brick sorts. Record every result. Tune color thresholds and timing delays. |
| 20 | Run 10 more sorts. Target: 24/24 in at least 8 of 10 runs. |
| 21 | Address remaining failure modes. Run 5 more sorts for confidence. |

**Week 3 milestone:** System sorts 24/24 in 8+ of 10 runs. All timing parameters finalized. Color thresholds calibrated.

### Week 4 (April 22-27): Hardening, Documentation, Polish

| Day | Task |
|-----|------|
| 22 | Stress testing: 20 consecutive runs. Log every result. Identify intermittent failures. |
| 23 | Fix any remaining issues. Lock all firmware parameters (no more changes after today). |
| 24 | Write operator instructions. Test with a naive user (friend/family who has never seen the system). Iterate instructions until they work perfectly. |
| 25 | Notebook Sections 6-8 (Testing data, Iteration, Final Design). Print photos. |
| 26 | Final assembly polish. Tidy wires. Check all bolts. Label with State Conference ID. Pack for transport. |
| 27 | BUFFER DAY. Do not schedule work. Available for emergencies only. |

---

## 11. Competition Day Plan

### 11.1 Transport Checklist

Before leaving for the competition:
- System in a padded box or case (the base plate is the bottom; pack foam around the chute and bins).
- LiPo battery charged to storage voltage (3.85V/cell) the night before. Charge to full (4.2V/cell) morning of competition.
- LiPo in a LiPo-safe bag during transport.
- Spare parts bag: 1 spare solenoid, 2 spare face plates, 1 spare cam wheel, extra M2/M3 hardware, extra wire, allen keys/screwdrivers.
- 24 LEGO bricks (6 red 2x2, 6 blue 2x2, 4 red 2x3, 8 blue 2x3). Bring extras in case judge drops one.
- Engineering notebook (bound, complete, signed).
- Operator instructions (printed, laminated).
- Multimeter (for emergency debugging).

### 11.2 Setup Procedure (15-minute window)

Minutes 0-3: Unpack. Place system on table. Connect LiPo battery. Verify display shows "SELF-TEST..." then "READY 0/24".

Minutes 3-5: Run one 24-brick sort as a confidence test. Verify 24/24. If any errors: diagnose quickly, fix if possible.

Minutes 5-8: Reset. Re-load 24 bricks in chute. Verify display shows "READY 0/24" again.

Minutes 8-10: Place operator instructions sheet prominently next to the system. Place engineering notebook beside the instructions.

Minutes 10-12: Do a visual check: all wires secure, no loose parts, belt tension correct, bins empty and in position.

Minutes 12-15: Leave the room. Do NOT hover. The judge operates the system alone.

### 11.3 Operator Instructions (Final Draft)

Title: LEGO Brick Sorter -- Operating Instructions

Step 1: Verify the display reads "READY 0/24". If it does not, disconnect and reconnect the battery cable (orange connector on the right side).

Step 2: Load all 24 LEGO bricks into the vertical tube at the left side of the system. Drop them in one at a time through the funnel opening. All 24 fit in the tube. Order does not matter. The tube will orient them automatically.

Step 3: Press the green START button on the front panel.

Step 4: The display will show "SORTING" and a count that increases as bricks are sorted. The system takes approximately 14 seconds to sort all 24 bricks. Do not touch the system during sorting.

Step 5: When complete, the display shows "COMPLETE 24/24" and a tone plays. The belt stops automatically.

Step 6: Check the bins (labeled on the front):
- Bin 1 (front left): 2x2 Red bricks (expect 6)
- Bin 2 (front center): 2x2 Blue bricks (expect 6)
- Bin 3 (front right): 2x3 Blue bricks (expect 8)
- Bin 4 (end): 2x3 Red bricks (expect 4)

If the display shows "ERROR":
- Press the STOP button (red, next to START).
- Wait 3 seconds for the system to reset.
- The display should return to "READY".
- Remove any visible jams (a brick stuck in the channel or chute).
- Re-load any unsorted bricks and press START to retry.

### 11.4 What Could Go Wrong at Competition (and mitigations)

| Risk | Likelihood | Mitigation |
|------|-----------|-----------|
| Part comes loose during transport | Medium | Pre-competition check. Loctite on critical bolts. |
| LiPo dead on arrival | Low | Charge morning of. Bring a USB power bank as backup for ESP32 (won't power solenoids, but proves logic works). |
| Belt tension changed | Medium | Tensioner has a lock nut. Check and re-tension during setup. |
| Judge loads bricks wrong | None | Self-aligning chute makes incorrect loading physically impossible. |
| Judge presses button during sort | Low | Operator instructions say "do not touch." Firmware ignores START while sorting. STOP halts safely. |
| Color sensor reads wrong under fluorescent lights | Low | Sensor is in an enclosed shroud. Ambient light blocked on all sides. |
| One solenoid fails | Very low | Other 3 bins still sort correctly. Only affected category goes to wrong bin. Spare solenoid in bag for emergency swap (5-minute fix). |

---

## 12. Equations for Engineering Notebook

These equations should appear in Section 4 of the notebook with full derivations.

### Eq 1: Belt Speed from Motor RPM
```
v_belt = (RPM_motor * pi * d_pulley) / 60
d_pulley = 20 teeth * 2mm pitch / pi = 12.73mm
At 600 RPM: v_belt = (600 * pi * 0.01273) / 60 = 0.40 m/s = 400 mm/s (max)
PWM at ~30% duty -> ~120 mm/s target
```

### Eq 2: Inter-Brick Spacing
```
spacing = v_belt * t_release = 120 * 0.333 = 40mm
gap = spacing - brick_length(max) = 40 - 23.7 = 16.3mm (minimum gap, 2x3)
```

### Eq 3: Break-Beam Size Discrimination
```
beam_gap = 19mm
2x3 crosses both beams: t = 19 / 120 * 1000 = 158ms
2x2 crosses only beam 1: 15.8mm < 19mm, beam 2 never triggers
Discrimination: binary, zero ambiguity
```

### Eq 4: Solenoid Push Analysis (with real force curve)
```
JF-0530B 6V: F_initial = 0.5N (at 10mm), F_holding = 5N (at 0mm)
Approximate average force: F_avg ~ 1.5N
Brick mass: 0.003 kg
Average acceleration: a = F/m = 1.5/0.003 = 500 m/s^2
Exit velocity after 10mm stroke: v = sqrt(2 * 500 * 0.01) = 3.16 m/s
Time to clear 7mm (edge of belt): t = 7/3160 = 2.2ms
Forward drift during push: 120 * 0.0022 = 0.26mm (negligible)
```

### Eq 5: Omniveyor Rejection (friction budget)
```
Normal force: 0.003 * 9.81 = 0.029N
Max lateral friction (mu=0.6): 0.018N = 18 millinewtons
Solenoid average force: 1500 millinewtons
Force advantage: 1500 / 18 = 83x
Conclusion: friction-based transport fundamentally inadequate at 3g brick mass.
```

### Eq 6: Power Budget
```
Total average power: 7.6W
Battery capacity: 3S 1300mAh = 14.4Wh
Runtime: 14.4 / 7.6 = 1.9 hours
Sort duration: ~14 seconds
Battery usage per sort: 7.6 * 14/3600 = 0.030 Wh = 0.2% of battery
```

### Eq 7: Color Sensing Sample Count
```
Belt speed: 120 mm/s
Brick length (min): 15.8mm
Sensor FOV (effective): ~8mm
Time in field: (15.8 + 8) / 120 * 1000 = 198ms
Integration time: 24ms
Samples per brick: 198 / 24 = 8 samples (minimum, for 2x2)
```

### Eq 8: Escapement Double-Release Timing
```
Cam lobe arc: 50 degrees at 3 rev/sec
Lobe transit time: (50/360) * (1/3) * 1000 = 46ms
Gravity drop time for next brick: sqrt(2 * 0.0114 / 9.81) * 1000 = 48ms
Blocking edge arrives 2ms before brick fully drops.
Safety margin: adjust lobe arc 5 degrees either way to tune.
```

---

## 13. Delegatable Task Specifications

These are self-contained task descriptions for splitting into Claude Code prompts or teammate assignments.

### TASK: FIRMWARE_CORE
**File:** main.cpp, config.h
**What:** State machine (POWER_ON -> IDLE -> SORTING -> COMPLETE/ERROR). Pin definitions. All constants with units. Serial logging for every state transition. START/STOP button handling with debounce.
**Depends on:** Nothing. This is the skeleton everything else plugs into.
**Test:** Compiles on ESP32. Press START -> serial prints "SORTING". Press STOP -> serial prints "IDLE".

### TASK: FIRMWARE_SELFTEST
**File:** selftest.h/.cpp
**What:** Power-on self-test as described in section 7.5. Tests I2C devices, break-beams, solenoids (pulse), belt motor (short run), stepper (one revolution + StallGuard).
**Depends on:** config.h pin definitions.
**Test:** Unplug TCS34725 -> self-test reports "FAIL: COLOR SENSOR". Block a beam -> self-test reports "FAIL: BEAM X".

### TASK: FIRMWARE_ESCAPEMENT
**File:** escapement.h/.cpp
**What:** TMC2209 UART configuration (current, microstepping, StealthChop, StallGuard). Step/dir control at configurable cadence. Chute-exit beam counting. StallGuard jam detection.
**Depends on:** config.h, TMCstepper library.
**Test:** Call escapement_init(). Call escapement_release(). Stepper turns one revolution silently. Chute beam count increments.

### TASK: FIRMWARE_SENSING
**File:** sensing.h/.cpp
**What:** Beam 1/2 ISRs with microsecond timestamps. Beam 2 timeout for 2x2 detection. TCS34725 continuous sampling triggered by beam 1 event. Classification function. Returns BrickCategory enum.
**Depends on:** config.h, Adafruit_TCS34725 library.
**Test:** Break beam 1 with a 2x3 brick -> serial logs "LARGE". Break with 2x2 -> logs "SMALL". Hold red brick over sensor -> logs "RED". Hold blue -> logs "BLUE".

### TASK: FIRMWARE_DIVERTER
**File:** diverter.h/.cpp
**What:** Per-brick tracking (BrickEvent circular buffer). Timer-based solenoid firing at computed delay after beam 1 event. Bin assignment lookup from BrickCategory. Solenoid GPIO control with on-time limit.
**Depends on:** config.h, sensing module (provides BrickEvent data).
**Test:** Simulate beam 1 event. After computed delay, correct solenoid GPIO goes HIGH for 40ms. Serial logs "FIRE SOL2 for brick 7 (2x2_BLUE)".

### TASK: FIRMWARE_FEEDBACK
**File:** feedback.h/.cpp
**What:** Bin confirmation beam monitoring. After each divert event, watch assigned bin beam for break within 500ms timeout. Update sorted_count and error_count. Trigger ERROR state if error_count exceeds threshold (configurable, default 3).
**Depends on:** config.h, diverter module (provides divert events).
**Test:** Manually break bin beam after solenoid fire -> sorted_count increments. Don't break -> error_count increments.

### TASK: FIRMWARE_DISPLAY
**File:** display.h/.cpp
**What:** SSD1306 OLED driver. Shows state (READY/SORTING/COMPLETE/ERROR), sorted count (N/24), per-bin counts, and error details. Updates at 10Hz (every 100ms) during sort.
**Depends on:** config.h, Adafruit_SSD1306 library.
**Test:** Call display_update() with mock data. Display shows formatted output.

### TASK: FIRMWARE_BELT
**File:** belt.h/.cpp
**What:** L298N PWM control for belt motor. Speed set via PWM duty cycle. Optional Hall-effect speed measurement with closed-loop PID correction.
**Depends on:** config.h.
**Test:** Call belt_start(120). Belt moves at approximately 120mm/s. Call belt_stop(). Belt stops.

### TASK: FIRMWARE_AUDIO
**File:** audio.h/.cpp
**What:** Piezo buzzer tones. Different tones for: self-test pass, sort start, brick sorted (per-bin tones), sort complete (melody), error.
**Depends on:** config.h.
**Test:** Call audio_play(TONE_COMPLETE). Buzzer plays a short melody.

### TASK: CAD_BELT_FRAME
**What:** Belt frame for 20mm GT2 belt, 460mm long. Two side rails, two roller mounts (5mm bore with bearings), belt bed mount (for aluminum bar), motor mount (JGB37-520), idler tensioner (M3 bolt + spring, 10mm travel).
**Output:** STL files, assembly drawing.
**Constraints:** Print on 220x220mm bed. Split into pieces if needed with tongue-and-groove joints.

### TASK: CAD_CHANNEL
**What:** Two channel walls (400mm x 15mm x 3mm). Pusher-side wall: three 24mm x 13mm slots at 160mm, 270mm, 380mm. Bin-side wall: three 30mm x 13mm openings aligned with slots. Sensor bridge at 60mm with 15mm x 15mm hole for TCS34725. All interior edges chamfered 0.5mm.
**Output:** STL files.

### TASK: CAD_CHUTE
**What:** Vertical chute, 22mm x 27mm internal, 310mm tall with flared top (35mm x 40mm, 20mm transition). Bottom section integrates with cam mechanism. NEMA 11 motor mount. Chute exit ramp to belt surface. Support plate with gussets.
**Output:** STL files for chute body (two halves if needed), cam wheel, motor mount, support plate.

### TASK: CAD_SOLENOID_MOUNT
**What:** U-bracket for JF-0530B (13x16x30mm body). Rod centerline at 6mm above belt surface. 5mm adjustment slot along belt axis. M3 mounting to belt frame.
**Output:** STL file. Qty: 3.

### TASK: CAD_BINS
**What:** Four bins per dimensions in section 3.6. Break-beam slot at entrance. Floor angled 10 degrees. Open top.
**Output:** STL files.

### TASK: CAD_SENSOR_SHROUD
**What:** Light-blocking shroud for TCS34725. 20x20x25mm external, fits on sensor bridge spanning channel. Open bottom looks down through 15mm x 15mm hole in channel ceiling. Snug fit around TCS34725 breakout board.
**Output:** STL file.

### TASK: NOTEBOOK_SECTION_4
**What:** Write the Analysis and Decision section (8 pages). Include: decision matrix, force analysis with real solenoid curve, timing analysis at 120mm/s, omniveyor rejection, break-beam geometry, power budget, belt speed tradeoff, and final decision statement.
**Format:** Prose with embedded equations. Hand-drawn or simple diagrams.
**Scoring note:** This is the highest-value section. Judges spend the most time here. Every claim must have a number behind it.

### TASK: OPERATOR_INSTRUCTIONS
**What:** One-page laminated instruction sheet per section 11.3. Include a labeled photo of the system. Test with a naive user before finalizing.

### TASK: ELECTRICAL_SCHEMATIC
**What:** Draw the complete system schematic per section 4.5. Can be hand-drawn (cleaner for notebook) or KiCad. Must show: power distribution, MOSFET circuits with flyback diodes, TMC2209 wiring, L298N wiring, I2C bus, all break-beam circuits with pull-ups, button circuits.

---

## 14. Risk Register (Final)

| # | Risk | Likelihood | Impact | Mitigation | Fallback |
|---|------|-----------|--------|-----------|---------|
| 1 | Brick jams in 22mm channel | Low | High | Sand walls, PTFE tape, verified with test print + real bricks | Widen to 24mm (still self-aligns 2x3). Reprint walls only. |
| 2 | Solenoid return spring too weak | Low | Medium | Test immediately when parts arrive. | Add external compression spring behind face plate. |
| 3 | Color misclassification | Low | High | Enclosed shroud, 8+ samples averaged, huge R/B spectral gap | Slow belt to 80mm/s for 12+ samples. Add second color sensor. |
| 4 | Cam double-release | Medium | High | 50-degree lobe arc, chute exit beam detection | Reduce to 45 degrees. Add mechanical anti-backflow finger (spring-loaded gate above cam). |
| 5 | GT2 belt splice fails | Medium | High | Test splice with 2kg pull before assembly | Switch to hybrid drive (6mm GT2 + 25mm PVC flat belt). |
| 6 | Parts arrive late | Low | High | Order from Amazon (2-3 day) + Adafruit (3-5 day) tonight. No AliExpress. | Begin with what arrives first. CAD + firmware proceed regardless. |
| 7 | Person unavailable for 1+ week | Medium | High | Modular task design: any person can pick up any task from the spec. | Two people can build the full system. Third person is insurance. |
| 8 | ESP32 GPIO phantom interrupts | Low | Medium | Avoided GPIO 36/39. External pull-ups on 34/35. | If issues persist, add RC debounce filter (10k + 100nF) on beam inputs. |
| 9 | Solenoids are 12V not 6V | Medium | Medium | Verify variant in cart before ordering. | Run at 11.1V from LiPo direct. Force will be slightly lower (11.1/12)^2 = 86%. Still adequate. |
| 10 | Notebook incomplete at competition | Low | High (20 pts) | Sections 1-5 done in weeks 1-2. Sections 6-7 in week 3. Section 8 in week 4. Prioritize over polish. | Even a partially complete notebook with strong Section 4 scores well. |
| 11 | System fails during judge operation | Very Low | Very High | Self-test on power-up catches dead subsystems. 20+ successful runs in week 3-4 testing. Error recovery in operator instructions. | Spare solenoid + face plate in bag. 5-minute swap during setup window. |
| 12 | Static electricity causes bricks to stick | Low | Medium | Ground the belt frame to the LiPo negative terminal. Anti-static spray on channel walls if needed. | PTFE tape on walls reduces static buildup. |
| 13 | Chute sways during escapement | Medium | Medium | Vertical support plate with two gussets. All connections bolted tight. | Add a second support plate on the opposite side for a triangulated mount. |

---

## 15. What Wins First Place

This section is strategic, not technical. It addresses the meta-question: what separates 1st place from 2nd and 3rd?

**Every team at nationals has a working sorter.** The baseline is a conveyor that sorts most bricks correctly. That gets you into the top 10. To win, you need:

1. **Perfect reliability.** 24/24 every time. The judges run the system ONCE. If it gets 23/24, you lose to the team that got 24/24. Our entire design philosophy prioritizes this: slower belt speed, wider timing margins, self-aligning chute, self-test, bin confirmation, error recovery instructions.

2. **A notebook that teaches the judges something.** The winning notebook is not a diary ("today we printed parts"). It is a technical document that demonstrates mastery of engineering principles. The omniveyor friction analysis, the solenoid force curve, the timing margin diagrams -- these are things judges will talk about in deliberation. Include them.

3. **Clean code that a judge can read.** The programming structure score is 20 points. Most teams submit spaghetti Arduino sketches. Our modular architecture with doc comments, descriptive names, and a clear state machine will stand out. Print the code and include it in the notebook appendix.

4. **Visible engineering.** The transparent channel wall section lets judges see the sorting mechanism. The OLED display shows real-time state. The per-bin LED indicators (optional) make the sort visually trackable. The piezo buzzer adds audio confirmation. The system feels alive, not just functional.

5. **Robust error handling.** The self-test, the bin confirmation feedback loop, the StallGuard jam detection, the ERROR state with recovery instructions -- these show the team thought about failure, not just success. This is what professional engineers do. Judges recognize it.

6. **The operator instructions work perfectly.** The judge IS the operator. If the instructions are confusing and the judge can't get the system to run, all 60 points for "Solution to Problem" are at risk. Test the instructions with a naive user. Iterate until they are bulletproof.

---

*End of Master Plan V3. This document, when split into task specifications, drives the entire project from order to competition day.*