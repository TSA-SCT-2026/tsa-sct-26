# NEMA17 Stepper Motor

Active status: active conveyor actuator.

## Traceability

| Field | Value |
|-------|-------|
| Purchased name | Usongshine Stepping Motor 17HS4401 |
| Model | 17HS4401 |
| BOM trace | AliExpress Mar 25, order ID `8210696713995240` |
| Received status | Yes |
| States-build use | Conveyor drive motor |
| Trust level | Manufacturer drawing transcription plus received-part observation |
| Next action | Use a 5mm bore mating part unless real measurement proves otherwise |

## Key Specs

| Feature | Value |
|---------|-------|
| Step angle | 1.8 degrees, listed tolerance 0.09 degrees |
| Rated voltage | 3.6V DC |
| Rated current | 1.5A per phase |
| Holding torque | At least 420mN m |
| Weight | 255g |

## Shaft Note

A 5mm ID bearing fit the received shaft very snugly. Treat this as a 5mm D-shaft for CAD and mating-part selection.

The 4.5mm drawing value appears to be the flat-related dimension, not the full shaft diameter.

## CAD model

STEP file: `stepper_17HS4401S.STEP` (this directory)
Use this file as the referenced conveyor motor geometry in Fusion 360.
The old trough-based assembly can be used as fallback prior art, but the active states plan first tries a proven downloaded conveyor assembly.

## Drawing Dimensions

| Feature | Value |
|---------|-------|
| Shaft flat-related dimension | 4.5mm, tolerance 0.1mm |
| Shaft length from D-flat start to tip | 18mm, tolerance 0.1mm |
| Shaft stickout from raised center boss | 21.5mm, tolerance 0.7mm |
| Shaft stickout from motor face edge | 23.5mm, tolerance 0.5mm |
| Body length | 42.3mm |
| Body width | 42.3mm |
| Body thickness | 38mm |
| Connector segment length | 16.2mm |
