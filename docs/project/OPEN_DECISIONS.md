# Open Decisions

Use this file for unresolved states-build choices that should not be treated as active architecture yet.

Status rule:
- `OPEN`: still undecided
- `DECIDED`: resolved and ready to move into the main architecture docs
- `DROPPED`: no longer worth pursuing
- `ARCHIVED`: preserved only for later nationals work

## Active Open Items

| Topic | Status | Current options | Current guidance |
|------|--------|-----------------|------------------|
| Conveyor CAD source | OPEN | Downloaded NEMA17 conveyor assembly, adapted existing roller design, simple scratch-built conveyor | Try downloaded geometry first. Use current custom roller work only if import or adaptation fails |
| Final chute angle | OPEN | 35 to 40 degrees | Start CAD at 40 degrees. With UHMW lining the angle is not the sensitive variable - verify with a short lined test segment but do not spend time iterating angle before lining the chute. |
| Selector Z stack | OPEN | Keep Z=90mm pivot, raise pivot, reduce chute slope, side-entry funnel | Model chute entry, chute exit, and bin funnel catch height together before large prints. Do not reuse chute exit height as chute entry height. |
| Selector sweep radius and funnel layout | OPEN | Increase radius, narrow funnels, stagger bins, increase servo spacing | At 35-degree spacing, 65mm to 75mm radius does not fit 60mm mouths without overlap. Check the arc before locking bin positions. |
| Servo position table | OPEN | Four positions around 35 degrees apart, adjusted to real bin guides | Start near 37, 72, 107, and 142 degrees. Lock only after CAD and physical bin alignment |
| Frame construction details | OPEN | Wood frame, 3D printed frame, hybrid frame | Use available material. Do not design around 2020 extrusion unless the user changes the material plan |
| Optional feed chute | OPEN | Stay manual one-at-a-time, add simple feed guide, add simple chute | Manual feed is the base states build. Add a feed chute only after the sorter works reliably |

## Decided For States

| Topic | Status | Decision |
|------|--------|----------|
| Feed mode | DECIDED | Manual one-at-a-time feed is active for states |
| Selector actuator | DECIDED | MG995/MG996/MG996R-class heavy servo from `docs/datasheet/motion/heavy_servo/` |
| Conveyor motor | DECIDED | NEMA17 remains active for conveyor motion |
| Color sensor | DECIDED | TCS3200/GY-31 color sensor, calibrated only with shroud installed |
| Size sensor family | DECIDED | Break-beam timing with two pairs in the sensing shroud |
| Side rail feed guide | DECIDED | Low side rails from X=0 to X=120, tapering to a 30mm shroud entry gap |
| Frame material family | DECIDED | Wood or 3D printed structure from available stock |
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
| 2020 extrusion frame | ARCHIVED | Not active because available materials are wood or printed structure |
