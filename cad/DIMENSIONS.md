# Critical Dimensions

Truth model:
- `LOCKED_TRUTH`: stable, physically invariant constraints
- `PROVISIONAL_ESTIMATE`: best estimate before purchased hardware is in hand

Current rule:
- LEGO brick geometry is `LOCKED_TRUTH`
- Long-side-across brick orientation is `LOCKED_TRUTH`
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

Orientation rule: bricks are long-side-across the conveyor, so the 23.7mm side of a 2x3 spans across the channel and the 15.8mm side runs along travel.
Reason: this keys the release zone to the long side so only one brick can occupy the chamber footprint at a time. Chamber, chute, and sensor geometry must be re-derived from this rule before freeze.
This orientation requirement is `LOCKED_TRUTH`.

---

## Feed chute

Status for this section: `PROVISIONAL_ESTIMATE`
This geometry is provisional and must be re-derived for the long-side-across orientation before freeze.

| Feature | Dimension |
|---------|-----------|
| Internal width | 25mm target |
| Internal depth | provisional re-derive |
| Chute loading orientation | Parallel to final long-side-across chamber orientation |
| Start gate location | Straight chute section above transition |
| Start gate motion | Side-sweep paddle, flush or near-flush when open |
| Exit opening height | 13.5mm |
| Top flare opening | 35mm x 40mm |
| Flare transition length | 20mm |
| Wall thickness | 3mm PLA |
| Interior surface | Adhesive-backed UHMW tape |

---

## Belt channel

Status for this section:
- `LOCKED_TRUTH` for belt strip accommodation constraints:
  - Internal channel width = 25mm target
- `PROVISIONAL_ESTIMATE` for all other values in this section
The channel and sensing geometry below remain provisional until the long-side-across layout is re-derived.

| Feature | Dimension |
|---------|-----------|
| Internal channel width | 25mm target |
| Channel wall height | 15mm |
| Wall thickness | 3mm PLA |
| Interior surface | Adhesive-backed UHMW tape on brick-contact faces |
| Belt material | `25mm x 3mm` neoprene strip is the active architecture. Treat `15mm x 3mm` as spare stock only |
| Belt closure | Scarf-spliced neoprene loop with rubber contact cement, prototype and validate before frame freeze |
| Transport length | 100-120mm |
| Chamber pitch | 18-22mm |
| Drive roller OD | 25mm |
| Drive roller bore | 5mm supported shaft with clampable flat profile |
| Drive pulley teeth | 20T provisional baseline |
| Motor pulley teeth | 20T provisional baseline |
| Timing belt pitch | 2mm provisional |
| Timing belt width | 6mm provisional |
| Pulley center distance | 80mm nominal with slot travel |
| Tension adjustment travel | 8mm provisional |
| Supported shaft bearing | MR85ZZ x2 provisional |
| Idler roller OD | 25mm (0.5mm crown) |
| Idler roller bearings | MR85ZZ 5mm ID x 8mm OD x 2.5mm |
| Roller flanges | 2mm both ends |
| Belt speed (phase 1) | 100mm/s |
| Belt bed | Integrated printed flat support path in the first CAD pass |
| Hall sensor gap | Optional diagnostic only |
| Hall magnets | Optional diagnostic only |

---

## Isolation chamber

Status for this section: `PROVISIONAL_ESTIMATE`
This chamber geometry is provisional and must be re-derived for the long-side-across orientation before freeze.

| Feature | Dimension |
|---------|-----------|
| Internal width (across belt) | 25mm target |
| Internal depth (along belt) | 27mm |
| Internal height above chamber floor | 15mm |
| Wall thickness | 3mm PLA |
| Stop wall damper | 3M rubber foot, 3mm |
| Entry opening height | 13.5mm |
| Stop wall micro-switch protrusion | 1.5mm required seat switch |

### Size beam positions
Reference wall = entry side (opposite stop wall). Re-derive beam positions for the long-side-across chamber before freezing the sensing layout.

| Beam | From reference wall |
|------|---------------------|
| Beam 1 | 5mm |
| Beam 2 | 21mm |

Spacing and detection are provisional until the long-side-across sensing geometry is re-derived.

### Color sensor window

| Feature | Dimension |
|---------|-----------|
| Window size | 12mm x 12mm |
| Window position (depth) | Centered at 13.5mm from reference wall |
| Window height (above chamber floor) | 5.7mm |
| Shroud depth | 15mm external |

### Queue pitch and timing

| Feature | Dimension |
|---------|-----------|
| Brick seat pitch | 18-22mm provisional |
| Reset-to-next-advance gap | 0-20ms safe overlap window, software only |
| Cold-start transport window | 100-120mm provisional |
| Steady-state advance distance | Chamber pitch only |

---

## Release mechanism

Status for this section: `PROVISIONAL_ESTIMATE`

The actuation family is frozen, but the exact support geometry still needs prototype work.

| Feature | Dimension |
|---------|-----------|
| Support removal method | Retracting support |
| Actuator style | 0530-class solenoid |
| Release envelope | Must clear chamber, selector, and conveyor envelopes |
| Reset confirmation | Physical confirmation required before feed restart |
| Return bias | Light spring or equivalent |
| Mounting interface | Printed support and solenoid mount, still provisional |
| Reset timing | PROVISIONAL_ESTIMATE |
| Reset-truth provision | Reserve a return-flag and micro-switch mounting feature in the first CAD pass |

---

## Operator-facing packaging

Status for this section: `PROVISIONAL_ESTIMATE`

| Feature | Dimension |
|---------|-----------|
| Start button label | Printed or engraved, visible at arm's length |
| Display viewing distance | Arm's length readable from front of machine |
| Display state words | READY, SORTING, SORT COMPLETE, ERROR |
| Bin labels | 2x2 RED, 2x2 BLUE, 2x3 BLUE, 2x3 RED |
| Label flats | Reserved on front frame surfaces and bin faces |
| Cable routing clearance | Keep harness outside chute, belt, and drop envelope |
| Conveyor service access | Motor pulley, drive roller, and tensioner must remain reachable without full teardown |

---

## Selector chute

Status for this section: `PROVISIONAL_ESTIMATE`
This is an indexed chute, not a circular disc.

| Feature | Dimension |
|---------|-----------|
| Selector body envelope | 100mm class provisional |
| Selector thickness | 8mm |
| Hub interface | Rigid 5mm flange-mount hub with center clearance pocket and M3 face-mount bolt pattern |
| Funnel opening size | 32mm x 22mm |
| Outlet offset from centerline | 40mm |
| Index positions | 1, 2, 3, 4 |
| Funnel wall taper | 10 degrees inward |
| Shaft hub | Rigid 5mm flange-mount hub with M3 face mounting |
| Selector top clearance from chamber floor | 5mm |
| Selector home method | Required micro-switch with mechanical flag |
| Routing mode | Active selector chute, evidence gate remains open in notebook only |

---

## Selector motion model

Status for this section: `PROVISIONAL_ESTIMATE`

The selector is a 4-position indexer, but do not freeze exact step counts or timing from this document yet.
The final motion model must be bench-derived from the real NEMA11, real microstepping choice, real selector inertia, and the chosen homing method.

---

## Bins

Status for this section: `PROVISIONAL_ESTIMATE`

| Bin | Direction | Category | Count | Internal |
|-----|-----------|----------|-------|----------|
| 1 | NW | 2x2 red | 6 | 40x40x90mm |
| 2 | NE | 2x2 blue | 6 | 40x40x90mm |
| 3 | SE | 2x3 blue | 8 | 40x45x110mm |
| 4 | SW default | 2x3 red | 4 | 40x45x70mm |

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
| Base plate | 610mm x 610mm x 6mm MDF or equivalent confirmed stock sheet |
| Mechanical envelope estimate | 380mm x 420mm |
| Hard limit | 610mm x 610mm |
| Conveyor packaging priority | Early, before large frame lock |

Confirm in full CAD before printing frame.

---

## Validation On Receipt Checklists

These checklists apply to all `PROVISIONAL_ESTIMATE` values above.

### Feed chute

- Verify printed chute internal width and depth with calipers.
- Verify single brick exit gate at 13.5mm target with real bricks.

### Belt channel and rollers

- Verify belt strip measured width and thickness from received strip stock.
- Verify actual NEMA 17 shaft diameter, flat depth, and usable shaft length against modeled motor pulley assumptions.
- Verify supported shaft diameter, bearing span, and usable shaft length against modeled drive roller assumptions.
- Verify idler bearing seat fit with received MR85 bearings.
- Verify crown tracking behavior under powered belt run.

### Isolation chamber and sensing interfaces

- Verify actual sensor body clearances for beam holders and color window stack.
- Verify switch body and actuator positions against printed mounts.

### Release mechanism and reset interfaces

- Verify the chosen release mechanism clears support cleanly without intruding into the chamber envelope.
- Verify any required return element or actuator mount against received unit dimensions.
- Verify physical reset confirmation using the chosen hardware once the concept is frozen.

### Selector and indexing interfaces

- Verify actual stepper shaft and hub dimensions from received hardware.
- Verify the required selector home switch using final mounted hardware.

### Bins and packaging

- Verify printed bin internal dimensions with real brick stack tests.
- Verify assembled footprint remains within 610mm x 610mm.
