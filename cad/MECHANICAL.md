# Mechanical Design

All dimensions are in `cad/DIMENSIONS.md`. This document covers design rationale,
construction method, and assembly notes.

---

## Brick dimensions (source of truth)

| Brick | Width (across belt) | Length (along belt) | Height | Count |
|-------|---------------------|---------------------|--------|-------|
| 2x2   | 15.8mm              | 15.8mm              | 11.4mm | 12 (6 red, 6 blue) |
| 2x3   | 15.8mm              | 23.7mm              | 11.4mm | 12 (4 red, 8 blue) |

Orientation rule: bricks are long-side-across the conveyor, meaning the 23.7mm side of a 2x3 spans across the belt channel while the 15.8mm side runs along travel.
Why this orientation is required: it keys the chamber footprint to the long side so only one brick can occupy the trapdoor region at a time. The feed chute, chamber, and beam layout below are provisional until they are re-derived from this orientation.

---

## Terminology

- Selector chute: the four-position routing assembly that sends one brick to one of the four bins.
- Index position: one of the four discrete selector chute states. The current mapping is 1 through 4, with the fourth position treated as home or default.
- Chamber pitch: the advance distance from one seated brick to the next in the compressed queue.
- Trapdoor platform: the hinged chamber floor that drops the brick after routing is committed.
- Supported roller shaft: the conveyor shaft that rides in frame bearings and carries the smooth drive roller.
- Safe restart condition: the combined physical confirmation that the chamber is clear, the platform is level, and the next feed can begin without overlap.

---

## Feed chute

Vertical rectangular tube, 25mm target width with provisional depth.
All 24 bricks load before the run. Gravity feeds the queue.

The listed width and depth are provisional placeholders until the long-side-across layout is re-derived.

- Width 25mm target: long-side-across chamber width with clearance margin.
- Depth: provisional until the long-side-across layout is re-derived.
- Exit opening height: 13.5mm. One brick (11.4mm) exits freely. Two stacked (22.8mm)
  cannot fit. Double-feed is geometrically impossible at this dimension.
- Do not print exit opening above 14.0mm. Measure with calipers after printing.
- Top flare: 35mm x 40mm over 20mm.
- Interior walls: PTFE tape on all 4 faces.

Bottom brick rests directly on belt. Belt friction pulls it out when belt is enabled.
No cam. No escapement.
The retired `cad/escapement/` directory is historical only.

---

## Conveyor stage

The production conveyor is an off-axis timing-belt stage feeding a supported smooth drive roller.

- Channel width: 25mm target.
- Channel walls: 3mm PLA, 15mm tall, PTFE-taped interior.
- Belt material: final selected neoprene strip remains open. Current received stock in `docs/project/BOM.xlsx` is `15mm x 3mm` and `25mm x 3mm`.
- Transport length: 100-120mm provisional.
- Chamber pitch: 18-22mm provisional. This is the steady-state advance distance, not the full transport length.
- Target belt speed: 100mm/s provisional.

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
- Motor pulley: 16 teeth.
- Drive pulley: 32 teeth.
- Belt pitch: 2mm.
- Belt width: 6mm.
- Center distance: 78mm.
- Tension adjustment travel: 8mm.

Idler roller:
- 25mm OD with 0.5mm crown.
- MR85ZZ bearings on M5 bolt axle.
- Spring-loaded tensioner slot.

Optional Hall sensing on the idler is diagnostic only. Conveyor correctness does not depend on it.

---

## Isolation chamber

One brick inside at a time. Geometry enforces this.

- Width (across belt): 25mm target.
- Depth (along belt): 27mm.
- Height above platform: 15mm.

Width and depth are provisional until the long-side-across chamber is re-derived.

Stop wall: entry side. 3M rubber foot pad (3mm) absorbs impact, prevents bounce.
Stop-wall micro-switch (Omron D2F): 1.5mm actuator protrusion, brick contact = LOW.
Side walls: continuous from belt channel. Brick cannot yaw.

Sensor integration:
- IR beam holes are provisional until the long-side-across sensing layout is re-derived.
- Color sensor window 12mm x 12mm in one chamber side wall at 5.7mm above platform surface.
- Black PLA shroud on sensor, 15mm deep.

---

## Trapdoor mechanism: class 3 lever

This is the drop mechanism. The design principle is: remove support, let gravity do it.
The solenoid never carries brick weight. It only moves a lever arm against a light spring.

### Overview

The chamber floor is a hinged platform. A class 3 lever arm runs alongside the platform's
far edge (the selector-side edge, away from the belt). The lever tip sits under a small
printed tab on the platform far edge, supporting it. When the solenoid fires, the lever
tip sweeps outward (away from the belt) and clears the tab. The platform far edge has
nothing under it and drops under gravity. Brick falls straight down.

Class 3 lever: fulcrum at one end, load (platform tab) at the other end, effort
(solenoid) between them, closer to the fulcrum. This gives mechanical disadvantage
in force but amplifies the solenoid's short stroke into a larger arc at the lever tip.

### Platform

- Platform: 22mm wide (along belt) x 20mm deep (across belt, toward selector) x 3mm thick PLA.
- Top surface: UHMW tape. Frictionless. Brick must not stick.
- Hinge: 3mm steel rod through printed ears on the platform belt-side edge, passing
  through brackets on the chamber entry wall. Hinge axis runs across the belt (parallel
  to entry wall). Platform pivots about this axis.
- Far edge: the selector-side edge. This is the unsupported edge when the lever clears.
- Platform tab: a 4mm wide x 3mm tall printed lip on the underside of the far edge,
  running the full 22mm width. The lever tip rests under this tab.

The platform is flush with the chamber floor when level. The hinge is at the belt side.
The far edge drops when unsupported.

### Class 3 lever arm

- Lever arm: printed PLA at 100% infill, 0.15mm layers. Forces are very low; PLA is fine.
- Length: 30mm total.
- Fulcrum: one end of the lever arm. M3 bolt through printed bracket on the chamber
  side wall (the 25mm wall on the selector-side outboard wall). M3 nylon locknut to prevent
  vibration loosening. This is the pivot.
- Lever tip: the other end (30mm from fulcrum). This end tucks under the platform tab.
  The tip has a 30-degree chamfer on its upper face for re-engagement.
- Solenoid contact point: 8mm from the fulcrum (between fulcrum and tip). This is the
  effort application point.
- Sweep direction: the lever arm sweeps outward, away from the belt, in the horizontal
  plane. The tip moves from under the tab to clear of it.

Lever geometry (class 3):
- Fulcrum at 0mm.
- Effort (solenoid contact) at 8mm.
- Load (platform tab) at 30mm.
- Mechanical advantage: 8/30 = 0.27 (disadvantage in force, advantage in displacement).
- Solenoid stroke: 8mm at effort point.
- Lever tip travel: 8mm * (30/8) = 30mm arc at tip.
- Required tip travel to clear tab: approximately 8-10mm.
- Available tip travel: 30mm. More than double what is needed.

This means even at half solenoid stroke (4mm, e.g. due to weak battery or worn spring),
the tip travels 15mm. That is still 50% more than required to clear the tab. The
mechanism has inherent stroke tolerance built into the lever ratio.

### Solenoid mounting

The purchased 0530-series solenoid mounts on the chamber side wall (same wall as the lever fulcrum),
plunger axis parallel to the belt travel direction (pointing inward toward the belt).
The plunger tip contacts the lever arm at the effort point (8mm from fulcrum).

When solenoid fires:
- Plunger extends toward the belt.
- Lever arm rotates about the fulcrum.
- Lever tip sweeps outward, away from the belt.
- Tab is cleared. Platform drops.

When solenoid de-energizes:
- Plunger spring retracts.
- Lever return spring (light tension spring anchored between lever arm mid-point and
  chamber wall) pulls lever tip back inward.
- Lever tip rides back under the platform tab via the chamfer as the platform returns.

The solenoid plunger moves parallel to the belt axis. It is not fighting gravity.
It is not supporting any weight. It is pushing a lever arm against a spring rated at
approximately 0.05-0.1N. This is the lightest possible solenoid load in this design.

### Platform return

A second light tension spring runs from the platform far edge (underside) to a hook
on the chamber wall above. This spring is in slight tension when the platform is level
(preload only, < 0.01N). When the platform drops, this spring stretches and pulls the
far edge back up once the brick has cleared.

Spring specification: 0.3-0.4mm wire, 4-5mm OD, 15mm free length. Lightest spring
in the assortment that reliably returns the platform in under 200ms.

The platform return spring and the lever return spring are independent. The platform
returns to level, and the lever tip rides under the tab simultaneously via the chamfer.
No sequencing required.

### Re-engagement (slam latch geometry)

The lever tip upper face has a 30-degree chamfer. The platform tab underside has a
matching 30-degree lead-in. As the platform returns to level, the tab's lead-in surface
contacts the lever tip chamfer and pushes the lever tip outward slightly, then the tip
snaps back under the tab once the tab clears the chamfer peak.

This is identical to a slam latch. The platform closing re-engages the lever
automatically with no active control. The lever return spring provides the snap-back
force. The chamfer angle determines the force required to push the lever tip aside
during re-engagement. At 30 degrees with a 0.1N return spring: re-engagement force
is 0.1 * tan(30) = 0.058N, which a 3g platform falling under gravity provides easily.

### Why this mechanism is correct

The fundamental problem: remove the platform support so the brick falls under gravity.

Direct plunger (vertical): solenoid supports platform weight during retraction. Plunger
bore friction adds to load. Wear accumulates. Solenoid is working hardest when most
loaded.

Sliding platform: moving support surface. Depends on friction, stroke consistency, brick position.
Not a trapdoor.

Class 3 lever sweeping laterally: solenoid decoupled from platform weight entirely.
Solenoid only overcomes the lever return spring (~0.05-0.1N). Platform weight is
carried by the lever arm structure, not the solenoid. Stroke is amplified so even
partial actuation clears the tab. Failure modes are geometric and testable.

---

## Selector chute

NEMA 11 stepper indexes the four-position selector chute through a 5mm aluminum shaft hub or clamp interface.
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

Home sensor: Omron D2F micro-switch, index 4 flag, default.

---

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

3. Print lever arm + platform + hinge bracket. Bench test mechanism:
   - Platform pivots smoothly on hinge rod.
   - Lever tip tucks under platform tab cleanly with no play.
   - Solenoid pushes lever tip clear of tab.
   - Lever return spring and platform return spring both reset within 200ms.
   - Slam latch re-engages: platform returning to level auto-resets lever.
   - 50 consecutive cycles with no failures.
   Gate: all pass before printing anything else.

4. Print selector chute prototype. Mount with shaft hub. Verify repeatable indexing at all 4 positions.

5. Print chamber body. Fit real bricks. Test full drop: brick seated, lever fires,
   brick falls, both springs return, lever re-latches. 20 consecutive cycles.

6. Print feed chute. Single-brick exit test with all 24 bricks.

7. Print remaining frame parts. Assemble belt and timing stage. Friction test.

8. Full integration: chute -> timing stage -> chamber -> selector chute -> bins.

9. Wire. Self-test. Calibrate. Run.

---

## Highest fabrication risk

The slam latch re-engagement. The chamfer angles on the lever tip and platform tab
must work together: steep enough that the returning platform pushes the lever aside
easily, but shallow enough that the lever holds the platform reliably when loaded.

30 degrees is the starting point. Test empirically:
- If the platform pushes the lever aside during normal operation (false release): increase
  chamfer angle toward 45 degrees.
- If the returning platform cannot push the lever aside for re-engagement: decrease
  chamfer toward 20 degrees.

This is the one geometry that requires iteration. Budget 3 prints of the lever arm
and platform tab only. These are small parts, fast to print.
