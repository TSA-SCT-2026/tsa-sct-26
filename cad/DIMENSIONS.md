# Critical Dimensions

Truth model:
- `LOCKED_TRUTH`: stable, physically invariant constraints
- `PROVISIONAL_ESTIMATE`: best estimate before purchased hardware is in hand

Current rule:
- LEGO brick geometry is `LOCKED_TRUTH`
- Widthwise brick orientation is `LOCKED_TRUTH`
- Belt strip accommodation for that orientation is `LOCKED_TRUTH`
- All purchased hardware fit values are `PROVISIONAL_ESTIMATE` until verified on receipt

---

## Bricks

Status for this section: `LOCKED_TRUTH`

| Dimension | 2x2    | 2x3    |
|-----------|--------|--------|
| Width     | 15.8mm | 15.8mm |
| Length    | 15.8mm | 23.7mm |
| Height    | 11.4mm | 11.4mm |
| Studs     | 1.7mm  | 1.7mm  |
| Weight    | ~2g    | ~3g    |

Orientation rule: bricks are widthwise across the conveyor, so the 15.8mm width runs across the channel and brick length runs along travel.
Reason: this holds across-channel fit constant for 2x2 and 2x3 bricks, reduces yaw risk, and makes the along-travel dimension measurable by the two fixed size beams.
This orientation requirement is `LOCKED_TRUTH`.

---

## Feed chute

Status for this section: `PROVISIONAL_ESTIMATE`

| Feature | Dimension |
|---------|-----------|
| Internal width | 20mm |
| Internal depth | 28mm |
| Exit opening height | 13.5mm |
| Top flare opening | 35mm x 40mm |
| Flare transition length | 20mm |
| Wall thickness | 3mm PLA |
| Interior surface | PTFE tape all 4 faces |

---

## Belt channel

Status for this section:
- `LOCKED_TRUTH` for belt strip accommodation constraints:
  - Internal channel width = 20mm
  - Belt material target width = 19mm strip
- `PROVISIONAL_ESTIMATE` for all other values in this section

| Feature | Dimension |
|---------|-----------|
| Internal channel width | 20mm |
| Channel wall height | 15mm |
| Wall thickness | 3mm PLA |
| Interior surface | PTFE tape |
| Belt material | 19mm neoprene x 3mm |
| Transport length | 100-120mm |
| Drive roller OD | 25mm |
| Drive roller bore | 6mm D-flat |
| Idler roller OD | 25mm (0.5mm crown) |
| Idler roller bearings | MR115ZZ 5mm ID x 11mm OD x 4mm |
| Roller flanges | 2mm both ends |
| Belt speed (phase 1) | 100mm/s |
| Belt bed | 3mm aluminum bar 22mm wide, PTFE tape |
| Hall sensor gap | 2mm from sensor face to idler rim |
| Hall magnets | 2x 3mm neodymium disc, 180 deg apart |

---

## Isolation chamber

Status for this section: `PROVISIONAL_ESTIMATE`

| Feature | Dimension |
|---------|-----------|
| Internal width (across belt) | 20mm |
| Internal depth (along belt) | 27mm |
| Internal height above platform | 15mm |
| Wall thickness | 3mm PLA |
| Stop wall damper | 3M rubber foot, 3mm |
| Entry opening height | 13.5mm |
| Stop wall micro-switch protrusion | 1.5mm |

### Size beam positions (in 27mm depth dimension)

Reference wall = entry side (opposite stop wall).

| Beam | From reference wall |
|------|---------------------|
| Beam 1 | 5mm |
| Beam 2 | 21mm |

Spacing: 16mm. Detection: BOTH beams LOW = 2x3. Anything else = 2x2.

### Color sensor window

| Feature | Dimension |
|---------|-----------|
| Window size | 12mm x 12mm |
| Window position (depth) | Centered at 13.5mm from reference wall |
| Window height (above platform) | 5.7mm |
| Shroud depth | 15mm external |

---

## Trapdoor platform

Status for this section: `PROVISIONAL_ESTIMATE`

| Feature | Dimension |
|---------|-----------|
| Platform width (along belt) | 22mm |
| Platform depth (across belt, toward disc) | 20mm |
| Platform thickness | 3mm PLA |
| Top surface | UHMW tape |
| Hinge rod diameter | 3mm steel |
| Hinge axis position | Belt-side edge of platform |
| Hinge ear OD | 8mm |
| Hinge ear bore | 3.2mm |
| Hinge bracket bore | 3.2mm |
| Platform tab width | 4mm (running full 22mm along platform far edge) |
| Platform tab height (below platform bottom) | 3mm |
| Platform tab lead-in chamfer | 30 degrees (adjust empirically) |
| Platform return spring wire | 0.3-0.4mm |
| Platform return spring OD | 4-5mm |
| Platform return spring free length | 15mm |
| Platform return spring force at level | < 0.01N (preload only) |

---

## Class 3 lever arm

Status for this section: `PROVISIONAL_ESTIMATE`

| Feature | Dimension |
|---------|-----------|
| Total lever arm length | 30mm |
| Fulcrum position | 0mm (one end) |
| Effort point (solenoid contact) | 8mm from fulcrum |
| Load point (lever tip, under platform tab) | 30mm from fulcrum |
| Mechanical advantage | 8/30 = 0.27 (disadvantage in force, advantage in displacement) |
| Lever tip travel at full solenoid stroke (8mm) | 30mm |
| Lever tip travel required to clear tab | 8-10mm |
| Stroke margin | 30mm / 10mm = 3x minimum |
| Lever arm cross-section | 4mm x 4mm PLA |
| Lever tip chamfer angle | 30 degrees (adjust empirically) |
| Lever return spring type | Tension spring |
| Lever return spring wire | 0.3mm |
| Lever return spring OD | 4mm |
| Lever return spring force at rest | 0.05-0.10N |
| Pivot bolt | M3 x 12mm with nylon locknut |
| Pivot bracket | Printed PLA on chamber side wall (20mm outboard wall) |
| Sweep direction | Outward, away from belt, horizontal plane |

### Solenoid contact geometry

| Feature | Dimension |
|---------|-----------|
| Solenoid plunger axis | Parallel to belt travel direction |
| Contact point on lever | 8mm from fulcrum |
| Solenoid stroke required at contact point | 8mm (nominal JF-0530B stroke) |
| Solenoid mount wall | Chamber side wall (same wall as lever fulcrum) |
| Solenoid axis height above platform | Matched to lever arm height |

---

## Chute selector disc

Status for this section: `PROVISIONAL_ESTIMATE`

| Feature | Dimension |
|---------|-----------|
| Disc diameter | 100mm |
| Disc thickness | 8mm |
| Center bore | 5mm (shaft hub, not direct bore) |
| Funnel opening size | 32mm x 22mm |
| Funnel center radius | 40mm |
| Funnel positions | 315, 45, 135, 225 degrees |
| Funnel wall taper | 10 degrees inward |
| Shaft hub | 5mm aluminum, set-screw |
| Disc top clearance from platform | 5mm |
| Home flag width | 3mm |
| Home flag protrusion | 2mm from rim |

---

## Stepper indexing

Status for this section: `PROVISIONAL_ESTIMATE`

1600 steps/rev (200 steps * 8x microstepping).

| Bin | Direction | Degrees | Steps | At 400 sps | At 2000 sps |
|-----|-----------|---------|-------|------------|-------------|
| 1 | NW | 315 | 1400 | 3500ms | 700ms |
| 2 | NE | 45 | 200 | 500ms | 100ms |
| 3 | SE | 135 | 600 | 1500ms | 300ms |
| 4 SW home | SW | 225 | 1000 | 2500ms | 500ms |

Firmware always takes shortest arc. Max adjacent move: 90 deg = 1000ms at 400 sps.

---

## Bins

Status for this section: `PROVISIONAL_ESTIMATE`

| Bin | Direction | Category | Count | Internal |
|-----|-----------|----------|-------|----------|
| 1 | NW | 2x2 red | 6 | 40x40x90mm |
| 2 | NE | 2x2 blue | 6 | 40x40x90mm |
| 3 | SE | 2x3 blue | 8 | 40x45x110mm |
| 4 SW DEFAULT | 2x3 red | 4 | 40x45x70mm |

---

## Drop fall geometry

Status for this section: `PROVISIONAL_ESTIMATE` (derived estimate)

| Feature | Value |
|---------|-------|
| Fall height | ~80mm |
| Fall time | 128ms |
| Brick velocity at bin entrance | 1.25 m/s |
| KE at impact | 0.0023 J |
| Bounce height (COR 0.5) | 20mm |
| Bin wall height | 70-110mm |
| Margin | 50-90mm |

---

## Footprint

Status for this section: `PROVISIONAL_ESTIMATE` until full assembled CAD is validated

| Feature | Dimension |
|---------|-----------|
| Base plate | 610mm x 610mm x 6mm MDF |
| Mechanical envelope estimate | 380mm x 420mm |
| Hard limit | 610mm x 610mm |

Confirm in full CAD before printing frame.

---

## Validation On Receipt Checklists

These checklists apply to all `PROVISIONAL_ESTIMATE` values above.

### Feed chute

- Verify printed chute internal width and depth with calipers.
- Verify single brick exit gate at 13.5mm target with real bricks.

### Belt channel and rollers

- Verify belt strip measured width and thickness from received strip stock.
- Verify drive shaft actual D profile against modeled bore assumptions.
- Verify idler bearing seat fit with received MR115 bearings.
- Verify crown tracking behavior under powered belt run.

### Isolation chamber and sensing interfaces

- Verify actual sensor body clearances for beam holders and color window stack.
- Verify switch body and actuator positions against printed mounts.

### Trapdoor platform and lever assemblies

- Verify spring free length and installed force from received spring assortments.
- Verify solenoid mounting and plunger alignment using received unit dimensions.

### Selector and indexing interfaces

- Verify actual stepper shaft and hub dimensions from received hardware.
- Verify home switch and flag engagement using final mounted hardware.

### Bins and packaging

- Verify printed bin internal dimensions with real brick stack tests.
- Verify assembled footprint remains within 610mm x 610mm.
