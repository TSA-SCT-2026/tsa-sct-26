# Bill of Materials

AliExpress lead time is 10-15 days (expect arrival March 28 - April 2). Adafruit ships in 2-5 days. Order immediately.

---

## AliExpress (order first: longest lead time)

| Part | Search term | Qty | Est. cost | Notes |
|------|-------------|-----|-----------|-------|
| JF-0530B solenoid 5V | JF-0530B solenoid | 3 | $6-9 | plow actuation |
| NEMA 11 stepper motor | NEMA 11 stepper 28mm 4-wire bipolar | 1 | $5-8 | escapement drive; 28mm body, 5mm shaft, 1.8 deg/step, 200 steps/rev |
| TMC2209 stepper driver module | TMC2209 stepper driver | 2 | $3-5 | 1 primary, 1 spare; use in standalone mode (no UART); handles 2000+ sps cleanly, silent stepping, better thermal than A4988 |
| JGB37-520 gearmotor 6V | JGB37-520 6V 300RPM | 2 | $8-12 | 1 belt drive, 1 spare; confirm 300 or 520 RPM variant at 6V; 6mm D-shaft fits standard GT2 pulleys; do NOT order 30 or 100 RPM variant |
| L298N motor driver | L298N module | 1 | $1-2 | belt speed control via PWM; do NOT substitute MX1508 (wrong pin interface, insufficient current headroom for JGB37) |
| LM2596 buck converter | LM2596 step down | 2 | $2-3 | one per power rail |
| GT2 belt 400mm closed loop 6mm | GT2 400mm belt 6mm | 2 | $3-4 | 1 primary, 1 spare |
| GT2 20-tooth pulley 5mm bore | GT2 20 tooth pulley 5mm | 4 | $3-4 | 2 primary, 2 spare |
| M3 hardware kit | M3 bolt nut standoff kit | 1 | $3 | structural fasteners |
| M2 hardware kit | M2 bolt nut kit | 1 | $2 | plow pin connections |
| Torsion spring assortment | torsion spring assortment small | 1 | $3-4 | plow arm return: test with actual solenoid |
| 100uF 25V electrolytic capacitor | 100uf 25v electrolytic capacitor | 10 | $1 | A4988 motor input cap (critical) + spares |
| 1000uF 25V electrolytic capacitor | 1000uf 25v electrolytic | 5 | $1-2 | bulk cap for logic rail ESP32 supply |
| IR break-beam sensor pairs (AliExpress) | IR break beam sensor pair | 4 | $4-6 | bin confirmation only (4 pairs used, 0 spare); binary detection, no timing precision needed |
| TCS34725 RGB color sensor | TCS34725 color sensor module | 1 | $3-5 | verify LED and INT pin broken out on board; same IC as Adafruit #1334 at lower cost |
| H206 slot optocoupler | H206 optocoupler slot sensor | 2 | $1-2 | belt speed encoder (1 primary, 1 spare) |
| Small aluminum heatsink kit | aluminum heatsink kit small | 1 | $2-3 | solenoid bodies |
| Thermal adhesive tape | thermal adhesive tape | 1 | $2 | attaching heatsinks |
| 2S LiPo 7.4V 2000mAh | 2S lipo 2000mah 7.4v | 2 | $16-20 | 1 primary, 1 spare; competition day power failure is unrecoverable |
| 2S LiPo balance charger | 2S balance charger | 1 | $5-6 | must test on LiPo before April 20 |
| Perfboard / protoboard | perfboard prototype board | 2 | $2-3 | final permanent wiring (not breadboard) |
| 22AWG stranded hookup wire assortment | 22awg hookup wire kit | 1 | $3-5 | permanent wiring (breadboard jumpers fall out) |

---

## Adafruit (ships fast, order same day)

| Part | Part # | Qty | Cost | Notes |
|------|--------|-----|------|-------|
| IR break-beam 3mm pair | #2168 | 2 packs | ~$8 | size detection only (2 pairs used, 2 spare); precision timing requires known-good parts |

Size detection beams must be Adafruit. Bin confirmation beams are AliExpress (see AliExpress section). The 19mm dual-beam timing method is sensitive to response consistency; generic beams are not acceptable for this function.

TCS34725 moved to AliExpress - same IC, saves ~$3, no functional difference.

---

## Display: pick one, order same day

| Option | Size | Cost | Notes |
|--------|------|------|-------|
| Pimoroni 1.3in ST7789 | 240x240 | $10-12 | best panel quality, best ESP32 support: recommended |
| Waveshare 1.69in ST7789 | 240x280 | $8-10 | slightly larger |
| Generic AliExpress ST7789 | 240x240 | $4-6 | works, QC varies |

Do not order SSD1306. It is monochrome. The animation requires color.
Order one spare display if budget allows. A dead panel during calibration costs a week.

---

## Local (same day)

| Part | Where | Cost | Purpose |
|------|-------|------|---------|
| Isopropyl alcohol | pharmacy | ~$3 | cleaning contacts and prints |
| Foam sheet or felt | dollar store | ~$2 | bin liners (if not using printed ramp bins) |

---

## Already owned (verify before ordering duplicates)

- ESP32 DevKit
- 1N4007 flyback diodes (verify quantity: need 3 minimum)
- Breadboard and jumper wires (for bringup only, not final build)
- Heat shrink tubing
- PLA filament (school printer)
- DC bench supply (development only, never for logged runs or demo)
- Resistor assortment (verify 4.7k available for I2C pullups if needed)

---

## Notes

**TMC2209 vs A4988:** TMC2209 chosen over A4988 for three reasons: handles 2000+ steps/sec cleanly (headroom for future speed tuning), better current regulation reduces heat during sustained operation, and StallGuard provides passive step-skip detection. Use in standalone mode (MS pins, no UART). Wiring is nearly identical to A4988.

**JGB37-520 over TT gearmotor:** TT motors at 1:48 (only ratio readily available) give ~107mm/s belt speed under load, below the escapement rate at target sps. JGB37-520 at 300-520 RPM gives 200-330mm/s loaded with comfortable headroom. 6mm D-shaft accepts standard GT2 pulleys without hunting for rare bore sizes. Higher gearbox torque than TT or N20 at comparable speeds. L298N is the correct driver: firmware uses one PWM enable pin + two direction pins matching L298N's interface. MX1508 uses two PWM inputs and cannot be substituted without rewriting belt control logic.

**IR break-beam split:** Adafruit #2168 for size detection only. The 19mm dual-beam timing method requires consistent emitter/receiver response characteristics. AliExpress pairs are fine for bin confirmation, which is binary detection with a 500ms timeout window.

**Why 2 LiPo batteries?** Competition day power failure with no spare is unrecoverable. Judges may request multiple consecutive demos. Two batteries = peace of mind.

**Why perfboard and hookup wire?** Breadboard jumpers vibrate out. For the final assembled system, everything solenoid-carrying and sensor-carrying needs to be soldered to perfboard. Do not skip this.

**H206 slot optocoupler** is for the belt speed encoder (slotted disk on belt pulley). See EMBEDDED.md for the PI controller approach. Print slots into the pulley face, mount the sensor next to it, get continuous high-bandwidth speed feedback.

**Spring assortment:** test each spring weight against the actual JF-0530B at operating voltage before committing. Target the heaviest spring the solenoid can still overcome. Faster retraction is the goal. The force margin is large (~94x) so there is substantial room to go heavy.

---

## Total estimated cost

$105-130 (TMC2209 slightly more than A4988, offset by moving TCS34725 and 2 Adafruit beam packs to AliExpress)
