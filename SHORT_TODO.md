# Short TODO

Updated April 21, 2026 after the sweep-defined bin arc pivot.

Current priority: finish the physical CAD dependency chain fast enough to print and troubleshoot. The chute, servo clearance, and swept brick envelope now define the bin catch geometry. Wider and more adjustable is better than a compact transition that jams at speed.

## Immediate CAD Sprint

1. Widen middle support and place selector
- Widen the middle support block enough for servo rotation clearance.
- Place the real MG995/MG996-class servo body with horn face up.
- Give the servo feet slotted adjustment so belt, chute, and bin arc alignment can be tuned physically.
- Keep the full assembly inside the 610mm x 610mm footprint.

2. Build the rounded high-wall servo chute
- Skip the static belt-to-chute ramp for the first pass.
- Set the chute floor slightly below belt top with a small horizontal gap.
- Round both exit-side chute walls and raise the side walls so they act as the containment lip.
- Leave screw access or physical room for a tiny add-on handoff lip if real-brick tests prove it is needed.
- Sweep the 2x3 brick envelope through the chute exit, not just the chute centerline.

3. Derive bins from the chute sweep
- Sketch the chute end rotation arc from the actual pivot and four selector positions.
- Expand the arc into the bin cavity and divider layout.
- Create the removable bin shell from the swept arc.
- Create a fixed bin platform from projected bin edges, with asymmetric locator features so the bin shell re-indexes after dumping.
- Preserve clear labels for 2x2 RED, 2x2 BLUE, 2x3 RED, and 2x3 BLUE.

4. Align conveyor motor and belt path
- Elevate and slot the motor mount feet across Y for pulley alignment.
- Laterally align the driven pulley and motor pulley before finalizing the print.
- Use equal GT2 16T pulleys and the 200mm belt starting center distance: 84mm.
- Add 1 to 3mm outward motor adjustment for belt tension.
- Extrude a simple GT2 belt placeholder for visual checks.

5. Hard stop and print
- Export the rounded chute first.
- Print the servo mount or pocket next if fit is uncertain.
- Print the bin platform before the larger bin shell when possible.
- Keep the printer running while later CAD details continue.

## Same-Day Physical Truth

- Dry-fit the chute on the servo horn and test sweep by hand.
- Run real 2x2 and 2x3 brick envelopes through the belt-to-chute gap.
- Drop real bricks from the chute exit into the bin arc and watch for bounce-outs.
- Check that the removable bin shell drops back onto the fixed platform in exactly one position.
- Confirm pulleys line up with a straightedge before tensioning the belt.

## Parallel Non-CAD Work

- Keep professor meeting prep short and focused on print orientation, wood-to-print joints, belt tracking, servo mount stiffness, and handoff geometry.
- Keep engineering notebook equations current when a CAD dimension depends on them.
- Save run logs to CSV once hardware starts moving.
- Wiring and firmware remain important, but physical sorting reliability is the bottleneck.

## Do Not Do

- Do not design per-bin funnels for the current sprint.
- Do not design a static belt-to-chute ramp unless real tests prove the direct gap fails.
- Do not revive hopper, chamber, start gate, ToF sizing, NEMA11 selector, or 2020 extrusion for the states build.
- Do not let exact draft CAD dimensions leak into tracked docs.
- Do not cut final wood before the chute, bin arc, conveyor height, and motor alignment agree.
