# Datasheet Library

Store part datasheets and official spec sheets here.

Goals:
- Keep critical hardware specs local and easy to find.
- Separate confirmed vendor specs from notes and assumptions.
- Support fast verification against `docs/project/BOM.md`.

## Folder map

- `motion/`: conveyor motor, heavy servo, fan, timing belts, pulleys, bearings
- `sensing/`: size-sensor candidates, color sensors, switches
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
| MG995/MG996-class heavy servo | Yes | Active selector actuator. Verify exact horn, body, voltage, and CAD import path. |
| NEMA11 stepper | Yes | Archived inventory for the previous design, not active states hardware. |
| `MR85ZZ` bearings | Yes | Candidate conveyor hardware only if the chosen conveyor uses them. |
| `608ZZ` bearings | Yes | Candidate conveyor hardware only if the chosen conveyor uses them. |
| `40mm` fan | Yes | Required if model label is present. |
| `GT2` belts | Pending spreadsheet update | Required only if the selected conveyor uses the GT2 timing path. |
| `GT2` pulleys | Pending spreadsheet update | Required only if the selected conveyor uses the GT2 timing path. |
| Break-beam sensor | Yes | Active size-sensor candidate while the size decision is open. |
| Distance sensor | Unconfirmed | Candidate only while the size decision is open. Do not mark ordered without spreadsheet trace. |
| TCS3200/GY-31 color sensor | Yes | Active color sensor. Calibrate only with the shroud installed. |
| Waveshare `2inch Mini LCD Screen` | No | Required after board or label match. |
| ISDT `608AC` charger | No | Required. Low CAD criticality but keep reference. |
| Zeee `3S` LiPo pack | No | Required with caution. Accept dimensions only after label and case match. |
