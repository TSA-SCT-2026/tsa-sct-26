# Master Plan V3 -- Addendum A

**Date:** March 21, 2026
**Applies to:** MASTER_PLAN_V3.md (read that first)

This addendum addresses four open questions and makes significant design changes.
Changes are marked with (CHANGE) for easy diffing against V3.

---

## A1. Escapement-Based Color Sensing (MAJOR CHANGE)

### A1.1 The Idea

Move the TCS34725 color sensor from the belt sensing zone into the escapement mechanism. Read color while the brick is stationary in the chute, BEFORE releasing it onto the belt. Size discrimination also moves to the chute via a single break-beam across the wider chute dimension.

### A1.2 Why This Is Better (from first principles)

The fundamental problem with belt-based color sensing is that the brick is moving. At any belt speed, there is a finite sensing window = brick_length / belt_speed. This creates a direct tradeoff between belt speed and classification quality. Every millisecond of integration time is borrowed from the speed budget.

If the brick is stationary, the sensing window is infinite. The sensor can take 50, 100, 200 samples. The classification becomes trivially reliable. The belt speed becomes completely decoupled from sensing quality.

This is not a minor optimization. It eliminates the entire category of "sensing timing" failure modes and removes the strongest argument for running the belt slowly.

### A1.3 How It Works Mechanically

The brick sits on the cam's flat chord, at the bottom of the chute, waiting to be released. During this dwell time (the ~280 degrees of cam rotation where the body blocks the exit, or ~260ms at 3 rev/sec), the brick is stationary and accessible.

**Color sensing:**
The TCS34725 is mounted on the OUTSIDE of the chute wall, looking through a 12mm x 12mm window cut in the chute wall. The window is positioned so it faces the flat face of the bottom brick as it sits on the cam chord. A small light-blocking hood (15mm deep, black PLA) surrounds the window on the outside, eliminating ambient light. The sensor's onboard white LED illuminates the brick through the window.

The sensor reads the brick's color while it is motionless. At 24ms integration time, 260ms of dwell gives ~10 clean samples from a stationary target. Even at 2.4ms integration time (fastest setting), that is 100+ samples. Noise is near zero because the brick is not moving relative to the sensor.

**Size sensing:**
In the chute, bricks are constrained by the walls. The chute internal cross-section is 22mm x 27mm. The 2x3 brick fills the 27mm dimension (23.7mm + 1.65mm clearance per side). The 2x2 brick only fills 15.8mm of the 27mm dimension (5.6mm clearance per side).

A single IR break-beam crosses the 27mm dimension of the chute at the cam level, positioned at 20mm from the wall (in the 27mm direction). A 2x3 brick (23.7mm in this dimension) blocks the beam. A 2x2 brick (15.8mm) does NOT block it (it only extends to 15.8mm, and the beam is at 20mm). Binary discrimination, exactly like the dual-beam approach on the belt, but with zero timing dependency.

```
CHUTE CROSS-SECTION (looking down):

     <-------- 27mm -------->
     |                       |
     |  [2x3 brick: 23.7mm] |   <- blocks beam at 20mm position
     |  |=================|  |
     |                       |
     |  [2x2 brick: 15.8mm] |   <- does NOT reach beam at 20mm
     |  |===========|        |
     |              ^        |
     |              |        |
     |         beam at 20mm  |
     |                       |
     <-------- 27mm -------->
```

The break-beam emitter is embedded in one chute wall at 20mm from the reference wall. The receiver is in the opposite wall. The beam fires horizontally across the chute. If the bottom brick blocks it: size = 2x3. If not: size = 2x2.

### A1.4 Sensing Sequence (firmware)

```
For each brick release cycle:

1. Brick N settles onto cam chord (after previous release)
2. Size beam reads: blocked = 2x3, clear = 2x2 (instantaneous, single digital read)
3. Color sensor accumulates samples for 100-200ms (during cam dwell)
4. Classification complete: category = f(size, color)
5. Cam rotates: brick N released onto belt
6. Firmware schedules pusher fire for brick N at known delay from release
7. Brick N+1 drops onto cam chord -> repeat from step 2
```

The classification is COMPLETE before the brick touches the belt. The belt's only job is transport. The firmware knows exactly which pusher to fire the moment the brick enters the belt.

### A1.5 What This Eliminates

- Two break-beams on the belt (size discrimination beams): eliminated. Replaced by one beam in the chute.
- Color sensor bridge/shroud on the belt: eliminated. Sensor is on the chute.
- Belt speed as a sensing constraint: eliminated entirely.
- The "color sensor collides with brick" error from V3 section 0.2: cannot happen (sensor is behind a wall).
- The "8 samples at 120mm/s" concern: irrelevant. 50+ samples from stationary brick.
- The beam gap timing calculation: eliminated. Size is a single digital read, not a timing measurement.

### A1.6 What This Adds
- One window cut in the chute wall (12mm x 12mm, at cam level).
- One sensor mount on the chute exterior.
- One light-blocking hood (simple 3D print).
- One break-beam pair in the chute walls at cam level (replaces two on the belt).
- Slightly more complex chute CAD (window + sensor mount + beam holes).

Net component count change: -1 break-beam pair, -1 sensor bridge on belt. The sensor itself is the same part, just mounted differently. This is strictly simpler.

### A1.7 Failure Modes

1. **Sensor window gets dirty/dusty:** the window is inside the chute, protected from the environment. Bricks sliding past may deposit ABS dust. Mitigation: smooth the window edges so bricks don't scrape. The window is below the queue (at the bottom brick only), so only one brick face passes it per cycle.

2. **Brick not seated properly on cam chord:** if the brick is tilted or not fully settled, the color reading may be off-center or include the chute wall. Mitigation: the cam chord flat is designed to seat the brick squarely. The queue weight above pushes the brick down onto the chord. The settling time is 260ms (plenty for a 3g brick to rest).

3. **Size beam misaligned:** if the beam position drifts from the 20mm mark (due to thermal expansion or chute flex), the discrimination threshold shifts. Mitigation: the gap between 15.8mm (2x2) and 23.7mm (2x3) is 7.9mm. The beam has 4mm of margin on each side of the discrimination threshold. Chute flex of 4mm would be a structural failure, not a sensor issue.

4. **Color sensor reads the cam surface instead of the brick:** the cam rotates under the brick. During the dwell period, the cam's flat chord is under the brick, but the brick face presented to the sensor is the brick's side face, not the bottom. The sensor window is in the chute WALL (not the floor), looking at the brick's side. The cam is below the sensor's field of view. Not a concern.

---

## A2. Belt Speed Revision (CHANGE)

### A2.1 Why Speed Now Goes Back Up

V3 dropped belt speed from 200mm/s to 120mm/s specifically because color sensing needed 8+ samples. With sensing moved to the escapement, this constraint vanishes. The belt can run as fast as the pushers can reliably fire.

### A2.2 What Limits Belt Speed Now

1. **Pusher timing window:** face plate width (20mm) + brick length (15.8mm min) divided by belt speed. At 200mm/s: window = 35.8 / 200 * 1000 = 179ms. Solenoid fires for 40ms. Margin: 139ms. Adequate.

2. **Cam-to-belt transition:** brick enters belt with zero forward velocity. Friction accelerates it. At 200mm/s, time to reach belt speed: 0.2 / 5 = 40ms. Distance: 0.2^2 / (2*5) = 4mm. The brick is at full speed within 4mm of entering the belt. The first pusher is 70-80mm downstream. No issue.

3. **Inter-brick collision risk:** at 3 bricks/sec and 200mm/s, spacing is 67mm. Largest brick is 23.7mm. Gap is 43mm. No collision possible.

4. **Mechanical belt reliability:** GT2 belt on GT2 pulleys at 200mm/s is well within spec. GT2 belts in 3D printers run at 300+ mm/s routinely.

### A2.3 Revised Belt Speed

**(CHANGE) Target belt speed: 200 mm/s.**

This gives a total sort time of:
```
Release: 24 / 3 = 8.0s
Drain (last brick to belt end): belt_length / 200 = variable, see section A3
Total: ~9-10s
```

Under 10 seconds for 24 bricks. Fast, visually impressive, and now fully reliable because sensing is decoupled from belt speed. This signals strong engineering because the judges see bricks flying off the belt at speed with perfect accuracy.

### A2.4 Speed Calibration Strategy

Start firmware at 150mm/s for initial testing. Increase in 25mm/s increments during week 3 testing. Lock the speed at the fastest rate that achieves 24/24 in 10 consecutive runs. If 200mm/s works: use it. If 175mm/s is the sweet spot: use that. The firmware constant is one line in config.h. This is exactly the kind of data-driven optimization that belongs in the notebook.

---

## A3. Belt Sizing and the AliExpress Belt (CHANGE)

### A3.1 The Problem with V3's Layout

V3 specifies a 460mm belt run. This requires a closed-loop belt of ~960mm circumference. The AliExpress listing maxes out at 650mm. Even Amazon does not stock 20mm-wide closed-loop GT2 belts above ~650mm. The V3 plan suggested splicing open-ended belt, which works but adds fabrication risk.

### A3.2 The Fix: Shorter Belt (enabled by escapement sensing)

Because sensing no longer happens on the belt, the belt does not need a dedicated sensing zone. The belt is purely for transport: chute exit -> pusher 1 -> pusher 2 -> pusher 3 -> belt end. The minimum belt length is determined by pusher spacing, which is determined by inter-brick spacing.

At 3 bricks/sec and 200mm/s, inter-brick spacing on belt = 200 * 0.333 = 67mm.
Minimum safe pusher spacing: 70mm (so only one brick is at a pusher at a time).
Three pushers at 70mm spacing: 210mm span.
Add 40mm before pusher 1 (chute-to-belt transition zone) and 40mm after pusher 3 (belt end / default bin): total active belt = 290mm.

Required belt circumference: 2 * (290 + 10mm margin per side) + pi * 12.7mm = 2 * 310 + 40 = **660mm**.

That is 10mm over the 650mm max from AliExpress. Options:

**Option 1:** Use the 640mm belt. Active surface = (640 - 40) / 2 = 300mm.
Layout:
- Chute exit to belt: 0mm
- Pusher 1: 75mm
- Pusher 2: 150mm
- Pusher 3: 225mm
- Belt end: 290mm

Pusher spacing: 75mm. Inter-brick: 67mm. Only one brick at a pusher at a time. This works.

**(CHANGE) Order: 20mm GT2 closed-loop belt, 640mm circumference, from AliExpress.**

**Option 2 (backup):** Use the 610mm belt. Active surface = (610 - 40) / 2 = 285mm.
Layout:
- Pusher 1: 70mm
- Pusher 2: 140mm
- Pusher 3: 210mm
- Belt end: 275mm

Pusher spacing: 70mm. Tighter but functional. Order this as backup.

### A3.3 How the Belt Works (physical explanation)

Since you asked for a clear explanation of how the GT2 belt-as-conveyor works:

```
SIDE VIEW:

          [DRIVE PULLEY]            [IDLER PULLEY]
            (motor)                  (tensioner)
           /  20-tooth  \          /  20-tooth  \
          /   GT2 pulley  \        /  GT2 pulley  \
    -----/=================\------/================\-----
    ^^^^^ FLAT SIDE UP (this is the transport surface) ^^^^^
         bricks ride here, pushed by friction

    -----\=================/------\================/-----
          \   TOOTHED SIDE  /      \  TOOTHED SIDE /
           \  engages       /        \  sits on    /
            \ pulley teeth /          \ pulley    /
             \____________/            \_________/

Cross-section of GT2 belt:
    ___________________________
    |  flat rubber side (top)  |  <-- bricks ride on this
    |__________________________|
    |\/\/\/\/\/\/\/\/\/\/\/\/\/|  <-- GT2 teeth (bottom), engage pulleys
    ----------------------------
```

The GT2 belt has teeth on one side and a flat rubber surface on the other. The teeth mesh with the GT2 pulley teeth -- this is "positive drive," meaning the belt cannot slip on the pulleys. The motor turns the drive pulley, which advances the belt at a speed determined exactly by motor RPM and pulley diameter. The flat rubber side faces up and carries bricks.

Bricks sit on the flat rubber surface. Friction between ABS plastic and rubber holds the brick in place as the belt moves. The brick weighs 3g, the friction coefficient is ~0.5, so the friction force is ~15mN -- easily enough to transport a stationary brick at constant speed.

The belt loops around both pulleys in a closed loop. The top surface (flat side up) is the active transport surface. The bottom surface (flat side down, teeth exposed) is the return path. The idler pulley has a tensioning mechanism (spring + M3 bolt) to keep the belt taut.

**Between the pulleys,** the belt needs support so it does not sag under the brick's weight. A flat aluminum bar (3mm thick, 20mm wide, 280mm long) sits between the pulleys, just below the belt's top surface. The belt slides over this bar. The bar is covered with PTFE tape to reduce friction between belt and bar.

**The channel walls** sit on either side of the belt, on top of the belt frame rails. They constrain the brick laterally so it travels in a straight line along the belt.

### A3.4 Drive Belt + Flat Bed Alternative (explained, for reference)

If the GT2-as-conveyor approach fails in testing (belt too slippery, teeth cause bumps, etc.), the fallback is:

```
SIDE VIEW (hybrid drive):

    [FLAT PVC BELT - 22mm wide, smooth rubber]
    ===== bricks ride on this smooth surface =====
    | sits on crowned rollers, driven by friction |

    [GT2 6mm DRIVE BELT - hidden below/beside]
    ===== engages GT2 pulleys, drives the rollers =====
    | motor -> GT2 belt -> drive roller -> PVC belt |
```

In this approach:
1. The motor turns a small GT2 pulley via a 6mm GT2 belt (positive drive, no slip).
2. The GT2 belt drives a roller (the drive roller has a GT2 pulley on one end and a rubber-coated cylinder on the other).
3. The rubber-coated cylinder is the drive roller for the flat PVC belt.
4. The flat PVC belt wraps around the drive roller and an idler roller, driven by friction between the rubber coating and the PVC.
5. Bricks ride on the PVC belt's smooth surface.

Pros: smoother transport surface, better for bricks.
Cons: the PVC belt can slip on the rollers (speed becomes uncertain), two belts to tension, more complex assembly.

This is the fallback. Start with the GT2-as-conveyor approach. If bricks bump on the teeth or the surface is too slippery, switch to hybrid.

---

## A4. Revised System Layout (CHANGE)

With the shorter belt and escapement-based sensing, the layout changes significantly.

```
     <-------------- 610mm footprint ------------------>

     [CHUTE 310mm tall]
     [with color sensor + size beam]
     [NEMA 11 + cam at bottom]
            |
            v (brick exits cam onto belt)
     [== 22mm CHANNEL (GT2 belt, 640mm loop) ===]
       |          |           |           |
       |      [PUSHER 1]  [PUSHER 2]  [PUSHER 3]
       |      bin:2x2red  bin:2x2blue bin:2x3blue
       |                                  |
       |                            [BELT END]
       |                         bin:2x3red (DEFAULT)
       v
     [BASE PLATE 610mm x 610mm]
```

Active belt length: ~290mm (vs 460mm in V3).
Total system length: chute base (~50mm) + belt frame (~320mm) + bin 4 (~40mm) = 410mm. Fits in 610mm with 200mm to spare for electronics bay.

### A4.1 Revised Pusher Positions (from belt start)

```
Pusher 1: 75mm   (2x2 red)
Pusher 2: 150mm  (2x2 blue)
Pusher 3: 225mm  (2x3 blue)
Belt end: 290mm  (2x3 red, default)
```

### A4.2 Revised Timing (at 200 mm/s)

```
Chute exit beam to pusher 1: 75 / 200 * 1000 = 375ms
Chute exit beam to pusher 2: 150 / 200 * 1000 = 750ms
Chute exit beam to pusher 3: 225 / 200 * 1000 = 1125ms
Chute exit beam to belt end: 290 / 200 * 1000 = 1450ms

Pusher timing window (2x2 brick, 20mm face plate):
  (15.8 + 20) / 200 * 1000 = 179ms
  Solenoid on-time: 40ms. Margin: 139ms.

Total sort time:
  Release: 24 / 3 = 8.0s
  Drain: 290 / 200 = 1.45s
  Total: ~9.5s
```

Under 10 seconds. Fast and reliable.

---

## A5. Revised Pin Assignment (CHANGE)

With sensing moved to the escapement, the two belt-mounted size beams (GPIO 34, 35) are replaced by a single chute-mounted size beam. The belt break-beam pins are freed up.

```c
// === SENSING (at escapement/chute) ===
#define PIN_SIZE_BEAM    GPIO_NUM_34   // Single beam in chute: blocked=2x3, clear=2x2
#define PIN_CHUTE_EXIT   GPIO_NUM_35   // Chute exit confirmation beam
// TCS34725 on I2C bus (GPIO 21 SDA, GPIO 22 SCL) -- unchanged

// === BELT BREAK-BEAMS (only bin confirmations now) ===
#define PIN_BIN1_BEAM    GPIO_NUM_16
#define PIN_BIN2_BEAM    GPIO_NUM_17
#define PIN_BIN3_BEAM    GPIO_NUM_5
#define PIN_BIN4_BEAM    GPIO_NUM_18

// Everything else unchanged from V3 section 4.2
```

Net change: two beam pins freed (were used for belt size beams). One fewer break-beam pair needed. Total break-beam pairs: 1 (chute size) + 1 (chute exit) + 4 (bin confirms) = 6. Same as V3, but simpler mounting and no timing-dependent beam gap measurement.

---

## A6. Revised Firmware Sensing Module (CHANGE)

The sensing module becomes dramatically simpler.

```c
// sensing.h -- REVISED for escapement-based sensing

enum BrickCategory {
    CAT_2x2_RED  = 0,
    CAT_2x2_BLUE = 1,
    CAT_2x3_BLUE = 2,
    CAT_2x3_RED  = 3,
    CAT_UNCERTAIN = 4
};

struct SenseResult {
    bool is_large;           // true = 2x3, false = 2x2
    float r_ratio;           // R / (R+G+B)
    uint8_t sample_count;    // how many color samples were taken
    BrickCategory category;  // final classification
};

// Called once per escapement cycle, BEFORE releasing the brick.
// Blocks for up to 200ms while accumulating color samples.
// Returns immediately for size (single digital read).
SenseResult sense_brick_in_chute() {
    SenseResult result;

    // Size: single digital read, no timing involved
    result.is_large = (digitalRead(PIN_SIZE_BEAM) == LOW);  // LOW = beam blocked = 2x3

    // Color: accumulate samples for 150ms (plenty of dwell time)
    uint32_t r_sum = 0, g_sum = 0, b_sum = 0;
    uint16_t r, g, b, c;
    result.sample_count = 0;
    uint32_t start = millis();

    while (millis() - start < 150) {
        tcs.getRawData(&r, &g, &b, &c);
        if (c > 100) {  // valid reading (not saturated or zero)
            r_sum += r;
            g_sum += g;
            b_sum += b;
            result.sample_count++;
        }
        // 24ms integration time -> ~6 samples in 150ms
        // Could use shorter integration for more samples
    }

    if (result.sample_count < 3) {
        result.category = CAT_UNCERTAIN;
        return result;
    }

    result.r_ratio = (float)r_sum / (float)(r_sum + g_sum + b_sum);

    // Classify
    bool is_red;
    if (result.r_ratio > 0.50f) is_red = true;
    else if (result.r_ratio < 0.30f) is_red = false;
    else { result.category = CAT_UNCERTAIN; return result; }

    if (!result.is_large && is_red)   result.category = CAT_2x2_RED;
    if (!result.is_large && !is_red)  result.category = CAT_2x2_BLUE;
    if (result.is_large && !is_red)   result.category = CAT_2x3_BLUE;
    if (result.is_large && is_red)    result.category = CAT_2x3_RED;

    return result;
}
```

**What disappeared:**
- Break-beam ISRs for size discrimination (no ISR needed, single digital read)
- Beam gap timing calculation (eliminated)
- Timeout-based 2x2 detection (eliminated)
- On-belt color sampling triggered by beam events (eliminated)
- The entire "sensing window vs belt speed" tradeoff (eliminated)

This is a massive firmware simplification. The sensing module goes from ~200 lines of timing-critical ISR code to ~50 lines of straightforward sequential code. The code score benefits because the judges can read and understand it immediately.

---

## A7. Revised Escapement Sequence (CHANGE)

The escapement now has a sense-then-release cycle:

```
1. Cam at dwell position (blocking edge holds queue, flat chord holds bottom brick)
2. sense_brick_in_chute() called. Runs for ~150ms. Returns category.
3. Category stored in firmware's per-brick tracking queue.
4. Cam rotates: lobe pushes bottom brick onto belt. Takes ~46ms.
5. Chute exit beam confirms brick released.
6. Next brick drops onto cam chord. Takes ~48ms.
7. Cam returns to dwell. Total cycle time: 150 + 46 + 48 + margin = ~260ms.
8. Repeat from step 2.

Effective release rate: 1 / 0.260 = 3.8 bricks/sec.
Can be reduced to 3/sec with 333ms cycle by extending color sampling to 200ms.
```

Note: the sensing time (150ms) is NOT wasted time. It happens during the cam dwell, which is necessary anyway (the cam must dwell to hold the queue while the previous brick clears). In V3, this dwell time was idle. Now it is productive.

---

## A8. AliExpress Ordering Strategy (CHANGE)

### A8.1 General Approach

AliExpress is fine for most parts. Shipping times vary (typically 7-20 days for standard, 3-7 for expedited). Since April 1 is not a hard deadline, order everything tonight and begin firmware + CAD work immediately. Parts arrive in waves; build in the order they arrive.

**Critical path items** (block physical testing, order with fastest shipping):
- GT2 belt (640mm, 20mm wide): needed for belt assembly
- Solenoids (6V variant!): needed for pusher testing
- NEMA 11 stepper: needed for escapement testing

**Non-critical items** (firmware/CAD work proceeds without them):
- Buck converters, L298N, wire, perfboard, resistors, MOSFETs
- Break-beam sensors (can test firmware with jumper wires simulating beams)
- OLED display
- Pulleys (can 3D print temporary ones)

### A8.2 Reconciled Order List (AliExpress + Amazon/Adafruit)

Using your existing AliExpress finds where possible:

| Item | Source | Qty | Cost (est) | Notes |
|------|--------|-----|-----------|-------|
| GT2 belt 20mm width, 640mm closed loop | AliExpress (your link) | 1 | $18 | ORDER IMMEDIATELY. This is the critical path part with longest ship time. Also order a 610mm as backup. |
| GT2 belt 20mm width, 610mm closed loop | AliExpress (same listing) | 1 | $18 | Backup belt length |
| GT2 20T pulley, 5mm bore | AliExpress (your link: 2GT Synchronous Wheel) | 2 | $3 | Verify 20mm belt width compatibility. "Width 10mm" option will NOT work. If no 20mm option: 3D print. |
| JGB37-520 6V 600RPM gearmotor | AliExpress (your link) | 1 | $10 | Verify 6V variant selected |
| IR break-beam 3mm pairs (4-pack) | AliExpress (your link: Infrared Circuit Breaker) | 2 packs | $14 | 8 pairs total. Need 6 installed + 2 spares. |
| Solenoid JF-0530B | AliExpress (your link: 0520 0530 Electric Magnet) | 4 | $17 | SELECT 6V VARIANT and 0530 size specifically. If listing does not offer 6V, order 12V and run on 11.1V LiPo direct (86% force, still adequate). |
| Torsion springs 0.4mm | AliExpress (your link) | 1 pack | $3 | Backup for solenoid return assist |
| Perfboard 5x7cm | AliExpress (your link) | 1 pack | $2 | MOSFET driver board + distribution board |
| L298N motor driver | AliExpress (your link) | 1 | $1 | Belt motor driver |
| Silicone wire 22AWG | AliExpress (your link) | 1 pack | $3 | Wiring |
| LM2596 buck converters | AliExpress (your link) | 2 | $3 | One set to 6V, one to 5V |
| TMC2209 stepper driver | AliExpress (your link) | 1 | $3 | Verify UART-capable |
| TCS34725 color sensor | AliExpress (your link) | 1 | $4 | The one with onboard LED |
| NEMA 11 stepper | AliExpress (your link) | 1 | $12 | Note: $6.58 shipping adds to cost |
| Solder wire | AliExpress (your link) | 1 | $2 | Lead-free |

**Subtotal AliExpress: ~$113**

| Item | Source | Qty | Cost (est) | Notes |
|------|--------|-----|-----------|-------|
| LiPo 3S 11.1V 1300mAh XT60 | Amazon | 1 | $15 | Ships 2-3 days. Need fast for testing. |
| LiPo balance charger | Amazon | 1 | $10 | Ships 2-3 days |
| SSD1306 OLED 0.96" I2C | Amazon | 1 | $4 | Ships 2-3 days |
| IRLZ44N MOSFET (10-pack) | Amazon | 1 | $8 | Ships 2-3 days. Solenoid drivers. |
| 1N4007 diodes (100-pack) | Amazon | 1 | $4 | Ships 2-3 days. Flyback + spares. |
| Resistor assortment (10k, 1k, 220ohm) | Amazon | 1 | $6 | Ships 2-3 days |
| Passive piezo buzzers | Amazon | 1 | $3 | Ships 2-3 days |
| M2+M3 screw assortment | Amazon | 1 | $8 | Ships 2-3 days |
| 5mm steel rod 100mm x2 | Amazon | 1 | $4 | Roller shafts |
| 625ZZ bearings (5mm ID) | Amazon | 4 | $5 | Roller bearings |

**Subtotal Amazon: ~$67**

**Hardware store (today):**
| Item | Cost |
|------|------|
| PTFE tape | $2 |
| 3mm aluminum flat bar 300mm | $3 |
| Clear acrylic 3mm (small piece) | $4 |

**Subtotal hardware store: ~$9**

**Grand total: ~$189**

### A8.3 Budget Trimming (to hit $150)

We are ~$39 over. Cuts in priority order:

1. Drop the 610mm backup belt (-$18). If the 640mm belt works, no backup needed. If it breaks, order a replacement (7-day turnaround is bad but survivable in week 2-3).
2. Drop the clear acrylic (-$4). Use sanded PLA or skip the transparent wall section.
3. Drop the steel rods (-$4). Print roller shafts in PLA at 100% infill, or use M5 bolts as shafts.
4. Print GT2 pulleys instead of buying (-$3). Parametric GT2 generators exist for OpenSCAD. At low loads this is fine.

**After cuts: ~$160.** Close enough. The LiPo + charger ($25) is the single most expensive item. If you already have a compatible LiPo from another project, that saves $25 immediately.

### A8.4 Items You Listed That Are NOT Needed

- **Heatsinks + thermal tape:** Not needed. No component runs hot enough to need heatsinking. The solenoids fire for 40ms at a time. The TMC2209 has thermal shutdown built in. The L298N might get warm but has its own heatsink on the board.
- **Slot-type optocoupler speed sensor ($0.95):** This was for belt speed measurement. Optional, and the A3144 Hall sensor is more robust for this if needed (but with GT2 positive drive, speed measurement is not critical). Skip unless budget allows.

---

## A9. Revised Timing Constants (for firmware config.h)

```c
// === Belt ===
constexpr float BELT_SPEED_MM_S = 200.0f;

// === Pusher positions (mm from belt start / chute exit) ===
constexpr float PUSHER1_POS_MM = 75.0f;
constexpr float PUSHER2_POS_MM = 150.0f;
constexpr float PUSHER3_POS_MM = 225.0f;
constexpr float BELT_END_MM = 290.0f;

// === Timing from chute exit beam to each pusher (ms) ===
constexpr float T_TO_PUSHER1_MS = PUSHER1_POS_MM / BELT_SPEED_MM_S * 1000.0f;  // 375ms
constexpr float T_TO_PUSHER2_MS = PUSHER2_POS_MM / BELT_SPEED_MM_S * 1000.0f;  // 750ms
constexpr float T_TO_PUSHER3_MS = PUSHER3_POS_MM / BELT_SPEED_MM_S * 1000.0f;  // 1125ms

// === Escapement ===
constexpr uint32_t SENSE_DURATION_MS = 150;    // color sampling time per brick
constexpr uint32_t RELEASE_CYCLE_MS = 280;     // total cycle: sense + release + settle
constexpr float RELEASE_RATE_HZ = 1000.0f / RELEASE_CYCLE_MS;  // ~3.6 bricks/sec

// === Solenoid ===
constexpr uint32_t SOLENOID_ON_MS = 40;

// === Color thresholds (calibrate with real bricks) ===
constexpr float RED_THRESHOLD_HIGH = 0.50f;    // above this = red
constexpr float RED_THRESHOLD_LOW = 0.30f;     // below this = blue
// between = uncertain, flag error
```

---

## A10. What Speeds Up the Build (AliExpress Shipping Context)

Since parts arrive in waves over 7-20 days, the build order is driven by arrival, not by V3's week-by-week plan. Here is what can happen BEFORE any parts arrive:

**Week of March 22-31 (waiting for parts):**
- ALL firmware modules except hardware testing: state machine, sensing logic, diverter logic, display module, self-test skeleton, audio tones. All of this compiles on a bare ESP32 with mock inputs.
- ALL CAD work: belt frame, channel walls, chute, cam wheel, solenoid mounts, bins, sensor shroud, base plate layout. Everything can be designed, printed, and dry-fitted without electronics.
- Engineering notebook sections 1-4 (Problem, Research, Ideation, Analysis). These require zero hardware.
- Electrical schematic drawing.
- Operator instructions draft.

**When first parts arrive (likely early-mid April):**
- Begin hardware integration immediately: wire sensors, test actuators, tune parameters.
- Follow V3's week 2-4 schedule from that point forward.

This means the effective build window starts NOW for firmware and CAD, not April 1. The parts arrival date only affects when you start physical integration.

---

## A11. Revised Equations for Notebook

With the design changes, update these equations in the notebook:

### Eq 3 (REPLACES V3 Eq 3): Chute-Based Size Discrimination

```
Chute internal width (27mm dimension):
  2x3 brick in this dimension: 23.7mm. Blocks beam at 20mm from wall.
  2x2 brick in this dimension: 15.8mm. Does NOT reach beam at 20mm.

Discrimination: single digital read. No timing involved.
Margin: beam position at 20mm. 2x3 edge at 23.7mm (3.7mm past beam).
  2x2 edge at 15.8mm (4.2mm short of beam).
  Total gap between sizes at beam position: 7.9mm.
  Beam alignment tolerance: +/- 3.7mm (half the gap). Very robust.
```

### Eq 7 (REPLACES V3 Eq 7): Color Sensing Sample Count

```
Sensing time: 150ms (during cam dwell, brick is stationary)
Integration time: 24ms
Samples: 150 / 24 = 6 samples minimum from a STATIONARY target.
At 2.4ms integration: 150 / 2.4 = 62 samples.

Compare to V3 belt-based sensing: 8 samples from a MOVING target.
Escapement sensing: 6-62 samples from a STATIONARY target.
Signal quality: dramatically better per sample (no motion blur, no position jitter).
```

### NEW Eq 9: Compact Belt Layout Validation

```
Belt circumference: 640mm
Pulley pitch diameter: 20 teeth * 2mm / pi = 12.73mm
Active surface: (640 - pi * 12.73) / 2 = (640 - 40) / 2 = 300mm

Pusher spacing: 75mm
Inter-brick spacing on belt: 200mm/s * 0.280s = 56mm (at 3.6 bricks/sec)
Minimum gap between consecutive bricks: 56 - 23.7 = 32.3mm
Bricks per pusher position at once: 1 (56mm spacing < 75mm pusher spacing)

Total sort time: 24 * 0.280 + 290/200 = 6.72 + 1.45 = 8.2 seconds
```

---

## A12. Summary of All Changes from V3

| V3 Spec | Addendum A Spec | Why |
|---------|----------------|-----|
| Color sensor on belt at 60mm, 15mm above surface | Color sensor on chute wall, reads stationary brick | Eliminates belt speed vs sensing tradeoff |
| Two break-beams on belt (19mm gap) for size | One break-beam in chute (single digital read) for size | Simpler, no timing dependency |
| Belt speed: 120 mm/s | Belt speed: 200 mm/s (calibrate upward from 150) | Sensing decoupled from belt speed |
| Belt length: 460mm active, ~960mm circumference | Belt length: 290mm active, 640mm circumference | Enabled by removing sensing zone from belt |
| Belt: open-ended 20mm GT2, spliced | Belt: 640mm closed-loop 20mm GT2 from AliExpress | No splice risk, standard product |
| 8 break-beam pairs total (2 size + 1 chute + 4 bin + 1 spare) | 6 break-beam pairs (1 size + 1 chute + 4 bin) | Fewer sensors, simpler wiring |
| Total sort time: ~14s | Total sort time: ~9s | Faster belt + no sensing delay on belt |
| Order mostly Amazon | Order mostly AliExpress, time-critical from Amazon | Budget optimization |

All other V3 specs (channel width, bin assignment, solenoid drivers, cam design, firmware architecture, notebook strategy, competition day plan) remain unchanged.

---

*End of Addendum A. Read with MASTER_PLAN_V3.md.*