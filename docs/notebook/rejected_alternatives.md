# Rejected Alternatives

This file is a branch list of ideas that were considered and not selected for the active states path.
Keep it short and honest. Expand a branch only when there is a real decision history to record.

## Full Designs

- Archived chamber and release-gate sorter
  - Included compressed queue, chamber sensing, release gate, and NEMA11 selector
  - Reason it is not selected for states: too much mechanism risk for the remaining sprint
  - Status: preserved in `_archive/previous-chamber-release-design-2026-04/` for later nationals work

- Automated compressed queue for states
  - Singulator branch: escapement
  - Singulator branch: cam gate
  - Singulator branch: Geneva style indexer
  - Singulator branch: star wheel
  - Singulator branch: dual pin feed
  - Reason it is not selected: manual one-at-a-time feed removes a major jam source and is allowed by the event

- NEMA11 selector for states
  - Branch: four-position stepper chute
  - Branch: hub-mounted indexed selector
  - Reason it is not selected: the MG995/MG996-class servo chute is simpler to wire, mount, and tune for four positions

- 2020 extrusion frame
  - Reason it is not selected: available materials are wood and 3D printing, so the active frame must match what can be built now

- Full custom conveyor before import attempt
  - Reason it is not selected: downloaded NEMA17 conveyor geometry can save the CAD weekend if it imports cleanly

- Two-stage lane split
  - First split branch: size then color
  - First split branch: color then size
  - Routing branch: separate lanes for each size
  - Routing branch: separate lanes for each color
  - Reason it is not selected: it increases packaging pressure inside the footprint and adds hardware without improving the sprint risk profile

## Subdesign Notes

- Feed automation can be revisited only after the manual-feed sorter works.
- Size sensor choice remains open and should be decided by the simplest reliable test evidence.
- Servo chute angle should be chosen from real-brick testing, not from CAD alone.
