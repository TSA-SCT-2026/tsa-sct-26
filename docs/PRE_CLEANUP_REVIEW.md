# Pre-cleanup Deletion Review

Review date: 2026-08-25

Comparison: final pre-cleanup development state `b65f4dc` against cleaned
baseline `8032ac3`. The live development checkout was inspected read-only. The
complete-history bundle and simulator archive were verified as available but
were not extracted because the checkout and Git history were sufficient.

Decision meanings:

- **RESTORE** - retain the authentic artifact in the publication tree.
- **EXTRACT** - carry forward only the useful facts or a compact curated subset.
- **KEEP DELETED** - obsolete, generated, duplicate, or low-signal material.
- **MANUAL REVIEW** - potentially useful, but identity, privacy, authorship, or
  final-hardware fit needs confirmation.

## Pre-cleanup artifacts reconsidered

| Old path | Removed by cleanup? | Decision | Why |
| --- | ---: | --- | --- |
| `docs/project/BOM.md` | Yes | EXTRACT | Its active-system summary and three explicitly user-supplied additions are now provenance fields in `hardware/BOM.csv`; restoring a second BOM would create two publication truths. |
| `docs/project/BOM.xlsx` | Yes | EXTRACT | All five sheets were inspected. Exact purchased models, pack quantities, and specifications were merged into the single public BOM; order IDs, sellers, abandoned parts, and the purchase ledger remain out of the public tree. |
| `docs/datasheet/README.md` | Yes | EXTRACT | Replaced with a concise index that explains the design constraint supported by every retained reference. |
| `docs/datasheet/electrical/stepper_driver/images/wiring.png` | Yes | RESTORE | Compact original pin and microstep map for the TMC2209 carrier used by the final conveyor. |
| `docs/datasheet/electrical/stepper_driver/images/board.png` | Yes | KEEP DELETED | Generic product photo; it adds no constraint beyond the retained pin map. |
| `docs/datasheet/electrical/stepper_driver/images/steps.png` | Yes | KEEP DELETED | Low-resolution generic adjustment instructions; final current setup still requires machine-specific validation. |
| `docs/datasheet/fasteners/bearing/cad/nsk_mr85zz.stp` | Yes | RESTORE | Small exact bearing envelope for shaft-support and pocket-fit checks. |
| `docs/datasheet/motion/nema17/cad/stepper_17hs4401s.step` | Yes | RESTORE | Supplier reference envelope for the exact NEMA17 purchase record and motor-mount clearance. |
| `docs/datasheet/motion/nema17/images/schematic.png` | Yes | RESTORE | Contains the mounting, shaft, connector, and winding geometry that informed fit and wiring. |
| `docs/datasheet/motion/nema17/images/specs.png` | Yes | RESTORE | Preserves current, torque, step-angle, speed, and mass constraints for the purchased 17HS4401. |
| `docs/datasheet/motion/nema17/images/manufacturer_drawing.png` | Yes | KEEP DELETED | Render-only product view; schematic/spec pages carry the useful engineering information. |
| `docs/datasheet/motion/nema17/images/motor.png` | Yes | KEEP DELETED | Generic product photo with no additional fit or electrical constraint. |
| `docs/datasheet/motion/heavy_servo/cad/solidworks/*` | Yes | MANUAL REVIEW | Potentially useful source geometry, but the installed servo is only recorded as MG995/MG996-class; publishing an exact model would overstate certainty and add about 3 MB. |
| `docs/datasheet/motion/heavy_servo/cad/step/*` | Yes | MANUAL REVIEW | Same model-identity issue; restore only after the installed servo and redistribution rights are confirmed. |
| `docs/datasheet/motion/heavy_servo/images/*` | Yes | KEEP DELETED | Generic vendor renders and source screenshots do not prove the installed component or project work. |
| `docs/datasheet/motion/timing_belt/cad/*` | Yes | KEEP DELETED | Large generic vendor geometry; the retained system-level conveyor STEP is stronger evidence. |
| `docs/datasheet/motion/timing_belt/images/*` | Yes | KEEP DELETED | Generic reference screenshot with no additional final-system evidence. |
| `docs/datasheet/motion/timing_pulley/cad/*` | Yes | KEEP DELETED | Duplicate generic component geometry already represented in the assembled conveyor export. |
| `docs/datasheet/motion/timing_pulley/images/*` | Yes | KEEP DELETED | Generic screenshot; no project-specific decision or measurement is visible. |
| `docs/datasheet/reference/lego_2x3/lego2x3_red.stp` | Yes | RESTORE | Compact reference for the longest supported brick envelope and chute/guide clearance. |
| `docs/datasheet/reference/lego_2x3/lego2x3_red.txt` | Yes | RESTORE | Preserves the reference model's source/identity note alongside the STEP. |
| `docs/datasheet/sensing/break_beam/cad/ir_break_beam_5mm.step` | Yes | MANUAL REVIEW | The purchase workbook records 3 mm sensors, so this 5 mm model must not be presented as the installed part without a physical check. |
| `docs/datasheet/sensing/color_sensor/datasheets/gy31_color_module.pdf` | Yes | RESTORE | Original module manual provides pinout, LED control, voltage range, and the full PCB envelope. |
| `docs/datasheet/sensing/color_sensor/datasheets/tcs230.pdf` | Yes | RESTORE | Original chip-family datasheet explains filter selection, frequency scaling, pulse output, and electrical limits used by the firmware. |
| `docs/datasheet/sensing/color_sensor/images/module.png` | Yes | KEEP DELETED | Product photo is redundant with the retained module manual. |
| `docs/datasheet/sensing/color_sensor/cad/tcs3200_color_sensor_module.step` | Yes | KEEP DELETED | About 12 MB; module manual and authentic manual measurements retain the relevant envelope much more compactly. |
| `docs/datasheet/sensing/color_sensor/cad/tcs3200_color_sensor_module.sldprt` | Yes | KEEP DELETED | Duplicate multi-megabyte source geometry with no additional verified project-specific evidence. |
| `docs/ARCHITECTURE.md` | Yes | EXTRACT | Strong final-system behavior was already consolidated into root `ARCHITECTURE.md`; no unique claim was lost. |
| `docs/build/ASSEMBLY.md` | Yes | EXTRACT | Assembly sequence remains in root `BUILD.md`; final-architecture fit notes from the manual measurement document were added there. |
| `docs/build/CALIBRATION.md` | Yes | EXTRACT | Calibration workflow remains in root `BUILD.md` with the required operator-assisted wording. |
| `wiring/ELECTRICAL.md` | Yes | EXTRACT | Pin map and power topology remain in root `BUILD.md`; a wiring image still requires machine validation. |
| `docs/runs/README.md` | Yes | KEEP DELETED | It contained only placeholders and no authentic physical run, log, benchmark, or result. |
| `docs/README.md` | Yes | KEEP DELETED | Navigation placeholder made redundant by the root documentation map. |
| `cad/README.md` | Yes | KEEP DELETED | Placeholder-only CAD overview; root README identifies the retained system STEP and its provenance gate. |
| `cad/assemblies/README.md` | Yes | KEEP DELETED | Empty staging guidance with no CAD artifact or evidence. |
| `cad/assemblies/full_system/README.md` | Yes | KEEP DELETED | Placeholder for missing render/source files. |
| `cad/assemblies/subsystems/README.md` | Yes | KEEP DELETED | Placeholder list without subsystem artifacts. |
| `simulation/` | Yes | KEEP DELETED | Inspected as a tested control-flow model, not a physics model. Joe explicitly requested that no simulator code, screenshot, or link be published. |
| `firmware/smoke_tests_extra/servo_tuning_pio/main.cpp` | Yes | KEEP DELETED | One-line wrapper content is preserved at `firmware/tools/servo_tuning/main.cpp`; old staging path is unnecessary. |
| `firmware/tsa_sorter/tsa_sorter.ino` | Yes | KEEP DELETED | Two-line Arduino IDE marker; PlatformIO entrypoint is the maintained build path. |

## Additional authentic sources checked

| Source | Decision | Why |
| --- | --- | --- |
| `/Users/joeroche/Documents/sct_manual_measurements.docx` | EXTRACT | Final NEMA17, GY-31, break-beam, and TMC2209 fit measurements were added to `BUILD.md`. Obsolete NEMA11/solenoid notes remain unpublished. |
| `/Users/joeroche/Developer/SCT/CLAUDE.md` | KEEP DELETED | Useful historical planning context, but its TCS34725/plow/solenoid architecture predates and conflicts with the final TCS3200/servo-chute system. |
| `/Volumes/T7/Archive/Hardware/lego_sorter_system_diagram.svg` and `.pdf` | MANUAL REVIEW | Authentic March planning diagram, but it depicts the abandoned TCS34725/plow architecture and should not be used as the final-system diagram. |
| Local Photos libraries and loose media folders | MANUAL REVIEW | A bounded search confirmed date-window candidates, but Joe will choose the originals. The exact shot list is in `/Users/joeroche/Developer/.portfolio-todo-parts/tsa.md`. |

No benchmark, deployment record, physical run log, or verified accuracy artifact
was found among the files removed by the immediate cleanup diff. The clean tree
therefore still correctly avoids performance claims.
