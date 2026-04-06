# Bill of Materials

**Source of truth:** `docs/project/BOM.xlsx` is the authoritative purchase record.

Integrity rule:
- Ordered sections in this markdown may summarize only rows that exist in `docs/project/BOM.xlsx`.
- If an item is not in the spreadsheet, this file must treat it as `not yet ordered`, `already owned`, or `unconfirmed`.
- Do not turn inferred or recommended parts into ordered purchases.

Current architecture:
- Conveyor drive is an off-axis `NEMA 17` timing-belt stage with a supported roller shaft
- Selector drive is `NEMA 11` stepper

Current receipt status:
- AliExpress orders recorded in `docs/project/BOM.xlsx` are received.
- Amazon orders recorded in `docs/project/BOM.xlsx` are not yet received.
- Measurement is allowed for received AliExpress hardware now.
- Amazon-dependent measurements stay blocked until Amazon delivery.

## Receipt checks

**Solenoid recorded in the purchase log:**
`0530 Linear Solenoid Electromagnet 12V` in `docs/project/BOM.xlsx`.

Important note:
- Repo design documents previously assumed a `6V` JF-0530B variant.
- The spreadsheet purchase log currently points to a `12V` `0530` listing instead.
- Treat the spreadsheet as hardware truth until the received part is measured and photographed.
- Before wiring, verify actual coil voltage, stroke, body size, and current draw on the received unit.

**Color sensor recorded in the purchase log:**
`TCS3200 GY-31 Color Sensor Module` in `docs/project/BOM.xlsx`.

Important note:
- Repo design documents previously assumed `TCS34725`.
- The spreadsheet purchase log currently points to `TCS3200 GY-31`.
- Treat the spreadsheet as hardware truth until the received module is verified.
- Final firmware pin map and calibration flow must match the actual received sensor.

**Timing-stage purchases:**
- The actionable buying items left today are the timing-stage parts, selector interface hardware, operator controls, low-friction liner, micro-switch kit, IO-expander insurance, and hardware-store fasteners.
- Engineering refactor direction: prefer the simplest standard timing parts that preserve speed and accuracy. The baseline is now `20T/20T`, with an optional `16T` experiment pulley for later speed-up testing.
- `MR85` remains the active support-bearing family for the current roller subsystem. No bearing reorder is required unless bench testing proves otherwise.

## Ordered Items From `docs/project/BOM.xlsx`

### Amazon Mar 25

Spreadsheet trace: `Amazon Mar 25` tab and `Summary` row for March 25.

| Part | Qty | Trace | Notes |
|------|-----|-------|-------|
| Throzzik 4700uF 25V Electrolytic Capacitor (10pcs) | 1 | `Amazon Mar 25` | `4700uF 25V 16x25mm` |
| Zeee 3S Lipo Battery 2200mAh 50C XT60 (2 Pack) | 1 | `Amazon Mar 25` | `11.1V 2200mAh 50C Shorty Pack XT60` |
| ISDT 608AC Lipo Balance Charger | 1 | `Amazon Mar 25` | `8A 200W 1-6S LiPo/Life/NiMH AC/DC XT60` |
| Waveshare 2inch Mini LCD Screen | 1 | `Amazon Mar 25` | `240x320 IPS ST7789VW SPI 262K RGB` |
| Hosyond ESP32 Dev Board (3 Pack) | 1 | `Amazon Mar 25` | `USB-C WiFi+BT Dual Core CP2102 Arduino IDE` |

Order total from spreadsheet: `$149.52`

### Amazon Mar 31

Spreadsheet trace: `Amazon Mar 31` tab and `Summary` row for March 31.

| Part | Qty | Trace | Notes |
|------|-----|-------|-------|
| BDS-HOME Spring Assortment Kit (200pcs) | 1 | `Amazon Mar 31` | `200pcs 20 sizes zinc plated compression and extension springs` |
| MAIYUM 63-37 Solder Wire 0.8mm 50g | 3 | `Amazon Mar 31` | `63-37 tin/lead 0.8mm 50g rosin core` |
| 18 AWG Wire 2-Pack Red/Black 25FT | 1 | `Amazon Mar 31` | `18AWG CCA stranded automotive 25FT per color red/black 2-pack` |
| LM2596 DC-DC Buck Converter (5-Pack) | 1 | `Amazon Mar 31` | `3.0-40V to 1.5-35V step down 5pcs` |

Order total from spreadsheet: `$62.87`

### AliExpress Mar 25

Spreadsheet trace: `AliExpress Mar 25` tab and `Summary` row for March 25.

| Part | Qty | Trace | Notes |
|------|-----|-------|-------|
| TMC2209 Silent Stepper Motor Driver | 3 | `AliExpress Mar 25 / 8210696713975240` | `TMC2209 replaces TMC2208/A4988` |
| ACP4020 40mm Axial Cooling Fan | 3 | `AliExpress Mar 25 / 8210696713805240` | `40x40x20mm DC12V Double Ball No RGB` |
| NEMA 11 Stepper Motor 1.8deg 34mm | 2 | `AliExpress Mar 25 / 8210696713745240` | `2-phase 4-lead 1.8deg 34mm high torque` |
| Raspberry Pi Heatsink Set (3pcs) | 1 | `AliExpress Mar 25 / 8210696713665240` | `Adhesive pure aluminum 3pcs` |
| 1N4007 Rectifier Diode (10pcs) | 1 | `AliExpress Mar 25 / 8210696713645240` | `1A 1000V DO-41 10pcs` |
| Braided PET Cable Sleeve | 1 | `AliExpress Mar 25 / 8210696713955240` | `Black 2m ID 6mm flame retardant` |
| Braided PET Cable Sleeve | 1 | `AliExpress Mar 25 / 8210696713925240` | `Black 3m ID 4mm flame retardant` |
| Braided PET Cable Sleeve | 1 | `AliExpress Mar 25 / 8210696713925240` | `Black 5m ID 4mm flame retardant` |
| Nylon Braid Sleeving Tight | 1 | `AliExpress Mar 25 / 8210696714075240` | `Black 5m 6mm flame retardant nylon` |
| M2 Hex Socket Cap Screw Set (482pcs) | 1 | `AliExpress Mar 25 / 8210696713905240` | `M2 DIN912 12.9 Allen 482pcs black` |
| PET Expandable Braided Sleeving | 1 | `AliExpress Mar 25 / 8210696713725240` | `Black 10m 2mm high density nylon mesh` |
| 1/4W Metal Film Resistor Kit (300pcs) | 1 | `AliExpress Mar 25 / 8210696713885240` | `300pcs 30 kinds 1% - 1K/10K/100K/220ohm/1M` |
| PTFE Thread Seal Tape 4M | 1 | `AliExpress Mar 25 / 8210696713705240` | `4M roll oil-free sealing tape` |
| Ceramic Capacitor 100nF 50V (100pcs) | 1 | `AliExpress Mar 25 / 8210696713865240` | `50V 100nF 0.1uF 104 100pcs` |
| TCS3200 GY-31 Color Sensor Module | 2 | `AliExpress Mar 25 / 8210696713785240` | `TCS230/TCS3200 color recognition module` |
| Biochemical Filter Foam Pad | 2 | `AliExpress Mar 25 / 8210696713765240` | `50x12x2cm 25PPI large hole black` |
| MR85ZZ Ball Bearing (10pcs) | 1 | `AliExpress Mar 25 / 8210696713585240` | `5x8x2.5mm MR85RS 10pcs blue seals` |
| Neoprene Rubber Gasket Strip | 1 | `AliExpress Mar 25 / 8210696713565240` | `2000x15x3mm anti-vibration neoprene` |
| Neoprene Rubber Gasket Strip | 1 | `AliExpress Mar 25 / 8210696713545240` | `2000x25x3mm anti-vibration neoprene` |
| Nylon Cable Ties (100pcs) | 1 | `AliExpress Mar 25 / 8210696714015240` | `Black 3x100mm 100pcs self-locking` |
| IR Break Beam Sensor (4 Pairs) | 2 | `AliExpress Mar 25 / 8210696713845240` | `3mm LED 3-5V DC 100cm 4 pairs per set` |
| NEMA17 Stepper Motor 17HS4401S | 1 | `AliExpress Mar 25 / 8210696713995240` | `42BYGH 1.5A 42N.cm 4-lead XH2.54 1pcs` |
| Inline Fuse Holder 18AWG (2pcs) | 1 | `AliExpress Mar 25 / 8210696713825240` | `Mini ATC/ATO 12V 5A 18AWG 2pcs` |
| PCB Prototype Board Double Sided (5pcs) | 1 | `AliExpress Mar 25 / 8210696713625240` | `5pcs 3x7cm blue double sided` |
| IRLZ44N N-Channel MOSFET TO-220 (10pcs) | 1 | `AliExpress Mar 25 / 8210696713605240` | `IRLZ44N TO-220 10pcs` |
| 0530 Linear Solenoid Electromagnet 12V | 3 | `AliExpress Mar 25 / 8210696714055240` | `0530 push/pull open frame DC 12V` |
| 608ZZ Deep Groove Ball Bearing (5pcs) | 1 | `AliExpress Mar 25 / 8210696714035240` | `8x22x7mm carbon steel 5pcs` |

Order total from spreadsheet: `$174.50`

## Still Needed And Not Yet Ordered

Current gap audit: use `SHORT_TODO.md`, `docs/ARCHITECTURE.md`, and current refactor decisions as the buying gate. Treat stale mechanism text elsewhere in the repo as prior art, not fixed truth.

| Item | Classification | Why |
|------|----------------|-----|
| GT2 closed-loop timing belt, short | Needed now for next Amazon order | `100T`, `200mm`, `6mm` wide, `2mm` pitch. You confirmed you have no GT2 stock, and the current timing-stage geometry wants one install candidate plus one spare. |
| GT2 closed-loop timing belt, long | Needed now for next Amazon order | `104T`, `208mm`, `6mm` wide, `2mm` pitch. This is the second install candidate plus one spare around the modeled center distance. |
| GT2 drive pulleys | Needed now for next Amazon order | Prefer a standard `20T` / `20T` pair with `5mm` bore and `6mm` belt width. This keeps the timing stage simple, easy to source, and still comfortably inside the target conveyor speed range. |
| GT2 experiment pulley | Optional add-on for the next Amazon order | One extra `16T`, `5mm` bore, `6mm` belt pulley gives a future `1.25x` speed-up path without reopening the whole timing stage. It is not required to start CAD or first-pass prototype validation. |
| Supported drive shaft | Needed now for next Amazon order | `5mm` precision shaft stock. You confirmed you have no shaft stock on hand, and the supported roller architecture requires it. |
| Shaft collars | Needed now for next Amazon order | `5mm` clamp shaft collars. You confirmed you have none on hand, and the supported shaft needs axial retention. |
| Start-gate actuator | Already owned | Use the owned small hobby servo first. Keep the heavier servo as backup only if the queue load proves too high. |
| Release-gate actuator | Already owned | The purchase log already contains `0530` solenoids, and the release architecture is now based on that family. |
| Selector hub | Needed now for next Amazon order | Buy a rigid `5mm` flange-mount hub with M3 face mounting. This is the cleanest way to mount the selector chute to the NEMA11 shaft without relying on a printed bore. |
| Selector home and chamber switch kit | Needed now for next Amazon order | Buy a small pack of micro-switches. The base architecture now requires one selector home switch and one stop-wall seat switch. Keep extras for release-reset truth instrumentation if prototype evidence demands it. |
| Low-friction liner material | Needed now for next Amazon order | Buy adhesive-backed UHMW tape. The purchase log only shows PTFE thread seal tape, which is not the production wear surface. |
| Belt support path | CAD, not purchased hardware | Freeze the first-pass conveyor bed as an integrated printed flat support path. Do not add separate metal bed stock unless real testing proves it is necessary. |
| Belt splice material | Needed now from hardware store or next Amazon order | The transport belt is raw neoprene strip, not a pre-made loop. Buy rubber contact cement or another proven neoprene splice adhesive so the strip can become a scarf-spliced endless belt. |
| Operator controls | Needed now for next Amazon order | One real momentary start button and one real power switch. Do not rely on tiny PCB tact switches for evaluator-facing controls. |
| M3 assembly hardware | Needed now from hardware store or existing stock | The roller clamps, selector mounting, and general prototype work need common `M3` screws, nuts, and washers. Current purchase log only shows an `M2` kit. |
| M5 idler axle hardware | Needed now from hardware store or existing stock | One `M5` bolt, washers, and a locknut remain the simplest idler axle solution. |
| Selector motor bracket | Needed now for next Amazon order | The `ST-M3` NEMA11 bracket is a clean low-risk shortcut compared with fabricating a one-off metal bracket. |
| IO expander board | Needed now for next Amazon order | Buy one as spare insurance. Keep it out of the base architecture until the final IO map proves it is necessary. |
| Rubber stop pad or foot | Needed now from hardware store or existing stock | Use one as the chamber stop-wall damper. Extra pieces can double as anti-slip feet if they do not hurt footprint or leveling. |
| Base plate stock | Needed now from hardware store, school stock, or scrap inventory | Confirm a `610mm x 610mm x 6mm` base plate material before frame packaging drifts past the footprint rule. |
| Separate belt tensioner hardware | Not actually needed under current architecture | Current plan is to use the slotted `NEMA17` mount first. Reopen only if packaging later proves the slot can not cover the adjustment range. |

No items currently fall into `Needed later but not urgent` for purchase planning. Anything structural that is still missing belongs in the list above, not in a vague later bucket.

## Already Owned / Stock On Hand

| Part | Status |
|------|--------|
| ESP32 DevKit | Present and functional |
| Breadboard and jumper wires | Present for bringup only |
| PLA filament | Confirmed at school printer |
| Soldering iron and solder | Present |
| LEGO bricks | 24 total: 6 red 2x2, 6 blue 2x2, 4 red 2x3, 8 blue 2x3 |
| LED indicator | Present |
| 6x6 tactile switches | User-confirmed on hand, internal-use only, not evaluator-facing controls |
| 0530 solenoids | Present from purchase log, active release actuators for the retracting-support design |
| Small SG90-class hobby servo | User-confirmed on hand, active start-gate prototype actuator |
| MG995 or MG996R-class servo | User-confirmed on hand, backup only if the small servo stalls under queue load |
| 608ZZ bearings | Treat as unused stock for now. No active subsystem depends on them |

## Spreadsheet Totals

| Source | Spreadsheet total |
|--------|-------------------|
| Amazon Mar 25 | `$149.52` |
| Amazon Mar 31 | `$62.87` |
| AliExpress Mar 25 | `$174.50` |
| **Grand total** | **`$386.89`** |
