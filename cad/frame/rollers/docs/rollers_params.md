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

Double check when hardware arrives:
- Confirm actual shaft diameter and flat dimension with calipers.
- Confirm MR115 real OD and width from received lot.
- Confirm belt strip measured width and thickness.
- Confirm timing pulley tooth count, belt width, and center distance against the real parts.
- Replace provisional fit values and regenerate STL files.
