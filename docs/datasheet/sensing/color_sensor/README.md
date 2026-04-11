# TCS3200 GY-31 Color Sensor

Active status: active color sensor for the states build.

## Traceability

| Field | Value |
|-------|-------|
| Purchased name | GY-31 TCS230/TCS3200 color sensor |
| BOM trace | AliExpress Mar 25, order ID `8210696713785240` |
| Received status | Yes |
| States-build use | Color sensing in the conveyor sensing station |
| CAD model source | GrabCAD TCS3200 color sensor module reference |
| Trust level | BOM-backed part plus module datasheets and community CAD |
| Next action | Calibrate with the final shroud and final belt geometry installed |

## Key Specs

| Feature | Value |
|---------|-------|
| Best detection distance | 10mm |
| Input voltage | 3V to 5V |
| Output | Light intensity to frequency |
| Module size | 33.2mm x 33.2mm x 25mm |
| Calibration condition | Final shroud installed, final belt geometry installed |

## Pins

| Pin | Function |
|-----|----------|
| GND | Ground |
| OUT | Output frequency |
| S0 | Output frequency scaling selection input |
| S1 | Output frequency scaling selection input |
| S2 | Photodiode type selection input |
| S3 | Photodiode type selection input |
| VCC | Power supply |
| LED | Illumination control |

## Wiring Note

Keep color sensor wiring away from motor and servo wiring where practical. Do not calibrate in open air and then trust the threshold in the installed machine.
