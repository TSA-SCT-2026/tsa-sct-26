# Short TODO

Immediate and near-term action items. Keep this list short and current.

---

## Urgent: order now (every day of delay costs schedule margin)

- [ ] AliExpress: finalize cart with all corrections from BOM.md and order
  - NEMA 11 stepper (28-40 body preferred, 4-wire bipolar, 5mm shaft)
  - TMC2209 x2
  - JGB37-520 6V 300-520RPM x2 (confirm RPM variant before checkout)
  - JF-0530B solenoid 5V x4 (buy 4, need 3, one spare)
  - TCS34725 x2 (buy spare, calibration-sensitive part)
  - GT2 400mm belt x2, GT2 20-tooth 5mm bore pulley x4
  - LM2596 x2, H206 x2
  - IR break-beams x4 pairs (bin confirmation only)
  - Torsion spring assortment, heatsinks, thermal tape
  - Perfboard x3, 22AWG wire kit
- [ ] Adafruit: IR break-beam #2168 x2 packs (size detection only), display (ST7789 color)
- [ ] Amazon: 2S LiPo 7.4V 2000mAh x2 (Zeee/Ovonic/Gens Ace), 2S balance charger

---

## This week: firmware (unblocked, do now)

- [ ] Flash existing firmware to ESP32, verify serial output and test harness over USB
- [ ] Run `test fullrun` over serial, confirm all 24 bricks route correctly, summary = (6,6,4,8)
- [ ] Run `test thermal 20`, verify WARNING and DANGER thresholds trigger
- [ ] Inject `sim jam`, verify ERROR_HALT halts and `sim reset` recovers
- [ ] Implement real solenoid timer sequence in actuators.cpp (esp_timer, no delay())
  - Wire available solenoid to ESP32 GPIO, flyback diode across terminals
  - Verify 20ms full / 40% hold / de-energize sequence fires correctly on real hardware
  - Touch solenoid after 20 back-to-back fires: validate thermal constants are reasonable
- [ ] Implement real stepper sequencer in actuators.cpp
  - STEPPER_SPS_NORMAL pulses on PIN_STEPPER_STEP via esp_timer
  - Pull PIN_STEPPER_EN LOW in begin() to enable TMC2209
  - Verify 200 pulses = 1 rotation when hardware arrives
- [ ] Implement hardware timer ISR stubs in sensors.cpp (write the ISR logic now, attach later)
  - onBeam1Break: start 64-bit hardware timer, push BEAM1_BREAK
  - onBeam2Break: read timer, push BEAM2_BREAK with gap_us
  - Timer overflow at SIZE_TIMEOUT_MS: push SIZE_TIMEOUT
- [ ] Update thermal.cpp: adjustedStepperRpm() returns RPM but actuators need SPS
  - Add adjustedStepperSps() using STEPPER_SPS_* constants from config.h
- [ ] Display module: implement ST7789 SPI driver (can stub render calls, get init working)

---

## This week: CAD (unblocked in parallel)

- [ ] Draw top-down footprint layout: verify full system fits 610x610mm before any frame CAD
- [ ] Chute transition piece CAD: print iteration 1 as soon as drawn
- [ ] Test chute transition with real bricks before any other structural CAD

---

## Open decisions (resolve before ordering second round)

- [ ] Confirm AC outlet availability at judging table (determines LiPo vs bench supply at competition)
  - Contact event organizers or find past TSA SCT competitor
  - See ISSUES.md for full context

---

## Waiting on parts (~April 1)

- Validate actual belt speed under load at operating PWM
- Validate color sensor readings under shroud LED, log R/G/B for all 4 brick types
- Confirm break-beam size detection geometry at actual belt speed
- Select final spring weight from assortment against actual solenoid
- Measure exact sensor-to-bin distances for timing constants in config.h
