# GT2 Closed-Loop Timing Belt

## Status

Active only if the selected states conveyor uses a GT2 timing stage.

User reports a recent GT2 belt and pulley order, but this markdown should not mark those parts ordered until matching rows exist in `docs/project/BOM.xlsx`.

Reference candidates:
- 104T, 208mm, 6mm wide, 2mm pitch
- 100T, 200mm, 6mm wide, 2mm pitch

## Reference Files

| Path | Source | Status | Notes |
|------|--------|--------|-------|
| `cad/gt2_belt_c200.step` | Community model, C200 = 200mm = 100T | Usable for layout | See caveats |
| `cad/gt2_belt_c200_source.sldprt` | Source CAD file | Reference | SolidWorks source |
| `images/gt2_belt_c200.png` | Preview image | Reference | |

## STEP file notes

The STEP model represents a 200mm (100T) closed-loop GT2 belt, 6mm wide.

Use it for:
- Verifying clearance around the belt path in Fusion
- Confirming belt does not interfere with frame walls, motor body, or adjacent parts

Do not use it for:
- Tooth profile accuracy (the model may be simplified)
- Exact belt thickness (belt thickness varies slightly by brand; treat as ~1.5mm)

For a 208mm belt, the belt path in Fusion will have a slightly longer straight section. Use the 200mm model for initial layout and adjust the center distance annotation for 208mm.

## Belt sizing reference

| Belt | Tooth count | Circumference | Recommended center distance (20T/20T) |
|------|-------------|---------------|--------------------------------------|
| 200mm | 100T | 200mm | 80mm |
| 208mm | 104T | 208mm | 84mm |

The previous integrated trough used 8mm of travel in the belt direction. Treat that as fallback reference, not active states conveyor truth.

## Installation notes

- Install both pulleys first, then route the timing belt through the chosen conveyor belt path.
- Snug pulley set screws before belt installation so axial position can still be corrected during dry alignment.
- Belt tension should be firm but not banjo-tight. The neoprene conveyor belt requires low slip force; over-tensioning the timing belt adds unnecessary bearing load.
- If the belt skips teeth under load, increase tension incrementally. If it skips immediately at any tension, recheck pulley alignment.
