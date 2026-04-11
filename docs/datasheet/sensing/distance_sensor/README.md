# Distance Sensor Notes

Exact module represented in this folder: `VL53L0X` breakout-style ToF sensor CAD and image references
Spreadsheet trace: not yet present in `docs/project/BOM.xlsx`
Received status: unconfirmed in spreadsheet
Source of dimensions: CAD assets and images stored in this folder
Date checked: 2026-04-08
CAD-ready: preliminary only

Integration notes for the sorter:
- Distance sensing is only a candidate for the undecided states size sensor.
- Do not treat the old dual rear-wall chamber layout as active.
- If this sensor family is selected, define a conveyor-mounted bracket and calibration method in `docs/project/OPEN_DECISIONS.md` before final CAD.
- If two identical I2C modules are used, leave room for independent `XSHUT` control and address assignment on the shared bus.
