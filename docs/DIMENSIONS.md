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
| Size beam positions (in 27mm dim) | Beam 1: 5mm from reference wall. Beam 2: 21mm from reference wall. |
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
| Chord depth (flat seat) | 4mm |
| Chord width (at 4mm depth) | 20.4mm (2 * sqrt(15^2 - 11^2) = 2 * sqrt(104)) |
| Shaft bore | 5mm (NEMA 11 shaft) |
| Cam dwell window | ~260ms at 3 rev/sec |
| Exit ramp length | 10mm |
| Exit ramp angle | 11.3 degrees (2mm drop over 10mm, arctan(2/10)) |

---

## Belt and Channel

| Feature | Dimension |
|---------|-----------|
| Belt material | Neoprene rubber strip, 19mm (3/4") wide x 3mm thick |
| Belt-to-wall clearance | 1.5mm per side (22mm channel - 19mm belt) / 2 |
| Wall-to-belt-surface gap | 0.5-1mm (walls rest on rails; belt moves freely below) |
| Belt loop length | ~700mm (290mm active + ~79mm roller wrap x2 + 20mm tensioner margin) |
| Belt splice | 20mm tapered overlap, flexible CA glue |
| Drive roller OD | 25mm (bare PLA; heat-shrink sleeve if friction test fails) |
| Drive roller width | 25mm |
| Drive roller bore | 6mm D-flat (direct motor shaft press-fit, M3 set screw) |
| Idler roller OD | 25mm edges, 25.5mm center (0.5mm crown) |
| Idler roller width | 25mm |
| Idler roller bore | 5mm round (M5 bolt axle, spring-loaded tensioner) |
| Roller flange height | 2mm both ends, both rollers |
| Roller center-to-center | 290mm |
| Active transport length | 290mm |
| Belt speed (target) | 200mm/s (calibrated empirically, Hall PID holds during run) |
| Belt bed support | 3mm aluminum flat bar, 280mm x 22mm, PTFE tape on top |
| Hall sensor gap | 2mm from sensor face to idler roller rim |
| Hall magnets | 2x 3mm neodymium disc, 180 degrees apart on idler rim |
| Channel internal width | 22mm |
| Channel height above belt | 15mm |
| Pusher slot width | 24mm |
| Pusher slot height | 13mm |
| Pusher slot chamfer | 1mm |

---

## Belt / Wall / Frame Cross-Section

Looking along the belt travel direction:

```
    PUSHER SIDE                              BIN SIDE

    [SOLENOID]                               [BIN]
        |                                      |
    [face plate 20x11mm]               [bin opening 30x13mm]
        |                                      |
    ====|====CHANNEL WALL 3mm thick============|====CHANNEL WALL 3mm thick====
    |   |   slot 24x13mm    |          |  opening 30x13mm  |                 |
    |   |   (face plate      |  22mm   |  (brick exits      |                 |
    |   |    passes through) | channel |   through here)    |                 |
    |   |                   |          |                   |                 |
    ====|===================|==========|===================|==================
    |                       |          |                                     |
    |  FRAME RAIL 5mm wide  | BELT     |  FRAME RAIL 5mm wide               |
    |                       | 19mm     |                                     |
    =========================BELT BED (aluminum bar 280x22mm, PTFE tape)======
```

Key relationships:
- Walls rest on frame rail tops. Belt runs 0.5-1mm below wall bottom edges.
- Belt (19mm) is 3mm narrower than channel (22mm): 1.5mm clearance each side.
- Slot bottom edges are at belt surface level. Face plate contacts brick center of mass.
- Bin opening (30mm) wider than pusher slot (24mm) to catch brick with slight drift.

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
