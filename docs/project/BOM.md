# Bill of Materials

**Source of truth:** `docs/project/BOM.xlsx` is the authoritative purchase record.

Integrity rule:
- Ordered sections in this markdown may summarize only rows that exist in `docs/project/BOM.xlsx`.
- If an item is not in the spreadsheet, this file must treat it as `not yet ordered`, `already owned`, or `unconfirmed`.
- Do not turn inferred or recommended parts into ordered purchases.

Current architecture:
- States build uses a `NEMA 17` conveyor, conveyor-mounted sensing station, and MG995/MG996-class servo rotary chute selector
- Size sensing is undecided and must not be recorded as a purchased active subsystem until the selected sensor traces to the spreadsheet or existing inventory
- Color sensing uses the BOM-backed `TCS3200 GY-31` module with shroud-installed calibration

Current receipt status:
- AliExpress orders recorded in `docs/project/BOM.xlsx` are received.
- Amazon orders recorded in `docs/project/BOM.xlsx` are treated as ordered. The user reports Amazon orders excluding the most recent pulley and belt order are in hand.
- Measurement is allowed for received hardware now.
- Any most-recent order not yet represented in `docs/project/BOM.xlsx` must be added to the spreadsheet before this markdown calls it ordered.

## Receipt checks

**Solenoid recorded in the purchase log, now inactive for states:**
`0530 Linear Solenoid Electromagnet 12V` in `docs/project/BOM.xlsx`.

Important note:
- Repo design documents previously assumed a `6V` JF-0530B variant.
- The spreadsheet purchase log currently points to a `12V` `0530` listing instead.
- Treat the spreadsheet as hardware truth until the received part is measured and photographed.
- This is archived inventory for the previous nationals candidate, not the active states release architecture.

**Color sensor recorded in the purchase log:**
`TCS3200 GY-31 Color Sensor Module` in `docs/project/BOM.xlsx`.

Important note:
- Repo design documents previously assumed `TCS34725`.
- The spreadsheet purchase log currently points to `TCS3200 GY-31`.
- Treat the spreadsheet as hardware truth until the received module is verified.
- Final firmware pin map and calibration flow must match the actual received sensor.

**Distance sensor architecture note:**
- ToF or distance sensing is only a candidate for the undecided states size sensor.
- `docs/datasheet/sensing/distance_sensor/` contains the reference CAD assets for that family.
- `docs/project/BOM.xlsx` does not yet contain a corresponding ToF purchase row, so this markdown must not mark ToF modules as ordered yet.
- Update the spreadsheet first, then update the ordered-items section here in the same change.

**Timing-stage purchases:**
- Timing-stage parts remain useful if the selected conveyor uses the GT2 path.
- The states build should not buy or require NEMA11 selector interface hardware.
- Recently ordered pulley and belt items should be added to `docs/project/BOM.xlsx` before this file treats them as ordered.

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

Current gap audit: use `SHORT_TODO.md`, `docs/ARCHITECTURE.md`, and current states-build decisions as the buying gate. Treat stale mechanism text elsewhere in the repo as archived prior art, not fixed truth.

| Item | Classification | Why |
|------|----------------|-----|
| Downloaded conveyor CAD | Needed now, no purchase if free | The states plan depends on importing or adapting a proven NEMA17 mini conveyor before custom CAD expands. |
| Size sensor final choice | Open decision | Break-beam sensors are recorded in the spreadsheet. ToF modules are not currently marked ordered in this markdown unless the spreadsheet is updated. |
| Operator controls | Needed from existing stock or hardware store | Use a real evaluator-facing start button and power switch if available. Do not rely on tiny PCB tact switches for the final interface. |
| Wood or printed frame stock | Needed from school stock, scrap, or hardware store | The active frame is not 2020 extrusion. Confirm available wood or printed structure before frame CAD drifts. |
| M3 assembly hardware | Needed from hardware store or existing stock | Servo mount, sensor brackets, shroud, and bins need common screws, nuts, and washers. Current purchase log only shows an `M2` kit. |
| Chute surface improvement | Optional after angle test | Only add tape or liner if real-brick chute angle testing shows sticking. |
| Recent pulleys and GT2 belt | Pending spreadsheet update | User reports a most recent order for pulleys and GT2 belt. Add real rows to `docs/project/BOM.xlsx` before marking them ordered here. |

Do not add NEMA11 selector hubs, release-gate parts, or chamber switches for the states build unless the architecture changes again.

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
| 0530 solenoids | Present from purchase log, archived inventory for the previous design |
| Small SG90-class hobby servo | User-confirmed on hand, optional accessory only |
| MG995 or MG996R-class servo | User-confirmed on hand, active servo chute selector actuator |
| 608ZZ bearings | Available inventory if the chosen conveyor uses them |

## Spreadsheet Totals

| Source | Spreadsheet total |
|--------|-------------------|
| Amazon Mar 25 | `$149.52` |
| Amazon Mar 31 | `$62.87` |
| AliExpress Mar 25 | `$174.50` |
| **Grand total** | **`$386.89`** |
