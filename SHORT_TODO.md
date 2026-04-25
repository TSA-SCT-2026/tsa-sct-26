# Short TODO

Updated April 25, 2026. See SPRINT.log for full sprint plan and print queue details.

## This Weekend (Apr 25-26)

Goal: Conveyor spinning manually by end of Saturday. Motor + sensing live by end of Sunday.

### Before Leaving for Library (do now)

- CAD the side plate drill jig (5mm shaft bore + two M3.2 screw guide holes per bearing holder spec)
- Slice all parts below into Cura and save to USB
- Organize USB into three Cura project folders (1, 2, or 3 printers)

### Print Queue - Critical Path First

1. Spacers (all four reprinted due to bearing holder redesign):
   spacer_drive_far, spacer_driver_hub, spacer_idler_left, spacer_idler_right
2. spacer_gt2_driver
3. side_plate_drill_jig (design today)
4. outside_motor_board_v7 (top part only, defer foot)
5. sensing_shroud
6. electronics_board
7. electronics_foot

Defer until after conveyor height is known from dry-fit: all feet (conveyor_foot, motor_mount_foot,
servo feet, bins_foot_v2). Defer bins: bins_v2.

### Saturday Assembly Sequence

1. Cut side plates and center support block
2. Drill 5mm shaft holes from measurement
3. Use drill jig to locate bearing holder screw holes
4. Dry-fit shafts, rollers, bearings, bearing holders
5. Assemble side plates + center support
6. Fit belt, spin by hand - this is the pass gate

### Sunday

1. Mount outside_motor_board_v7 top, align GT2 pulleys with straightedge
2. Install 200mm GT2 belt, tension, run motor at low speed
3. Mount sensing_shroud, install break-beams and color sensor
4. Wire and confirm readings in serial output

## This Week

- Measure final conveyor height from dry-fit, print and fit feet
- CAD and print servo feet
- Physical chute-to-bin alignment with real bricks
- Firmware: full sort pipeline
- Calibration: break-beam timing, color thresholds (shroud installed)
- Operator UX: labels, display states, cable routing
- Full 24-brick accuracy runs

## Do Not Do

- Do not print feet before conveyor height is known from the real assembly
- Do not cut final wood before side plate geometry is confirmed
- Do not calibrate color in open air
- Do not revive hopper, chamber, NEMA11 selector, ToF sizing, or 2020 extrusion
