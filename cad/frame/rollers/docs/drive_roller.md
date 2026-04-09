# drive_roller.scad Part Card

## Role

Smooth roller that drives the neoprene conveyor belt.
Mounts on the supported conveyor shaft and transmits shaft torque to belt motion.

## Mates With

- Supported conveyor shaft (5mm D-profile)
- Shaft collars on each side for axial retention
- Belt strip in the 25mm channel
- Idler roller on the opposite end of the belt path

## Required Hardware

- Two shaft collars (one each side of roller) for axial positioning
- Supported shaft and frame bearing hardware from the conveyor assembly

## Core Geometry

- 25mm outer diameter roller body
- 25mm contact face width
- 2mm flanges on both sides
- D-bore: 5.20mm round, 4.50mm to flat

## Retention Method

Torque is transmitted through the D-flat on the shaft. The printed bore grips the flat wall.
Axial position is locked by shaft collars seated against the outer flange faces.
No split clamp. No screw hardware in the roller body.

## Locked Truth Dependency

Belt strip target and channel constraints that support long-side-across brick transport.

## Provisional Estimates Used

- Shaft bore round: 5.20mm
- Shaft bore to flat: 4.50mm
- Flange OD

## Basic Install Steps

1. Slide roller onto the supported shaft by hand.
2. Align roller centerline to conveyor channel centerline.
3. Place one shaft collar against each outer flange face.
4. Tighten collars while checking the roller spins freely between them.
5. Verify no axial play and no lateral rub against channel walls.

## Pass or Fail Checks

Pass:
- Roller sits square on shaft
- No visible wobble at operating speed
- No axial shift under repeated belt load

Fail:
- Bore loose enough to rotate on shaft under belt friction
- Collar seated against belt surface instead of flange face
- Roller rubs frame or channel walls

## Common Mistakes

- Collar too far inboard, contacting the belt face instead of the flange
- Skipping coupon-based shaft-fit selection before printing full roller
- Bore too loose to grip the D-flat reliably

## Double Check When Hardware Arrives

- Verify shaft fit on the real conveyor shaft using the shaft-fit coupon.
- Verify collar fits and clears the flange OD without overlapping the belt face.
- Verify roller runs true with no visible wobble at operating speed.
