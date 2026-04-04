# drive_pulley.scad Part Card

## Role

- Powered roller that drives the conveyor belt strip.
- Mounts on the motor D shaft and transmits motor torque to belt motion.

## Mates With

- Motor D shaft from JGB37-520 conveyor motor
- Belt strip in the 20mm channel
- Idler roller on opposite end of belt path

## Required Hardware

- One M3 clamp screw
- One M3 nut or one M3 heat set insert for clamp pocket
- Motor shaft and mounting hardware from conveyor motor assembly

## Core Geometry

- 25mm outer diameter roller body
- 20mm contact face width
- 2mm flanges on both sides
- Split clamp D bore hub

## Locked Truth Dependency

- Belt strip target and channel constraints that support widthwise brick transport.

## Provisional Estimates Used

- D bore round: 6.20mm
- D bore to flat: 5.70mm
- Flange OD and split clamp envelope
- Insert pocket size for clamp hardware

## Basic Install Steps

1. Slide pulley onto motor D shaft by hand.
2. Align pulley centerline to conveyor channel centerline.
3. Install clamp screw and nut or insert hardware.
4. Tighten clamp in small steps while rotating pulley by hand.
5. Verify no slip under light belt load.

## Pass or Fail Checks

Pass:
- Pulley sits square on shaft
- No visible wobble at operating speed
- No shaft slip during repeated start and stop cycles

Fail:
- Cracking at split clamp slot
- Pulley rubs frame or channel walls
- Bore is loose enough to twist on shaft

## Common Mistakes

- Over tightening clamp screw before confirming runout
- Skipping coupon based D bore selection
- Assuming friction fit is enough without clamp hardware

## Double Check When Hardware Arrives

- Verify D shaft fit on real motor shaft.
- Verify clamp screw and insert hardware match modeled geometry.
- Verify pulley runs true with no visible wobble at operating speed.
