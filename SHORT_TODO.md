# Short TODO

Updated April 20, 2026 after the CAD ownership pivot.

Current priority: finish the physical CAD dependency chain fast enough to print and troubleshoot. The developer owns bins to chute to conveyor. Work downstream first because the receiving bins define the chute catch geometry, the chute defines conveyor exit height, and the conveyor feet plus woodworking come last.

## Immediate CAD Sprint

1. Receiving bins and funnel cassette
- Design four labeled receiving bins with wide forgiving funnel mouths.
- Keep interiors separated by category.
- Preserve removal access and label visibility.
- Check the 610mm x 610mm footprint early.
- Print one bin or funnel coupon as soon as it answers a real catch question.

2. Servo rotary chute selector
- Design the chute around the actual bin funnel catch zones.
- Keep the chute short, stiff, UHMW-lined, and easy to reprint.
- Treat servo angles as calibration outputs.
- Print the smallest chute or horn adapter coupon that checks slide, stiffness, or fit.

3. Conveyor handoff and support
- Set conveyor height from the chute entry, not from old conveyor coordinates.
- Derive conveyor feet and exit handoff only after the chute entry is known.
- Keep the sensing station near the feed end.
- Do not optimize speed before real handoff works.

4. Woodworking
- Cut and drill permanent wood only after bins, chute, conveyor height, and handoff stack agree.
- Use temporary blocks, clamps, and shims while finding the stack.

## Same-Day Physical Truth

- Print or mock up the first funnel catch feature quickly.
- Test real brick drop into the funnel before committing to the full bin set.
- Test a UHMW-lined chute segment before relying on chute angle.
- Test conveyor-to-chute handoff with one brick before permanent feet.

## Parallel Non-CAD Work

- Keep professor meeting prep short and focused on print orientation, wood-to-print joints, belt tracking, servo mount stiffness, and handoff geometry.
- Keep engineering notebook decision matrices moving, but hardware CAD now wins the schedule.
- Save run logs to CSV once hardware starts moving.
- Wiring and firmware remain important, but physical sorting reliability is the bottleneck.

## Do Not Do

- Do not revive hopper, chamber, start gate, ToF sizing, NEMA11 selector, or 2020 extrusion for the states build.
- Do not let exact draft CAD dimensions leak into tracked docs.
- Do not cut final wood before the downstream-to-upstream stack is physically plausible.
