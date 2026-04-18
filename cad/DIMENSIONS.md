# Critical Dimensions

Truth model:
- `LOCKED_TRUTH`: stable, physically invariant constraints
- `DECIDED_FOR_STATES`: active states-build choice
- `PROVISIONAL_ESTIMATE`: best estimate before CAD import or physical testing
- `OPEN_DECISION`: unresolved and tracked in `docs/project/OPEN_DECISIONS.md`

## Bricks

Status for this section: `LOCKED_TRUTH`

| Dimension | 2x2 | 2x3 |
|-----------|-----|-----|
| Width | 15.8mm | 15.8mm |
| Length | 15.8mm | 23.7mm |
| Height without studs | 11.4mm | 11.4mm |
| Stud height | 1.7mm | 1.7mm |
| Weight | about 2g | about 3g |

Manual feed orientation target: studs up, long side along conveyor travel.

As-fed 2x3 brick envelope:
- X travel length: 23.7mm
- Y cross-belt width: 15.8mm

Reason: this is easiest for an evaluator to place repeatably and gives the sensing station a predictable in-flight profile. If physical testing shows a different orientation is more reliable, update this file and the feed label together.

## Footprint

Status for this section: `LOCKED_TRUTH`

| Feature | Dimension |
|---------|-----------|
| Maximum footprint | 610mm x 610mm |
| CAD boundary | 610mm x 610mm construction rectangle |
| Bin access | Front or front-side removal preferred |
| Cable routing | Outside conveyor, chute, servo horn, and bin paths |

## Frame

Status for this section: `DECIDED_FOR_STATES` for wood frame direction, `PROVISIONAL_ESTIMATE` for exact dimensions

| Feature | Dimension or guidance |
|---------|-----------------------|
| Material | Wood frame from available stock, with 3D printed brackets where useful |
| 2020 extrusion | Not active for frame or conveyor support |
| Belt surface height | `OPEN_DECISION` - Starting point is about 72mm from base (12mm cleats plus 60mm conveyor-local Z). Cleat height is gated on teammate's servo chute entry height requirement. If the chute needs more clearance, cleats grow to 20-25mm. Do not finalize or print motor mount feet, exit lip, or motor board drill template until confirmed. |
| Minimum height check | Belt exit plus exit lip must feed the chute entry; chute exit must land in the bin funnel catch window |
| Base plate | Start around 590mm x 590mm, adjust within the 610mm x 610mm limit after bin and chute layout |
| Frame style | Low base plate with short cleats, standoffs, or brackets supporting the existing conveyor side plates. Do not use full-length duplicate side panels or tall 180mm conveyor legs unless testing proves they solve a real problem |
| Stiffness target | No visible sensor or chute drift during servo motion |

## Conveyor

Status for this section: `PROVISIONAL_ESTIMATE`

| Feature | Dimension or guidance |
|---------|-----------------------|
| CAD source | Tutorial-style conveyor from `docs/project/CONVEYOR_BUILD_GUIDE.md`, modified for team measurements |
| Motor | NEMA17 17HS4401S reference |
| Shaft and bearing path | 5mm shaft with matching 5mm-ID bearings |
| Usable top-run length | 300mm to 400mm target |
| Side plate length | 370mm |
| Side plate height | 80mm |
| Side plate thickness placeholder | 8mm |
| Side plate inner gap | 50mm |
| Conveyor belt centerline | Y=25mm within the 50mm inner side-plate gap |
| Side plate outside width | 66mm from two 8mm plates plus 50mm gap |
| Idler shaft center | Conveyor-local X=30mm, Z=50mm |
| Drive shaft center | Conveyor-local X=340mm, Z=50mm |
| Roller center distance | 310mm |
| Conveyor side plate bottom above base | 12mm starting point |
| Upper belt surface | Conveyor-local Z=60mm, about 72mm above base with the 12mm starting support height |
| Lower return nominal line | Conveyor-local Z=40mm |
| Belt width | 25mm active neoprene strip |
| Minimum clear brick width | 15.8mm as-fed brick width plus side clearance; 30mm guide and shroud gaps are for belt clearance, belt tracking, and yaw tolerance |
| Wood block support | Wood block under top belt run, not 2020 extrusion |
| Wood block length | About 255mm to 260mm with current rollers, adjusted from dry-fit to clear both roller flange envelopes by about 10mm or more |
| Wood block width | About 20mm to 50mm, measured from Home Depot stock and kept inside side plate gap |
| Wood block thickness | About 10mm to 12mm with the current 20mm roller diameter, or revised after lower-return clearance check |
| Wood block top height | About 2mm below upper belt surface. Conveyor-local Z=58 is only the current layout anchor |
| Crossmember spacers | Optional 49.5mm to 50mm inside-gap spacers. Fit only where they clear drive hardware, belt wrap, set screw access, exit lip, and chute entry. Do not widen the frame to fit a spacer |
| Side plate material | Thin wood, hardboard, or plywood, measured after local purchase |
| Belt speed phase 1 | Start slow enough for reliable sensing, then tune |
| Belt exit | Direct handoff into servo chute entry |
| Tension method | Slotted idler bearing holders plus NEMA17 slots in the outside motor board |
| Timing belt and pulley | Active first-build path is 20T motor pulley to 20T driven pulley with a 200mm GT2 belt |
| 20T-to-20T center distance | 80mm nominal. Final check pending confirmed conveyor height. |
| Motor pulley Y position | Motor pulley face flush with motor output shaft tip |
| Driven pulley Y position | Driven pulley seated against inner bearing holder face with a 2mm printed spacer between pulley and holder. Both 20T pulleys must be coplanar in Y before tightening set screws. |
| NEMA17 package | Body outside the right-side grounded motor board, shaft pointing inward toward the conveyor, GT2 pulleys in the rear/right service bay |
| NEMA17 20T motor point | Conveyor-local X=268.6mm, Z=14mm from the side-view skeleton sketch |
| NEMA17 motor-board point | `OPEN_DECISION` - Base-relative Z depends on confirmed conveyor cleat height. At 12mm cleats: Z=26mm. At 20mm cleats: Z=34mm. At 25mm cleats: Z=39mm. Do not finalize until teammate's chute height is known. |
| Motor board | Vertical board parallel to the right side plate, grounded through 2-stage L bracket with gussets on motor and frame sides, feet with M3 screw holes |
| Motor board X span | X=220mm to X=330mm first CAD sketch |
| Motor board height | 80mm minimum |
| Motor board thickness | 6mm to 8mm placeholder, measured from real wood |
| Motor board Y placement | Inner face about 25mm outboard from right side plate outside face. If left inner side plate face is Y=0, motor board inner face is about Y=83mm |
| Motor board center clearance | 22mm diameter at the projected base-relative X=268.6mm, Z=26mm motor shaft point |
| NEMA17 slots | Four 12mm x 3.5mm slots on the 31mm x 31mm bolt pattern, angled about 26.8 degrees toward the drive shaft |
| Motor board through holes | Four base-relative corner holes at X=230/Z=8, X=320/Z=8, X=230/Z=72, X=320/Z=72. Use 4.3mm for M4 preferred or 3.3mm for M3 fallback |
| Motor clearance check | GT2 belt envelope must clear motor board, side plate, bearing holders, brace hardware, bolt heads, and pulley set screws |
| 25mm neoprene strip | Active first belt material |
| 15mm neoprene strip | Spare stock unless testing shows a use |

## Sensing Station

Status for this section: `DECIDED_FOR_STATES` for size and color sensors

| Feature | Dimension or guidance |
|---------|-----------------------|
| Size sensor family | Break-beam timing, two pairs inside sensing shroud |
| Break beam pair A location | X=150 in assembly (15mm into shroud front) |
| Break beam pair B location | X=190 in assembly (55mm into shroud front) |
| Break beam height | Z=63, 3mm above belt surface |
| Break beam hole diameter | 3.3mm through shroud side walls |
| Size timing logic | Blocked duration at known belt speed; 15.8mm vs 23.7mm brick length |
| Color sensor | TCS3200/GY-31 module in sensing shroud roof |
| Color sensor face distance to brick | About 19mm from sensor face to top of brick studs |
| Sensing shroud location | X=135 to X=200 in assembly |
| Sensing shroud mount | Removable screw-mounted tabs or flanges to the wood conveyor bed or rigid wood frame member near the feed side, not to the belt or chute |
| Sensing shroud outer width | 38mm in Y |
| Sensing shroud outer Y span | Y=6mm to Y=44mm when centered in the 50mm side-plate gap |
| Sensing shroud outer height | 36mm above belt surface |
| Sensing shroud inner width | 30mm in Y |
| Sensing shroud inner Y span | Y=10mm to Y=40mm when centered in the 50mm side-plate gap |
| Sensing shroud inner height | 32mm above belt surface |
| Sensing shroud length | 65mm in X |
| Sensing shroud wall thickness | 4mm |
| Sensing shroud entry chamfer | 3mm on inner edges of front opening |
| Color sensor pocket depth | 8 to 10mm into roof, sensor face flush with inner ceiling |
| Side rail guide entry gap | 38mm at X=0 |
| Side rail guide exit gap | 30mm at X=120 |
| Side rail height above belt | 6mm |
| Side rail thickness | 4mm |
| Side rail first build | Separate screw-on, taped, or clamped rails until brick centering is proven. Do not merge into the shroud until the rail gap is verified with real bricks |
| Exit lip location | X=355 to X=370 in assembly |
| Exit lip height above belt | First test top at conveyor-local Z=65 to Z=66, 5 to 6mm above belt surface |
| Exit lip ramp angle | About 45 degrees from vertical; optional 3mm fillet to soften |
| Exit lip span | 50mm in Y, fills inner gap |
| Exit sensor | Not in base design; use transit time timer. Add only if testing requires it. |

Color calibration is valid only with the shroud installed in the final screw-mounted position.

## Servo Rotary Chute Selector Geometry

Status for this section: `DECIDED_FOR_STATES` for actuator family and selector concept; `PROVISIONAL_ESTIMATE` for pivot height, chute radius, funnel geometry, and belt handoff until frame CAD and real-brick tests confirm the stack.

Coordinate warning: the conveyor CAD currently uses a local belt-surface datum around Z=60mm in some files, while the low-frame assembly may place that surface around 72mm from the machine base. Do not mix those datums. Use construction points, axes, and planes in a visible assembly skeleton sketch, then convert conveyor-local Z values into the final machine-base coordinate system before locking the selector position.

### Servo Mount and Pivot

| Feature | Dimension | Notes |
|---------|-----------|-------|
| Servo family | MG995/MG996/MG996R-class heavy servo | See `docs/datasheet/motion/heavy_servo/` |
| Servo reference path | `docs/datasheet/motion/heavy_servo/` | Use reference CAD, then verify against the real servo |
| Servo pivot axis height | Start at Z=90mm | Working machine-base datum, not final until chute entry, exit, and bin funnel heights agree |
| Servo rotation axis | Vertical | Output shaft points up, chute rotates in horizontal plane |
| Servo mount type | Pocket or cradle | Must match real servo body footprint and fastener pattern |
| Frame attachment | To be determined | CAD around the working pivot first, then adapt the mount to the final frame |

### Chute Arm and Channel

| Feature | Dimension | Notes |
|---------|-----------|-------|
| Sweep radius, pivot to exit in plan view | 65mm to 75mm only if funnels are narrowed or staggered | At 35-degree spacing, 50mm mouths need about 83mm radius and 60mm mouths need about 100mm radius to avoid overlap |
| Arm design | Rigid servo horn adapter or 3D-printed extension from pivot boss | Servo horn bolts to arm; minimize flex |
| Chute internal width | 30mm | Clears the 15.8mm as-fed width and gives tolerance for yaw or partial rotation during handoff |
| Chute internal height | 15mm | Accommodates 11.4mm brick body plus 1.7mm studs with 1.9mm clearance |
| Chute wall thickness | 3mm | Structural, accounts for print tolerances |
| Chute length, entry to exit | 100mm to 150mm | Smooth ramp, no sharp edges or steps |

### Chute Slope and Exit Position

| Feature | Dimension | Notes |
|---------|-----------|-------|
| Chute angle from horizontal | Start at 40 degrees | Test 30, 35, 40, and 45 degrees with UHMW and real bricks |
| Old 65mm centerline drop check | About 42mm drop | Z=90mm pivot gives about Z=48mm centerline before wall offsets |
| Old 75mm centerline drop check | About 48mm drop | Z=90mm pivot gives about Z=42mm centerline before wall offsets, so longer is lower, not higher |
| Chute entry height | Derived from belt exit trajectory | Do not reuse the chute exit height for the entry |
| Chute exit height | Derived from chute slope and bin funnel catch window | Must be checked in CAD against the final bin and funnel geometry |
| Chute exit XY position | Derived from sweep radius and servo angle | Exact offset depends on frame geometry and pivot location |

### Bin Funnel and Receiving

| Feature | Dimension | Notes |
|---------|-----------|-------|
| Funnel mouth opening | Start 50mm to 60mm | 70mm only if the bin layout is staggered or the sweep radius is increased |
| Funnel vertical catch window | Align to measured chute exit height | Do not use the old Z=78-83mm mouth height unless the full Z stack is re-derived |
| Funnel narrowing height | 15mm to 20mm | Guides brick from catch mouth into bin |
| Funnel internal width at base | 30mm | Matches chute internal width; linear transition in 15-20mm height |
| Funnel wall angle | 35 to 45 degrees | Inward from vertical; optimize for UHMW slide friction and real-brick validation |
| Bin internal width | 80mm | Four bins total, fits four 30mm-wide chute ports |
| Bin internal depth | 80mm | Front-to-back, allows brick settling and stable stacking |
| Bin internal height | 60mm | Total depth from base (Z=0) to top (Z=60) |
| Bin base height | Z=0mm | Ground level, machine frame reference |
| Bin top opening | Z=60mm | If the funnel catches above this, model it as an added top funnel; if it catches below this, model a side-entry catch and verify unloading clearance |

### Bin Positioning and Servo Sweep

| Feature | Dimension | Notes |
|---------|-----------|-------|
| Total servo sweep | 105 degrees | Covers all four bin positions with tolerance |
| Servo positions | Start at 37, 72, 107, 142 degrees | Starting table in bin order: 2x2 RED, 2x2 BLUE, 2x3 RED, 2x3 BLUE. Lock after bin layout and real servo testing |
| Position spacing | Start at 35 degrees | Increase spacing, increase radius, narrow funnels, or stagger bins if catch zones overlap |
| Servo positioning accuracy required | Design for several degrees of tolerance | Do not assume plus or minus 2 degrees until measured on the real frame |
| Bin arrangement | Under chute arc, likely staggered if radius stays short | Four bins positioned to catch each servo position without funnel overlap |

Verification in Fusion: use a revolute joint at the working Z=90mm pivot, then check two things separately: the chute exit XY sweep clears adjacent funnels, and the chute entry/exit Z stack matches the conveyor exit and bin catch window. Test with real bricks before final print.

### Servo-to-Conveyor Handoff

| Feature | Dimension | Notes |
|---------|-----------|-------|
| Belt surface height | Derived from final frame CAD | Current low-frame start is about 72mm from base, but global height is not locked until handoff is tested |
| Chute entry height | Derived from the belt exit and exit lip | Entry height must be near the incoming brick path, not copied from chute exit height |
| Conveyor-to-chute connection | Gravity handoff or close-guided handoff | Choose after final belt height and exit-lip test |
| Exit lip position | 5 to 6mm above belt surface | Separate belt-mounted feature, not servo-mounted |
| Handoff validation | Required before large prints | Test with real bricks at the selected belt speed |

### UHMW Lining (mandatory material, not CAD geometry)

| Feature | Specification | Notes |
|---------|---------------|-------|
| Material | Ultra-high-molecular-weight polyethylene (UHMW) | Applied by physical adhesive or mechanical press-fit before first run |
| Applied to | Chute interior walls and funnel interior walls | Reduces friction, improves slide reliability and brick speed control |
| Installation timing | Before first run | Glued or pressed into final CAD parts after printing and quality check |
| Slide angle validation | 30 to 40 degrees | Empirical test with real brick + UHMW surface; angle determines brick velocity |
| Validation method | Physical test with real bricks, not simulation | Slide test before locking in funnel angle |
| Expected outcome | Brick slides steadily without sticking or accelerating excessively | Adjust funnel angle or UHMW surface if needed after test |

## Bins

Status for this section: `PROVISIONAL_ESTIMATE`

| Bin label | Count target | Internal target |
|-----------|--------------|-----------------|
| 2x2 RED | 6 bricks | 80mm x 80mm x 60mm |
| 2x2 BLUE | 6 bricks | 80mm x 80mm x 60mm |
| 2x3 RED | 4 bricks | 80mm x 80mm x 60mm |
| 2x3 BLUE | 8 bricks | 80mm x 80mm x 60mm |

Arrangement: four bins under the chute arc. Exact positions are derived from the final pivot point, chute length, and tested servo angles.

## Manual Feed Guide

Status for this section: `DECIDED_FOR_STATES`

| Feature | Dimension or guidance |
|---------|-----------------------|
| Base feed mode | One brick at a time |
| Optional later feature | Simple feed chute only after the base sorter works |
| Orientation label | Studs up, long side along travel |
| Guide width | Belt width or narrower guide that still clears the 15.8mm as-fed brick width, with tolerance for yaw before the shroud |
| Anti-wander guide | Low side rails from X=0 to X=120, tapering toward the shroud |

## Operator-Facing Packaging

Status for this section: `DECIDED_FOR_STATES`

| Feature | Requirement |
|---------|-------------|
| Start button label | Printed or engraved, visible at arm's length |
| Display viewing distance | Arm's length readable from front of machine |
| Display state words | READY, SORTING, SORT COMPLETE, ERROR |
| Bin labels | 2x2 RED, 2x2 BLUE, 2x3 RED, 2x3 BLUE |
| Label flats | Reserved on bin faces and control area |
| Cable routing | Clean and strain-relieved |
| Fan | Keep if needed for electronics cooling |

## Validation Checklists

These checks apply to `PROVISIONAL_ESTIMATE` and `OPEN_DECISION` values above.

### Conveyor

- Import or model the chosen conveyor
- Verify belt width and brick clearance
- Verify motor and pulley clearance
- Verify handoff from belt exit to chute entry
- Verify footprint with frame and bins visible

### Sensing

- Fit the two break-beam pairs without blocking brick travel
- Verify break-beam timing separates 2x2 from 2x3 at the selected belt speed
- Fit the color sensor shroud and verify no light leaks
- Verify shroud clearance with real bricks
- Record calibration with the final shroud installed

### Servo Chute

- Measure or import the actual servo model
- Test chute angle with real bricks
- Verify four servo angles in CAD
- Verify bin guide overlap at each position
- Verify servo power and wiring clearance

### Frame And UX

- Verify 610mm x 610mm footprint
- Verify bin removal clearance
- Verify labels are visible
- Verify the start control and display are readable from arm's length
