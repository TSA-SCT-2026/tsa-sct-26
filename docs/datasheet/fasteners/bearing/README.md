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

## Reference Files

| Path | Source | Status | Notes |
|------|--------|--------|-------|
| `cad/nsk_mr85zz.stp` | NSK manufacturer model | Usable for layout | See caveats |
| `archive/cad_model_terms.txt` | Download terms | Reference only | |

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
| States conveyor | Active current path | Use with the 5mm shaft tutorial-style conveyor unless hardware changes |

The 608ZZ bearings in the BOM are spare conveyor inventory only if the user changes to a larger shaft and bearing path.
