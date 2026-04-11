# Electrical System Documentation

Status date: April 11, 2026

## Power Architecture

- Main supply: LiPo pack for all meaningful runs
- Logic rail: regulated 3.3V for ESP32 and sensors
- Servo rail: voltage and current appropriate for the MG995/MG996-class chute servo
- Motor rail: conveyor stepper driver motor supply
- Common ground across logic, servo, and motor rails

## Non-Negotiable Protection

- Bulk capacitor at the stepper driver motor power input
- Shared ground return sized for conveyor and servo current
- Strain relief at moving interfaces
- Flyback diode on any coil used in experimental or accessory wiring

## Motion Drivers

- Conveyor feed axis: NEMA17 stepper with dedicated driver
- Sorting mechanism: MG995/MG996-class servo rotary chute selector
- Fan remains available for driver or electronics cooling
- Do not wire a NEMA11 selector as part of the active states build

## Sensor Wiring Notes

- Size sensor family is undecided
- Break-beam sensors are candidate size hardware
- Distance or ToF sensing is candidate size hardware only until selected
- Color sensor wiring must be kept away from motor and servo leads where possible
- Keep the color shroud installed for all calibration and validation runs

## Routing Constraints

- Separate motor-current paths from signal paths where practical
- Keep high-current loop areas small on motor and servo wiring
- Add strain relief near the servo and any moving harness
- Keep belt tension access and pulley guards serviceable if the chosen conveyor uses them

## Pre-Power Checks

- Verify no shorts between motor, servo, and logic rails
- Verify connector pinout matches the current hardware map
- Verify servo power polarity before plugging into the servo
- Verify conveyor direction at low speed
- Verify sensor values appear in serial output before a full run
