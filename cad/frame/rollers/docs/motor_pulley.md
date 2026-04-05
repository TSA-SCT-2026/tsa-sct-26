# motor_pulley.scad Part Card

## Role

- Timing pulley that drives the off-axis conveyor stage.
- Mounts on the NEMA17 motor shaft and transmits motion to the toothed belt.

## Mates With

- NEMA17 motor shaft
- Toothed timing belt
- Timing belt driven pulley on the supported conveyor shaft

## Required Hardware

- One M3 socket head screw for the clamp
- One M3 hex nut for the clamp nut trap
- Motor shaft and mounting hardware from the conveyor motor assembly

## Core Geometry

- Timing pulley envelope sized from the provisional tooth count
- Split clamp shaft-fit hub
- Hub and clamp hardware that keep the pulley centered on the shaft
- The model is a timing envelope and fit aid, not a final tooth profile

## Locked Truth Dependency

- Belt ratio, center distance, and motor clearance constraints that support the conveyor stage.

## Provisional Estimates Used

- Motor pulley tooth count and pitch diameter
- Shaft clamp bore round and to-flat dimensions
- Clamp screw axis offset and pocket sizes
- Hub OD and belt width envelope

## Basic Install Steps

1. Slide pulley onto the motor shaft by hand.
2. Align pulley centerline to the timing belt plane.
3. Press an M3 hex nut into the nut trap.
4. Insert the M3 screw from the opposite recessed head side.
5. Tighten in small steps until the pulley grips the shaft without cracking.
6. Verify no slip under light belt load.

## Pass or Fail Checks

Pass:
- Pulley sits square on shaft
- No visible wobble at operating speed
- No shaft slip during repeated start and stop cycles

Fail:
- Cracking at split clamp slot
- Pulley rubs frame or motor mount
- Bore is loose enough to twist on shaft
- Clamp screw path intersects the shaft bore

## Double Check When Hardware Arrives

- Verify NEMA17 shaft fit on the real motor shaft.
- Verify your real M3 screw head and hex nut match the modeled pockets.
- Verify pulley runs true with no visible wobble at operating speed.
