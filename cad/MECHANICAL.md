# Mechanical Design

All dimensions are in `cad/DIMENSIONS.md`. This document covers design rationale,
construction method, and assembly notes.

---

## Brick dimensions (source of truth)

| Brick | Width (across belt) | Length (along belt) | Height | Count |
|-------|---------------------|---------------------|--------|-------|
| 2x2   | 15.8mm              | 15.8mm              | 11.4mm | 12 (6 red, 6 blue) |
| 2x3   | 23.7mm              | 15.8mm              | 11.4mm | 12 (4 red, 8 blue) |

Orientation rule: bricks are long-side-across the conveyor, meaning the 23.7mm side of a 2x3 spans across the belt channel while the 15.8mm side runs along travel.
Why this orientation is required: it keys the chamber footprint to the long side so only one brick can occupy the release zone at a time. The feed chute, chamber, and beam layout below are provisional until they are re-derived from this orientation.

---

## Terminology

- Selector chute: the four-position routing assembly that sends one brick to one of the four bins.
- Index position: one of the four discrete selector chute states. The current mapping is 1 through 4, with the fourth position treated as home or default.
- Chamber pitch: the advance distance from one seated brick to the next in the compressed queue.
- Start gate: the upstream queue stop above the transition. It holds the stack off the transition before the run begins.
- Release gate: the chamber support-removal mechanism. Active actuation family: solenoid-driven retracting support.
- Trapdoor platform: historical term for the earlier release concept. Treat as stale unless a legacy note is being discussed.
- Supported roller shaft: the conveyor shaft that rides in frame bearings and carries the smooth drive roller.
- Safe restart condition: the combined physical confirmation that the chamber is clear, the release path is reset, and the next feed can begin without overlap.

---

## Feed chute

Vertical rectangular tube, 25mm target width with provisional depth.
All 24 bricks load before the run. Gravity feeds the queue, but the start gate keeps the full stack off the transition until the run begins.

The listed width and depth are provisional placeholders until the long-side-across layout is re-derived.

- Width 25mm target: long-side-across chamber width with clearance margin.
- Depth: provisional until the long-side-across layout is re-derived.
- Chute loading orientation: keep the chute parallel to the final long-side-across chamber orientation. Do not add an orientation-swap transition in the first CAD pass.
- Start gate location: in the tall straight chute section above the transition, not in the one-brick throat.
- Start gate motion: side-sweep rotary paddle that retracts flush or near-flush into a wall pocket.
- Exit opening height: 13.5mm. One brick (11.4mm) exits freely. Two stacked (22.8mm)
  cannot fit. Double-feed is geometrically impossible at this dimension.
- Do not print exit opening above 14.0mm. Measure with calipers after printing.
- Top flare: 35mm x 40mm over 20mm.
- Interior walls: line with adhesive-backed UHMW tape. Do not treat plumber's thread tape as a wear surface.

Bottom brick rests directly on belt. Belt friction pulls it out when belt is enabled.
No cam. No escapement.
The retired `cad/escapement/` directory is historical only.

---

## Conveyor stage

The production conveyor is an off-axis timing-belt stage feeding a supported smooth drive roller.

- Channel width: 25mm target.
- Channel walls: 3mm PLA, 15mm tall, with adhesive-backed UHMW tape on the brick-contact faces.
- Belt material: `25mm x 3mm` neoprene strip is the active architecture. Treat the received `15mm x 3mm` strip as spare stock unless testing proves a real need to reopen it.
- Transport length: 100-120mm provisional.
- Chamber pitch: 18-22mm provisional. This is the steady-state advance distance, not the full transport length.
- Target belt speed: 130-300mm/s tuning range. Design the baseline for stable low-speed validation first, then tune upward.

The channel width and chamber dimensions above are provisional until the long-side-across layout is re-derived.

Drive architecture:
- NEMA17 stepper drives a toothed timing belt.
- Timing belt drives a supported shaft at the conveyor end.
- Supported shaft carries the smooth drive roller that contacts the neoprene belt.
- Drive roller is not mounted directly on the motor shaft.

Timing stage intent:
- Motor shaft load is isolated from the conveyor roller load.
- Ratio can be changed by swapping pulley tooth counts.
- The conveyor can be tuned without changing the supported shaft geometry.

Timing-stage reference values:
- Motor pulley: 20 teeth.
- Drive pulley: 20 teeth.
- Belt pitch: 2mm.
- Belt width: 6mm.
- Center distance: 80mm nominal, with slot travel to cover nearby belt options.
- Tension adjustment travel: 8mm.

Idler roller:
- 25mm OD with 0.5mm crown.
- MR85ZZ bearings on M5 bolt axle.
- Fixed axle in the first-pass architecture. Use the slotted NEMA17 mount for tension first.

Optional Hall sensing on the idler is diagnostic only. Conveyor correctness does not depend on it.
Conveyor bed: use an integrated printed flat support path in the first CAD pass. Do not add a separate metal bed unless real testing proves the printed path is insufficient.

---

## Isolation chamber

One brick inside at a time. Geometry enforces this.

- Width (across belt): 25mm target.
- Depth (along belt): 27mm.
- Height above chamber floor: 15mm.

Width and depth are provisional until the long-side-across chamber is re-derived.

Stop wall: downstream end, opposite the chamber entry reference side. 3M rubber foot pad (3mm) absorbs impact and prevents bounce.
Stop-wall micro-switch is required for chamber seat truth.
Side walls: continuous from belt channel. Brick cannot yaw.

Sensor integration:
- IR beam holes are provisional until the long-side-across sensing layout is re-derived.
- Color sensor window 12mm x 12mm in one chamber side wall at 5.7mm above chamber floor.
- Black PLA shroud on sensor, 15mm deep.

---

## Release mechanism

The chamber release is a solenoid-driven retracting support.
The support holds one seated brick, then retracts clear so gravity does the rest.
The exact support geometry is still a prototype zone, but the actuation family is now frozen.

### Requirements

- Release support must be removed cleanly from one seated brick
- The solenoid must only retract support. It should not carry brick weight as a permanent support.
- The release path must not intrude into the conveyor, selector, or sensing envelopes
- The mechanism must return to a safe reset state without relying on a hidden assumption
- Reset confirmation must be physical, not guessed from time alone
- First CAD pass must reserve a mechanical flag and micro-switch mounting provision for release-support return truth, even if the switch is not installed in the first prototype

### Notes for CAD

- Keep the retracting support geometry small and simple
- Leave room for a light return spring or similar return bias
- Keep the solenoid body outside the chamber envelope
- Keep the wiring and protection consistent with a coil-based actuator

## Selector chute

NEMA 11 stepper indexes the four-position selector chute through a rigid 5mm flange-mount hub with M3 face mounting.
Not press-fit. Not printed bore. Non-negotiable.
This is not a circular disc.

Index geometry:
- Four fixed outlet positions, one per bin.
- Index spacing: 90 degrees equivalent between the four states.
- Funnel opening size: 32mm x 22mm.
- Funnel wall taper: 10 degrees inward, all sides.
- Print: 0.15mm layers, 80% infill.

Motor mount: metal bracket (commercial NEMA 11 mounting bracket or 2mm aluminum flat bar).
Do not rely on printed PLA alone for motor mounting.

Selector routing note:
- The selector chute is the active routing path for now.
- Downstream routing alternatives belong in the notebook, not in this active mechanical spec.

Selector home micro-switch is required in the base architecture.

## Bins

4 bins at NW, NE, SE, SW (45 degrees from belt axis).

| Bin | Direction | Category | Count | Internal W x D x H |
|-----|-----------|----------|-------|---------------------|
| 1 | NW | 2x2 red | 6 | 40mm x 40mm x 90mm |
| 2 | NE | 2x2 blue | 6 | 40mm x 40mm x 90mm |
| 3 | SE | 2x3 blue | 8 | 40mm x 45mm x 110mm |
| 4 | SW DEFAULT | 2x3 red | 4 | 40mm x 45mm x 70mm |

Floor: 10 degrees toward back wall. Break-beam at entrance. Removable via slot-and-tab.
Funnel guides between selector chute and bins: 60mm x 60mm entrance, angled to bin opening.

---

## Operator-facing packaging

These features matter for first-time evaluator use and are part of the mechanical design.

- Start button is labeled or engraved so the operator does not guess.
- Display face is readable from arm's length while standing in front of the machine.
- Bin labels are fixed and large: 2x2 RED, 2x2 BLUE, 2x3 BLUE, 2x3 RED.
- Cable routing is kept outside the brick path, chute opening, and selector drop envelope.
- Label flats are reserved on the front-facing frame surfaces and bin faces.
- No harness crosses the conveyor belt top plane.

---

## Assembly order

1. Print chute transition and validate with real bricks. Do not start larger prints until feed is clean.

2. Print conveyor timing stage fit parts and validate packaging:
   - Motor pulley
   - Drive roller
   - Idler roller
   - Tensioner and supported shaft interfaces
   - Confirm belt tracking envelope and pulley alignment

3. Print the release mechanism prototype for the chosen refactor direction. Bench test support removal and reset:
   - Brick support is removed cleanly
   - Brick falls without sticking
   - Return path resets without binding
   - Reset confirmation is physical and repeatable
   - 50 consecutive cycles with no failures
   Gate: all pass before printing anything else.

4. Print selector chute prototype. Mount with shaft hub. Verify repeatable indexing at all 4 positions.

5. Print chamber body. Fit real bricks. Test full release: brick seated, release fires,
   brick falls, reset returns. 20 consecutive cycles.

6. Print feed chute. Single-brick exit test with all 24 bricks.

7. Print remaining frame parts. Assemble belt and timing stage. Friction test.

8. Full integration: chute -> timing stage -> chamber -> selector chute -> bins.

9. Wire. Self-test. Calibrate. Run.

---

## Highest fabrication risk

The release reset geometry. The chosen mechanism must clear support cleanly, reset
repeatably, and stay inside the chamber envelope.

Test empirically:
- If the release path sticks during normal operation, simplify the geometry before adding force.
- If the reset path is unreliable, reduce friction and revisit the return bias.
- If the mechanism intrudes into the chamber, selector, or conveyor envelope, shrink the mount.

This is the one geometry that requires iteration. Budget a few small prototype prints
around the final chosen release concept, not around the old release-concept assumption.
