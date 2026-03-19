# Mechanical Design Architecture

## Brick dimensions (source of truth)

| Brick | Length | Width | Height | Count | Colors |
|-------|--------|-------|--------|-------|--------|
| 2x2   | 15.8mm | 15.8mm | 11.4mm | 12 | 6 red, 6 blue |
| 2x3   | 23.7mm | 15.8mm | 11.4mm | 12 | 4 red, 8 blue |

Both bricks share the same width and height. Only length differs by 7.9mm. This single fact drives most mechanical and sensor decisions in the project.

## Physical constants

```
Belt width:              25mm
Belt type:               GT2 closed loop, 400mm, 6mm wide
Pulley teeth:            20
Pulley pitch diameter:   12.73mm (20 * 2mm pitch / pi)
Pulley center-to-center: 180mm
Belt speed (operating):  200mm/s
Brick spacing:           28mm center-to-center (23.7mm brick + 4.3mm buffer)
Feed rate:               5 bricks per second
Total sort time:         ~10 seconds for 24 bricks
Footprint:               610mm x 610mm
Materials:               3D printed PLA for all structural parts, fasteners and electronics excepted
```

Belt length math:
```
GT2 20-tooth pulley pitch diameter = 12.73mm
Belt wrap contribution = pi * 12.73 = 39.96mm, rounds to 40mm
400mm belt -> center distance = (400 - 40) / 2 = 180mm
```

System belt usage:
```
Chute base transition:       50mm
Taper channel:               60mm
Sensing zone:                30mm
Sensor to first plow buffer: 30mm
3 plows at ~28mm each:       84mm
Last plow to belt end:       20mm
Total belt needed:           274mm
Belt usable top run:         ~360mm
Margin:                      ~86mm
```

## Parts to print (in order)

1. Chute transition piece: print and test first, before anything else
2. Stepper cam disk: simple geometry, early iteration is cheap
3. Sensor shroud for color sensor: needed for calibration validation
4. Everything else once the chute transition is proven

Do not print the full frame until the chute transition piece reliably feeds bricks at belt speed. That piece is the gate for the entire mechanical build.

## Gravity chute

Near-vertical stack angle: ~75 degrees from horizontal. Bricks orient studs-up for low contact area and minimal friction. Gravity feeds them down into the escapement.

```
Chute inner width:  17.3mm (15.8mm brick + 1.5mm clearance)
Stack height (all 2x3): 24 * 11.4mm = 273mm
Realistic mixed stack:  ~240mm
```

The inner profile must be rectangular to maintain brick orientation. Round pipe cannot constrain rotation. PVC pipe is not a viable substitute. Aluminum square tube would be dimensionally more accurate than printed PLA but adds sourcing complexity and is harder to integrate with the printed frame. Printed PLA is the correct choice with proper tolerance allowance.

A brick loaded sideways jams at the chute entrance rather than deep in the system. This is the correct failure mode: visible, immediate, and recoverable.

### Chute transition piece (highest risk item in the project)

The transition from near-vertical to horizontal at the chute base is a compound geometry problem. FDM tolerances bite hard on curved internal profiles. This piece must:
- Redirect bricks smoothly from ~75 degrees to horizontal
- Not allow bricks to tumble, jam, or stack at the curve
- Work reliably at the escapement release rate with bricks of both lengths

Print this first. Test with real bricks. Iterate until it works every time. Budget 3 iterations minimum. Do not proceed to full frame CAD until this passes. If this piece cannot be made to work reliably, the entire near-vertical chute concept needs revision before anything else is built.

## Stepper cam escapement

Single-lobe cam disk mounted on the stepper motor shaft. One full rotation releases exactly one brick and blocks the queue for the remainder of the rotation. This is mechanically guaranteed; it cannot release a partial brick.

```
Approximate lobe dimensions: 12mm height, 15mm width
One full rotation = 200 steps (1.8 degrees per step)
Release portion = ~50 steps (90 degrees)
At target speed: 30ms per release
```

The cam disk geometry does not require a reference CAD model. A single-lobe disk is trivial to draw from these dimensions: a circle with one raised protrusion. The lobe pushes the bottom brick onto the belt as it passes; the rest of the disk holds the queue. Iterate the lobe profile with real bricks if the first print does not push cleanly.

The stepper holds its position electrically between releases using reduced current. This means the queue cannot backdrive the escapement when the motor is stationary.

## Taper channel

```
Entrance width: 30mm
Exit width:     17.3mm
Taper length:   60mm
Position:       between chute exit and sensing zone
```

Forces bricks square before sensors and plows. A severely yawed brick jams here, not at the plow. Better failure mode.

## Sensing zone

```
Total zone length:     ~30mm
IR break-beam spacing: 19mm (along travel axis)
Color sensor standoff: 5-10mm above belt surface
```

The color sensor shroud is part of the channel roof geometry. Print it as one piece with the channel. Ambient light is the primary failure mode for the color sensor. The shroud must block all light except the sensor's integrated LED.

### Shroud design requirements

The shroud is not just a box around the sensor. It needs to:

1. Seal against the belt surface (or come very close) on the leading and trailing edges so ambient light cannot enter from the direction of travel. A gap of even 2-3mm at the belt surface at this standoff distance leaks enough light to shift color readings.
2. Allow bricks to enter and exit freely without snagging. The opening should be wider than the brick but use a labyrinth or overlapping lip geometry to block direct light paths rather than a simple open slot.
3. Have a flat ceiling at a consistent standoff distance (5-10mm). Variation in standoff distance across the brick changes the integrated light level and potentially the ratio. Consistent geometry matters.
4. Be CAD'd with the color sensor dimensions in mind. Mount the sensor board flush against the shroud ceiling, not floating inside it.

This piece needs to be iterated. Print it early, test it with a handheld flashlight pointed at the belt from various angles while the sensor is logging readings. If the ratio shifts when ambient light changes, the shroud is not tight enough.

### Brick orientation enforcement at chute entrance

The chute width (17.3mm) already prevents 2x3 bricks from entering rotated 90 degrees. 23.7mm does not fit in 17.3mm. A 2x2 is square so rotation does not matter for it. Studs up vs studs down does not affect size or color measurements.

The remaining risk is a brick loaded completely on its side (11.4mm height presenting instead of 15.8mm length). Make the chute entrance opening narrow enough in the vertical dimension to prevent this. If a brick cannot enter on its side at the top of the chute, this failure mode is eliminated before it can cause a jam or misclassification.

## Plow diverters

Three plows, Class 3 lever geometry, solenoid actuated, spring return.

Lever geometry:
```
Pivot to solenoid attachment: 8mm (L1)
Pivot to plow tip:            24mm (L2)
Mechanical advantage ratio:   3.0
Solenoid stroke:              10mm
Tip travel:                   30mm (adequate to clear 15.8mm brick width on 25mm belt)
```

Tip travel needed calculation:
```
(belt width / 2) + (brick width / 2) + buffer = 12.5 + 7.9 + 4 = 24.4mm -> use 30mm
```

The solenoid connects to the lever arm via a pinned connection (M2 pin through a drilled hole or small clevis). Not a hook or slot. A pinned connection eliminates rattle, wear, and angular play at speed.

A slot in the channel wall accommodates the arc the solenoid rod travels. At these angles the arc is nearly linear. This is not a fabrication problem.

Chamfer the leading face of each plow arm at 35 degrees. Slightly yawed bricks get nudged square rather than jamming against the face.

### Plow spacing along belt

```
Footprint per plow along belt: ~32mm (lever length + 8mm safety gap)
3 plows total:                 ~96mm
```

### Spring selection

The force margin is large (~94x what the spring requires to return the arm). Use the heaviest spring from the assortment that the solenoid can still reliably overcome. Heavier spring means faster retraction, which is the optimization target. Test with the actual solenoid at operating voltage before committing to a spring weight.

## Bin design

Four bins, one per brick type. Bricks arrive at 200mm/s and will bounce out of empty bins without some form of energy absorption.

Options: foam sheet glued to bin floor, felt lining, or a short downward ramp printed into the bin geometry that redirects kinetic energy downward rather than back toward the belt. The ramp approach is preferable because it is zero-cost (print it into the bin), durable, and does not require gluing or sourcing foam in the right thickness.

## Fastener conventions

- Structural frame joints: M3
- Lever arm pin connections: M2
- Sensor and electronics mounts: M3 preferred
- Pre-order both M2 and M3 hardware kits

## Print tolerances

FDM variance is roughly 0.2mm to 0.5mm. All critical dimensions need explicit tolerance notes in CAD:

- Belt channel width (25mm): ±0.3mm acceptable
- Chute inner width (17.3mm): ±0.2mm, tighter is better, too tight and bricks drag
- Pulley center distance (180mm): tensioner slot provides 5mm of adjustment
- Plow lever arm lengths: ±0.5mm acceptable, ratio is what matters not absolute

Test fit all printed channel sections with real bricks before assembling the full frame.

## Assembly order

1. Chute transition piece validated in isolation
2. Full frame with all channel sections
3. Dry assembly: no wiring, walk a brick through by hand from chute to belt end
4. Fix all print fit issues before wiring anything
5. Wiring and electronics integration
6. First powered test
