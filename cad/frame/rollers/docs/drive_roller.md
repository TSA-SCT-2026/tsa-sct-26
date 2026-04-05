# drive_roller.scad Part Card

## Role

- Smooth roller that drives the neoprene conveyor belt.
- Mounts on the supported conveyor shaft and transmits shaft torque to belt motion.

## Mates With

- Supported conveyor shaft
- Belt strip in the 20mm channel
- Idler roller on opposite end of belt path

## Required Hardware

- One M3 socket head screw for the clamp
- One M3 hex nut for the clamp nut trap
- Supported shaft and frame bearing hardware from the conveyor assembly

## Core Geometry

- 25mm outer diameter roller body
- 20mm contact face width
- 2mm flanges on both sides
- Split clamp shaft-fit hub

## Locked Truth Dependency

- Belt strip target and channel constraints that support widthwise brick transport.

## Provisional Estimates Used

- Shaft clamp bore round: 5.20mm
- Shaft clamp bore to flat: 4.50mm
- Clamp screw axis offset: 4.8mm from shaft center
- Flange OD and split clamp envelope
- Screw head pocket and hex nut trap size for clamp hardware

## Clamp Layout

The split clamp is on one side of the hub, not through the shaft center.

- The shaft-fit bore runs through the middle of the roller.
- The split slot opens from the bore to the outer hub wall.
- The M3 clamp screw crosses the hub beside the bore so it can squeeze the split closed.
- One side has a recessed screw head pocket.
- The other side has a recessed hex nut trap.

## Basic Install Steps

1. Slide roller onto the supported shaft by hand.
2. Align roller centerline to conveyor channel centerline.
3. Press an M3 hex nut into the nut trap.
4. Insert the M3 screw from the opposite recessed head side.
5. Tighten in small steps until the roller grips the shaft without cracking.
6. Verify no slip under light belt load.

## Pass or Fail Checks

Pass:
- Roller sits square on shaft
- No visible wobble at operating speed
- No shaft slip during repeated start and stop cycles

Fail:
- Cracking at split clamp slot
- Roller rubs frame or channel walls
- Bore is loose enough to twist on shaft
- Clamp screw path intersects the shaft bore

## Common Mistakes

- Over tightening clamp screw before confirming runout
- Skipping coupon based shaft-fit selection
- Assuming friction fit is enough without clamp hardware
- Treating the clamp screw like a set screw and aiming it at the shaft center

## Double Check When Hardware Arrives

- Verify supported shaft fit on the real conveyor shaft.
- Verify your real M3 screw head and hex nut match the modeled pockets.
- Verify roller runs true with no visible wobble at operating speed.
