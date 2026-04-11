# IR Break-Beam Sensor

Active status: active candidate for the undecided states size sensor.

## Traceability

| Field | Value |
|-------|-------|
| Purchased name | 4 Pairs Infrared Circuit Breaker Sensor 3mm LED Lamp Distance Sensor Counting Module DC 3-5V |
| BOM trace | AliExpress Mar 25, order ID `8210696713845240` |
| Received status | Yes |
| States-build use | Candidate size sensing at the conveyor sensing station |
| Trust level | AliExpress listing plus received-part verification possible now |
| Next action | Test timing or beam geometry with real bricks before choosing this as the final size sensor |

## CAD Status

CAD-ready: yes, using `cad/ir_break_beam_5mm.step` as a modifiable starting point.

Note: Light hole center location is not required for CAD in this project. The LEGO brick sorter's tolerance, about 2mm to 3mm, and the use of adjustable slotted mounts make precise hole center unnecessary. Assume the 3mm hole is roughly centered on the 10mm x 8mm face and proceed with slotted mounting.

## Dimensions

| Feature | Value |
|---------|-------|
| Body length | 20mm |
| Body width | 10mm |
| Body height | 8mm |
| Light hole diameter | 3mm |
| Cable length | 300mm |
| Cable exit | Rear of body |
| Connector style | Bare tinned ends or 2.54mm female Dupont |

## Wiring

| Side | Wire | Function |
|------|------|----------|
| Transmitter | Red | VCC, 3V to 5V |
| Transmitter | Black | GND |
| Receiver | Red | VCC, 3V to 5V |
| Receiver | Black | GND |
| Receiver | White | OUT, NPN active low, requires 10k pull-up to VCC |

## Electrical Notes

| Feature | Value |
|---------|-------|
| Operating voltage | 3V to 5V DC |
| Emitter current | 20mA at 5V |
| Receiver output | Sinks up to 100mA |
