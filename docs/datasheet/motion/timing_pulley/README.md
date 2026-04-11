# GT2 20T 5mm Bore Timing Pulley

## Status

Active only if the selected states conveyor uses a GT2 timing stage.

User reports a recent pulley and GT2 belt order, but this markdown should not mark those parts ordered until matching rows exist in `docs/project/BOM.xlsx`.

Reference part: 20T GT2 pulley, 5mm bore, for 6mm belt.

## Reference Files

| Path | Source | Status | Notes |
|------|--------|--------|-------|
| `cad/gt2_20t_5mm.step` | Downloaded from parts library | Usable for layout | See caveats below |
| `cad/gt2_20t_5mm_source.sldprt` | Source CAD file | Reference | Not needed for Fusion workflow |
| `images/gt2_20t_screenshot.png` | Reference image | Reference only | Original community model screenshot |

## STEP file caveats

The STEP file is a community model for a GT2 20T 5mm bore pulley. It matches the general geometry but was not verified against the specific WINSINN variant.

Use it for:
- Layout and spacing in Fusion
- Belt path visualization
- Confirming axial alignment between motor and driven pulleys

Do not trust it for:
- Exact flange OD (varies by brand)
- Set screw position
- Hub width (may differ slightly from purchased part)

Verify exact hub width, flange OD, and belt section width against the received part before freezing pulley axial positions in CAD.

## Expected geometry (typical GT2 20T 5mm bore)

| Feature | Approximate value |
|---------|------------------|
| Belt pitch | 2mm GT2 |
| Tooth count | 20 |
| Pitch diameter | 12.73mm |
| Tooth OD | ~14.6mm (varies by tooth profile) |
| Belt width | 6mm |
| Total hub width | ~11mm (belt section + flanges) |
| Bore | 5mm |
| Set screw | M3, radial, typically 1 or 2 |
| Flange OD | ~15-16mm |

## Usage in this project

If the GT2 timing path is used, both motor shaft and driven shaft can use 20T GT2 pulleys.

- Motor shaft: NEMA17 D-shaft, 5mm. Set screw bears against the D-flat.
- Driven shaft: 5mm round hardened shaft. Set screw bears on round shaft (acceptable for light torque).

Same STEP file can be used for both instances in Fusion since tooth count and bore are identical.

## Center distance for belt sizing

With two 20T pulleys and 2mm pitch GT2 belt:
- 100T belt (200mm): center distance = 80mm
- 104T belt (208mm): center distance = 84mm

The previous integrated trough motor slots provided 8mm of adjustment in the belt direction around the 80mm nominal center distance. Treat that as fallback reference.

Before freezing pulley placement in CAD:
- check that the belt path clears the selected conveyor frame
- check that the drive-pulley set screw is reachable before belt installation
