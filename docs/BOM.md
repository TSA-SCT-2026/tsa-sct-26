# Bill of Materials

Order tonight. AliExpress: 7-20 day lead time. Amazon: 2-3 days.

---

## AliExpress (order now, critical path items first)

| Part | Qty | Est. Cost | Notes |
|------|-----|-----------|-------|
| GT2 belt 20mm wide, 640mm closed loop | 1 | $18 | Main belt. SELECT 640mm, 20mm width. Do not order 6mm or 10mm wide. |
| JF-0530B solenoid | 4 | $17 | SELECT 6V variant. Same listing sells 12V and 24V. 3 used, 1 spare. |
| NEMA 11 stepper 34mm | 1 | $12 | 4-wire, 5mm shaft, 1.8 deg/step. |
| JGB37-520 gearmotor | 1 | $10 | SELECT 6V, 600 RPM. Same listing sells 7-960 RPM. 6mm D-shaft. |
| IR break-beam 3mm (4-pack) | 2 packs | $14 | 8 pairs total. Need 6 installed + 2 spare. |
| GT2 20-tooth pulley 5mm bore | 2 | $3 | Belt drive and idler. Verify 20mm belt width compatible. |
| TCS34725 color sensor | 1 | $4 | Must have onboard white LED. |
| TMC2209 stepper driver | 1 | $3 | Verify UART-capable variant. |
| LM2596 buck converter | 2 | $3 | One to 6V (belt+solenoids), one to 5V (logic). |
| L298N motor driver | 1 | $1 | Belt motor PWM and direction control. |
| Silicone wire 22AWG assorted | 1 | $3 | Permanent wiring. |
| Perfboard 5x7cm | 1 pack | $2 | MOSFET driver board. |
| Torsion springs 0.4mm | 1 pack | $3 | Spring return assist on pushers. |

**AliExpress subtotal: ~$93**

---

## Amazon (order with AliExpress session)

| Part | Qty | Est. Cost | Notes |
|------|-----|-----------|-------|
| LiPo 3S 11.1V 1300mAh XT60 | 1 | $15 | Main power. Must be 3S (11.1V), NOT 2S (7.4V). |
| LiPo 3S balance charger | 1 | $10 | Required for safe charging. |
| IRLZ44N MOSFET | 10 | $8 | Logic-level MOSFET for solenoid drivers. 3 used + spares. |
| 1N4007 diodes | 100 | $4 | Flyback diodes on solenoids + spares. |
| Resistor assortment (10k, 1k, 220 ohm) | 1 | $6 | Pull-ups for break-beams, gate resistors. |
| Passive piezo buzzers | 2 | $3 | Audio feedback. |
| M2 + M3 screw assortment | 1 | $8 | Frame and mount fasteners. |
| Display (SSD1306 OLED 0.96" I2C) | 1 | $4 | Status display. Must have I2C interface. |

**Amazon subtotal: ~$58**

---

## Hardware store

| Part | Est. Cost | Notes |
|------|-----------|-------|
| 3mm aluminum flat bar, 300mm length | $3 | Belt bed support. Do not 3D print this. |
| PTFE tape (plumber's tape) | $2 | Low-friction surface on belt bed. |

**Hardware store subtotal: ~$5**

---

## Already owned (verify quantities)

| Part | Check |
|------|-------|
| ESP32 DevKit | present and functional |
| SR5150 or 1N4007 flyback diodes | at least 3 on hand |
| 100uF electrolytic capacitors | at least 3 (TMC2209 VM, 6V rail, 5V rail) |
| 4.7k resistors | at least 2 (I2C pull-ups if needed) |
| Breadboard + jumper wires | for bringup only |
| PLA filament | confirmed available |
| Isopropyl alcohol | for cleaning before assembly |
| LEGO bricks | 24 total: 6 red 2x2, 6 blue 2x2, 4 red 2x3, 8 blue 2x3 |

---

## Total estimate

| Source | Est. Cost |
|--------|-----------|
| AliExpress | ~$93 |
| Amazon | ~$58 |
| Hardware store | ~$5 |
| **Total** | **~$156** |

Over the $150 budget by ~$6. To cut: drop the second break-beam pack if spares are acceptable to skip (-$7).

---

## Do not order

| Part | Why not |
|------|---------|
| 2S LiPo (7.4V) | Design requires 6V rail with headroom from 11.1V. 2S is too low. |
| Any TT gearmotor | Only available at 1:48 ratio. Too slow under load. |
| A4988 stepper driver | TMC2209 handles higher speeds, runs cooler, has StallGuard. |
| SSD1306 display | Monochrome. Display needs color for brick animation. Use SSD1306 only if color display fails to arrive. |
| 400mm GT2 belt | Wrong length. Design uses 640mm closed loop. |
| Lever arm hardware (pivots, long rods) | Design uses direct solenoid push. No lever. |
| Taper channel parts from V1/V2 plan | Design change removed taper channel. Channel is straight 22mm internal. |
