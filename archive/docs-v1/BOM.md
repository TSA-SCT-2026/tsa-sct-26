# Bill of Materials

AliExpress: order now, 10-15 day lead time (expect arrival ~April 1).
Adafruit + Amazon: order any time this week, both ship in 2-5 days.

---

## AliExpress main cart

| Part | Qty | Price | Why this / watch out for |
|------|-----|-------|--------------------------|
| JF-0530B solenoid | 4 | $4.36 | Actuates the 3 plow diverters. Buy 4 (use 3, keep 1 spare). **Must select 5V at checkout** - listing also sells 12V and 24V which will not work on this power rail. |
| NEMA 11 stepper 34mm | 1 | $5.83 + $6.58 ship | Drives the cam escapement that releases one brick at a time. 34mm body, 4-wire, 5mm shaft, 1.8 deg/step. The $6.58 shipping is steep - check if Usongshine store has it cheaper before ordering. |
| JGB37-520 gearmotor 6V | 2 | $9.56 | Belt drive motor. Chosen over TT motors because TT motors at the only available ratio are too slow under load. This one has a proper 6mm D-shaft that fits standard pulleys, and high enough RPM with headroom. **Must select 600 RPM at 6V at checkout** - same listing sells everything from 7 RPM to 960 RPM. Buy 2 (1 spare). |
| GT2 belt 400mm closed loop | 2 | $2.71 | Standard 3D printer belt. 400mm is exactly the length the belt math was designed around. **Confirm 400mm selected at checkout**. Buy 2 (1 spare). |
| IR break-beam sensor x4 pairs | 1 pack | $6.78 | Used for bin confirmation only - detects a brick arriving in each bin. Does not need to be precise, just needs clean on/off signal. AliExpress quality is fine for this job. |
| H206 slot optocoupler | 2 | $0.95 | Belt speed encoder. A slotted disk printed into the belt pulley passes through this sensor generating pulses - that's how the firmware knows actual belt speed. Buy 2. |
| Torsion springs assortment | 1 | $2.65 | Spring-returns the plow arms to neutral after each brick. We buy an assortment because the right spring weight can only be determined by testing against the actual solenoid - want the heaviest spring it can still reliably overcome. |
| Perfboard | 3 | $1.69 | Final soldered wiring substrate. Breadboard jumpers fall out under vibration. Buy 3 because wiring mistakes happen. |
| Aluminum heatsinks | 1 kit | $1.99 | Glue onto solenoid bodies. Cheap insurance during long calibration sessions that run many more cycles than the competition demo. |
| Thermal adhesive tape | 1 roll | $1.97 | Attaches heatsinks to solenoids. |
| L298N motor driver | 1 | ~$1.19 | Controls the belt motor speed and direction via PWM. The firmware's pin assignments are built around this specific driver interface - do not substitute the MX1508 (different pin layout, firmware would need to be rewritten). |


## AliExpress dollar express cart

| Part | Qty | Price | Why this / watch out for |
|------|-----|-------|--------------------------|
| GT2 20-tooth pulley 6mm bore | 4 | $1.51 | Mounts on JGB37 motor shaft to drive the belt. **Must select 6mm bore** - the JGB37 output shaft is 6mm. Buy 4 (2 primary, 2 spare). |
| 22AWG stranded wire kit | 1 | $3.19 | Permanent wiring for the final build. Breadboard jumper wires are not reliable under vibration - everything gets soldered with this. |
| LM2596 buck converter | 2 | $1.58 | Steps the 7.4V LiPo down to 5V. We run two separate rails - one for solenoids (noisy switching), one for the microcontroller and sensors (clean). Keeps electrical noise from crashing the ESP32. Confirm buying 2 units. |
| TMC2209 stepper driver | 2 | $3.39 | Controls the NEMA 11 stepper. Chosen over the cheaper A4988 because it handles higher speeds (important for pushing throughput later), runs cooler, and is more reliable. **Confirm the listing ships TMC2209 and not A4988** - some listings bundle both. Buy 2 (1 spare). |
| TCS34725 color sensor | 2 | $3.68 | Reads brick color (red vs blue) by measuring reflected light ratios. Same chip as the $7 Adafruit version, just cheaper here. Calibration-sensitive - if one is slightly off it affects classification accuracy, so we buy 2. Verify the board has the LED and INT pin broken out. |


## Adafruit

| Part | Part # | Qty | Price | Why this / watch out for |
|------|--------|-----|-------|--------------------------|
| IR break-beam 3mm pair | #2168 | 2 packs | ~$8 | Used for size detection - two beams 19mm apart, and we measure how long a brick takes to pass between them. This timing measurement is precise enough that it only works with consistent, quality sensors. AliExpress break-beams work fine for the bin sensors (just on/off) but not here. 2 packs = 4 pairs (2 used, 2 spare). |

---

## Already owned - no order needed

| Part | Status |
|------|--------|
| Waveshare 1.69in ST7789V2 display (240x280 IPS, SPI) | Confirmed correct. Color display, right driver chip, right interface. Good size for judges to read. |
| ESP32 DevKit | Confirmed. Main microcontroller. |
| SR5150 flyback diodes | Confirmed acceptable. Go across solenoid terminals to protect the ESP32 from voltage spikes when solenoids de-energize. Need 3 minimum. |
| 100uF electrolytic capacitors | Confirm have 3+. Goes right next to stepper driver - protects it from back-EMF on power-on. |
| 1000uF electrolytic capacitors | Confirm have 2. Goes on logic rail near ESP32 - buffers the supply during solenoid switching. |
| 4.7k resistors | Confirm have 2. May be needed as I2C pullups if color sensor has communication issues. |

---

## Amazon

| Part | Qty | Price | Why |
|------|-----|-------|-----|
| 2S LiPo 7.4V 2000mAh (Zeee / Ovonic / Gens Ace) | 2 | ~$16-20 each | Main power source. AliExpress LiPo has inconsistent cell quality - not worth the risk for a competition where a dead battery mid-demo is unrecoverable. Judges may ask for multiple demos back-to-back. Buy 2. |
| 2S balance charger (ISDT or SkyRC B6) | 1 | ~$15-25 | Required for safely charging 2S LiPo. Must do a full test run on LiPo (not bench supply) before April 20 to confirm nothing browns out under real load. |

---

## Local / hardware store

| Part | Notes |
|------|-------|
| M3 bolts, nuts, standoffs | Structural frame fasteners. Grab a small kit or pull from existing hardware. |
| M2 bolts and nuts | Plow lever pin connections. |
| Isopropyl alcohol | Cleaning printed parts and contacts before assembly. |

---

## Do not order

| Part | Why not |
|------|---------|
| MX1508 motor driver | Wrong pin interface for the firmware. L298N is already in the cart. |
| Any TT gearmotor | Only available at 1:48 ratio which gives ~107mm/s belt speed under load - below the escapement rate at target speed. JGB37 replaces it. |
| A4988 stepper driver | Replaced by TMC2209 which handles higher speeds and runs cooler. |
| SSD1306 display | Monochrome. The display needs color for brick animation and status screens. |
| Rocker switch (power interrupt) | Adds a failure risk (accidental bump mid-run) without improving the judge experience. Labeled start button is the right call per the scoring rubric. |

---

## Estimated totals

| Source | Estimate |
|--------|----------|
| AliExpress | ~$55-65 |
| Adafruit | ~$8-10 |
| Amazon | ~$50-60 |
| Local hardware | ~$5-10 |
| **Total** | **~$120-145** |
