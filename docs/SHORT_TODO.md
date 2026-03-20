# Short TODO

Immediate and near-term action items. Keep this list short and current.

---

## Right now: hardware you already have (no parts needed)

- [ ] Flash firmware to ESP32, confirm serial output at 115200 baud
- [ ] `help` in serial monitor - verify test harness responds
- [ ] `test fullrun` - verify run summary shows bins = (6, 6, 4, 8)
- [ ] `test thermal 20` - verify WARNING then DANGER trigger in output
- [ ] `sim jam` - verify ERROR_HALT, then `sim reset` - verify recovery to IDLE
- [ ] Walk one brick manually: `sim start`, `sim beam1`, `sim timeout`, `sim color 240 18 20 278`, `sim colordone`, `sim bin 2` - watch state transitions
- [ ] Wire available solenoid to ESP32 GPIO with flyback diode across terminals
  - Override actuators::plowFire() temporarily to drive real GPIO
  - Fire 20 back-to-back, touch solenoid body - validates thermal constants
  - Confirm 20ms full / 40% hold / de-energize sequence on real hardware
- [ ] Nichibo 23022 motor: tape paper flag to shaft, run at 6V or 12V, count rotations for 10s
  - RPM = rotations x 6; belt speed = (RPM / 60) x 40mm
  - If above 200mm/s: usable as dev stand-in for wiring/firmware validation
  - Do not build final system around it - JGB37 is the real part

---

## Urgent: order now

- [ ] AliExpress - confirm before checkout:
  - Remove MX1508 from cart (redundant, L298N covers belt motor)
  - GT2 belt: 400mm closed loop variant selected
  - Solenoid: 5V variant selected, quantity 4 (need 3, 1 spare)
  - JGB37-520: 300 or 520 RPM at 6V selected (not 100RPM, not 30RPM)
  - H206 optocoupler: buy 2
  - TCS34725: buy 2 (calibration-sensitive, spare is essential)
  - LM2596: buy 2 units
  - Perfboard: buy 3 not 2
  - TMC2209: confirm getting TMC2209 not A4988 from dollar express listing
- [ ] AliExpress - add to cart:
  - NEMA 11 stepper: 28-40 or 28-50mm body, 4-wire bipolar, 5mm shaft
  - GT2 20-tooth pulley 5mm bore x4
- [ ] Adafruit: IR break-beam #2168 x2 packs (size detection only), ST7789 color display
- [ ] Amazon: 2S LiPo 7.4V 2000mAh x2 (Zeee/Ovonic/Gens Ace), 2S balance charger

---

## This week: firmware (unblocked now)

- [ ] Implement real solenoid timer sequence in actuators.cpp using esp_timer (no delay())
  - t=0: GPIO high (SOL_FULL_PWM)
  - t=SOL_FULL_MS: drop to SOL_HOLD_PWM
  - t=SOL_DEENERGIZE_MS: GPIO low
- [ ] Implement real stepper sequencer in actuators.cpp
  - esp_timer firing STEPPER_SPS_NORMAL pulses on PIN_STEPPER_STEP
  - Pull PIN_STEPPER_EN LOW in begin() to enable TMC2209
  - Use gThermal.adjustedStepperSps() for thermal-throttled rate
- [ ] Implement hardware timer ISR logic in sensors.cpp (write now, attach when hardware arrives)
  - onBeam1Break: start 64-bit hardware timer, push BEAM1_BREAK event
  - onBeam2Break: read timer, push BEAM2_BREAK with gap_us filled
  - Timer overflow at SIZE_TIMEOUT_MS: push SIZE_TIMEOUT
- [ ] Display module: ST7789 SPI driver init, state screens (READY/SORTING/COMPLETE/ERROR)

---

## This week: CAD (unblocked in parallel)

- [ ] Top-down footprint layout: verify full system fits 610x610mm before any frame CAD
- [ ] Chute transition piece: CAD and print iteration 1, test with real bricks
- [ ] Gate: do not proceed to full frame CAD until chute transition is reliable

---

## Open decisions

- [ ] Confirm AC outlet at judging table (determines LiPo vs bench supply at competition)

---

## Waiting on parts (~April 1)

- Measure actual belt speed under load at operating PWM duty cycle
- Log color sensor R/G/B for all 4 brick types under shroud LED
- Confirm break-beam size detection geometry at actual belt speed
- Select final spring weight from assortment against actual solenoid
- Measure exact sensor-to-bin distances, update timing constants in config.h
