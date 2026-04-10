# GT2 20T 5mm Bore Timing Pulley

## Ordered part

WINSINN GT2 20 Teeth 5mm Bore for 6mm Belt (from BOM, Amazon order pending delivery).

## Reference files in this directory

| File | Source | Status | Notes |
|------|--------|--------|-------|
| `GT2_20T.STEP` | Downloaded from parts library | Usable for layout | See caveats below |
| `gt2_20t_5mm.sldprt` | Source CAD file | Ignore unless you have SolidWorks | Not needed for Fusion workflow |
| `gt2_20t_screenshot.png` | Reference image | Reference only | Original community model screenshot |
| `pulley-gt2-20-tooth-5-bore-gt2-1.snapshot.1.zip` | Original download archive | Ignore | Unzip only if you need original sources |

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

Both motor shaft and driven shaft use 20T GT2 purchased pulleys.

- Motor shaft: NEMA17 D-shaft, 5mm. Set screw bears against the D-flat.
- Driven shaft: 5mm round hardened shaft. Set screw bears on round shaft (acceptable for light torque).

Same STEP file can be used for both instances in Fusion since tooth count and bore are identical.

## Center distance for belt sizing

With two 20T pulleys and 2mm pitch GT2 belt:
- 100T belt (200mm): center distance = 80mm
- 104T belt (208mm): center distance = 84mm

The integrated trough motor slots provide 8mm of adjustment in the belt direction around the 80mm nominal center distance.

Before freezing pulley placement in CAD:
- check that the belt path clears the trough bridge window
- check that the drive-pulley set screw is reachable before belt installation
