# Electrical

Use a fused 3S LiPo input. Share ground across the ESP32, stepper driver, servo power, buck converter, sensors, and display.

## Power

- 3S LiPo: main input through inline fuse.
- 12V motor rail: NEMA17 stepper driver motor input.
- 5V buck output: servo, sensors, display as current allows.
- 3.3V ESP32 rail: ESP32 logic only.

Do not run sorting tests from a bench supply. Use the LiPo setup intended for the machine.

## Required Protection

- Inline fuse on LiPo input
- Bulk capacitor at the stepper driver motor input
- Common ground between logic and motor control
- Flyback diode on any coil load
- Strain relief on LiPo, motor, and servo wiring

## Pin Map

- Stepper STEP: set in firmware config.
- Stepper DIR: set in firmware config.
- Chute servo PWM: set in firmware config.
- Break-beam A: set in firmware config.
- Break-beam B: set in firmware config.
- TCS3200 S0 to S3: set in firmware config.
- TCS3200 OUT: set in firmware config.
- Display SPI: set in firmware config.
