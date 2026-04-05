# Bill of Materials

**Source of Truth:** `docs/BOM.xlsx` is the authoritative purchase record. Everything recorded there is already ordered. This markdown file is the clean summary of what is ordered plus the few remaining timing-stage purchases.

Current architecture:
- Conveyor drive is an off-axis `NEMA 17` timing-belt stage with a supported roller shaft
- Selector drive is `NEMA 11 stepper`

---

## Receipt checks

**Solenoid currently recorded in purchase log:**
`0530 Linear Solenoid Electromagnet 12V` in `docs/BOM.xlsx`.

Important note:
- Repo design documents previously assumed a 6V JF-0530B variant.
- The spreadsheet purchase log currently points to a 12V 0530 listing instead.
- Treat the spreadsheet as hardware truth until the received part is measured and photographed.
- Before wiring, verify actual coil voltage, stroke, body size, and current draw on the received unit.

**Conveyor and selector motion drivers:**
Use dedicated stepper drivers for both motion axes. Conveyor uses NEMA 17.
Selector uses NEMA 11. Reduce hold current when static and provide airflow near the drivers.

**Timing-belt conveyor stage:**
- Use a toothed belt between the NEMA 17 and the supported drive roller.
- The only actionable buying items left in this markdown are the timing-stage parts below.
- Use a guarded, adjustable tension path so ratio changes do not require a full rebuild.

**PCF8574 I2C expander:**
Default address 0x20. No conflict with SSD1306 (0x3C) or other reserved I2C peripherals.
Shares existing SDA/SCL bus (GPIO 21/22). Adds 8 digital I/O pins for buffer/future use.
Arduino library: PCF8574 by xreef (PlatformIO). Powers from 3.3V.

**Color sensor currently recorded in purchase log:**
`TCS3200 GY-31 Color Sensor Module` in `docs/BOM.xlsx`.

Important note:
- Repo design documents previously assumed `TCS34725`.
- The spreadsheet purchase log currently points to `TCS3200 GY-31`.
- Treat the spreadsheet as hardware truth until the received module is verified.
- Final firmware pin map and calibration flow must match the actual received sensor.

**Fuse:**
Inline blade fuse holder on LiPo positive lead before power switch. 5A automotive
blade fuse. Replaceable. Bring 10 spare fuses to competition. If it blows you know
immediately and can swap in 10 seconds.

**Remaining timing-stage purchases:**

Current CAD assumptions are:
- `GT2 / 2mm pitch`
- `6mm belt width`
- `16T` motor pulley
- `32T` driven pulley
- `78mm` nominal pulley center distance
- `8mm` tension-adjust travel

That geometry works best if you buy the two standard closed-loop belt lengths that straddle the modeled pitch length instead of betting on a single custom size.

| Part | Qty | Exact spec / search term | Buy note |
|------|-----|---------------------------|---------|
| GT2 closed-loop timing belt, short | 2 | `GT2 closed loop belt 100T 200mm 6mm wide 2mm pitch` | One install candidate plus one spare. Fits if the printed stage lands slightly short. |
| GT2 closed-loop timing belt, long | 2 | `GT2 closed loop belt 104T 208mm 6mm wide 2mm pitch` | One install candidate plus one spare. Fits if the printed stage lands slightly long. |
| GT2 motor pulley | 2 | `GT2 16 tooth 5mm bore 6mm belt width aluminum pulley double flange` | One install plus one spare. Clamp or dual set-screw style is preferred. |
| GT2 driven pulley | 2 | `GT2 32 tooth 5mm bore 6mm belt width aluminum pulley double flange` | One install plus one spare. Match belt pitch and width to the motor pulley. |
| Supported drive shaft | 2 | `5mm precision ground shaft rod 200mm` | One install plus one spare. Cut to final length after frame landings are fixed. |
| Drive-shaft bearings | 4 | `MR115ZZ flanged bearing 5x11x4` | Two active bearings for the supported shaft plus two spare. This is in addition to the idler-bearing count already recorded in `docs/BOM.xlsx`. |
| Shaft collars | 4 | `5mm clamp shaft collar aluminum` | Two active collars for axial retention plus two spare. |

Preferred tension strategy:
- Use the printed slotted `NEMA17` mount as the active tension adjuster.
- Do not buy a separate belt tensioner unless packaging later proves the slot can not cover the full adjustment range.

Timing-stage subtotal is provisional and is not included in the grand total yet.

---

## AliExpress (already ordered via `docs/BOM.xlsx`)

| # | Part | Qty | Est. Cost | Validated spec / ordering note |
|---|------|-----|-----------|-------------------------------|
| 1 | 0530 Linear Solenoid Electromagnet 12V | 3 | $17 | Spreadsheet purchase record item. Measure actual stroke, body size, and current draw on receipt before wiring. |
| 2 | NEMA 11 stepper 34mm | 2 | $12 | Selector axis plus spare. 4-wire bipolar, 5mm shaft, 1.8 deg/step. |
| 3 | NEMA17 Stepper Motor 17HS4401S | 1 | $10 | Conveyor feed axis. 42BYGH class, 1.5A, 4-lead. |
| 4 | TMC2209 stepper driver | 3 | $6 | Two active axes plus one spare. UART-capable variant. |
| 5 | IR break-beam 3mm (4-pair pack) | 3 | $21 | 12 pairs: 2 size beams + 1 entry + 1 home + 4 bin confirm + 4 spare. |
| 6 | TCS3200 GY-31 color sensor module | 2 | $8 | Spreadsheet purchase record item. Firmware interface and calibration must be updated to the received module. |
| 7 | LM2596 buck converter | 3 | $5 | 2 used (motor rail trim and logic rail) + 1 spare. |
| 8 | IRLZ44N MOSFET | 10 | $8 | Logic-level N-channel. 1 used for solenoid driver. 9 spare. |
| 10 | 22AWG silicone wire assorted | 1 | $5 | Permanent wiring. |
| 11 | Perfboard 5x7cm | 2 packs | $4 | MOSFET driver board + spare. |
| 12 | JST-XH connector kit (2-pin and 4-pin) | 1 | $6 | Transport connectors, polarized. |
| 13 | Compression spring assortment (5-8mm OD) | 1 | $5 | Belt idler tensioner spring. Various rates. |
| 14 | Tension spring assortment (4-6mm OD, 0.3-0.4mm wire) | 2 | $8 | Platform return spring AND lever return spring. Light rates only. |
| 15 | Sub-miniature snap-action micro-switch (Omron D2F equiv) | 10 | $6 | Pin-plunger actuator type. Stop wall (1), selector home (1), shelf-level (1), 7 spare. |
| 16 | TMC2209 heatsink (aluminum, adhesive) | 3 | $3 | For both active stepper drivers plus one spare. |
| 17 | 5mm shaft hub (aluminum, set-screw or clamp) | 2 | $5 | Selector chute mounting. Non-negotiable. 1 primary + 1 spare. |
| 18 | 3mm OD polished steel rod 200mm | 4 | $6 | Platform hinge rod. Cut to 25mm sections with Dremel. |
| 19 | MR115ZZ flanged ball bearing (5mm ID x 11mm OD x 4mm) | 4 | $6 | Idler roller bearings. 2 used + 2 spare. |
| 20 | NEMA 11 motor mount bracket (metal) | 2 | $8 | Selector axis bracket. 1 used + 1 spare. Do not rely on printed PLA alone. |
| 21 | Heat shrink tubing assortment | 1 | $4 | |
| 22 | M2 + M3 heat-set insert assortment | 1 | $5 | All printed structural parts at screw locations. |
| 23 | M2 + M3 screw assortment + M3 nylon locknuts | 1 | $8 | Lever pivot: M3 x 12mm + nylon locknut. All fasteners. |
| 24 | Small rubber foot pads (3mm, self-adhesive, 3M or equiv) | 1 pack | $4 | Stop wall damper, system feet. |
| 25 | PCF8574 I2C GPIO expander module | 3 | $3 | Address 0x20 default. No bus conflict. 8 extra digital I/O. 1 primary + 2 spare. |
| 26 | 30mm 5V brushless fan | 2 | $4 | Stepper driver thermal management. Runs from 5V rail. 1 primary + 1 spare. |
| 27 | Inline blade fuse holder + 5A automotive blade fuses (10-pack) | 1 | $4 | LiPo main line protection. Replaceable at competition. |

**AliExpress subtotal: ~$171**

---

## Amazon (already ordered via `docs/BOM.xlsx`)

| # | Part | Qty | Est. Cost | Validated spec / ordering note |
|---|------|-----|-----------|-------------------------------|
| 1 | LiPo 3S 11.1V 1300mAh XT60 | 2 | $30 | Competition + backup. **3S NOT 2S**. |
| 2 | LiPo 3S balance charger | 1 | $15 | Required for safe charging. |
| 3 | SSD1306 OLED 0.96" I2C 128x64 | 1 | $5 | Primary display. Address 0x3C. No GPIO conflict. |
| 4 | Waveshare 1.69" IPS LCD (ST7789V2, SPI) | 1 | $12 | Attempt SPI at wiring time. Fallback to SSD1306. |
| 5 | Neoprene rubber strip 1/8" x **3/4"** x 10ft | 1 | $8 | Belt. 19mm wide. **NOT 1 inch**. Search: TORRAMI neoprene strip. |
| 6 | A3144 Hall sensor 3-pack | 1 | $4 | Optional diagnostic speed logging only. Not required for conveyor correctness. |
| 7 | 3mm neodymium disc magnets 10-pack | 1 | $4 | Idler roller. 2 used + 8 spare. |
| 8 | PTFE tape 1/2" (plumber tape) | 2 | $6 | Chute walls, belt channel, belt bed. |
| 9 | UHMW tape 1/2" | 1 | $10 | Platform top surface. Non-negotiable for clean brick drop. |
| 10 | 3mm aluminum flat bar 300mm | 1 | $5 | Belt bed support. Do not substitute with printed PLA. |
| 11 | Loctite Ultragel flexible CA glue | 1 | $8 | Belt splice, rubber pads, assembly. Flexible grade only. |
| 12 | 1N4007 diode 100-pack | 1 | $7 | Flyback on solenoid. 1 used, 99 spare. |
| 13 | 100uF 25V electrolytic capacitors 10-pack | 1 | $5 | TMC2209 VM cap, 6V rail cap. |
| 14 | 1000uF 25V electrolytic capacitors 5-pack | 1 | $6 | Logic 5V rail bulk cap near ESP32. |
| 15 | 4.7k resistor 100-pack | 1 | $5 | I2C pull-ups (SDA/SCL). |
| 16 | 10k resistor 100-pack | 1 | $5 | Pull-ups for input-only GPIO pins (34, 35, 36, 39). |
| 17 | 1k resistor 100-pack | 1 | $5 | MOSFET gate resistors, TMC2209 UART series resistor. |
| 18 | Passive piezo buzzers | 3 | $5 | 1 used + 2 spare. |
| 19 | Braided cable sleeve 4mm (1m) | 1 | $6 | Wire bundle management. |
| 20 | Zip tie assortment | 1 | $4 | |
| 21 | Velcro cable ties | 1 | $6 | Reusable, for areas needing rework access. |
| 22 | ESP32 heatsink (small aluminum or copper) | 2 | $5 | Optional thermal insurance for sustained runs. |
| 23 | Isopropyl alcohol 99% 500ml | 1 | $6 | Cleaning all surfaces before assembly. |
| 24 | Rocker switch 15A 12V DC (illuminated or plain) | 1 | $6 | Main system power on/off. Wires inline after fuse. Bring user's existing LED for panel indicator. |

**Amazon subtotal: ~$198**

---

## Hardware store (already ordered or stock on hand)

| Part | Est. Cost | Notes |
|------|-----------|-------|
| Sandpaper 220 and 400 grit | $5 | Smooth printed channel interiors. |

**Hardware store subtotal: ~$5**

---

## Already owned (verify before ordering)

| Part | Check |
|------|-------|
| ESP32 DevKit | Present and functional |
| Breadboard + jumper wires | Present (bringup only, not in final build) |
| PLA filament | Confirmed at school printer |
| Soldering iron + solder | Present |
| LEGO bricks | 24 total: 6 red 2x2, 6 blue 2x2, 4 red 2x3, 8 blue 2x3 |
| LED indicator | Present (user confirmed) |

---

## Total

| Source | Est. Cost |
|--------|-----------|
| AliExpress | ~$171 |
| Amazon | ~$198 |
| Hardware store | ~$5 |
| **Total** | **~$374** |

---

## Do not order

| Part | Why not |
|------|---------|
| L298N | Not used in the active dual-stepper architecture. |
| Torsion springs | Not used. Lever mechanism uses only tension + compression springs. |
| Servo motors | Not used anywhere. |
| A4988 stepper driver | TMC2209 only. Quieter, more efficient, has StallGuard. |
| 2S LiPo | Requires 3S (11.1V) for 6V rail headroom via LM2596. |
| Metal lever arm hardware | Lever arm is printed PLA. Forces are trivial. |
| Additional solenoids beyond 4 | 4 ordered, 1 used, 3 spare. |
| Metal flywheel for selector indexer | Stepper + TMC2209 is smooth enough. Flywheel increases inertia and worsens stop accuracy. |
