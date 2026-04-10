# GT2 Closed-Loop Timing Belt

## Ordered parts (pending delivery)

From BOM (Amazon order pending):
- Primary candidate: `uxcell 208-2GT-6` (104T, 208mm, 6mm wide, 2mm pitch)
- Secondary candidate: 100T (200mm) closed loop belt (if ordered separately)

## Reference files in this directory

| File | Source | Status | Notes |
|------|--------|--------|-------|
| `gt2 belt C200.STEP` | Community model, C200 = 200mm = 100T | Usable for layout | See caveats |
| `gt2 belt C200.SLDPRT` | Source CAD file | Ignore | SolidWorks source |
| `gt2 belt C200.PNG` | Preview image | Reference | |
| `gt2-belt-w6_c200-1.snapshot.3.zip` | Original archive | Ignore | |

## STEP file notes

The STEP model represents a 200mm (100T) closed-loop GT2 belt, 6mm wide.

Use it for:
- Verifying clearance around the belt path in Fusion
- Confirming belt does not interfere with frame walls, motor body, or adjacent parts

Do not use it for:
- Tooth profile accuracy (the model may be simplified)
- Exact belt thickness (belt thickness varies slightly by brand; treat as ~1.5mm)

For the 208mm belt (ordered part), the belt path in Fusion will have a slightly longer straight section. Use the 200mm model for initial layout and adjust the center distance annotation for 208mm.

## Belt sizing reference

| Belt | Tooth count | Circumference | Recommended center distance (20T/20T) |
|------|-------------|---------------|--------------------------------------|
| 200mm | 100T | 200mm | 80mm |
| 208mm | 104T | 208mm | 84mm |

The integrated trough motor slots give 8mm of travel in the belt direction, covering the 80mm nominal center distance and nearby belt-fit adjustments from one printed body.

## Installation notes

- Install both pulleys first, then route the timing belt through the trough bridge window.
- Snug pulley set screws before belt installation so axial position can still be corrected during dry alignment.
- Belt tension should be firm but not banjo-tight. The neoprene conveyor belt requires low slip force; over-tensioning the timing belt adds unnecessary bearing load.
- If the belt skips teeth under load, increase tension incrementally. If it skips immediately at any tension, recheck pulley alignment.
