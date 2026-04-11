# TMC2209 Stepper Driver Module

Active status: active conveyor driver candidate for the NEMA17 conveyor motor.

## Traceability

| Field | Value |
|-------|-------|
| BOM trace | AliExpress Mar 25, order ID `8210696713975240` |
| Received status | Yes |
| Purchased description | TMC2209 silent stepper motor driver, includes heatsink |
| States-build use | NEMA17 conveyor driver |
| Trust level | Listing transcription plus module reference images in this folder |
| Next action | Verify current limit on the real module before conveyor run testing |

## Key Specs

| Feature | Value |
|---------|-------|
| Motor supply range | 5.5V to 38V |
| Logic voltage | 3.3V or 5V module logic |
| Output current | 2A listed maximum |
| Recommended operating current | 1.7A listed |
| Microstepping | Up to 256 microsteps listed |
| Control mode | Step and direction or UART, depending on board configuration |

## Current Limit Notes

Measure `VREF` between the reference point and `GND` on the real module. Do this with motor power on, but with the stepper motor unplugged.

Do not rely on USB power alone while measuring the driver current reference.

The listing note says increasing `VREF` increases drive current. Record the actual `VREF` value used in bringup notes before full conveyor testing.

## Handling Warnings

- Check module orientation before plugging it into a carrier or wiring harness.
- Do not connect or disconnect the stepper motor while the driver is powered.
- Keep the heatsink clear of pins so it cannot short adjacent conductors.
- Use heatsink and fan cooling if the driver gets hot during conveyor testing.
- Keep the bulk capacitor on the stepper driver motor power input.
