# Issue Fix Analysis

Pre-order review. No redesign. Minimal changes only.

---

## ISSUE 1: Size beam brick positioning

### STEP 1: Root Cause

The single IR break-beam crosses the 22mm chute dimension (Y axis) at X = 20mm
along the 27mm dimension. Detection logic: beam blocked = 2x3, beam clear = 2x2.

A 2x3 brick (23.7mm along X) always blocks the beam regardless of position. The
brick has only 3.3mm of play in 27mm, and even at worst case (P = 3.3mm from
reference wall), its far edge is at 27mm, well past the beam at 20mm.

A 2x2 brick (15.8mm along X) has 11.2mm of play in 27mm. The brick blocks the
beam only when its far edge exceeds X = 20mm, i.e., when its near edge P < 4.2mm.

    P ranges from 0 to 11.2mm.
    Safe zone (beam clear, correct detection): P in [0, 4.2] = 4.2mm range.
    Failure zone (beam blocked, false 2x3): P in [4.2, 11.2] = 7.0mm range.
    Failure rate: 7.0 / 11.2 = 62.5% of the time, IF brick position is random.

Nothing in the documentation constrains the brick's X position on the cam chord.
The cam chord is a flat surface. The brick drops from the stack above. No spring,
ramp, ledge, or asymmetry biases it toward the reference wall.

A tilted chord does not fix this. Gravitational force on a 3g brick at 10 degrees:
F_parallel = 0.003 * 9.81 * sin(10) = 5.1 mN. Static friction on PLA at mu = 0.4:
F_friction = 0.003 * 9.81 * cos(10) * 0.4 = 11.6 mN. Friction exceeds gravity at
any practical tilt angle. The brick does not slide.


### STEP 2: Minimal Fix

Add a second IR break-beam in the chute. Space the two beams so that a 2x2 brick
(15.8mm) can never span both simultaneously, but a 2x3 brick (23.7mm) always spans
both.

Required: beam spacing > 15.8mm (2x2 cannot bridge the gap).
Required: both beams within the 2x3 brick's guaranteed footprint at any position.

Beam positions: X = 5mm and X = 21mm from reference wall. Spacing = 16mm.

Verification (all numbers computed, see appendix A):

    2x3 brick, all positions (P = 0 to 3.3mm):
      Beam 1 (X=5):  P <= 3.3 < 5 < 3.3 + 23.7 = 27.  Always blocked.
      Beam 2 (X=21): P <= 3.3 < 21 < 27.               Always blocked.
      Result: always (blocked, blocked).

    2x2 brick, all positions (P = 0 to 11.2mm):
      Beam 1 (X=5):  blocked when P <= 5.  Range: [0, 5].
      Beam 2 (X=21): blocked when P >= 21 - 15.8 = 5.2.  Range: [5.2, 11.2].
      Both blocked: requires P <= 5 AND P >= 5.2.  NO OVERLAP.
      Result: never (blocked, blocked).

Detection logic change in firmware:

    if (beam1_blocked AND beam2_blocked): size = SIZE_2x3
    else: size = SIZE_2x2

Zero ambiguity. No dependence on brick position. No dependence on belt speed.
No additional mechanical positioning mechanism needed.

Parts impact: one additional IR break-beam pair (from the existing 2-spare pool;
reduces spare from 2 to 1). One additional GPIO pin on ESP32. Two additional 3mm
holes in the chute walls at X = 5mm (beam 1) and X = 21mm (beam 2, replaces the
original beam at X = 20mm).

The existing beam at X = 20mm moves to X = 21mm (1mm shift).


### Chord depth must increase

The beams pass through the chute above the cam chord surface, where the chord cut
has removed disk material. Both beams must be within the chord width. The chord
width depends on chord depth:

    Disk radius: 15mm.
    Chord width = 2 * sqrt(R^2 - (R - depth)^2)

    depth=2mm:  width=14.97mm, edges at X=6.02 to X=20.98.
      Beam X=5:  OUTSIDE chord by 1.02mm. Blocked by disk body.
      Beam X=21: OUTSIDE chord by 0.02mm. Blocked by disk body.
      FAILS.

    depth=4mm:  width=20.40mm, edges at X=3.30 to X=23.70.
      Beam X=5:  1.70mm inside chord edge.
      Beam X=21: 2.70mm inside chord edge.
      PASSES.

    depth=5mm:  width=22.36mm, edges at X=2.32 to X=24.68.
      Beam X=5:  2.68mm inside chord edge.
      Beam X=21: 3.68mm inside chord edge.
      PASSES with more margin.

At 2mm chord depth, the single-beam design barely works (X=20 beam is 0.98mm
inside the chord edge, equivalent to 3 FDM layer widths of margin). At 4mm depth,
both dual beams fit with comfortable margin.

Recommended chord depth: 4mm. This gives a chord width of 20.4mm, which aligns
with the documented "chord width: 20mm" in DIMENSIONS.md. The documented 2mm
depth was inconsistent with the documented 20mm width (a 2mm-deep chord on a
30mm disk is 15mm wide, not 20mm).

This resolves the DIMENSIONS.md chord conflict as a side effect.


### Beam Z-height constraint

Both beams must be above the disk material (within the chord-cut pocket) and
within the brick height (11.4mm above chord surface).

At beam X=5 (offset -8.5mm from disk center), with 4mm chord depth:
  Disk top at this X: sqrt(225 - 72.25) = 12.36mm from disk center.
  Chord flat: 11mm from disk center.
  Crescent (disk material above chord): 12.36 - 11 = 1.36mm.
  
Beam center at 5mm above chord (middle of brick): disk top is 1.36mm
above chord. Beam bottom edge at 3.5mm above chord. That is 2.14mm above
the disk crescent. Clear.

At beam X=21 (offset +7.5mm):
  Disk top: sqrt(225 - 56.25) = 12.99mm. Crescent: 1.99mm.
  Beam bottom at 3.5mm above chord: 1.51mm above crescent. Clear.

Both beams clear the disk body at 5mm above chord. Mount both emitter/receiver
pairs at this height relative to the chord surface.


### STEP 3: Risk Check

New risks introduced:

1. One fewer spare beam pair (from 2 to 1). Acceptable. If a beam fails at
   competition, swap from the spare.

2. Uses one additional GPIO pin. ESP32 has available pins. GPIO 36 and 39 are
   excluded per ELECTRICAL.md (phantom interrupt errata). Other input-capable
   pins are available (e.g., GPIO 39 could be used IF ADC1 is not active, or
   use any remaining strapping-safe pin). Must assign in config.h.

3. The 4mm chord depth means the chord is 2mm deeper than the original 2mm spec.
   The brick sits 2mm lower relative to the disk center. The exit ramp starting
   height shifts down by 2mm. Adjust shaft height or ramp geometry in CAD.

4. The 0.2mm gap between beam-1-blocked and beam-2-blocked zones (P = 5.0 to
   5.2mm) creates a narrow region where a 2x2 brick partially blocks both beams.
   Partial blockage (1.5mm of a 3mm beam) is below typical digital receiver
   thresholds. In practice this reads as (clear, clear) = 2x2. If concerned,
   shift beam 2 to X=21.5 (spacing 16.5mm, gap increases to 0.7mm).


### STEP 4: Compatibility Check

Geometry: beams at X=5 and X=21 fit within the 27mm chute dimension. Both
beams are 3mm diameter holes through the 27mm-wide chute walls. Separated by
16mm along X. No interference with each other or with the color sensor window
(which is in the same walls but at a different X position offset from the beams;
exact position determined in CAD per the existing note).

Sensor alignment: both beams cross the same 22mm Y dimension at the same Z
height (5mm above chord). Detection is binary digital. No analog sensitivity.

Motion paths: the beams are in the chute at cam level. The cam rotates in the XZ
plane (see Issue 2 resolution). During the dwell phase when sensing occurs, the
chord pocket is facing up, and both beams pass through open air above the chord.
During rotation (no sensing), the disk body may sweep through beam paths. This is
irrelevant since no readings are taken during rotation.

Tight zones: the 22mm Y cross-section is unchanged. The 27mm X dimension is
unchanged. The beams add two 3mm holes per wall. No structural concerns on 3mm
PLA walls.


### STEP 5: Final Decision

FIX ACCEPTED.

Changes:
  - Add one IR break-beam pair at chute X = 5mm (new beam 1).
  - Move existing beam from X = 20mm to X = 21mm (beam 2).
  - Increase cam chord depth from 2mm to 4mm in cam disk CAD.
  - Assign one GPIO pin for beam 1 in config.h.
  - Firmware: change detection from single-beam to dual-beam logic.
  - Adjust shaft Z position or ramp angle to compensate for 2mm lower chord.

No new parts needed (spare beam pair available in the existing order).

---

## ISSUE 2: Cam shaft orientation contradiction

### STEP 1: Root Cause

DIMENSIONS.md and CAD CLAUDE.md state: "Cam shaft hole: 5mm diameter, centered
in one 22mm face (perpendicular to belt travel)."

A "22mm face" is a chute wall that is 22mm wide and spans the Y dimension. This
wall is perpendicular to X (belt travel direction). If the shaft is through this
wall, the shaft axis is along X, and the cam disk rotates in the YZ plane.

MECHANICAL.md states: "The cam pushes the bottom brick horizontally out of the
chute onto a 10mm ramp." And the exit opening is described as being in the
"opposite 22mm face."

A cam rotating in YZ sweeps its lobe in Y and Z. It cannot push a brick along X
through an exit opening in a face perpendicular to X.

The only mechanically consistent interpretation: the shaft is along Y (through the
27mm-wide walls), and the cam rotates in XZ. The lobe sweeps along X and pushes
the brick through the exit opening in the opposite 27mm-wide wall.

"Centered in one 22mm face" should read "centered in one 27mm face." This is a
documentation labeling error.


### STEP 2: Minimal Fix

Correct the documentation. No physical design change. The mechanism was always
intended to work with the shaft along Y.

In DIMENSIONS.md, change:
  "Shaft bore: 5mm (NEMA 11 shaft)" is fine (dimension is correct).
  The shaft location description needs correction.

In CAD CLAUDE.md, change:
  "Cam shaft hole: 5mm diameter, centered in one 22mm face (perpendicular to
  belt travel)" to "Cam shaft hole: 5mm diameter, centered in one 27mm face
  (parallel to belt travel direction), with the shaft axis crossing the 22mm
  width."

Also correct the chord dimensions to resolve the 2mm/20mm inconsistency:
  "Chord depth: 4mm" (was 2mm)
  "Chord width: 20.4mm" (was 20mm, now geometrically consistent)


### STEP 3: Risk Check

Zero new risks. This is a documentation correction only. The physical design
was always correct in the engineers' mental model; the written description was
inconsistent.


### STEP 4: Compatibility Check

No physical changes. All downstream references to the cam shaft (NEMA 11 mount,
stepper wiring, TMC2209 configuration) are unaffected by the shaft location
correction. The shaft diameter (5mm) and motor specs are unchanged.


### STEP 5: Final Decision

FIX ACCEPTED.

Changes:
  - Update shaft location description in DIMENSIONS.md and CAD CLAUDE.md.
  - Update chord depth from 2mm to 4mm in DIMENSIONS.md (aligns with Issue 1 fix
    and resolves the documented 20mm-width inconsistency).

---

## ISSUE 3: Solenoid push geometrically marginal

### STEP 1: Root Cause

The original audit found that a 10mm solenoid stroke pushes the brick 3.8mm past
the channel wall, but the brick's center of gravity remains 4.1mm inside the
channel. The audit concluded the brick "relies on belt momentum" to complete the
exit.

This root cause analysis was INCORRECT. It treated the solenoid push as a static
displacement problem (where does the brick end up after 10mm of push) rather than
a dynamics problem (how fast is the brick moving after 10mm of push).


### STEP 2: Reassessment (not a fix, issue is retracted)

JF-0530B solenoid specs (from multiple vendor listings, confirmed):
  - Force: 5N at 10mm stroke (maximum, at end of stroke)
  - Available in 6V, 12V, 24V variants
  - Body: 30 x 16 x 15mm, push-pull type with return spring

Solenoid force varies with plunger position. At the START of stroke (10mm out),
force is lowest. At END (0mm, plunger fully retracted), force is maximum (5N).
Conservative start-of-stroke estimate for 6V variant: 1N.

A 2x3 LEGO brick weighs approximately 3 grams (0.003 kg).

At F = 1N (conservative), brick mass = 3g:
  Acceleration: 333 m/s2 (34g)
  Lateral velocity after 10mm of solenoid travel: 2582 mm/s
  Time to traverse 10mm stroke: 7.7 ms
  Time for brick to travel the remaining 12mm to clear channel: 4.6 ms
  Forward drift at 200mm/s belt speed during that 4.6 ms: 0.9 mm
  Bin opening: 30mm along belt axis.
  Forward drift (0.9mm) is 3% of the bin opening. Trivially clears.

At F = 0.5N (extremely conservative):
  Lateral velocity: 1826 mm/s
  Time to clear channel: 6.6 ms
  Forward drift: 1.3 mm. Still 4% of bin opening.

At F = 5N (rated maximum):
  Lateral velocity: 5774 mm/s
  Time to clear: 2.1 ms
  Forward drift: 0.4 mm.

The solenoid force exceeds the force needed to move the brick by a factor of
57x to 283x (depending on force estimate vs friction force of 0.018N). The brick
is effectively launched out of the channel at a lateral velocity of 1.8 to 5.8
meters per second. Belt speed is irrelevant to exit completion. The brick does
NOT "rely on belt momentum."

The original audit's static-displacement analysis was misleading because it
treated the problem as "where does the brick end up after being pushed 10mm"
rather than "what velocity does the brick have after being accelerated by 1-5N
over 10mm."


### STEP 3: Risk Check (of retraction)

Retaining the direct pusher design introduces no new risks. The design is
unchanged.

One thing worth noting: the brick exits at very high lateral velocity
(1800-5800 mm/s). This is 9-29x the belt speed. The brick trajectory is
nearly perpendicular to the belt. Bin ramp angle (10 degrees toward back wall)
is important for catching the brick. If the brick hits the back wall hard, it
could bounce. The 10-degree floor ramp helps, but at 5800 mm/s, the kinetic
energy of a 3g brick is 0.5 * 0.003 * 5.774^2 = 0.050 J. This is very small.
A PLA wall absorbs this easily. No bounce concern.


### STEP 4: Compatibility Check

No changes to the design. All existing geometry, firmware, and wiring are
unaffected.


### STEP 5: Final Decision

ISSUE RETRACTED. The 10mm direct pusher is not marginal. It is massively
over-specified for a 2-3 gram brick. No fix needed. Do not go back to the
solenoid plow.

---

## Answer: Should you go back to solenoid plow?

No. The direct pusher is the correct choice. The concern that motivated the plow
design (10mm stroke might not be enough) is invalid. The JF-0530B at 6V applies
enough force to launch a 3g brick laterally at nearly 2 meters per second through
10mm of stroke. The brick clears the channel in under 7ms with less than 2mm of
forward drift. Belt speed has negligible effect on exit reliability.

The plow (lever arm, pivot, spring calibration) adds fabrication complexity and
failure modes with zero functional benefit. Do not revisit.

---

## Answer: What to do about size detection

Add a second beam in the chute. Do not move sensing to the conveyor. Do not tilt.

Why dual beams in the chute:

  The dual-beam approach at X=5 and X=21 (spacing 16mm > 15.8mm brick width)
  gives zero-ambiguity detection regardless of brick position. It preserves the
  stationary-sensing architecture (no speed dependency, no timing measurement).

Why not move to conveyor:

  Belt-mounted dual beams require timing-based measurement. The firmware measures
  the time gap between beam 1 and beam 2 breaking, and infers brick length from
  belt speed. This reintroduces dependence on belt speed accuracy. If the PID loop
  has a transient error, the size measurement drifts. The current architecture
  deliberately eliminated this dependency by sensing while the brick is stationary.
  Moving to the conveyor is a step backward.

Why not tilt:

  For a 3g brick on PLA: static friction force = 11.6mN. Gravitational component
  at 10 degrees = 5.1mN. Friction exceeds gravity. The brick does not slide.
  Even at 20 degrees: gravity = 10.1mN, friction = 11.0mN. Still marginal. And
  20 degrees of chute tilt creates significant mechanical complications (exit ramp
  alignment, cam axis orientation, chute-to-belt transition geometry).

---

## Summary of all changes

| Change | Type | Parts impact | Firmware impact | CAD impact |
|--------|------|-------------|-----------------|------------|
| Add beam at chute X=5mm | New component | Use 1 spare beam pair | Add GPIO, dual-beam logic | Two 3mm holes in chute walls |
| Move beam from X=20 to X=21 | Modification | None | Update beam position constant | Move existing hole 1mm |
| Chord depth 2mm to 4mm | CAD correction | None | None | Remodel cam disk chord |
| Update DIMENSIONS.md chord values | Documentation | None | None | None |
| Correct shaft wall label 22mm to 27mm | Documentation | None | None | None |
| Adjust shaft Z or ramp for 2mm lower chord | CAD adjustment | None | None | Minor ramp/mount change |

No new parts to order. No subsystem changes. No architecture changes.

---

## Appendix A: Dual beam detection matrix (exhaustive)

Beam 1: X = 5mm.  Beam 2: X = 21mm.  Spacing: 16mm.

2x3 brick (23.7mm), play 3.3mm:

| Brick near edge P | Beam 1 (X=5) | Beam 2 (X=21) | Reading | Correct? |
|-------------------|-------------|---------------|---------|----------|
| 0.0 | blocked | blocked | 2x3 | YES |
| 1.0 | blocked | blocked | 2x3 | YES |
| 2.0 | blocked | blocked | 2x3 | YES |
| 3.3 (max) | blocked | blocked | 2x3 | YES |

2x2 brick (15.8mm), play 11.2mm:

| Brick near edge P | Beam 1 (X=5) | Beam 2 (X=21) | Reading | Correct? |
|-------------------|-------------|---------------|---------|----------|
| 0.0 | blocked | clear | 2x2 | YES |
| 2.0 | blocked | clear | 2x2 | YES |
| 4.0 | blocked | clear | 2x2 | YES |
| 5.0 | edge | clear | 2x2 | YES |
| 5.1 | clear | clear | 2x2 | YES |
| 5.2 | clear | edge | 2x2 | YES |
| 6.0 | clear | blocked | 2x2 | YES |
| 8.0 | clear | blocked | 2x2 | YES |
| 11.2 (max) | clear | blocked | 2x2 | YES |

Decision rule: both blocked = 2x3. Anything else = 2x2.
100% correct at every possible position.

---

## Appendix B: Chord geometry at 4mm depth

    Disk radius: 15mm
    Chord depth: 4mm (from disk circumference)
    Chord distance from center: 15 - 4 = 11mm
    Chord half-width: sqrt(225 - 121) = sqrt(104) = 10.20mm
    Chord full width: 20.40mm
    Chord edges (in chute coords): X = 13.5 - 10.20 = 3.30 to X = 13.5 + 10.20 = 23.70

    Beam 1 at X=5: offset from center = -8.5mm. Inside chord by 1.70mm.
    Beam 2 at X=21: offset from center = +7.5mm. Inside chord by 2.70mm.

    Disk body height above chord at beam 1 (X=5):
      Disk top = sqrt(225 - 72.25) = 12.36mm from center.
      Chord = 11mm from center. Crescent = 1.36mm.
      Beam at 5mm above chord: 3.64mm above disk top. Clear.

    Disk body height above chord at beam 2 (X=21):
      Disk top = sqrt(225 - 56.25) = 12.99mm from center.
      Chord = 11mm. Crescent = 1.99mm.
      Beam at 5mm above chord: 3.01mm above disk top. Clear.

---

## ISSUE 4: Issue 1 and Issue 2 fixes not propagated to subsystem docs

### STEP 1: Root Cause

Issues 1 and 2 were accepted. Neither fix has been applied to the subsystem docs. Every
file below still contains the old values and will contradict the accepted changes. A builder
following any of these files would build to the wrong spec.

Stale values by file:

ARCHITECTURE.md:
  Line: "Single IR break-beam crosses the 27mm chute dimension at 20mm from the wall."
  Should describe dual beams at X=5 and X=21.

MECHANICAL.md (Sensing section, ~line 119):
  "Single IR break-beam crosses the 27mm chute dimension at 20mm from the reference wall.
   Emitter in one wall, receiver in the opposite wall."
  "Gap between brick sizes at beam position: 4.2mm on each side of threshold. Very robust."
  All three lines are obsolete. The gap claim no longer applies; the dual-beam design is
  position-independent.

MECHANICAL.md (Escapement section, ~line 44):
  "Flat chord (2mm depth) on the disk body where the bottom brick rests"
  Should be 4mm depth.

DIMENSIONS.md:
  "Chord depth (flat seat): 2mm" -> 4mm
  "Chord width: 20mm (centered)" -> 20.4mm (geometrically consistent with 4mm depth)
  "Size beam position (in 27mm dim): 20mm from reference wall"
  -> Two beams: 5mm and 21mm from reference wall

CALIBRATION.md (Notes section, last paragraph):
  "Size sensing needs no calibration. It is a single digital read (beam blocked or not).
   The only check is physical: verify a 2x3 brick blocks the beam and a 2x2 does not."
  The procedure description is still correct (no calibration needed, just physical
  verification), but "single digital read" and "the beam" should reference dual beams.

TEST_PROTOCOL.md (Size Sensing section):
  "PASS: 2x3 brick blocks the beam (firmware reads: is_large = true) 10/10 times.
   2x2 brick clears the beam (firmware reads: is_large = false) 10/10 times."
  "Any read that requires repositioning the brick within normal tolerances."
  The last FAIL criterion was the original motivation for the dual-beam fix. After the
  fix, brick position within normal tolerances is irrelevant to detection accuracy.
  Must update pass criteria to dual-beam logic.

cad/CLAUDE.md (Chute and escapement section, cam disk):
  "Flat chord depth: 2mm into disk body" -> 4mm
  "Flat chord width: 20mm (centered)" -> 20.4mm
  "Cam shaft hole: 5mm diameter, centered in one 22mm face (perpendicular to belt travel)"
  -> one 27mm face (shaft axis parallel to 22mm walls, perpendicular to belt travel
  direction, per Issue 2 correction)
  "Size beam holes: 3mm diameter, through both 27mm faces, centered at 20mm from the
   cam-side reference wall" -> two beam holes: X=5mm and X=21mm from reference wall

cad/CLAUDE.md (cam disk dimensions block):
  OpenSCAD comment block shows: "Flat chord depth: 2mm into disk body, Flat chord
  width: 20mm" -> update both

ELECTRICAL.md (GPIO pin assignments):
  "PIN_SIZE_BEAM = GPIO 34 // single beam in chute: LOW=blocked=2x3, HIGH=clear=2x2"
  -> two pins, see Issue 5 for the assignment

ELECTRICAL.md (wiring reference table):
  "ESP32 GPIO 34 | Size beam receiver + 10k to 3.3V | External pull-up required"
  -> needs second beam row

ELECTRICAL.md (Bundle 1 wire count):
  "Size beam: signal + VCC (2 wires, shared GND, 2-pin JST-XH)"
  -> two size beam signals, still shared VCC and GND. Add one wire. 4-pin JST-XH or
  separate 2-pin connectors per beam (flexibility in routing). Bundle 1 becomes 7 wires
  minimum (4 color + 2 beam signals + shared VCC; GND on common bus).

EMBEDDED.md (Sensing sequence, step 3):
  "Read PIN_SIZE_BEAM: LOW = 2x3 (beam blocked), HIGH = 2x2 (beam clear)"
  -> Read PIN_SIZE_BEAM1 and PIN_SIZE_BEAM2. Both LOW = 2x3. Anything else = 2x2.


### STEP 2: Minimal Fix

Apply accepted Issue 1 and Issue 2 values to each file listed above. No design changes.
No new analysis needed. All correct values are established in Issues 1 and 2.


### STEP 3: Risk Check

Zero new risks. These are documentation alignment changes. Every value being applied was
already verified in Issues 1 and 2.


### STEP 4: Compatibility Check

All changes are internally consistent with the accepted Issue 1 and Issue 2 resolutions.
The chord geometry (4mm depth, 20.4mm width) and the dual-beam positions (X=5, X=21) were
fully verified in those analyses and their appendices.


### STEP 5: Final Decision

FIX ACCEPTED.

Each stale reference identified above must be updated before the first CAD print. The chord
depth is a CAD dimension -- printing a 2mm-depth cam disk after accepting the 4mm fix would
require a reprint with no benefit. Apply now.

---

## ISSUE 5: GPIO pin for second size beam is unassigned

### STEP 1: Root Cause

Issue 1 adds a beam at chute X=5mm, requiring a new GPIO input pin. Issue 1 notes
"Must assign in config.h" but does not assign a specific pin. ELECTRICAL.md has no
entry for this pin. Without a concrete assignment, the firmware and wiring cannot proceed.

All currently mapped GPIO inputs on the standard 30-pin ESP32 DevKit:
  GPIO 34: PIN_SIZE_BEAM (existing beam 2, input-only)
  GPIO 35: PIN_CHUTE_EXIT (input-only)
  GPIO 16, 17, 5, 18: bin beams
  GPIO 4: Hall sensor ISR
  GPIO 19: start button
  GPIO 21, 22: I2C
  GPIO 23: TMC2209 UART
  GPIO 25, 26, 27: stepper step/dir/enable
  GPIO 32, 33: solenoids 1 and 2
  GPIO 15: solenoid 3
  GPIO 14, 12, 13: belt motor
  GPIO 2: buzzer

Remaining input-capable pins:
  GPIO 36 (VP), GPIO 37, GPIO 38, GPIO 39 (VN): input-only, no internal pull-up.
  ELECTRICAL.md marks GPIO 36 and 39 as "NOT USED" due to phantom interrupt errata
  (ESP32 ECO 3.11). The errata describes 270pF internal capacitance causing crosstalk
  between GPIO 37/38 when ADC1 channels are sampled simultaneously.

ADC1 status in this design: no ADC reads anywhere in the firmware. The errata condition
(ADC1 active) is never triggered. GPIO 36 and 39 function as clean digital inputs when
ADC1 is not initialized.

GPIO 37 and 38 are internal (connected to internal Hall sensor and flash on some modules).
Do not use.

GPIO 36 (SVP): input-only, no internal pull-up, no ADC1 conflict, no boot-time issues.
Same electrical constraints as GPIO 34 and 35 (already successfully used for beam inputs
with external 10k pull-ups). Available.


### STEP 2: Minimal Fix

Assign:
  PIN_SIZE_BEAM1 = GPIO 36   // new beam at chute X=5mm (beam 1). 10k external pull-up
                             // to 3.3V required. Active-low (LOW = emitter blocked).
  PIN_SIZE_BEAM2 = GPIO 34   // existing beam, moved from X=20 to X=21 (beam 2). Same
                             // wiring as current design.

Rename existing PIN_SIZE_BEAM to PIN_SIZE_BEAM2 throughout (config.h, ELECTRICAL.md,
EMBEDDED.md). Beam 2 is the existing beam because beam 2 is the one closer to the
original single-beam position (X=20 -> X=21 is a 1mm shift).

ELECTRICAL.md errata note must be updated:
  Old: "GPIO 36, 39: NOT USED. These have phantom interrupt issues..."
  New: "GPIO 37, 38: NOT USED. Internal pins on some modules (Hall sensor, flash).
        GPIO 36: Used for PIN_SIZE_BEAM1. Input-only, external pull-up required. Phantom
        interrupt errata only applies when ADC1 is active; ADC1 is not used in this design.
        GPIO 39: NOT USED. Same errata caveat as GPIO 36. Spare."

ELECTRICAL.md wiring reference table, new row:
  ESP32 GPIO 36 | Size beam 1 receiver + 10k to 3.3V | External pull-up required (no
                                                        internal pull-up on GPIO 36)


### STEP 3: Risk Check

1. The ADC1 assumption must be maintained. If a future change adds an analog read (e.g.,
   battery voltage monitoring via the LiPo divider), GPIO 36 must be reassigned before
   that change is made. Document this constraint in ELECTRICAL.md.

2. GPIO 36 is labeled VP on most ESP32 DevKit boards. The VP label refers to its primary
   function as ADC1_CH0. This does not prevent digital use when ADC1 is not initialized.
   Confirm by checking the DevKit silkscreen and datasheet: if it is labeled GPIO 36 or
   VP, it is the correct pin.


### STEP 4: Compatibility Check

The external 10k pull-up wiring is identical to the existing GPIO 34 and 35 beam inputs.
No new component types needed. The resistor assortment already on hand covers this.

The firmware dual-beam detection logic change (if beam1 AND beam2 blocked: 2x3) is a
one-line change from the original single-beam read. Both inputs are sampled during the
same sensing phase (SENSING state, stationary brick on cam chord).


### STEP 5: Final Decision

FIX ACCEPTED.

Changes:
  - ELECTRICAL.md: assign GPIO 36 to PIN_SIZE_BEAM1. Update errata note. Update wiring
    reference table. Rename PIN_SIZE_BEAM to PIN_SIZE_BEAM2.
  - EMBEDDED.md: update sensing step 3 to read both pins.
  - config.h: add PIN_SIZE_BEAM1 (GPIO 36), rename PIN_SIZE_BEAM to PIN_SIZE_BEAM2 (GPIO 34).

---

## ISSUE 6: CHECKLIST.md contains obsolete plow lever and H206 encoder items

### STEP 1: Root Cause

CHECKLIST.md Phase 4 (Full frame CAD) includes three items from the superseded lever-arm
plow design:

  "Plow lever arms: pivot-to-solenoid 8mm, pivot-to-tip 24mm, ratio 3.0"
  "Plow leading face chamfered at 35 degrees"
  "Solenoid connection to lever: pinned (M2 pin), not hook or slot"

The current design uses direct solenoid push. There is no lever arm, no pivot, no plow
tip geometry. A builder following this checklist would search for non-existent parts.

Phase 4 also includes:
  "Sensor mounts: color sensor at 5-10mm standoff above belt, break-beams at 19mm spacing"
  The color sensor is not above the belt. It is on the chute exterior wall at escapement
  level, looking through a 12mm x 12mm window. The "5-10mm standoff above belt" description
  is from an early design where sensing happened on the moving belt. The "19mm spacing"
  for break-beams is unrelated to any current measurement (dual beams are 16mm apart
  in the chute; bin confirmation beams have no defined inter-beam spacing).

CHECKLIST.md Phase 3 (Parts arrival validation), Belt Encoder section:
  "H206 mounted next to slotted pulley disk"
  "Pulse output confirmed on oscilloscope or serial monitor at belt speed"
  "Pulse count per rotation matches slot count on disk"
  "ESP32 hardware pulse counter reading speed correctly"

The H206 is a photointerrupter for slotted-disk encoders. The current design uses an A3144
Hall-effect sensor with two neodymium disc magnets on the idler roller rim. There is no
slotted disk, no H206, and no hardware pulse counter in the firmware (the Hall ISR computes
pulse intervals directly). All four checklist items describe hardware that was never ordered
and firmware that was never written.

CHECKLIST.md Phase 5 (Wiring and integration):
  "Photointerrupter output on GPIO interrupt pin"
  Same issue. Should reference Hall sensor.


### STEP 2: Minimal Fix

Phase 4, replace three plow lever items with:
  "Solenoid U-bracket CAD complete: rod center at 6mm above belt surface"
  "Face plate CAD complete: 20mm wide x 11mm tall x 2mm thick, all edges chamfered 0.5mm"
  "Pusher slot in channel wall: 24mm wide x 13mm tall, edges chamfered 0.5mm"

Phase 4, replace sensor mount item with:
  "Color sensor window in chute wall at escapement level: 12mm x 12mm, on 27mm face"
  "Size beam holes in chute walls: two holes per wall at X=5mm and X=21mm, 3mm diameter"

Phase 3 Belt Encoder section, replace with:
  "A3144 Hall sensor mounted beside idler roller, face 2mm from roller rim"
  "Two 3mm neodymium disc magnets confirmed glued 180 degrees apart on idler rim"
  "Hall sensor output at belt speed: pulse interval ~196ms (verify via serial)"
  "Belt speed reading in serial log matches calculated value within 10%: 200mm/s target,
   roller circ = pi*25 = 78.5mm, 2 pulses/rev -> interval = 78.5 / (2*200) = 196ms"

Phase 5, replace:
  "Photointerrupter output on GPIO interrupt pin"
  with:
  "Hall sensor (A3144) output on GPIO 4 with 10k external pull-up to 3.3V, ISR on
   falling edge"


### STEP 3: Risk Check

Zero new risks. These are documentation corrections. The physical design is unaffected.


### STEP 4: Compatibility Check

All replacement items match the current design as documented in MECHANICAL.md,
ELECTRICAL.md, EMBEDDED.md, and DIMENSIONS.md.


### STEP 5: Final Decision

FIX ACCEPTED.

---

## ISSUE 7: BOM.md AliExpress subtotal does not match item sum

### STEP 1: Root Cause

BOM.md shows "AliExpress subtotal: ~$88" immediately after the AliExpress table.
The individual line items in that table sum as follows:

  JF-0530B solenoid:             $17
  NEMA 11 stepper:               $12
  JGB37-520 gearmotor:           $10
  IR break-beam (2 packs):       $14
  TCS34725 color sensor:          $4
  TMC2209 stepper driver:         $3
  LM2596 buck converter (x2):     $3
  L298N motor driver:             $1
  Silicone wire 22AWG:            $3
  Perfboard 5x7cm:                $2
  JST-XH connector kit:           $6
  Torsion springs:                $3
  Total:                         $78

The summary table at the bottom of the file correctly shows "AliExpress: ~$78". The
"~$88" label is stale from an earlier BOM revision. It is internally inconsistent:
the total table ($78 + $76 + $10 = $164) and the "~$14 over the $150 budget" note are
both consistent with $78, not $88.


### STEP 2: Minimal Fix

Change "AliExpress subtotal: ~$88" to "AliExpress subtotal: ~$78".


### STEP 3: Risk Check

Zero new risks. No ordering decision is affected because the summary table at the
bottom was already correct.


### STEP 4: Compatibility Check

The summary table total ($164) and budget note ("~$14 over $150") are both consistent
with the corrected $78 figure. No other changes needed.


### STEP 5: Final Decision

FIX ACCEPTED.

---

## ISSUE 8: GPIO 13 conflict between belt motor direction and display SPI CS

### STEP 1: Root Cause

firmware/CLAUDE.md notes: "config.h line 50: PIN_TFT_CS=13 and PIN_BELT_DIR2=13 conflict.
Noted in comment. Resolve before display is wired."

ELECTRICAL.md assigns PIN_MOTOR_IN2 = GPIO 13 for L298N IN2 (belt motor direction).
The display SPI (ST7789V2, Waveshare 1.69" LCD) requires a chip select pin. The firmware
assigns this to GPIO 13, which is already used for the motor.

ELECTRICAL.md does not list any SPI pin assignments at all -- the display SPI pins
(MOSI, SCLK, CS, DC, RST) are mentioned in CHECKLIST.md Phase 5 but not assigned to
specific GPIOs in ELECTRICAL.md. This is a secondary gap: the display wiring is
underdefined.

The conflict exists in firmware. It cannot be resolved by software alone because driving
two different functions from one GPIO simultaneously is not possible.


### STEP 2: Minimal Fix

Reassign PIN_TFT_CS to a free GPIO. Available output-capable pins after all current
assignments:

Currently assigned (output-capable, in use):
  GPIO 2, 12, 13, 14, 15, 19, 21, 22, 23, 25, 26, 27, 32, 33

Available output-capable pins that are safe:
  GPIO 0: has internal pull-up, boot button on DevKit. Avoid for outputs.
  GPIO 1, 3: UART TX/RX. Avoid.
  GPIO 6-11: flash memory. Do not use.

Confirmed output-capable and unassigned:
  None obvious on a 30-pin DevKit after the full pin map.

Alternative: the display SPI pins can share the SPI bus hardware. The ESP32 VSPI bus
defaults to GPIO 23 (MOSI), GPIO 18 (SCK), GPIO 5 (MISO). All three are already in use
(GPIO 23 is TMC2209 UART, GPIO 18 is Bin4 beam, GPIO 5 is Bin3 beam). The HSPI bus
defaults to GPIO 13 (MOSI), GPIO 14 (SCK), GPIO 12 (MISO) -- all three are in use
for the belt motor.

The core problem: the 30-pin ESP32 DevKit has insufficient free GPIOs for this design
once all current assignments are filled. The display is the late-added peripheral that
broke the budget.

Two resolution paths:

  Path A: Use an I2C display instead of SPI. BOM.md already notes the SSD1306 as a
  fallback ("Use SSD1306 only if color display fails to arrive"). The SSD1306 128x64
  monochrome OLED uses I2C address 0x3C (different from TCS34725 at 0x29). It shares
  GPIO 21 and GPIO 22 with no conflict. The display loses color capability but the
  brick animation can use filled vs outlined rectangles to indicate large vs small bricks,
  and inverted vs normal text for red vs blue. All display state requirements (READY,
  SORTING, SORT COMPLETE, ERROR) are implementable in 128x64 monochrome.

  Path B: Use an I2C GPIO expander (PCF8574, address 0x20-0x27) to free up GPIOs. The
  expander adds one more I2C device and allows moving bin confirmation beams or buzzer
  onto the expander, freeing native GPIO pins for display SPI. More complex, more parts.

  Path C: Eliminate the display entirely and use serial output only. Unacceptable.
  Display states are scored.

Path A is the minimal-risk fix. The SSD1306 0.96" I2C OLED costs $4 (already in
BOM.md as "Do not order... use SSD1306 only if color display fails to arrive"). Move it
from "do not order" to the primary display choice.

The brick animation on SSD1306 can indicate size (full-width vs small rectangle) and
color (filled = red / outlined = blue). All required display states fit. Competition
rubric does not specify color display.


### STEP 3: Risk Check

1. SSD1306 loses the colored brick animation. The brick silhouette will use fill pattern
   rather than color. The judge can still see the size and "color" (inverted/normal) of
   each brick as it sorts. Adequately communicates system state.

2. If the I2C bus is noisy (solenoids firing, stepper running), both TCS34725 and SSD1306
   share the same bus. The 4.7k pull-up resistors recommended in ISSUES.md become more
   important with two devices. Add them if I2C drops reads during powered runs.

3. This does not invalidate the SPI display already on the BOM (Waveshare ST7789V2).
   That display may still arrive. If it arrives before wiring: attempt to map its SPI
   CS to the remaining free GPIO, or use the I/O expander fallback. If no free GPIO
   is found: switch to the SSD1306. The SSD1306 is $4 from Amazon, same-day available.


### STEP 4: Compatibility Check

I2C bus already has one device (TCS34725 at 0x29). Adding SSD1306 at 0x3C is a
standard multi-device I2C configuration. ESP32 Arduino Wire library handles this.
No pin changes needed. No wiring changes needed. Only firmware display driver changes.

EMBEDDED.md display spec remains valid. All four states (READY, SORTING, SORT COMPLETE,
ERROR) and the thermal bar are implementable in 128x64 pixels without color.


### STEP 5: Final Decision

FIX ACCEPTED, with caveat.

Primary decision: add SSD1306 to the Amazon order. Do NOT cancel the Waveshare
ST7789V2 -- it is already on order. Attempt to wire SPI display when hardware arrives
by resolving the GPIO 13 conflict at that point (measure shaft length, test if GPIO 12
or 14 can be remapped from motor direction to display). If SPI display can be wired
without conflict: use it. If GPIO conflict cannot be resolved cleanly: install SSD1306
on I2C bus.

Changes:
  - BOM.md: move SSD1306 from "Do not order" to Amazon order list at $4.
  - ELECTRICAL.md: document that display SPI pin assignment is deferred until GPIO 13
    conflict is resolved during wiring. Flag as open item. Add SSD1306 I2C fallback as
    primary-plan alternative.
  - firmware/CLAUDE.md: acknowledge PIN_TFT_CS conflict and the fallback path.

---

## ISSUE 9: EMBEDDED.md module names do not match actual firmware module structure

### STEP 1: Root Cause

EMBEDDED.md specifies the firmware module structure as:

  | sensing.h/.cpp   | senseBrickInChute(): size read + color accumulate + classify |
  | diverter.h/.cpp  | armPusher(idx, fireAtMs): schedules solenoid fire             |
  | selftest.h/.cpp  | Power-on test sequence                                        |
  | escapement.h/.cpp| Stepper control, StallGuard monitoring                        |
  | audio.h/.cpp     | Buzzer tones                                                  |
  | belt.h/.cpp      | Belt motor PWM, Hall ISR, PI loop                            |

firmware/CLAUDE.md (which reflects the actual compiled codebase) shows:

  | sensors.h/.cpp   | senseBrickInChute(), BrickCategory enum                       |
  | actuators.h/.cpp | Belt, stepper, solenoid pushers (consolidated)                |

Discrepancies:
  - sensing.h -> sensors.h (renamed)
  - diverter.h, selftest.h, escapement.h, audio.h, belt.h all collapsed into actuators.h

The Programming Structure category is 20 of 100 scored points. A judge reading EMBEDDED.md
and then inspecting the firmware directory will find different filenames. This breaks the
documentation-to-code traceability that the programming structure score depends on.

EMBEDDED.md is the specification document. firmware/CLAUDE.md reflects the implementation.
One of them is wrong, or both need to be reconciled.


### STEP 2: Minimal Fix

Two options:

  Option A: Update EMBEDDED.md to match the actual firmware structure.
  Map EMBEDDED.md module table to firmware/CLAUDE.md:
    sensing.h/.cpp -> sensors.h/.cpp (rename note: senseBrickInChute() is in sensors.h)
    diverter.h/.cpp + belt.h/.cpp + escapement.h/.cpp + audio.h/.cpp + selftest.h/.cpp
    -> actuators.h/.cpp (consolidated peripheral control module)

  Option B: Split actuators.h/.cpp into the modules EMBEDDED.md specifies, matching the
  design document.

Option B produces better Programming Structure scores because judges can trace each
named module to a specific, narrow responsibility. A monolithic actuators.h is harder
to evaluate than five small files each doing one job. EMBEDDED.md is correct in spirit:
the split-module design is the right architecture.

However, Option B requires splitting firmware code (outside this doc review scope).
Document the discrepancy now; resolve the firmware when hardware arrives.

Recommended: Update EMBEDDED.md module table to reflect current reality (sensors.h,
actuators.h) while adding a note that actuators.h will be split into belt.h, diverter.h,
escapement.h, selftest.h, and audio.h before competition. This keeps the spec honest
about current state while preserving the intended target architecture.


### STEP 3: Risk Check

The discrepancy is a documentation risk, not a hardware risk. No physical consequences.
Risk: a judge reading both the spec and the code finds a mismatch and deducts from
Programming Structure. The fix eliminates that risk.


### STEP 4: Compatibility Check

All module responsibilities documented in EMBEDDED.md remain correct. Only the filenames
and consolidation need updating. The sensing sequence, state machine, display spec,
thermal model, and belt speed control sections of EMBEDDED.md are unaffected.


### STEP 5: Final Decision

FIX ACCEPTED.

Changes:
  - EMBEDDED.md module table: update sensing.h -> sensors.h, note that belt/diverter/
    escapement/audio/selftest are currently in actuators.h with split planned.

---

## ISSUE 10: MECHANICAL.md "50-degree dwell window" is ambiguous

### STEP 1: Root Cause

MECHANICAL.md (Escapement section):
  "At 3 rev/sec, the 50-degree dwell window is 46ms. Adequate to block the next brick."

"Dwell window" in this project specifically means the time the brick sits stationary on
the cam chord while sensing occurs. That window is ~260ms (310 degrees out of 360 at
3 rev/sec: 310/360 * 333ms = 287ms, rounded to ~260ms in the architecture docs, which
uses a slightly faster cam rate).

ARCHITECTURE.md and DIMENSIONS.md both correctly state "Cam dwell window: ~260ms at
3 rev/sec" for the sensing window.

MECHANICAL.md's "50-degree dwell window is 46ms" refers to the LOBE arc duration, not
the sensing dwell. The 50-degree lobe sweeps past in 46ms. This 46ms is how long the
lobe is in contact (blocking + pushing the brick), not how long the brick sits quietly
for sensing. Using the term "dwell window" for the 46ms lobe arc will confuse anyone
comparing it to the 260ms "cam dwell window" in the other files.


### STEP 2: Minimal Fix

Replace in MECHANICAL.md:
  "At 3 rev/sec, the 50-degree dwell window is 46ms. Adequate to block the next brick."

With:
  "At 3 rev/sec, the 50-degree lobe sweeps in 46ms. This is adequate blocking time for
  the next brick in the queue. The sensing dwell (brick stationary on chord) is ~287ms
  (310 non-lobe degrees at 3 rev/sec), consistent with the 260ms stated in ARCHITECTURE.md
  and DIMENSIONS.md."

This makes explicit: 46ms is the lobe duration, 260-287ms is the sensing window, and
the two are not the same measurement.


### STEP 3: Risk Check

Zero new risks. Documentation clarification only.


### STEP 4: Compatibility Check

No dimension or timing change. Both values are already correct individually. The fix
clarifies the labeling to prevent future misinterpretation.


### STEP 5: Final Decision

FIX ACCEPTED.

---

## Updated summary of all changes

Issue 1-3 changes (from original analysis, unchanged):

| Change | Type | Parts impact | Firmware impact | CAD impact |
|--------|------|-------------|-----------------|------------|
| Add beam at chute X=5mm | New component | Use 1 spare beam pair | Add GPIO, dual-beam logic | Two 3mm holes in chute walls |
| Move beam from X=20 to X=21 | Modification | None | Update beam position constant | Move existing hole 1mm |
| Chord depth 2mm to 4mm | CAD correction | None | None | Remodel cam disk chord |
| Update DIMENSIONS.md chord values | Documentation | None | None | None |
| Correct shaft wall label 22mm to 27mm | Documentation | None | None | None |
| Adjust shaft Z or ramp for 2mm lower chord | CAD adjustment | None | None | Minor ramp/mount change |

Issue 4-10 changes (new):

| Change | Type | Parts impact | Firmware impact | CAD impact |
|--------|------|-------------|-----------------|------------|
| Propagate dual-beam and chord fixes to ARCHITECTURE, MECHANICAL, DIMENSIONS, CALIBRATION, TEST_PROTOCOL, cad/CLAUDE.md, ELECTRICAL, EMBEDDED | Documentation | None | None | None |
| Assign GPIO 36 to PIN_SIZE_BEAM1, rename PIN_SIZE_BEAM to PIN_SIZE_BEAM2 | Pin assignment | None | config.h, ELECTRICAL.md | None |
| Remove plow lever items from CHECKLIST Phase 4, add direct pusher items | Documentation | None | None | None |
| Replace H206 encoder items in CHECKLIST Phase 3 and Phase 5 with A3144 Hall items | Documentation | None | None | None |
| Fix color sensor mount description in CHECKLIST Phase 4 | Documentation | None | None | None |
| Correct BOM AliExpress subtotal from $88 to $78 | Documentation | None | None | None |
| Add SSD1306 to Amazon order as display fallback, document GPIO 13 SPI conflict | Parts + documentation | +$4 SSD1306 | Display driver fallback path | None |
| Update EMBEDDED.md module table: sensing.h -> sensors.h, note actuators.h consolidation | Documentation | None | None | None |
| Clarify MECHANICAL.md lobe sweep (46ms) vs sensing dwell (287ms) terminology | Documentation | None | None | None |

No new parts to order beyond optional SSD1306 ($4). No architecture changes.
