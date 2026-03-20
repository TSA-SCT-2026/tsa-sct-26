# Bill of Materials

AliExpress lead time is 10-15 days (expect arrival March 28 - April 2). Adafruit ships in 2-5 days. Order immediately.

---

## AliExpress main cart

| Part | Cart listing | Qty | Est. cost | Notes |
|------|-------------|-----|-----------|-------|
| JF-0530B solenoid 5V | "0520 0530 Electric Magnet Push Pull Linear Solenoid" ZYT Premium Selection Store | 4 | $4.36 | plow actuation; confirm 5V variant at checkout; buy 4 (3 used, 1 spare) |
| NEMA 11 stepper 34mm | "Nema 11 step motor 2phase 4 lead 1.8 degree 34mm" MannHwa Smart Home Electrical | 1 | $5.83 + $6.58 ship | 34mm body, 4-wire bipolar, 5mm shaft, 200 steps/rev; check Usongshine store for free shipping alternative before ordering |
| JGB37-520 gearmotor 6V | "6V/12V/24V DC Gear Motor JGB37-520" Tungfull Office Store | 2 | $9.56 | belt drive (1 primary, 1 spare); confirm 300 or 520 RPM variant at 6V; do NOT order 30 or 100 RPM |
| GT2 belt 400mm | "2MGT 2GT Belt Width 6/10/15mm Rubber Closed Loop" YuRu Parts Store | 2 | $2.71 | confirm 400mm closed loop at checkout; 1 primary, 1 spare |
| IR break-beam pairs (bin confirmation) | "4 Pairs Infrared Circuit Breaker Sensor 3mm LED" Hello Hello Home Store | 1 pack (4 pairs) | $6.78 | bin confirmation only; binary detection, no timing precision needed |
| H206 slot optocoupler | "Slot Type IR Optocoupler Speed Sensor Module 3.3V-5V" DIY-Victor Store | 2 | $0.95 | belt speed encoder (1 primary, 1 spare) |
| Torsion springs | "10 Pcs V Type Spring 0.4mm Wire Diameter Torsional Spring" Nezha Hardware Store | 1 | $2.65 | plow arm return; test each weight against actual solenoid before committing |
| Perfboard | "PCB Board Prototype Board Blue 2x8/3x7/4x6/5x7/7x9cm" Shop1104159084 | 3 | $1.69 | final permanent wiring; buy 3 (one spare for wiring mistakes) |
| Aluminum heatsinks | "Aluminum Heatsink Radiator Heat sink for Electronic IC" Cotexer online Store | 1 | $1.99 | solenoid bodies (~20x20mm target size) |
| Thermal adhesive tape | "10m/Roll Thermal Conductive Adhesive Tape" VZOSTUP Official Store | 1 | $1.97 | attaching heatsinks to solenoids |
| L298N motor driver | "L298N DC Motor Driver Module Smart Car Robot" | 1 | ~$1.19 | belt speed control via PWM; do NOT substitute MX1508 (wrong pin interface for firmware) |


## AliExpress dollar express cart

| Part | Cart listing | Qty | Est. cost | Notes |
|------|-------------|-----|-----------|-------|
| GT2 20-tooth pulley | "2GT 20Teeth Synchronous Wheel Belt Width 6/9/10/15mm" | 4 | $1.51 | CRITICAL: select correct bore; NEMA 11 shaft = 5mm bore; JGB37 output shaft = 6mm bore; verify JGB37 shaft diameter before ordering |
| 22AWG stranded wire | "10 Colors Silicone Electrical Wire 22 AWG Stranded Tinned Copper" | 1 | $3.19 | permanent wiring; breadboard jumpers vibrate out |
| LM2596 buck converter | "LM2596 DC to DC Buck Converter 3.0-40V to 1.5-35V" | 2 | $1.58 | one per power rail; confirm buying 2 units |
| TMC2209 stepper driver | "TMC2209 A4988 Nema 17 Stepper Motor driver TMC2209" | 2 | $3.39 | 1 primary, 1 spare; confirm TMC2209 ships (not A4988); use standalone mode (MS1/MS2 pins, no UART); EN pin is active-low |
| TCS34725 color sensor | "TCS34725 Low Power IR Blocking Filter RGB Light Color Sensor" | 2 | $3.68 | 1 primary, 1 spare; verify LED and INT pin broken out on board; calibration-sensitive part, spare is essential |


## Adafruit (order same day as AliExpress)

| Part | Part # | Qty | Cost | Notes |
|------|--------|-----|------|-------|
| IR break-beam 3mm pair | #2168 | 2 packs | ~$8 | size detection ONLY; 2 pairs used, 2 spare; precision timing requires known-good parts; do not substitute AliExpress for this function |
| ST7789 color TFT display | Pimoroni 1.3in or Waveshare 1.69in | 1 (+1 spare if budget) | $8-12 | must be ST7789, must be color; do NOT order SSD1306 (monochrome, animation requires color); spare prevents week-long delay if panel dies during calibration |

---

## Amazon (order any time, ships fast)

| Part | Qty | Est. cost | Notes |
|------|-----|-----------|-------|
| 2S LiPo 7.4V 2000mAh | 2 | $16-20 | Zeee, Ovonic, or Gens Ace; do not buy from AliExpress; 1 primary, 1 spare; competition day failure with no spare is unrecoverable |
| 2S balance charger | 1 | $15-25 | ISDT or SkyRC B6 style; must test on LiPo before April 20 |

---

## Local / hardware store

| Part | Where | Notes |
|------|-------|-------|
| M3 bolt/nut/standoff kit | hardware store or AliExpress | structural frame fasteners |
| M2 bolt/nut kit | hardware store or AliExpress | plow lever pin connections |
| Isopropyl alcohol | pharmacy | cleaning contacts and prints |

---

## Already owned (verify before ordering)

- ESP32 DevKit
- SR5150 or 1N4007 flyback diodes: need 3 minimum (SR5150 confirmed acceptable)
- 100uF 25V electrolytic capacitors: need 3+ (TMC2209 motor input caps)
- 1000uF 25V electrolytic capacitors: need 2 (logic rail bulk caps)
- 4.7k resistors: need 2 (I2C pullups if color sensor drops reads)
- Breadboard and jumper wires (bringup only, not final build)
- Heat shrink tubing
- PLA filament (school printer)
- DC bench supply (development only, never for logged runs or competition demo)

---

## Do NOT order

- MX1508 motor driver: wrong pin interface for firmware (two PWM inputs vs enable+direction), insufficient current margin for JGB37 under sustained load
- Rocker switch: adds power interrupt failure risk, does not improve judge experience over labeled momentary button
- SSD1306 display: monochrome, animation requires color
- TT gearmotor 1:48: too slow under load, replaced by JGB37-520
- A4988 stepper driver: replaced by TMC2209

---

## Notes

**Solenoid voltage:** The JF-0530B listing shows 5V/6V/12V/24V variants. Must select 5V. All power architecture, PWM hold timing, and thermal model constants are designed around 5V. Wrong voltage = wrong behavior or burnout.

**JGB37-520 RPM variant:** The same listing covers 7 RPM to 960 RPM. Must select 300 RPM or 520 RPM at 6V. At 300 RPM loaded down to 65% = ~195 RPM = ~246 mm/s belt speed. At 520 RPM = ~430 mm/s ceiling with PWM throttle. Either works. 520 RPM gives more headroom for pushing escapement sps later.

**GT2 pulley bore:** NEMA 11 stepper shaft is 5mm. JGB37-520 output shaft is 6mm. These are different bore sizes. Verify JGB37 shaft diameter in the product listing and order the correct bore for each.

**TMC2209 wiring difference from A4988:** EN pin is active-low (pull LOW to enable). A4988 used SLEEP pulled HIGH. MS1 and MS2 set microstepping (both LOW = full step). No RESET pin, no MS3 pin. Otherwise identical interface.

**TCS34725 spare is not optional:** Color sensor is the most calibration-sensitive part. A marginal unit can pass basic tests but drift under specific lighting. If it fails during calibration week you lose the time to reorder and recalibrate. Buy 2.

**Why 2 LiPo batteries:** Judges may request back-to-back demos. Competition day power failure with no spare is unrecoverable. Two batteries = peace of mind.

**Perfboard x3:** You will make a wiring mistake on one board. The third is insurance.

---

## Total estimated cost

AliExpress: ~$55-65
Adafruit: ~$16-20
Amazon: ~$35-50
Local: ~$5-10
Total: ~$110-145

