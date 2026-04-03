# Electrical System Documentation

Status date: April 3, 2026

## Power architecture

- Main supply: LiPo pack for all meaningful runs
- Logic rail: regulated 3.3V for ESP32 and sensors
- Actuator rail: 6V path for belt motor, stepper driver input, and release solenoid
- Common ground across logic and actuator rails

## Non-negotiable protection

- Flyback diode on every solenoid coil
- Bulk capacitor at stepper driver motor power input
- Gate resistor on solenoid MOSFET drive
- Shared ground return sized for motor and solenoid current

## Release solenoid path

- Solenoid: JF-0530B, controlled by `PIN_RELEASE`
- Drive device: logic-level N-MOSFET low-side switching
- Control pulse: `SOLENOID_ON_MS` from firmware config
- Solenoid drives class 3 lever release only

## Sensor wiring notes

- Size beams and bin beams use stable pull-up strategy per input type
- Color sensor wiring must be shielded from motor leads where possible
- Keep shroud installed for all calibration and validation runs

## Routing constraints

- Separate motor-current paths from signal paths where possible
- Keep high-current loop areas small on solenoid and motor wiring
- Add strain relief near all moving mechanical interfaces

## Pre-power checks

- Verify diode polarity on all solenoids
- Verify MOSFET drain/source orientation
- Verify no shorts between actuator rail and logic rail
- Verify connector pinout matches firmware GPIO map
