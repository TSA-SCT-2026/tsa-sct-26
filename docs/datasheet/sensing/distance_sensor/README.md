# Distance Sensor Notes

Active status: candidate only for the undecided states size sensor.

## Traceability

| Field | Value |
|-------|-------|
| Exact module represented | `VL53L0X` breakout-style ToF sensor CAD and image references |
| BOM trace | Not present in `docs/project/BOM.xlsx` |
| Received status | Unconfirmed in spreadsheet |
| States-build use | Candidate size sensing only |
| Trust level | CAD assets and images stored in this folder |
| Date checked | 2026-04-08 |
| CAD-ready | Preliminary only |
| Next action | Do not choose ToF until the hardware status, conveyor bracket, and calibration method are documented in `docs/project/OPEN_DECISIONS.md` |

## Reference Files

| Path | Purpose |
|------|---------|
| `cad/vl53l0x.step` | Usable CAD layout model |
| `cad/vl53l0x.f3d` | Source CAD reference |
| `images/components.png` | Component reference |
| `images/dimensions.jpg` | Dimension reference |
| `images/module.png` | Visual reference |
| `images/sensor.png` | Sensor reference |

## Integration Notes

- Distance sensing is only a candidate for the undecided states size sensor.
- Do not treat the old dual rear-wall chamber layout as active.
- If this sensor family is selected, define a conveyor-mounted bracket and calibration method in `docs/project/OPEN_DECISIONS.md` before final CAD.
- If two identical I2C modules are used, leave room for independent `XSHUT` control and address assignment on the shared bus.
