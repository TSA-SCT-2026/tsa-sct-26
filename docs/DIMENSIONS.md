# Critical Dimensions

Single reference for every dimension needed for CAD and calibration.
If a value here conflicts with a subsystem doc, this file wins.

---

## Bricks

| Dimension | 2x2 | 2x3 |
|-----------|-----|-----|
| Width (along belt) | 15.8mm | 23.7mm |
| Depth (across belt) | 15.8mm | 15.8mm |
| Height (with studs) | 11.4mm | 11.4mm |
| Stud protrusion above body | 1.7mm | 1.7mm |
| Weight | ~2g | ~3g |

---

## Chute

| Feature | Dimension |
|---------|-----------|
| Internal cross-section | 22mm x 27mm |
| Total height | 310mm |
| Brick stack zone | 274mm (top) |
| Cam/exit zone | 36mm (bottom) |
| Top flare opening | 35mm x 40mm |
| Flare transition length | 20mm |
| Wall thickness | 3mm PLA |
| Size beam position (in 27mm dim) | 20mm from reference wall |
| Color sensor window | 12mm x 12mm, at cam level |
| Sensor shroud depth | 15mm (external, black PLA) |

Why 22mm x 27mm: the 22mm dimension self-aligns 2x3 bricks (23.7mm blocked, 15.8mm passes).
The 27mm dimension allows 1.65mm clearance per side for the 23.7mm brick face.

---

## Escapement (cam)

| Feature | Dimension |
|---------|-----------|
| Disk diameter | 30mm |
| Lobe extension | 8mm |
| Lobe arc | 50 degrees (tune +/-5 deg for double-release prevention) |
| Chord depth (flat seat) | 2mm |
| Shaft bore | 5mm (NEMA 11 shaft) |
| Cam dwell window | ~260ms at 3 rev/sec |
| Exit ramp length | 10mm |
| Exit ramp angle | 5 degrees decline |

---

## Belt and Channel

| Feature | Dimension |
|---------|-----------|
| Belt circumference | 640mm (GT2 closed loop) |
| Belt width | 20mm |
| Active transport length | 290mm |
| Belt speed (target) | 200mm/s |
| Belt bed support length | 290mm |
| Belt bed material | 3mm aluminum flat bar, PTFE tape on top |
| Pulley | 20-tooth GT2, 6mm bore |
| Channel internal width | 22mm |
| Channel height above belt | 15mm |
| Pusher slot width | 24mm |
| Pusher slot height | 13mm |
| Pusher slot chamfer | 1mm |

---

## Pusher Positions (from chute exit)

| Pusher | Position | Category | Bin |
|--------|----------|----------|-----|
| 1 | 75mm | 2x2 red | 1 |
| 2 | 150mm | 2x2 blue | 2 |
| 3 | 225mm | 2x3 blue | 3 |
| Belt end | 290mm | 2x3 red | 4 (DEFAULT) |

Inter-brick spacing at 200mm/s and 3 bricks/sec: 67mm. Minimum pusher spacing: 70mm.
Pusher spacing in this design: 75mm. Safe.

---

## Pusher (face plate and solenoid)

| Feature | Dimension |
|---------|-----------|
| Face plate width | 20mm (along belt axis) |
| Face plate height | 11mm |
| Face plate thickness | 2mm |
| Face plate chamfer | 0.5mm all edges |
| Solenoid rod height above belt | 6mm (aligned to brick center of mass) |
| Solenoid on-time | 40ms |
| Timing window (2x2 brick) | 179ms at 200mm/s |

---

## Timing (at 200mm/s)

| Event | Delay from chute exit beam |
|-------|---------------------------|
| Brick arrives at pusher 1 | 375ms |
| Brick arrives at pusher 2 | 750ms |
| Brick arrives at pusher 3 | 1125ms |
| Brick arrives at belt end | 1450ms |
| Total sort time (24 bricks) | ~9.5s |

---

## Bins

| Bin | Category | Internal W x L x H |
|-----|----------|---------------------|
| 1 | 2x2 red (6 bricks) | 30mm x 30mm x 80mm |
| 2 | 2x2 blue (6 bricks) | 30mm x 30mm x 80mm |
| 3 | 2x3 blue (8 bricks) | 30mm x 35mm x 100mm |
| 4 | 2x3 red (4 bricks, default) | 30mm x 35mm x 60mm |

Floor angle: 10 degrees toward back wall. Break-beam slot at entrance.

---

## Footprint

Base plate: 610mm x 610mm x 6mm MDF/plywood.
Total system length: ~410mm (fits with 200mm spare for electronics bay).
Footprint constraint: 610mm x 610mm hard limit.
