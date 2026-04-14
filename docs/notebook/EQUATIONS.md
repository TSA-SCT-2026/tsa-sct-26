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
belt_surface_z = about 68mm
entry_drop = 2mm to 4mm
chute_entry_floor_z = about 64mm to 66mm
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
belt_surface_z = about 68mm
lip_height = 5mm to 6mm
exit_lip_top_z = about 73mm to 74mm
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
