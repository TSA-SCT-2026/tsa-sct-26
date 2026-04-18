# Session Context - 2026-04-18

Working tracker. Cross off items as completed. Will be finalized at session end.

## Agent todos (my work)

- [ ] Update MECHANICAL.md: motor mount (2-stage L bracket + gussets), center board extended to touch side plates, pulley Y alignment approach
- [ ] Trim CAD_PLAN.md to remaining steps only. Remove all completed sections. Keep design intent for shroud/lip/rails so another agent with measurements can build guides.
- [ ] Update DIMENSIONS.md: add pulley Y positioning info, flag conveyor height as OPEN_DECISION (gated on teammate servo chute height, likely 20-25mm taller than 12mm cleats)
- [ ] Update OPEN_DECISIONS.md: add conveyor height / frame feet height item (gated on teammate)
- [ ] Update ASSEMBLY.md Stage 1: reflect 2-stage L bracket with gussets for motor mount
- [ ] Write professor meeting script as local file in private repo (../tsa-sct-26-local-instructions/)
- [ ] Run sync.sh status on private repo after writing there
- [ ] Commit all public repo changes
- [ ] Push public repo
- [ ] Finalize SESSION_CONTEXT.md with summary, final thoughts, next steps

## Human todos (ordered highest to lowest cognitive intensity)

1. Get conveyor height estimate from teammate - unblocks motor board Z, feet height, and pulley alignment. Even a rough "20mm" or "22mm" is enough to proceed.
2. Engineering notebook: write 3-5 decision matrices (size sensor, actuator, feed mode, frame material, chute vs other routing) - these are independent of hardware, all decisions already made
3. CAD: add screw holes between side plate and center board (use CAD positions from the extended center board touching side plates)
4. Review and cut down professor script (agent will produce longer than needed, trim to what feels right)
5. Wood: cut base plate, side plates, center board; drill from CAD positions; defer motor board area until conveyor height is confirmed
6. Optional (Sat/Sun): draft sensing shroud CAD - higher value than rails or lip, forces brick clearance check, useful for professor meeting as a visual

## Key info to survive compaction

- Conveyor is mostly done in CAD. Pulleys aligned: motor pulley at motor tip, driven pulley against bearing holder with 2mm spacer.
- Conveyor height NOT locked. Currently 12mm cleats. Teammate's servo chute may require 20-25mm. Until confirmed: do not finalize motor board Z, feet height, or exact pulley alignment calculation.
- Motor mount redesigned to 2-stage L bracket with gussets on motor side and frame side. Feet have M3 holes. Feet are currently just blocks (may need gussets if height grows).
- Center belt support expanded from floating to touching both side plates. Enables screws from outside.
- Exit lip: defer until conveyor height is confirmed. Wedge ramp, 5-6mm above belt, redirects brick downward.
- Sensing shroud and side rails: not yet modeled. Keep enough design intent in CAD_PLAN for another agent to design if given measurements.
- Servo chute: teammate is working on it. DO NOT touch servo chute guide docs.
- CONVEYOR_BUILD_GUIDE.md: not being updated this session (reference doc, conveyor mostly done from it).
- Professor meeting: Monday. Need script of machine description + design questions focused on CAD inspection (supports, joint method, stiffness, print settings).
