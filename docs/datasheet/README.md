# Retained Component References

This is a selective extract from the authentic pre-cleanup component library.
It is not a general vendor-download mirror. Each retained file supports a
specific final-system constraint.

| Reference | Constraint it informed |
| --- | --- |
| [`electrical/stepper_driver/images/wiring.png`](electrical/stepper_driver/images/wiring.png) | TMC2209 carrier pin orientation and MS1/MS2 microstep selection. Validate the installed carrier revision before wiring. |
| [`motion/nema17/images/schematic.png`](motion/nema17/images/schematic.png) | NEMA17 mounting pattern, 5 mm shaft, body/shaft envelope, and winding labels used for motor-mount and connector clearance. |
| [`motion/nema17/images/specs.png`](motion/nema17/images/specs.png) | 17HS4401 current, holding-torque, step-angle, and mass reference used to check driver and structure assumptions. |
| [`motion/nema17/cad/stepper_17hs4401s.step`](motion/nema17/cad/stepper_17hs4401s.step) | Reference envelope for the conveyor motor mount; it is supplier geometry, not project-authored CAD. |
| [`fasteners/bearing/cad/nsk_mr85zz.stp`](fasteners/bearing/cad/nsk_mr85zz.stp) | MR85ZZ bearing envelope used for shaft-support and pocket-fit checks. |
| [`reference/lego_2x3/lego2x3_red.stp`](reference/lego_2x3/lego2x3_red.stp) and [`reference/lego_2x3/lego2x3_red.txt`](reference/lego_2x3/lego2x3_red.txt) | Longest supported brick envelope used for guide, shroud, and chute clearance. |
| [`sensing/color_sensor/datasheets/gy31_color_module.pdf`](sensing/color_sensor/datasheets/gy31_color_module.pdf) | GY-31 module pinout, 2.7-5.5 V range, LED control, and 33.2 x 33.2 x 25 mm module envelope. |
| [`sensing/color_sensor/datasheets/tcs230.pdf`](sensing/color_sensor/datasheets/tcs230.pdf) | TCS230/TCS3200-family filter selection, output-frequency scaling, pulse-output behavior, and electrical limits behind firmware sampling. |

## Deliberate exclusions

- Generic product photos, branded
  belt/pulley vendor models are excluded because the system-level conveyor STEP
  already carries the useful assembled geometry. Other parts, such as the IR break beams, may require modifications to CAD depending on size. Cross check parts to order with CAD models.

These files remain third-party references. Their presence does not imply
project authorship or grant redistribution rights beyond their original terms.
