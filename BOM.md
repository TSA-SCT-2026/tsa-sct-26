# Bill of Materials

Nothing has been ordered as of March 18 2026. AliExpress lead time is 10-15 days. Order today.

---

## AliExpress (order first -- longest lead time)

| Part | Search term | Qty | Est. cost |
|------|-------------|-----|-----------|
| JF-0530B solenoid 5V | JF-0530B solenoid | 3 | $6-9 |
| NEMA 11 stepper motor | NEMA 11 stepper 28mm | 1 | $5-8 |
| A4988 stepper driver module | A4988 stepper driver | 2 | $2-3 |
| TT gearmotor 6V | TT motor gearmotor 6V | 2 | $3-4 |
| L298N motor driver | L298N module | 1 | $1-2 |
| LM2596 buck converter | LM2596 step down | 2 | $2-3 |
| GT2 belt 400mm closed loop 6mm | GT2 400mm belt 6mm | 2 | $3-4 |
| GT2 20-tooth pulley 5mm bore | GT2 20 tooth pulley 5mm | 4 | $3-4 |
| M3 hardware kit | M3 bolt nut standoff kit | 1 | $3 |
| M2 hardware kit | M2 bolt nut kit | 1 | $2 |
| Torsion spring assortment | torsion spring assortment small | 1 | $3-4 |
| 100uF 25V electrolytic capacitor | 100uf 25v electrolytic capacitor | 10 | $1 |
| Small aluminum heatsink kit | aluminum heatsink kit small | 1 | $2-3 |
| Thermal adhesive tape | thermal adhesive tape | 1 | $2 |
| 2S LiPo 7.4V 2000mAh | 2S lipo 2000mah 7.4v | 1 | $8-10 |
| 2S LiPo balance charger | 2S balance charger | 1 | $5-6 |

---

## Adafruit (ships fast, order same day as AliExpress)

| Part | Part # | Qty | Cost |
|------|--------|-----|------|
| IR break-beam 3mm pair | #2168 | 4 packs | ~$16 |
| TCS34725 RGB color sensor | #1334 | 1 | ~$7 |

4 packs of #2168 = 8 pairs. 6 needed (2 size sensing + 4 bin confirmation). 2 spare.

---

## Display -- pick one, order same day

| Option | Size | Cost | Notes |
|--------|------|------|-------|
| Pimoroni 1.3in ST7789 | 240x240 | $10-12 | best panel quality, best ESP32 library support |
| Waveshare 1.69in ST7789 | 240x280 | $8-10 | slightly larger screen |
| Generic AliExpress ST7789 | 240x240 | $4-6 | works, QC varies |

Do not order SSD1306. It is monochrome. The animation requires color.

---

## Local (same day, any hardware or grocery store)

| Part | Where | Cost | Purpose |
|------|-------|------|---------|
| Isopropyl alcohol | pharmacy | ~$3 | cleaning contacts and prints |
| Foam sheet or felt | dollar store | ~$2 | bin liners to absorb brick impact |

---

## Already owned (verify before ordering duplicates)

- ESP32 DevKit
- 1N4007 flyback diodes
- Breadboard and jumper wires
- Heat shrink tubing
- PLA filament (school printer)
- DC bench supply (development only, not for demo)

---

## Notes

**Why 2 GT2 belts and 4 pulleys?** One belt and two pulleys for the main conveyor. Spares in case of a print-induced tension issue or belt damage during assembly.

**Why 2 A4988 drivers?** One spare. These are destroyed by wiring mistakes (missing capacitor, power applied in wrong order). Have a backup before integration day.

**Why 2 TT gearmotors?** One drives the belt. One spare. They are cheap and failure during calibration would cost a week of lead time.

**Foam or felt for bins?** Bricks arrive at 200mm/s and bounce out of empty hard bins. Foam lining absorbs the energy. A printed downward ramp inside each bin is an alternative that costs nothing extra -- design it into the bin CAD and skip the foam.

---

## Total estimated cost

$80-100 out of pocket
