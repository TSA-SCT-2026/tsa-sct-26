# Open Decisions

Use this file for unresolved states-build choices that should not be treated as active architecture yet.

Status rule:
- `OPEN`: still undecided
- `DECIDED`: resolved and ready to move into main docs
- `DROPPED`: no longer worth pursuing
- `ARCHIVED`: preserved only for later nationals work

## Active Open Items

| Topic | Status | Current guidance |
|------|--------|------------------|
| Final exit bin geometry | OPEN | Active CAD critical path. The four receiving bins are derived from the swept chute and brick envelope, then checked for footprint, catch height, removal clearance, and label visibility. |
| Conveyor support height and feet | OPEN | Downstream of final bin and chute handoff geometry. Do not reuse old cleat or feet dimensions. |
| Servo feet and selector mount | OPEN | Downstream of final pivot location, bin catch height, and frame contact points. Use slotted feet so small alignment corrections do not require reprinting. |
| Exit lip or handoff guide | OPEN | First pass skips a static ramp. Leave physical room for a small add-on lip only if final belt surface, chute entry, brick momentum, and real-brick tests prove it is needed. |
| Permanent woodworking | OPEN | Downstream of final bin, chute, conveyor feet, and servo feet relationships. |
| Final chute angle | OPEN | Choose from UHMW-lined real-brick tests. Bare plastic behavior is not final. |
| Selector sweep radius and bin arc layout | OPEN | Must be checked against the swept chute exit and 2x3 brick envelope, including yaw margin, divider clearance, servo error, and footprint. |
| Servo position table | OPEN | Servo angles are calibration outputs. Start only after the bins and chute are physically aligned. |
| Optional feed chute | OPEN | Manual feed is the base states build. Add a feed chute only after the sorter works reliably. |

## Decided For States

| Topic | Status | Decision |
|------|--------|----------|
| Feed mode | DECIDED | Manual one-at-a-time feed is active for states |
| Conveyor approach | DECIDED | Tutorial-style conveyor logic with team hardware and real dry-fit measurements |
| Selector actuator | DECIDED | MG995/MG996/MG996R-class heavy servo from `docs/datasheet/motion/heavy_servo/` |
| Conveyor motor | DECIDED | NEMA17 remains active for conveyor motion |
| Color sensor | DECIDED | TCS3200/GY-31 color sensor, calibrated only with shroud installed |
| Size sensor family | DECIDED | Break-beam timing with two pairs in the sensing shroud |
| Frame material family | DECIDED | Wood frame from available stock, with printed brackets where useful |
| Conveyor support material | DECIDED | Wood support under the top belt run, replacing the 2020 extrusion support idea |
| Footprint | DECIDED | Stay within 610mm x 610mm |

## Archived For Later Nationals Work

| Topic | Status | Notes |
|------|--------|-------|
| Compressed 24-brick queue | ARCHIVED | Preserved under `_archive/previous-chamber-release-design-2026-04/` |
| Start gate | ARCHIVED | Not part of the states build |
| Isolation chamber | ARCHIVED | Not part of the states build |
| Solenoid release gate | ARCHIVED | Purchased solenoids remain inventory, but not active architecture |
| Dual-ToF chamber sizing | ARCHIVED | Do not reopen for states unless the user explicitly changes the size sensor decision |
| NEMA11 selector | ARCHIVED | Purchased NEMA11 motors remain inventory, but not active selector architecture |
| 2020 extrusion frame | ARCHIVED | Replaced by the wood frame path for states |
| 2020 extrusion belt support | ARCHIVED | Replaced by the wood support path |
