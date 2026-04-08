# Bearing Block

## Purpose

Supports the driven 5mm shaft at two points so the shaft load does not reach the motor bearings. Two blocks are needed per shaft.

## Geometry

| Feature | Value |
|---------|-------|
| Block size | 14mm (X, shaft axis) x 16mm (Y, lateral) x 22mm (Z, height) |
| Shaft bore | 5.2mm diameter, through full X |
| Bearing pocket | 8.15mm diameter, 2.6mm deep from +X face |
| Pocket chamfer | 0.35mm lead-in |
| M3 mounting holes | 2x through Y, at Z = +/-7mm from shaft center |
| M3 nut trap | 2x hex trap at -Y face, M3 nut (5.5mm AF x 2.6mm deep) |

## Bearing used

MR85ZZ: 5mm ID, 8mm OD, 2.5mm wide.

Two blocks per shaft. Order does not matter as long as the bearing pocket faces outward on both ends.

## Shaft layout with two blocks

Position blocks symmetrically around the drive roller. Example layout for 25mm wide channel with 3mm walls:

| Component | X position (from roller center) |
|-----------|--------------------------------|
| Drive roller center | 0 |
| Drive roller edge (flange) | +/- 14.5mm |
| Bearing block center | +/- 21.5mm (adjust to clear roller) |
| Bearing block outer face | +/- 28.5mm |
| Driven pulley (purchased 20T) | beyond outer bearing block on one side |

Adjust spacing based on actual frame wall positions. The shaft (200mm) has ample length.

## Mounting to frame

Mounting holes run through the Y faces (lateral). The block bolts to the frame side wall with 2x M3 screws from outside.

M3 nut traps are at the -Y face (inner face, toward the channel). Insert M3 hex nuts before assembly since the inner face may not be accessible after the block is mounted.

## Print orientation

Best circularity for the bearing pocket: print with shaft axis (X) vertical. The pocket then faces upward and prints without support.

If printing flat (X horizontal), verify bearing press-fit using the bearing coupon before printing full blocks.

## Bearing press fit

The pocket is 8.15mm diameter. For MR85ZZ (8mm OD), this gives 0.15mm clearance. Light press fit is expected. If the pocket is slightly oversized from your printer, a small drop of retaining compound is acceptable as a backup.
