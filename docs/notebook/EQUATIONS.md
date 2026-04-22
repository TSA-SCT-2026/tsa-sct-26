# Equations Used in Design

This file logs non-trivial equations used to derive dimensions, timing, or firmware logic for the engineering notebook.

Rule: write equations symbolically first. List suggested coefficients separately with source and confidence. Do not simplify formulas with draft CAD numbers unless every value is source-backed.

## Variable Source Key

- `locked`: competition rule, LEGO nominal geometry, BOM row, exact purchased listing, datasheet-backed standard part, real measurement, or finalized CAD export.
- `measured`: measured from the real assembled machine.
- `test_coefficient`: starting value to try during testing, not source truth.
- `open`: blocked or unresolved.

## Break-Beam Brick Length

Equation:

```text
measured_brick_length = belt_speed * beam_blocked_time
```

Threshold form:

```text
beam_blocked_time_threshold = length_threshold / belt_speed
```

Midpoint threshold:

```text
length_threshold = (short_brick_travel_length + long_brick_travel_length) / 2
```

Variables:

| Variable | Unit | Source |
|----------|------|--------|
| `short_brick_travel_length` | mm | `locked`: 2x2 nominal brick length in the chosen feed orientation |
| `long_brick_travel_length` | mm | `locked`: 2x3 nominal brick length in the chosen feed orientation |
| `belt_speed` | mm/s | `measured`: from encoder, step timing validation, or two-beam timing |
| `beam_blocked_time` | s | `measured`: firmware timestamp |
| `length_threshold` | mm | derived |

Suggested coefficients:
- Use LEGO nominal lengths for the first threshold.
- Replace threshold with calibration data after shroud-installed tests.

Used in:
- Firmware size classification.
- Sensing station calibration.
- Engineering notebook explanation of size sensing.

## Belt Speed From Two Break-Beam Pairs

Equation:

```text
belt_speed = beam_spacing / time_between_beams
```

Variables:

| Variable | Unit | Source |
|----------|------|--------|
| `beam_spacing` | mm | `measured`: final shroud beam center distance |
| `time_between_beams` | s | `measured`: time between matching leading-edge or trailing-edge events |
| `belt_speed` | mm/s | derived |

Suggested coefficients:
- Beam spacing should be large enough to make timing noise small compared with the 2x2 vs 2x3 length difference.
- Final value comes from the actual shroud.

Used in:
- Size sensing cross-check.
- Handoff timing.
- Belt speed calibration.

## Conveyor Transit Time

Equation:

```text
travel_time = travel_distance / belt_speed
```

Variables:

| Variable | Unit | Source |
|----------|------|--------|
| `travel_distance` | mm | `measured`: final distance from sensing reference point to handoff reference point |
| `belt_speed` | mm/s | `measured`: calibrated belt speed |
| `travel_time` | s | derived |

Suggested coefficients:
- Use a conservative delay during early tests so the chute reaches position before the brick commits to the exit.
- Replace with measured timing after repeated runs.

Used in:
- Servo move timing.
- Handoff scheduling.

## Servo Chute Arc Spacing

Equation:

```text
arc_spacing = 2 * sweep_radius * sin(angle_spacing / 2)
```

Variables:

| Variable | Unit | Source |
|----------|------|--------|
| `sweep_radius` | mm | `open`: derived from final bin and chute layout |
| `angle_spacing` | degrees | `test_coefficient` until real servo positions are calibrated |
| `arc_spacing` | mm | derived |
| `bin_arc_catch_width` | mm | `open`: derived from swept brick envelope, catch tests, and available footprint |

Suggested coefficients:
- Sweep the 2x3 brick envelope, not only the chute centerline.
- Include brick width, yaw margin, divider clearance, and servo error before locking bin dividers.
- Use the bin arc as the catch feature for the current sprint instead of separate per-bin funnels.

Used in:
- Sweep-defined bin arc layout.
- Servo chute selector geometry.
- Decision matrix rationale for chute-first catch geometry.

## Equal-Pulley GT2 Belt Center Distance

General open-belt equation:

```text
L_belt = 2 * C + pi * (D1 + D2) / 2 + (D1 - D2)^2 / (4 * C)
```

Equal-pulley form:

```text
C = (L_belt - pi * D) / 2
```

GT2 pulley pitch diameter:

```text
D = tooth_count * belt_pitch / pi
```

Equal GT2 pulley shortcut:

```text
C = (L_belt - tooth_count * belt_pitch) / 2
```

Variables:

| Variable | Unit | Source |
|----------|------|--------|
| `L_belt` | mm | `locked`: purchased closed-loop belt length when confirmed from BOM or physical part |
| `C` | mm | derived center distance between pulley shafts |
| `D1`, `D2`, `D` | mm | derived pitch diameter |
| `tooth_count` | teeth | `locked`: pulley specification when confirmed from purchased part |
| `belt_pitch` | mm/tooth | `locked`: GT2 standard pitch |

Suggested coefficients:
- Current sprint uses a 200mm closed-loop GT2 belt and equal 16T GT2 pulleys.
- With equal 16T GT2 pulleys, `tooth_count * belt_pitch = 32mm`, so nominal `C = 84mm`.
- Put the motor mount slightly inside nominal and allow 1mm to 3mm outward adjustment for belt tension.

Used in:
- Motor mount horizontal spacing.
- GT2 belt placeholder.
- Pulley alignment and tension setup.

## Chute Vertical Drop

Equation:

```text
vertical_drop = chute_path_length * sin(chute_angle_from_horizontal)
```

Exit height:

```text
chute_exit_z = chute_entry_z - vertical_drop
```

Variables:

| Variable | Unit | Source |
|----------|------|--------|
| `chute_path_length` | mm | `open`: final chute geometry |
| `chute_angle_from_horizontal` | degrees | `test_coefficient`: chosen from UHMW-lined real-brick slide tests |
| `chute_entry_z` | mm | `open`: derived from conveyor handoff |
| `vertical_drop` | mm | derived |
| `chute_exit_z` | mm | derived |

Suggested coefficients:
- Test several chute angles with UHMW and real bricks.
- Use the shallowest angle that slides reliably without creating excessive speed or bounce.

Used in:
- Chute entry and exit stack.
- Bin arc catch height.

## Exit Lip Height

Equation:

```text
exit_lip_top_z = belt_surface_z + lip_height_above_belt
```

Variables:

| Variable | Unit | Source |
|----------|------|--------|
| `belt_surface_z` | mm | `open`: final conveyor support height |
| `lip_height_above_belt` | mm | `test_coefficient`: real-brick handoff test |
| `exit_lip_top_z` | mm | derived |

Suggested coefficients:
- Start with a small lift above the belt and test real handoff behavior.
- Current sprint skips a static ramp on the first pass.
- Leave physical room for a small add-on lip if real-brick tests show nose dive or short fall.
- Final value must be derived from the final belt surface, chute entry, and real-brick handoff behavior.

Used in:
- Conveyor-to-chute handoff.
- Exit lip or handoff guide coupon.

## Bin Capacity

Equation:

```text
bin_internal_volume = internal_width * internal_depth * internal_height
```

Brick volume approximation:

```text
brick_envelope_volume = brick_width * brick_length * brick_height
```

Capacity margin:

```text
capacity_margin = bin_internal_volume / (expected_brick_count * brick_envelope_volume)
```

Variables:

| Variable | Unit | Source |
|----------|------|--------|
| `internal_width` | mm | `open`: teammate bin geometry |
| `internal_depth` | mm | `open`: teammate bin geometry |
| `internal_height` | mm | `open`: teammate bin geometry |
| `expected_brick_count` | count | `locked`: competition brick set |
| `brick_width` | mm | `locked`: LEGO nominal geometry |
| `brick_length` | mm | `locked`: LEGO nominal geometry |
| `brick_height` | mm | `locked`: LEGO nominal geometry |
| `capacity_margin` | ratio | derived |

Suggested coefficients:
- Use generous capacity because falling bricks do not pack perfectly.
- Validate that the bin arc catches cleanly before treating storage volume as solved.

Used in:
- Bin sizing rationale.
- Engineering notebook explanation for receiving bin margin.
