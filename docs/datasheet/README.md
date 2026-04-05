# Datasheet Library

Store part datasheets and official spec sheets here.

Goals:
- Keep critical hardware specs local and easy to find.
- Separate confirmed vendor specs from notes and assumptions.
- Support fast verification against `docs/project/BOM.md`.

## Folder map

- `motion/`: stepper motors, timing belts, pulleys, bearings
- `sensing/`: break-beam sensors, color sensors, switches, Hall sensors
- `power/`: batteries, chargers, buck converters, fuse holders
- `electrical/`: semiconductors and passives (drivers, MOSFETs, diodes, capacitors, resistors)
- `materials/`: tapes, rubber strips, structural material sheets
- `modules/`: display modules, ESP32 boards, expansion boards
- `fasteners/`: screws, inserts, shafts, collars, mechanical hardware kits
- `reference/`: manufacturer selection guides and generic application notes

## Naming rules

- Use this filename pattern: `partnumber_or_shortname__vendor__rev_or_date.ext`
- Keep names ASCII and lowercase where possible.
- Prefer PDF. If source is a web page, save a PDF export and include source URL in the file metadata or sibling note.

## Scope rules

- Keep only durable hardware references here.
- Do not store run logs here. Use `docs/runs/`.
- Do not store meeting notes here. Use `docs/project/MEETINGS.md`.

## Datasheet Checklist

Only track parts here where datasheet or catalog can be treated as truth after a sanity check.

| Part | Have it now? | Datasheet status |
|------|--------------|------------------|
| NEMA17 Stepper Motor `17HS4401S` | Yes | Required. Sanity-check shaft type and usable stickout first. |
| NEMA11 stepper | Yes | Required only if exact maker or model is identifiable from label. |
| `MR115ZZ` bearings | Yes | Required. Standard bearing family after marking and fit sanity check. |
| `608ZZ` bearings | Yes | Required. Standard bearing family. |
| `40mm` fan | Yes | Required if model label is present. |
| Ordered `5mm` shaft stock | No | Required. Use industrial catalog as truth. |
| Ordered `5mm` shaft collars | No | Required. Use industrial catalog as truth. |
| Ordered `GT2` belts | No | Required. Use exact ordered spec or manufacturer catalog. |
| Ordered `GT2` pulleys | No | Required. Use exact ordered spec or manufacturer catalog. |
| Waveshare `2inch Mini LCD Screen` | No | Required after board or label match. |
| ISDT `608AC` charger | No | Required. Low CAD criticality but keep reference. |
| Zeee `3S` LiPo pack | No | Required with caution. Accept dimensions only after label and case match. |
