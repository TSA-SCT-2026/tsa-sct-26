# Component Matrix

Compact component reference for the sorter. Raw datasheets, CAD reference files, and vendor files may live beside this matrix when they are useful for build replication.

- ESP32 dev board: controller, 3.3V logic.
- TMC2209: stepper driver, with bulk capacitance on motor input.
- NEMA17 stepper: conveyor motor driven by TMC2209.
- MG995/MG996-class servo: chute selector, powered from a strong 5V supply.
- IR break-beam pair: size sensing, two pairs in shroud.
- TCS3200/GY-31: color sensing, calibrated only inside installed shroud.
- Waveshare 2 inch Mini LCD: operator display.
- LM2596 buck converter: 5V rail, set output before connecting loads.
- 3S LiPo with XT60: fused main power.
- 1N4007 diode: coil protection.
- 4700uF capacitor: stepper driver motor input stability.
- MR85ZZ and 608ZZ bearings: conveyor support.
- GT2 belt and pulleys: conveyor drive.
