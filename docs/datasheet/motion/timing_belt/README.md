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

The motor mount bracket slots (12mm long, 3.5mm wide) give 8.5mm of travel, covering both belt options from a single bracket position.

## Installation notes

- Loop the belt around both pulleys before mounting them on the shafts.
- Install the belt in the conveyor last. Routing into the frame after the shaft is installed is difficult.
- Belt tension should be firm but not banjo-tight. The neoprene conveyor belt requires low slip force; over-tensioning the timing belt adds unnecessary bearing load.
- If the belt skips teeth under load, increase tension incrementally. If it skips immediately at any tension, recheck pulley alignment.
