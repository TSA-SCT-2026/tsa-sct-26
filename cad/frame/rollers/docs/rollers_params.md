# rollers_params.scad Notes

Intent:
- Define one parameter contract for the timing pulley, drive roller, idler roller, and fit coupons.
- Separate stable geometry constraints from provisional hardware estimates.

Locked truth inputs:
- Brick width: 15.8mm
- Widthwise brick orientation across the conveyor
- Channel width: 20mm
- Belt strip target width: 19mm

Provisional estimates:
- Shaft-fit values for the supported drive shaft and timing pulley shaft
- MR115 bearing pocket fit values
- Timing belt pitch, pulley tooth counts, and ratio
- Center distance and tension adjustment travel
- Crown sagitta and flange envelope details
- Magnet pocket details

Current receipt state:
- AliExpress hardware is received and should be measured now where it affects this subsystem.
- Amazon hardware is still pending and remains blocked for measurement-dependent updates.

Double check with received hardware:
- Confirm actual shaft diameter and flat dimension on the received NEMA17 motor shaft.
- Confirm MR115 real OD and width from the received lot.
- Confirm timing pulley tooth count and fit constraints against received motion hardware.
- Keep belt strip width and thickness as provisional until Amazon belt material is received.
- Replace only the provisional values that are now backed by received-part measurements, then regenerate STL files.
