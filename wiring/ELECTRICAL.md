# Electrical System Documentation

Status date: April 3, 2026

## Power architecture

- Main supply: LiPo pack for all meaningful runs
- Logic rail: regulated 3.3V for ESP32 and sensors
- Actuator rail: motor-voltage path for conveyor stepper driver, selector stepper driver, and release actuator supply
- Common ground across logic and actuator rails

## Non-negotiable protection

- Flyback diode on every inductive release coil
- Bulk capacitor at each stepper driver motor power input
- Gate resistor on any low-side coil driver MOSFET
- Shared ground return sized for both stepper phases and any release actuator current

## Motion drivers

- Conveyor feed axis: NEMA 17 stepper with dedicated driver and off-axis timing-belt stage to a supported drive roller
- Selector chute indexer axis: NEMA 11 stepper with dedicated driver
- Do not share one stepper driver between both axes
- Reduce hold current on both axes when static to limit heat
- Keep the conveyor pulley path guarded and serviceable so ratio changes do not require a redesign

## Release actuator path

- Final release actuator and driver depend on the chosen mechanism
- If the release actuator is coil-based, use logic-level low-side switching and keep the flyback diode in place
- If the release actuator is servo-based or another positional device, wire it per the vendor guidance and keep the power rail isolated from motor noise
- Verify the actual voltage, current, and connector style on the received part before final wiring

## Sensor wiring notes

- Dual ToF modules need clean shared I2C wiring plus independent XSHUT control for address assignment
- Bin-confirm beams use stable pull-up strategy per input type
- Color sensor wiring must be shielded from motor leads where possible
- Keep shroud installed for all calibration and validation runs

## Routing constraints

- Separate motor-current paths from signal paths where possible
- Keep high-current loop areas small on actuator and motor wiring
- Add strain relief near all moving mechanical interfaces
- Do not rely on motor-shaft support as the production roller mount
- Keep the belt tension access and pulley guard removable for service

## Pre-power checks

- Verify diode polarity on all coil-based actuators
- Verify MOSFET drain/source orientation
- Verify no shorts between actuator rail and logic rail
- Verify connector pinout matches the current hardware map
