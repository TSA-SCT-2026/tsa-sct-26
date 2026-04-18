# Equations Used in Design

This file logs non-trivial equations used to derive dimensions, timing, or firmware logic for the engineering notebook. Each entry includes the equation, the reason it matters, and where it is used.

## Servo Funnel Arc Spacing

Equation:

```text
spacing = 2 * radius * sin(angle_between_positions / 2)
```

Current use:

```text
angle_between_positions = 35 degrees
spacing = 2 * radius * sin(17.5 degrees)
spacing is about 0.60 * radius
```

Example values:

- radius 65mm gives about 39mm center spacing
- radius 75mm gives about 45mm center spacing
- radius 83mm gives about 50mm center spacing
- radius 95mm gives about 57mm center spacing
- radius 100mm gives about 60mm center spacing
- radius 110mm gives about 66mm center spacing

Explanation:

The servo chute points at four bins spread by roughly 35 degrees. Adjacent funnel centers are separated by the chord length between two points on the chute sweep circle. This equation decides whether 50mm to 60mm funnel mouths fit without overlap.

Used in:

- `docs/project/SERVO_CHUTE_HANDOFF.md`
- `docs/project/SERVO_BINS_CAD_TUTORIAL.md`
- `cad/DIMENSIONS.md` selector and bin funnel layout

## Chute Vertical Drop

Equation:

```text
vertical_drop = chute_centerline_length * sin(chute_angle_from_horizontal)
exit_z = entry_z - vertical_drop
```

Current use at 40 degrees:

```text
vertical_drop_65 = 65mm * sin(40 degrees) = about 42mm
vertical_drop_75 = 75mm * sin(40 degrees) = about 48mm
```

If the reference centerline starts at Z=90mm:

```text
exit_z_65 = 90mm - 42mm = about Z=48mm
exit_z_75 = 90mm - 48mm = about Z=42mm
```

Explanation:

The chute exit gets lower as the sloped centerline gets longer. This prevents the mistaken assumption that a longer chute arm raises the exit. The result controls whether the bin needs a top-entry funnel, a side-entry funnel, or a changed Z stack.

Used in:

- `docs/project/SERVO_CHUTE_HANDOFF.md`
- `cad/DIMENSIONS.md`
- servo chute and bin Z-stack CAD checks

## Conveyor-to-Chute Entry Drop

Equation:

```text
entry_drop = belt_surface_z - chute_entry_floor_z
chute_entry_floor_z = belt_surface_z - entry_drop
```

Current first CAD target:

```text
belt_surface_z = about 72mm
entry_drop = 2mm to 4mm
chute_entry_floor_z = about 68mm to 70mm
```

Explanation:

The chute floor should sit slightly below the top of the belt so the brick leaves the conveyor cleanly without hitting a vertical wall or falling through a large gap. The first target is about a 3mm drop from belt surface to chute entry floor.

Used in:

- conveyor-to-chute handoff CAD
- servo chute entry placement
- `cad/DIMENSIONS.md` handoff notes
- `docs/project/SERVO_BINS_CAD_TUTORIAL.md` Z-stack workflow

## Exit Lip Height Above Belt

Equation:

```text
exit_lip_top_z = belt_surface_z + lip_height
```

Current first CAD target:

```text
belt_surface_z = about 72mm
lip_height = 5mm to 6mm
exit_lip_top_z = about 77mm to 78mm
```

Explanation:

The exit lip redirects horizontal brick motion from the belt into the chute entry. Its top height is derived from the belt surface so it stays valid if the conveyor is raised or lowered in the frame.

Used in:

- conveyor exit lip CAD
- conveyor-to-chute handoff test coupon
- `cad/DIMENSIONS.md`
- `docs/project/CONVEYOR_BUILD_GUIDE.md`

## Bin Internal Volume

Equation:

```text
bin_volume = internal_width * internal_depth * internal_height
```

Current bin target:

```text
bin_volume = 80mm * 80mm * 60mm
bin_volume = 384000 cubic mm
bin_volume = 384 cubic cm
```

Unit conversion:

```text
1 cubic cm = 1000 cubic mm
```

Explanation:

This checks that each receiving bin has far more capacity than its expected brick count. The extra margin is intentional because the bin is also a catch and settling volume, not just a packed storage box.

Used in:

- bin CAD sizing
- `docs/project/SERVO_BINS_CAD_TUTORIAL.md`
- `cad/DIMENSIONS.md` bin internal target

## Bin Volume Margin

Equation:

```text
volume_margin = bin_volume / perfect_packed_brick_volume
```

Current 2x3 blue bin check:

```text
perfect_packed_brick_volume_for_8_2x3 = about 34.5 cubic cm
volume_margin = 384 cubic cm / 34.5 cubic cm
volume_margin = about 11.1
```

Explanation:

The 2x3 blue bin has the largest expected count at 8 bricks. This margin shows why a bin that looks much larger than the brick volume is still justified: bricks fall randomly, bounce, and need room to settle without blocking the funnel.

Used in:

- bin volume decision
- receiving bin CAD sizing
- engineering notebook explanation for why the bins are larger than perfect-packed volume

## Break-Beam Brick Length From Blocked Time

Equation:

```text
measured_brick_length = belt_speed * beam_blocked_time
```

Equivalent threshold form:

```text
beam_blocked_time_threshold = length_threshold / belt_speed
```

Current size targets:

```text
2x2 length along travel = 15.8mm
2x3 length along travel = 23.7mm
length_threshold = (15.8mm + 23.7mm) / 2
length_threshold = 19.75mm
```

Explanation:

The size sensor uses blocked duration through a break-beam pair. At a known belt speed, a longer brick blocks the beam longer. The midpoint between expected 2x2 and 2x3 travel lengths is the first firmware threshold, then real calibration data should refine it.

Used in:

- firmware size classification
- sensing shroud break-beam timing
- `docs/ARCHITECTURE.md`
- `cad/DIMENSIONS.md`
- `docs/project/CAD_PLAN.md`

## Break-Beam Belt Speed Cross-Check

Equation:

```text
belt_speed = distance_between_beams / time_between_matching_edges
```

Current sensing station geometry:

```text
beam_a_x = 150mm
beam_b_x = 190mm
distance_between_beams = 40mm
```

So:

```text
belt_speed = 40mm / time_between_pair_a_and_pair_b
```

Explanation:

The two break-beam pairs provide a speed cross-check so firmware does not depend only on commanded stepper speed. The time between the leading edge at pair A and the leading edge at pair B estimates actual belt speed during the brick pass.

Used in:

- firmware speed validation
- firmware size classification confidence
- sensing shroud geometry
- `docs/ARCHITECTURE.md`
- `cad/DIMENSIONS.md`
- `docs/project/CAD_PLAN.md`

## Transit Time Between Events

Equation:

```text
transit_time = travel_distance / belt_speed
```

Explanation:

This equation is used whenever firmware estimates how long a brick takes to move from one known X position to another, such as from the sensing station to the belt exit or from route command to handoff. The value must be calibrated with the real belt speed before speed optimization.

Used in:

- firmware handoff timing
- route-ready timing
- conveyor-to-chute validation
- future run log analysis

## Conveyor Drive Belt Pull and Drag Margin

Equations:

```text
pulley_ratio = driven_pulley_teeth / motor_pulley_teeth
usable_motor_torque = holding_torque * usable_torque_fraction
roller_torque = usable_motor_torque * pulley_ratio * timing_stage_efficiency
available_belt_pull = roller_torque / roller_radius
net_belt_pull = available_belt_pull - estimated_total_drag
```

Current assumptions:

```text
NEMA17 holding torque = 0.42 N m
timing_stage_efficiency = 0.90
roller_radius = 10mm = 0.010m
usable_torque_fraction = 0.10 for very conservative moving estimate
usable_torque_fraction = 0.25 for conservative normal moving estimate
```

Available belt pull:

```text
20T motor to 20T driven at 10 percent torque:
available_belt_pull = (0.42 N m * 0.10 * (20 / 20) * 0.90) / 0.010m
available_belt_pull = 3.8N

20T motor to 20T driven at 25 percent torque:
available_belt_pull = (0.42 N m * 0.25 * (20 / 20) * 0.90) / 0.010m
available_belt_pull = 9.4N

20T motor to 60T driven at 10 percent torque:
available_belt_pull = (0.42 N m * 0.10 * (60 / 20) * 0.90) / 0.010m
available_belt_pull = 11.3N

20T motor to 60T driven at 25 percent torque:
available_belt_pull = (0.42 N m * 0.25 * (60 / 20) * 0.90) / 0.010m
available_belt_pull = 28.4N

60T motor to 20T driven at 10 percent torque:
available_belt_pull = (0.42 N m * 0.10 * (20 / 60) * 0.90) / 0.010m
available_belt_pull = 1.3N

60T motor to 20T driven at 25 percent torque:
available_belt_pull = (0.42 N m * 0.25 * (20 / 60) * 0.90) / 0.010m
available_belt_pull = 3.1N
```

Estimated drag budget:

```text
estimated_total_drag = belt_support_drag + bearing_drag + tracking_drag + brick_drag + splice_or_startup_margin
```

Current drag cases:

```text
good_build_drag = 1.5N
conservative_real_build_drag = 5.0N
bad_build_drag = 15.0N
```

Net belt pull after subtracting all estimated drag:

```text
20T motor to 20T driven, 10 percent torque:
net after good 1.5N drag = 2.3N
net after conservative 5.0N drag = -1.2N
net after bad 15.0N drag = -11.2N

20T motor to 20T driven, 25 percent torque:
net after good 1.5N drag = 7.9N
net after conservative 5.0N drag = 4.4N
net after bad 15.0N drag = -5.6N

20T motor to 60T driven, 10 percent torque:
net after good 1.5N drag = 9.8N
net after conservative 5.0N drag = 6.3N
net after bad 15.0N drag = -3.7N

20T motor to 60T driven, 25 percent torque:
net after good 1.5N drag = 26.9N
net after conservative 5.0N drag = 23.4N
net after bad 15.0N drag = 13.4N

60T motor to 20T driven, 10 percent torque:
net after good 1.5N drag = -0.2N
net after conservative 5.0N drag = -3.7N
net after bad 15.0N drag = -13.7N

60T motor to 20T driven, 25 percent torque:
net after good 1.5N drag = 1.6N
net after conservative 5.0N drag = -1.9N
net after bad 15.0N drag = -11.9N
```

Explanation:

The active first-build conveyor choice is 20T motor to 20T driven. It keeps enough torque margin for a smooth conveyor while preserving speed and simple packaging. The 20T motor to 60T driven path has much more force margin but is slower. The 60T motor to 20T driven path is a speed-up path and should not be used unless the bare conveyor proves very low drag.

Used in:

- conveyor timing-stage pulley choice
- motor board and slot planning
- conveyor hand test target
- `docs/project/CONVEYOR_BUILD_GUIDE.md`
- `cad/DIMENSIONS.md`

## GT2 Equal-Pulley Center Distance And Motor Point

Equations:

```text
pitch_diameter = tooth_count * belt_pitch / pi
center_distance = (belt_length - pi * pitch_diameter) / 2
vertical_drop = drive_shaft_z - motor_shaft_z
horizontal_offset = sqrt(center_distance^2 - vertical_drop^2)
motor_shaft_x = drive_shaft_x - horizontal_offset
slot_angle = atan(vertical_drop / horizontal_offset)
motor_board_z = motor_shaft_z + conveyor_side_plate_base_offset
```

Current 20T-to-20T use:

```text
tooth_count = 20
belt_pitch = 2mm
belt_length = 200mm
drive_shaft_x = 340mm
drive_shaft_z = 50mm
motor_shaft_z = 14mm

pitch_diameter = 20 * 2mm / pi
pitch_diameter = about 12.73mm

center_distance = (200mm - pi * 12.73mm) / 2
center_distance = 80mm

vertical_drop = 50mm - 14mm
vertical_drop = 36mm

horizontal_offset = sqrt(80mm^2 - 36mm^2)
horizontal_offset = about 71.4mm

motor_shaft_x = 340mm - 71.4mm
motor_shaft_x = about 268.6mm

slot_angle = atan(36mm / 71.4mm)
slot_angle = about 26.8 degrees

motor_board_z = 14mm + 12mm
motor_board_z = 26mm
```

Explanation:

The outside motor board uses the active 20T motor pulley to 20T driven pulley path with a 200mm GT2
belt. These equations locate the motor shaft from a visible side-view skeleton sketch: drive shaft
point, 80mm construction circle, and Z=14 construction line. Because the conveyor side-plate bottom
starts 12mm above the base, the motor board sketch uses the projected base-relative motor point at
X=268.6mm, Z=26mm before adding the 22mm center hole and four NEMA17 adjustment slots.

Used in:

- outside NEMA17 motor-board CAD
- GT2 pulley center-distance layout
- timing-belt adjustment slot angle
- `docs/project/CONVEYOR_BUILD_GUIDE.md`
- `cad/DIMENSIONS.md`
