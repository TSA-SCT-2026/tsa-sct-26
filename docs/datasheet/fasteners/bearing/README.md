# MR85ZZ Ball Bearing

## Ordered part

MR85ZZ Ball Bearing, 10pcs (AliExpress Mar 25, received).

Listed in BOM as: `5x8x2.5mm MR85RS 10pcs blue seals`.

## Dimensions (verified from standard spec)

| Feature | Value |
|---------|-------|
| Inner diameter (ID) | 5mm |
| Outer diameter (OD) | 8mm |
| Width | 2.5mm |
| Type | Deep groove ball bearing, double-sealed |
| Material | Chrome steel |

## Reference files in this directory

| File | Source | Status | Notes |
|------|--------|--------|-------|
| `NSK_MR85ZZ.stp` | NSK manufacturer model | Usable for layout | See caveats |
| `PARTserver22026040816232479510624d0b6156e.zip` | Original download | Ignore | Archive only |
| `readme-and-terms-of-use-3d-cad-models.txt` | Download terms | Reference only | |

## STEP file caveats

The NSK STEP model is from the NSK bearing manufacturer. It matches standard MR85ZZ geometry precisely. The OD (8mm), ID (5mm), and width (2.5mm) are standard and do not vary between reputable bearing brands.

Use it for:
- Confirming bearing pocket geometry in Fusion
- Verifying clearance if the chosen conveyor uses MR85ZZ bearing pockets

## CAD Pocket Values

The archived roller subsystem used these printed pocket targets from `rollers_params.scad`:

| Feature | Value |
|---------|-------|
| Pocket diameter | 8.15mm (0.15mm press fit clearance) |
| Pocket depth | 2.6mm (0.1mm deeper than bearing width) |
| Lead chamfer | 0.35mm |

These values come from the bearing fit coupon. Verify with your printer before printing final parts.

## Usage In This Project

| Location | Quantity | Notes |
|----------|----------|-------|
| States conveyor | TBD | Use only if the selected conveyor path needs MR85ZZ bearings |

The 608ZZ bearings in the BOM are also candidate conveyor inventory only if the selected conveyor uses them.
