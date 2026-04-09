# idler_roller.scad Part Card

## Role

- Free spinning roller on the non driven side of the belt path.
- Supports belt tension and tracking.

## Mates With

- MR85ZZ bearings
- M5 axle bolt or shaft
- Belt strip

## Required Hardware

- Two MR85ZZ bearings
- One M5 axle with matching washers and locknut

## Core Geometry

- 25mm outer diameter
- 25mm crowned contact face
- 2mm flanges on both sides
- Dual bearing pockets

## Locked Truth Dependency

- Belt strip and channel geometry constraints needed for stable long-side-across brick transport in the timing stage.

## Provisional Estimates Used

- Crown sagitta: 0.50mm
- Bearing pocket diameter: 8.15mm
- Bearing pocket depth: 2.60mm
- Axle clearance diameter

## Basic Install Steps

1. Press first bearing into one pocket until seated.
2. Press second bearing into opposite pocket.
3. Mount idler on M5 axle and tighten hardware.
4. Spin by hand and confirm smooth rotation.

## Pass or Fail Checks

Pass:
- Bearings seat square and remain retained
- Idler spins smoothly with low friction
- Belt tracks centered during run

Fail:
- Bearing pocket cracks or deforms during install
- Idler binds on axle
- Belt walks to one side under steady speed

## Common Mistakes

- Pressing bearings without checking pocket fit coupon first
- Misaligned axle causing side load on bearings
- Using crown value without confirming belt tracking in test runs

## Double Check When Hardware Arrives

- Verify bearing pocket insertion force with real MR85 parts.
- Verify axial retention and smooth spin on real M5 axle.
- Verify belt tracking and crown performance under run load.
