# motor_pulley.scad Part Card

## Role

Layout reference and fit aid for the NEMA17 timing stage motor pulley.
The production path uses purchased 20T GT2 pulleys on both shafts.
This model has no tooth profile and does not replace the purchased part.

## Mates With

- NEMA17 motor shaft (5mm D-profile)
- Toothed timing belt
- Timing belt driven pulley on the supported conveyor shaft

## Required Hardware

None for the printed reference model.
The purchased 20T GT2 pulley uses its own set screw. Follow the pulley manufacturer's
torque guidance for that screw.

## Core Geometry

- Timing pulley envelope sized from the provisional tooth count and pitch
- Belt-width flanges on each side (1.5mm each)
- D-bore for shaft reference only: 5.20mm round, 4.50mm to flat

## When to Use the Printed Version

The printed model is for:
- Fusion 360 layout verification before hardware arrives
- Shaft-fit coupon reference (bore dimensions match the purchased pulley bore target)

Do not use the printed model as a functional timing pulley in the final build. The smooth
cylinder body will not mesh with belt teeth.

## Basic Install Steps (purchased pulley)

1. Slide the purchased 20T GT2 pulley onto the motor shaft.
2. Align pulley so the belt will run centered in the timing stage.
3. Set axial position so the pulley belt plane matches the driven pulley belt plane.
4. Tighten the set screw against the shaft flat per the pulley manufacturer's guidance.
5. Verify pulley spins true with no wobble.

## Pass or Fail Checks

Pass:
- Pulley sits square on shaft
- No visible wobble at operating speed
- Belt plane of motor pulley aligned with belt plane of driven pulley
- No shaft slip under expected belt load

Fail:
- Pulley rubs frame or motor mount
- Belt tracks off-center due to axial misalignment
- Shaft slip under motor torque

## Double Check When Hardware Arrives

- Verify the purchased pulley bore fits the real motor shaft.
- Verify pulley width and belt plane position before tightening the set screw.
- Confirm belt plane alignment between motor pulley and driven pulley before running.
