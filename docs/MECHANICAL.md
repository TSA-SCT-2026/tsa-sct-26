# Mechanical Design

All dimensions are in docs/DIMENSIONS.md. This document covers design rationale,
construction method, and assembly notes.

## Brick dimensions (source of truth)

| Brick | Width (along belt) | Depth (across belt) | Height | Count |
|-------|-------------------|---------------------|--------|-------|
| 2x2 | 15.8mm | 15.8mm | 11.4mm | 12 (6 red, 6 blue) |
| 2x3 | 23.7mm | 15.8mm | 11.4mm | 12 (4 red, 8 blue) |

---

## Chute

Vertical rectangular tube, 22mm x 27mm internal, 310mm tall.
All 24 bricks load before the run starts. Gravity feeds the queue.

**Why 22mm x 27mm:** The 22mm dimension is the channel width (same as the belt channel).
Bricks travel studs-up, length along the 27mm axis. The 27mm gives 1.65mm clearance per
side for the 23.7mm brick face. PVC pipe is not a substitute: it cannot maintain brick
orientation through the escapement.

Top is flared to 35mm x 40mm over 20mm height for easy loading. A 3D-printed guide at the
flare entrance shows the correct brick orientation (studs up, length along travel axis).

**Construction:** 3mm PLA, printed as rectangular tube. If printer bed is too small for
310mm vertical: print in two halves with a tongue-and-groove joint, CA glue + external sleeve.

**Sensor integration:** At the cam level, two features are cut into the chute wall:
- 12mm x 12mm window for the TCS34725 color sensor (looking at brick's flat side face)
- Break-beam holes in the opposite walls for the size beam

---

## Escapement

Single-lobe cam disk on NEMA 11 stepper shaft. One full rotation releases one brick.
Self-indexing via step counting. Cannot double-feed by design.

**Cam profile:**
- 30mm disk diameter, 8mm lobe extension, 50-degree lobe arc
- Flat chord (4mm depth) on the disk body where the bottom brick rests
- Sharp lobe trailing edge: acts as the blocking surface for the next brick
- Print at 0.12mm layer height, 100% infill

**Tuning:** If double-releases occur, increase lobe arc by 5 degrees and reprint.
At 3 rev/sec, the 50-degree lobe sweeps in 46ms. This is adequate blocking time for the
next brick in the queue. The sensing dwell (brick stationary on chord) is ~287ms (310
non-lobe degrees at 3 rev/sec), consistent with the ~260ms stated in ARCHITECTURE.md.

**Exit:** The cam pushes the bottom brick horizontally out of the chute onto a 10mm ramp
that drops 2mm over its length (11.3 degrees), depositing the brick onto the moving belt.
The ramp is integrated into the chute base piece (same printed part as cam motor mount).
Its lower edge must be flush with or slightly above the belt surface - never below (a lip
catches bricks). The ramp is critical. Without it, the brick drops abruptly and may bounce.

**Thermal management:** TMC2209 current reduction between releases. StallGuard detects
jams (stall flagged via UART). No additional hardware needed.

---

## Belt Channel

22mm wide x 15mm tall above belt surface. Self-aligns 2x3 bricks: the 23.7mm dimension
is blocked by the 22mm channel width, so 2x3 bricks can only travel in the correct
orientation. 2x2 bricks (15.8mm) pass through freely.

**Wall construction:** Two 3mm PLA walls, 290mm long.
- Pusher side: three slots (24mm wide x 13mm tall) at pusher positions. Edges chamfered
  1mm to prevent brick studs from catching.
- Bin side: three openings (30mm wide x 13mm tall) aligned with pusher slots.
- Interior surfaces: smooth. Sand after printing.

**Belt:** Neoprene rubber strip, 19mm (3/4") wide x 3mm thick, cut to ~700mm and spliced into
a loop. Taper both ends over 20mm, overlap 20mm, flexible CA glue (Loctite Ultragel) or
neoprene cement. No tooth alignment needed. Flat splice passes over smooth rollers cleanly.

**Drive roller:** 25mm OD, 25mm wide, 6mm D-bore. Motor D-shaft presses directly in.
M3 set screw through hub for axial retention. Bare PLA surface contacts belt by friction.
If friction test fails: wrap with heat-shrink tubing. Print: 0.2mm layers, 100% infill.

**Idler roller:** 25mm OD at edges, 25.5mm at center (0.5mm crown keeps belt centered),
25mm wide, 5mm round bore. Rides on M5 bolt axle. Spring-loaded tensioner: M5 bolt
through 10mm slot in frame end plate, compression spring pushes idler to tension belt.
2mm flanges on both rollers as hard stops against belt walk.

**Hall sensor:** A3144 Hall sensor mounted on belt frame beside idler roller, face 2mm
from roller rim. Two 3mm neodymium disc magnets glued 180 degrees apart on the roller rim.
10k pull-up on sensor output to 3.3V. 2 pulses per revolution. See ELECTRICAL.md.

**Belt bed:** 3mm aluminum flat bar, 280mm x 22mm, between the rollers under the belt.
Covered with PTFE tape. The neoprene belt slides over this. Do not 3D print (FDM warps).

---

## Pushers

Three identical direct solenoid pusher assemblies at 75mm, 150mm, 225mm from chute exit.

**No lever arm.** The solenoid rod pushes the face plate directly. Spring return is passive
and fast. This is simpler and more reliable than a lever design.

**Solenoid mount:** 3D-printed U-bracket clamps the JF-0530B body (13mm x 16mm x 30mm).
Two M2 screws through bracket into solenoid mounting tabs. Bracket mounts to belt frame
rail via M3 bolts through a 5mm-travel slot (for fine-tuning position along belt axis).
Solenoid rod center: 6mm above belt surface (aligned to brick center of mass).

**Face plate:** 20mm wide (along belt axis) x 11mm tall x 2mm thick. PLA at 100% infill
or 2mm aluminum sheet. Attached to solenoid rod via a press-fit sleeve with CA glue.
All edges chamfered 0.5mm. Face surface flat, perpendicular to push direction.

**Timing window (2x2 brick at 200mm/s):** (15.8 + 20) / 200 * 1000 = 179ms.
Solenoid on-time: 40ms. Margin: 139ms. Adequate even with significant timing drift.

---

## Sensing (at escapement level)

**Size beam:** Two IR break-beams cross the 22mm chute interior (Y axis), mounted in
the 27mm-wide walls at X=5mm (beam 1) and X=21mm (beam 2) from the reference wall.
10k pull-up on each receiver output to 3.3V. Detection logic: both beams blocked = 2x3.
Anything else = 2x2. Position-independent: correct at every possible brick position within
the 27mm chute. See ISSUE_FIXES.md for full analysis and proof.

**Color sensor:** TCS34725 on chute wall exterior, aligned with the 12mm x 12mm window.
Black PLA shroud (15mm deep) surrounds the window, blocking all ambient light.
The sensor's onboard LED is the only illumination. At 24ms integration time, the
260ms cam dwell yields 10+ samples from a stationary brick. Use 6ms integration for
more samples if needed.

Mount sensor with shroud flush against the chute wall. No gap between shroud and chute.
Calibrate only with the shroud installed. See docs/CALIBRATION.md.

---

## Bins

Four bins, labeled on the outside: "2x2 RED", "2x2 BLUE", "2x3 BLUE", "2x3 RED".

| Bin | Category | Internal W x L x H |
|-----|----------|---------------------|
| 1 | 2x2 red | 30mm x 30mm x 80mm |
| 2 | 2x2 blue | 30mm x 30mm x 80mm |
| 3 | 2x3 blue | 30mm x 35mm x 100mm |
| 4 | 2x3 red (default) | 30mm x 35mm x 60mm |

Floor angled 10 degrees toward the back wall: bricks slide to rest, cannot bounce out.
Break-beam slot at entrance of each bin. Bins 1-3 beside the belt channel; bin 4 at
the belt end below the exit point.

---

## Base Plate and Frame

**Base plate:** 610mm x 610mm x 6mm MDF or plywood. Finish with clear polyurethane.
Pre-drill mounting holes for belt frame, chute support, and bins.

**Belt frame:** Two side rails (290mm x 8mm x 40mm) plus cross-braces. PLA or aluminum.
Belt sits between rails; channel walls sit on top of rails.

**Chute support:** Vertical plate (310mm x 50mm x 5mm) bolted to base plate with two
triangular gussets (50mm x 50mm x 5mm). Chute must not sway during escapement operation.

**Electronics bay:** Underneath belt frame and beside chute. ESP32, TMC2209, MOSFET board,
buck converters, and terminal strips mount here. Route cables cleanly with ties.

---

## Assembly order

1. Print test pieces: channel section, chute transition, cam disk.
2. Test fit with real LEGO bricks before full-size prints.
3. Assemble belt frame with rollers and belt. Test belt motion with motor only.
4. Attach channel walls to belt frame.
5. Install solenoid mounts and face plates. Manually fire each: verify brick exits cleanly.
6. Assemble chute and cam. Load 10 bricks, run escapement, verify single releases.
7. Mount sensors (size beam, color sensor with shroud, chute exit beam, bin beams).
8. Wire everything. Install bins.
9. Full integration: 24-brick sort attempt.
10. Calibrate color thresholds. Run 10-run reliability test. Log all runs.

---

## Highest fabrication risk

The chute exit ramp (transition from cam to belt) is the hardest piece to get right.
FDM tolerances compound here. Print this first. Test with real bricks before printing
anything else. Budget 3 iterations. Everything else gates on this piece working.
