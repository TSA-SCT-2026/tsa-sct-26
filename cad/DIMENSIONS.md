# Critical Dimensions and Variable Register

This file records dimensions that are safe to use and variables that must be derived from the real build.

## Truth Model

- `LOCKED_TRUTH`: competition rule, LEGO brick standard, BOM row, exact purchased listing, datasheet-backed standard part, real measurement, or finalized CAD export.
- `DECIDED_FOR_STATES`: active design choice, but exact geometry may still be open.
- `PROVISIONAL_VARIABLE`: a value to derive from the assembly, not a number to copy.
- `OPEN_DECISION`: unresolved and tracked in `docs/project/OPEN_DECISIONS.md`.

Do not promote a draft CAD coordinate into `LOCKED_TRUTH`. If physical fitting changes a dimension, update the variable source or remove the old number.

## Locked Truth

| Item | Value | Source |
|------|-------|--------|
| Maximum footprint | 610mm x 610mm | Competition constraint |
| Brick set | 6 red 2x2, 6 blue 2x2, 4 red 2x3, 8 blue 2x3 | Competition task |
| Bin labels | 2x2 RED, 2x2 BLUE, 2x3 RED, 2x3 BLUE | Active architecture |
| 2x2 brick footprint | 15.8mm x 15.8mm | LEGO nominal geometry |
| 2x3 brick footprint | 15.8mm x 23.7mm | LEGO nominal geometry |
| Brick body height | 11.4mm plus 1.7mm studs | LEGO nominal geometry |
| Manual feed orientation | Studs up, long side along travel | Active architecture |
| Size sensing family | Two-pair break-beam timing | Active architecture |
| Color sensor family | TCS3200/GY-31 in installed shroud | Active architecture |
| Conveyor motor | NEMA17 conveyor motor | Active architecture and purchased hardware |
| Selector actuator | MG995/MG996/MG996R-class heavy servo | Active architecture and purchased hardware |
| Frame material family | Wood frame with printed brackets where useful | Active architecture |

Part-specific package dimensions belong in the matching datasheet folder or measured part notes. Do not duplicate them here unless the value drives an active interface.

## Active Variables

Use this table instead of old coordinate-heavy CAD instructions.

| Variable | Status | How to derive it |
|----------|--------|------------------|
| `belt_surface_z` | `OPEN_DECISION` | Measure from the final supported conveyor assembly after bin and chute entry requirements are known. |
| `conveyor_support_height` | `OPEN_DECISION` | Derived from `belt_surface_z`, external bearing-holder geometry, side plate geometry, and final handoff stack. |
| `motor_board_z` | `OPEN_DECISION` | Project from the final conveyor shaft and pulley path after support height is locked. |
| `conveyor_foot_height` | `OPEN_DECISION` | Derived from the final supported conveyor, not from old cleat values. |
| `servo_foot_height` | `OPEN_DECISION` | Derived from final servo pivot, bin arc catch height, and frame contact points after the bin and chute stack is modeled. |
| `servo_foot_slot_range` | `PROVISIONAL_VARIABLE` | Starting slot range for physical selector alignment after belt, chute, and bin arc placement. |
| `exit_lip_z` | `OPEN_DECISION` | Derived from `belt_surface_z`, chute entry height, and real-brick handoff tests. |
| `chute_entry_z` | `OPEN_DECISION` | Derived from belt exit trajectory and exit lip test. Do not copy chute exit height. |
| `chute_exit_z` | `OPEN_DECISION` | Derived from chute slope, chute length, and bin arc catch height. |
| `servo_pivot_xyz` | `OPEN_DECISION` | Derived from final conveyor exit, chute sweep, and frame member locations. |
| `sweep_radius` | `OPEN_DECISION` | Derived from chute length, servo clearance, swept brick envelope, and footprint. |
| `bin_arc_catch_width` | `OPEN_DECISION` | Derived from servo repeatability, chute exit scatter, brick yaw margin, divider clearance, and available arc spacing. |
| `bin_body_geometry` | `OPEN_DECISION` | Active CAD critical path. Must preserve four separate categories, sweep-defined catch geometry, removable access, and label visibility. |
| `shroud_inner_width` | `PROVISIONAL_VARIABLE` | Brick width plus yaw and lateral margin, verified with real bricks and guide rails. |
| `shroud_inner_height` | `PROVISIONAL_VARIABLE` | Brick height plus stud clearance, sensor clearance, and print tolerance. |
| `break_beam_spacing` | `PROVISIONAL_VARIABLE` | Large enough for speed cross-check, fitted to the final shroud and sensor modules. |
| `rail_entry_gap` | `PROVISIONAL_VARIABLE` | Wide enough for easy manual placement, then tapered or guided to reliable sensing alignment. |
| `rail_exit_gap` | `PROVISIONAL_VARIABLE` | Matched to measured shroud entry after real-brick fit. |
| `wood_block_length` | `PROVISIONAL_VARIABLE` | Cut from dry-fit clearance between roller and belt-wrap keep-out zones. |
| `wood_block_width` | `PROVISIONAL_VARIABLE` | Measured from actual stock and final side-plate gap. |

## Derived Equations

Non-trivial equations belong in `docs/notebook/EQUATIONS.md`. They must be written symbolically first, with suggested coefficients listed separately.

Key formulas currently used:
- `measured_brick_length = belt_speed * beam_blocked_time`
- `belt_speed = beam_spacing / time_between_beams`
- `travel_time = travel_distance / belt_speed`
- `arc_spacing = 2 * sweep_radius * sin(angle_spacing / 2)`
- `chute_vertical_drop = chute_path_length * sin(chute_angle)`
- `exit_lip_top_z = belt_surface_z + lip_height_above_belt`
- `equal_pulley_center_distance = (belt_length - pulley_tooth_count * belt_pitch) / 2`

## Validation Gates

### Conveyor

- Verify belt tracks without rubbing the shroud, rails, side plates, or support block.
- Verify motor, pulleys, set screws, and belt path remain accessible.
- Verify handoff from belt exit to chute entry with real bricks before locking feet or lip geometry.
- Verify the full assembly stays inside the 610mm x 610mm footprint.

### Sensing

- Fit the two break-beam pairs without blocking brick travel.
- Verify break-beam timing separates 2x2 from 2x3 at the selected belt speed.
- Fit the color sensor shroud and verify no light leaks.
- Calibrate color only with the final shroud installed.

### Servo Chute

- Verify the real servo body and horn before locking a pocket.
- Test chute angle with UHMW and real bricks.
- Verify four servo angles on the real frame.
- Verify each bin arc catch window receives the swept brick envelope with margin.
- Verify servo power and wiring clearance.

### Frame And UX

- Verify bin removal clearance.
- Verify labels are visible.
- Verify the start control and display are readable from arm's length.
- Verify cable routing is clean and strain-relieved.
