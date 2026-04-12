# Component Reference Library

Store part datasheets, catalog notes, CAD references, and verification notes here.

Goals:
- Keep critical hardware specs local and easy to find.
- Separate confirmed vendor specs, received-part observations, and CAD convenience models.
- Make the next build, wiring, or CAD action obvious.
- Support fast verification against `docs/project/BOM.md` and `docs/project/BOM.xlsx`.

## Folder map

- `motion/`: conveyor motor, heavy servo, fan, timing belts, pulleys, bearings
- `sensing/`: size-sensor candidates, color sensors, switches
- `power/`: batteries, chargers, buck converters, fuse holders
- `electrical/`: semiconductors and passives (drivers, MOSFETs, diodes, capacitors, resistors)
- `materials/`: tapes, rubber strips, structural material sheets
- `modules/`: display modules, ESP32 boards, expansion boards
- `fasteners/`: screws, inserts, shafts, collars, mechanical hardware kits
- `reference/`: manufacturer selection guides and generic application notes

## Folder Rules

For each real component folder, prefer this shape when cleanup time allows:

| Path | Purpose |
|------|---------|
| `README.md` | The actionable component summary |
| `datasheets/` | PDFs or official spec sheets |
| `cad/` | STEP, STL, SLDPRT, IPT, F3D, or IGS files |
| `images/` | Reference screenshots or vendor images |
| `archive/` | Original zip downloads, snapshots, and untouched vendor bundles |

Do not move CAD that may already be referenced by Fusion unless the user asks for a file reorganization pass.

## Component README Standard

Each component README should answer these questions near the top:

- Active status: active, candidate, archived inventory, or reference only
- BOM trace: spreadsheet order ID, tab, or `not present in docs/project/BOM.xlsx`
- Received status
- States-build use
- Critical CAD dimensions
- Wiring or power notes if relevant
- Trust level: official spec, received-part observation, or community CAD convenience model
- Next action

Use tables for pinouts and dimensions. Keep raw copied listing text below the actionable summary only if it still helps.

## Naming rules

- Use this filename pattern for newly added cleaned references: `partnumber_or_shortname__vendor__rev_or_date.ext`
- Keep names ASCII and lowercase where practical.
- Prefer PDF for official references. If source is a web page, save a PDF export and include source URL in the file metadata or sibling note.

## Scope rules

- Keep only durable hardware references here.
- Do not store run logs here. Use `docs/runs/`.
- Do not store meeting notes here. Use `docs/project/MEETINGS.md`.

## States Build Dashboard

Use this as a fast action board. Purchase truth still lives in `docs/project/BOM.xlsx`.

| Part | Build role | BOM or inventory status | Reference status | Next action |
|------|------------|-------------------------|------------------|-------------|
| NEMA17 Stepper Motor `17HS4401S` | Active conveyor actuator | BOM-backed, received | CAD and dimensions present | Use 5mm shaft observation for mating parts, not the 4.5mm flat dimension |
| MG995/MG996-class heavy servo | Active servo rotary chute selector | User-confirmed on hand | SolidWorks assembly with horns plus separate STEP path | Keep both CAD sources. Use STEP for Fusion layout and SolidWorks files for horn and assembly context |
| TCS3200/GY-31 color sensor | Active color sensor | BOM-backed, received | Datasheets and CAD present | Calibrate only with the final shroud and belt geometry installed |
| Break-beam sensor | Active size sensor | BOM-backed, received | CAD reference present | Two-pair timing layout in the sensing shroud |
| Distance sensor, `VL53L0X` family | Size-sensor candidate | Not present in `docs/project/BOM.xlsx` | CAD reference only | Do not mark ordered. Define conveyor bracket and calibration plan before selection |
| `GT2` belts | Conveyor timing candidate | Pending spreadsheet update | CAD reference present | Use only if the selected conveyor uses the GT2 timing path |
| `GT2` pulleys | Conveyor timing candidate | Pending spreadsheet update | CAD reference present | Verify received hub width, flange OD, and set screw access before freezing CAD |
| `MR85ZZ` bearings | Conveyor candidate | BOM-backed, received | CAD reference present | Use only if the selected conveyor path needs them |
| `608ZZ` bearings | Conveyor candidate | BOM-backed, received | Datasheet folder not yet built | Use only if the selected conveyor path needs them |
| `40mm` fan | Optional electronics cooling | BOM-backed, received | CAD reference present | Use if driver or electronics temperature requires it |
| Waveshare `2inch Mini LCD Screen` | Operator display candidate | BOM-backed, Amazon Mar 25 | Datasheet folder not yet built | Add a module reference before final wiring or display mount CAD |
| ISDT `608AC` charger | Battery support equipment | BOM-backed, Amazon Mar 25 | Datasheet folder not yet built | Low CAD priority. Keep safety and operating reference when available |
| Zeee `3S` LiPo pack | Main power source | BOM-backed, Amazon Mar 25 | Datasheet folder not yet built | Verify exact pack dimensions and connector before final battery mounting |
| NEMA11 stepper | Archived inventory | BOM-backed, received | Not active | Do not use for states selector work unless architecture is explicitly reopened |
