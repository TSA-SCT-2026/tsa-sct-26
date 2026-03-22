# Bill of Materials

Order tonight. AliExpress: 7-20 day lead time. Amazon: 2-3 days.

---

## AliExpress (order now, critical path items first)

| Part | Qty | Est. Cost | Notes |
|------|-----|-----------|-------|
| JF-0530B solenoid | 4 | $17 | SELECT 6V variant. Same listing sells 12V and 24V. 3 used, 1 spare. |
| NEMA 11 stepper 34mm | 1 | $12 | 4-wire, 5mm shaft, 1.8 deg/step. |
| JGB37-520 gearmotor | 1 | $10 | SELECT 6V, 600 RPM. Same listing sells 7-960 RPM. 6mm D-shaft. |
| IR break-beam 3mm (4-pack) | 2 packs | $14 | 8 pairs total. Need 6 installed + 2 spare. |
| TCS34725 color sensor | 1 | $4 | Must have onboard white LED. |
| TMC2209 stepper driver | 1 | $3 | Verify UART-capable variant. |
| LM2596 buck converter | 2 | $3 | One to 6V (belt+solenoids), one to 5V (logic). |
| L298N motor driver | 1 | $1 | Belt motor PWM and direction control. |
| Silicone wire 22AWG assorted | 1 | $3 | Permanent wiring. |
| Perfboard 5x7cm | 1 pack | $2 | MOSFET driver board. |
| JST-XH connector kit (2-pin and 4-pin) | 1 | $6 | Wire bundles for transport disassembly. Polarized, snap-lock. |
| Torsion springs 0.4mm | 1 pack | $3 | Spring return assist on pushers. |

**AliExpress subtotal: ~$78**

---

## Amazon (order with AliExpress session)

| Part | Qty | Est. Cost | Notes |
|------|-----|-----------|-------|
| LiPo 3S 11.1V 1300mAh XT60 | 1 | $15 | Main power. Must be 3S (11.1V), NOT 2S (7.4V). |
| LiPo 3S balance charger | 1 | $10 | Required for safe charging. |
| IRLZ44N MOSFET | 10 | $8 | Logic-level MOSFET for solenoid drivers. 3 used + spares. |
| Resistor assortment (10k, 1k, 220 ohm) | 1 | $6 | Pull-ups for break-beams, gate resistors. |
| Passive piezo buzzers | 2 | $3 | Audio feedback. |
| M2 + M3 screw assortment | 1 | $8 | Frame and mount fasteners. |
| Waveshare 1.69in IPS LCD (ST7789V2, 240x280, SPI) | 1 | $12 | Color display. GPIO 13 SPI CS conflict must be resolved at wiring time -- see ELECTRICAL.md. |
| SSD1306 OLED 0.96" I2C (128x64) | 1 | $4 | Fallback display if SPI CS conflict cannot be resolved. I2C address 0x3C, shares GPIO 21/22 with no conflict. |
| Neoprene rubber strip 1/8" x 3/4" x 10ft | 1 | $8 | Transport belt. 3/4 inch = 19mm. Do NOT order 1 inch -- too wide for 22mm channel. Search: TORRAMI neoprene strip. Cut to ~700mm, splice with 20mm tapered overlap. |
| A3144 Hall sensor (3-pack) | 1 | $2 | Belt speed feedback. Digital output, active-low. 1 used + spares. |
| 3mm neodymium disc magnets (10-pack) | 1 | $2 | 2 glued on idler roller rim, 180 degrees apart. |

**Amazon subtotal: ~$80**

### Optional insurance (~$6, only if neoprene friction fails during day-1 test)

| Part | Est. Cost |
|------|-----------|
| GT2 6mm closed-loop belt 150mm + two 6mm bore 20-tooth GT2 pulleys | ~$4 |
| Heat-shrink tubing 25mm diameter (already in wire kit likely) | ~$2 |

---

## Hardware store

| Part | Est. Cost | Notes |
|------|-----------|-------|
| 3mm aluminum flat bar, 300mm length | $3 | Belt bed support. Do not 3D print this. |
| PTFE tape (plumber's tape) | $2 | Low-friction surface on belt bed. |
| Loctite Ultragel (flexible CA glue) | $5 | Belt splice and general assembly. Flexible - does not crack under flexion at the splice. |

**Hardware store subtotal: ~$10**

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
| AliExpress | ~$78 |
| Amazon | ~$80 |
| Hardware store | ~$10 |
| **Total** | **~$168** |

~$18 over the $150 budget. To cut: skip second break-beam pack (-$7), skip Ultragel if CA glue is on hand (-$5). Optional insurance (~$6) is separate. The SSD1306 ($4) is a display fallback that eliminates the GPIO 13 conflict risk -- worth keeping.

---

## Do not order

| Part | Why not |
|------|---------|
| 2S LiPo (7.4V) | Design requires 6V rail with headroom from 11.1V. 2S is too low. |
| Any TT gearmotor | Only available at 1:48 ratio. Too slow under load. |
| A4988 stepper driver | TMC2209 handles higher speeds, runs cooler, has StallGuard. |
| SSD1306 display (second unit) | One unit is on order as the display fallback. Do not order a second. |
| GT2 belt of any width | Transport belt is neoprene rubber strip. GT2 (toothed) on smooth rollers is not positive drive and not friction drive - it is worse than either. Do not use. |
| GT2 toothed pulleys (unless using fallback) | Rollers are smooth cylinders. Toothed pulleys only needed for the GT2-motor-to-roller fallback, which requires day-1 friction test to fail first. |
| Lever arm hardware (pivots, long rods) | Design uses direct solenoid push. No lever. |
| Taper channel parts from V1/V2 plan | Design change removed taper channel. Channel is straight 22mm internal. |
