# rollers_params.scad Notes

Intent:
- Define one parameter contract for the timing pulley, drive roller, idler roller, and fit coupons.
- Separate stable geometry constraints from provisional hardware estimates.

Locked truth inputs:
- Brick width: 15.8mm
- Long-side-across brick orientation across the conveyor
- Channel width: 25mm target

Provisional estimates:
- Selected belt strip width from received stock or later purchase
- Shaft-fit values for the supported drive shaft and timing pulley shaft
- MR85 bearing pocket fit values
- Timing belt pitch, pulley tooth counts, and ratio
- Center distance and tension adjustment travel
- Crown sagitta and flange envelope details
- Magnet pocket details

Current receipt state:
- AliExpress hardware is received and should be measured now where it affects this subsystem.
- Amazon hardware is still pending and remains blocked for measurement-dependent updates.

Double check with received hardware:
- Confirm actual shaft diameter and flat dimension on the received NEMA17 motor shaft.
- Confirm MR85 real OD and width from the received lot.
- Confirm timing pulley tooth count and fit constraints against received motion hardware.
- Keep belt strip width and thickness provisional until the final conveyor strip is explicitly selected from received stock or a later order.
- Replace only the provisional values that are now backed by received-part measurements, then regenerate STL files.
