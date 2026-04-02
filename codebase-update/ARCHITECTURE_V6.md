# SCT 2026: System Architecture V6

## What this builds

A fully automated LEGO brick sorting system that classifies 24 bricks by size and color
and drops them into 4 labeled bins. Built for TSA System Control Technology nationals,
May 1 2026. The evaluator loads all 24 bricks, presses start, and does nothing else.
Target: 24 bricks sorted correctly, maximum reliability first, speed tuned after.

---

## Core design principle

**Every correctness guarantee is enforced by geometry and binary state, not timing.**

- One brick occupies the isolation chamber at a time. Physical geometry prevents two.
- Sensing only happens when the system is confirmed static. No motion noise.
- Routing is set by a stepper-indexed chute selector before the brick is released.
- Release is a gravity trapdoor. Support is removed. Physics does the rest.

The belt carries no timing responsibility. All decisions are made while the brick is
motionless. Belt speed has zero effect on classification or routing correctness.

---

## Pipeline

```
[feed chute: 24 bricks loaded]
    -> [narrow belt: 20mm channel, 100-120mm transport]
    -> [isolation chamber: stop wall, micro-switch confirm, one brick physically possible]
    -> [STATIC: dual IR size beams + TCS34725 color -> classification locked]
    -> [stepper indexes 4-position chute selector disc to correct bin]
    -> [solenoid fires class 3 lever: tip sweeps out from under platform tab]
    -> [platform far edge unsupported: drops under gravity]
    -> [brick falls through aligned disc funnel into correct bin]
    -> [bin entry beam confirms arrival]
    -> [platform returns to level: lever tip re-engages tab via chamfer]
    -> [token restored: belt feeds next brick]
```

---

## Trapdoor mechanism summary

The chamber floor is a hinged platform. A class 3 lever arm runs alongside the platform's
far edge (the disc-facing edge, away from the belt). The lever tip tucks under a small
tab on the platform far edge — this is what supports that edge.

The solenoid (JF-0530B 6V) mounts near the lever hinge. It fires a short lateral stroke.
The class 3 geometry amplifies this into a larger arc at the lever tip, which sweeps
outward away from the belt, clearing the platform tab. The platform far edge is
unsupported. Gravity drops it. The brick falls straight down.

The solenoid never carries the platform weight. It only overcomes the lever return spring.
This is the minimum possible work for an actuator in this system.

Return: solenoid de-energizes, lever return spring pulls tip back inward. Platform
returns to level under its own return spring. Lever tip chamfer re-engages the tab
automatically as the platform levels — slam latch geometry, no active re-latching.

---

## Sensing

**Size:** Dual IR break-beams in chamber walls at X=5mm and X=21mm. 16mm spacing.
Both blocked = 2x3. Anything else = 2x2. Binary, position-independent, static.

**Color:** TCS34725 on chamber wall exterior through 12mm x 12mm window. Black PLA shroud.
R/(R+G+B) ratio averaged over 8+ samples during static settle phase.

Both reads happen only when belt is OFF and micro-switch confirms brick is seated.

---

## Chute selector

NEMA 11 stepper (TMC2209) drives a 100mm disc with 4 funnel openings at 90-degree
intervals, arranged at NW/NE/SE/SW (45 degrees from belt axis). Disc is secured with
a 5mm aluminum shaft hub — not printed bore. StallGuard monitors for jam. Periodic
re-home every 8 bricks catches any step count drift.

Stepper indexes BEFORE the trapdoor fires. The brick cannot fall until the disc is
at the correct position and confirmed by step count.

---

## Bin assignment

| Bin | Direction | Category | Count | Pusher |
|-----|-----------|----------|-------|--------|
| 1 | NW | 2x2 red | 6 | 315 deg |
| 2 | NE | 2x2 blue | 6 | 45 deg |
| 3 | SE | 2x3 blue | 8 | 135 deg |
| 4 | SW DEFAULT | 2x3 red | 4 | 225 deg |

Bin 4 is the default and home position. Any classifier failure lands here.
Rarest category minimizes contamination damage.

---

## Key decisions locked

- **Gravity trapdoor with class 3 lever.** Support removed, physics does it. Not a pusher.
- **Class 3 lever sweeps outward (away from belt).** Solenoid decoupled from platform weight.
- **No escapement cam.** Single-brick constraint enforced by chamber geometry + token.
- **Static-only sensing.** No classification during motion. Period.
- **Pre-committed routing.** Disc positioned before brick falls. Passive post-drop.
- **Token = occupancy truth.** No timing estimates of chamber state.
- **5mm aluminum shaft hub.** Printed disc bore is not acceptable.
- **Default = rarest (bin 4).** Minimum contamination on classifier failure.

---

## Competition compliance

| Requirement | V6 implementation |
|-------------|-------------------|
| Two or more sensors | Dual size beams (2), color sensor (3), entry beam (4), stop micro-switch (5), 4 bin beams (9 total) |
| Manual start/stop | Momentary button on GPIO 19 |
| Programmable controller | ESP32 DevKit |
| Feedback loop | Chamber sensors confirm occupancy; bin beams confirm sort; ERROR_HALT on miss |
| Motorized conveyor | JGB37-520 belt motor |
| Automated sorting mechanism | NEMA 11 stepper + chute selector disc |

---

## Timing budget (phase 1, conservative)

| Phase | Duration |
|-------|----------|
| Belt delivers brick (120mm at 100mm/s) | 1200ms |
| Entry beam + belt stop | 10ms |
| Settle | 50ms |
| Size beams (instantaneous) | 5ms |
| Color accumulation (8 samples at 24ms) | 192ms |
| Classification lock | 1ms |
| Stepper index (worst case 180 deg at 400 sps) | 2000ms |
| Lever fires + brick falls (80ms + 128ms) | 208ms |
| Platform + lever return | 200ms |
| Bin confirm | 50ms |
| Token restore + belt restart | 20ms |
| **Total per brick** | **~3936ms** |
| **24 bricks** | **~94 seconds** |

Phase 1 is deliberately slow. Speed optimization after 24/24 confirmed:
- Stepper to 2000 sps: worst case 400ms, saves 1600ms per brick
- Belt to 200mm/s: saves 600ms per brick
- Color samples to 4: saves 96ms per brick
- Overlap stepper with color sensing (phase 2 optimization)

Target after optimization: under 10 seconds for 24 bricks.

---

## Subsystem docs

- Physical design: docs/MECHANICAL.md
- Wiring and power: docs/ELECTRICAL.md
- Firmware architecture: docs/EMBEDDED.md
- Critical dimensions: docs/DIMENSIONS.md
- Parts list: docs/BOM.md
- Color calibration: docs/CALIBRATION.md
- Build checklist: docs/CHECKLIST.md
- Pass/fail acceptance: docs/TEST_PROTOCOL.md
- Engineering notebook: docs/NOTEBOOK.md
- Competition info: docs/competition/COMPETITION_INFO.md
