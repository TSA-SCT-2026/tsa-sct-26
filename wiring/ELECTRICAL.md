# Electrical System Documentation

Status date: April 3, 2026

## Power architecture

- Main supply: LiPo pack for all meaningful runs
- Logic rail: regulated 3.3V for ESP32 and sensors
- Actuator rail: motor-voltage path for conveyor stepper driver, selector stepper driver, and release solenoid supply
- Common ground across logic and actuator rails

## Non-negotiable protection

- Flyback diode on every solenoid coil
- Bulk capacitor at each stepper driver motor power input
- Gate resistor on solenoid MOSFET drive
- Shared ground return sized for both stepper phases and solenoid current

## Motion drivers

- Conveyor feed axis: NEMA 17 stepper with dedicated driver and off-axis timing-belt stage to a supported drive roller
- Selector chute indexer axis: NEMA 11 stepper with dedicated driver
- Do not share one stepper driver between both axes
- Reduce hold current on both axes when static to limit heat
- Keep the conveyor pulley path guarded and serviceable so ratio changes do not require a redesign

## Release solenoid path

- Solenoid: purchased 0530-series unit, controlled by `PIN_RELEASE`
- Drive device: logic-level N-MOSFET low-side switching
- Control pulse: `SOLENOID_ON_MS` from firmware config
- Solenoid drives class 3 lever release only
- Verify actual coil voltage and current on the received part before final wiring

## Sensor wiring notes

- Size beams and bin beams use stable pull-up strategy per input type
- Color sensor wiring must be shielded from motor leads where possible
- Keep shroud installed for all calibration and validation runs

## Routing constraints

- Separate motor-current paths from signal paths where possible
- Keep high-current loop areas small on solenoid and motor wiring
- Add strain relief near all moving mechanical interfaces
- Do not rely on motor-shaft support as the production roller mount
- Keep the belt tension access and pulley guard removable for service

## Pre-power checks

- Verify diode polarity on all solenoids
- Verify MOSFET drain/source orientation
- Verify no shorts between actuator rail and logic rail
- Verify connector pinout matches firmware GPIO map
