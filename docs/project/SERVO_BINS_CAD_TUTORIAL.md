# Servo Selector and Bin CAD Tutorial

## Purpose

This tutorial explains how to CAD the servo rotary chute selector and the four receiving bins for the states sorter.

The goal is not a perfect theoretical model. The goal is a simple, printable, adjustable mechanism that routes real LEGO bricks into the correct labeled bin every time.

Use this document while working in Fusion 360. Keep the dimensions parametric until the conveyor handoff, chute slide, servo alignment, and bin catch tests pass with real bricks.

## What This Subassembly Does

The conveyor carries one brick through the sensing station. After size and color are known, the MG995/MG996-class servo rotates a chute toward one of four bins. The brick leaves the belt, enters the chute, slides down the UHMW-lined channel, exits into a wide funnel mouth, and drops into the correct bin.

The bins are not just storage boxes. They are part of the routing mechanism. The funnel mouths give the servo chute several degrees of tolerance so the system does not need perfect servo accuracy.

## Do Not Build an Input Hopper Here

The active states build uses manual one-at-a-time feed. Do not spend this CAD session designing an input hopper or automated feed queue. The parts in this tutorial are receiving bins and bin funnels under the servo chute.

A simple feed guide can be added later, but only after the base sorter routes bricks reliably.

## Design Priorities

Use this priority order:

1. Correct bin every time
2. No missed chute entries
3. No brick catch points
4. Easy evaluator operation
5. Compact footprint
6. Speed

Do not shrink the funnels just to make the CAD look neat. The wide catch region is what protects the run from servo slop, frame tolerance, bounce, and brick pile interference.

## Core Numbers

Start with these values:

- Servo family: MG995/MG996/MG996R-class heavy servo
- Servo body reference: about 40mm x 19mm x 43mm, verify real part
- Servo rotation axis: vertical, output shaft up
- Working pivot height: Z=90mm from machine base
- Bin internal width: 80mm
- Bin internal depth: 80mm
- Bin internal height: 60mm
- Bin wall thickness: 3mm
- Bin floor thickness: 3mm
- Chute internal width: 30mm
- Chute internal height: 15mm
- Chute wall thickness: 3mm
- First chute angle: 40 degrees from horizontal
- Chute test angles: 30, 35, 40, 45 degrees
- First chute sweep radius: 95mm
- Funnel mouth width: 50mm first, 60mm if spacing allows
- Funnel throat width: 30mm
- Funnel transition distance: 15mm to 20mm
- Servo positions: 37, 72, 107, 142 degrees
- Footprint limit: 610mm x 610mm

Bin labels in servo order:

1. 2x2 RED
2. 2x2 BLUE
3. 2x3 RED
4. 2x3 BLUE

## Fusion Parameters

Create these user parameters before sketching:

- `bin_w`: 80 mm, internal bin width
- `bin_d`: 80 mm, internal bin depth
- `bin_h`: 60 mm, internal bin height
- `wall`: 3 mm, printed wall thickness
- `floor_t`: 3 mm, printed floor thickness
- `funnel_mouth_w`: 50 mm, tangential catch width
- `funnel_mouth_d`: 50 mm, radial catch depth
- `funnel_throat_w`: 30 mm, matches chute internal width
- `funnel_transition`: 18 mm, guide transition distance
- `chute_w`: 30 mm, internal chute width
- `chute_h`: 15 mm, internal chute height
- `chute_wall`: 3 mm, chute wall thickness
- `sweep_r`: 95 mm, pivot to funnel center in top view
- `servo_z`: 90 mm, working pivot height
- `bin_gap`: 3 mm, clearance between bin bodies if separate
- `label_w`: 65 mm, front label flat width
- `label_h`: 18 mm, front label flat height

Keep these parameters adjustable:

- `sweep_r`
- `funnel_mouth_w`
- `funnel_mouth_d`
- `servo_z`
- chute length
- chute angle
- funnel catch height
- servo pocket screw locations

## Why the Funnel Exists

A plain box opening can work in a perfect CAD animation. It is risky on the real machine.

The funnel protects against:

- servo angle error
- horn and pivot slop
- frame tolerance
- printed part tolerance
- brick yaw at chute exit
- bounce from hard plastic
- deflection from bricks already in the bin

The important event is not storage volume. The important event is the brick leaving the chute and entering the correct catch zone. A funnel turns a narrow hit into a wide hit.

The funnel can be simple. It does not need to be a huge cone. It can be a flared catch mouth with angled guide walls that narrow into the bin.

## Connected or Separate Bins

Use this rule:

- Prototype: one bin and one funnel
- Final: connected removable cassette, or two connected two-bin modules

Directly connecting the final bins is a good idea if it saves space and locks the alignment. The interiors must stay separate. Bricks must never migrate from one category to another.

Recommended final layout:

- Four separate bin cavities
- Shared walls where adjacent bins touch
- Shared base plate or rear spine
- Front label flats
- Optional finger grips for removal
- No shared internal openings between bins

Avoid one giant connected print if it causes these problems:

- too large for the printer
- difficult to remove from the machine
- blocks servo screws
- blocks wiring access
- forces a full reprint after one funnel change

If in doubt, make two mirrored two-bin modules. They are easier to print and still hold alignment better than four loose boxes.

## Top View Layout

Start from the servo pivot, not from the bin boxes.

1. Create a top-level component named `Servo_Bin_Assembly`.
2. Create a sketch on the base plane.
3. Draw a 610mm x 610mm construction boundary.
4. Place a construction point for the servo pivot.
5. Draw four construction rays from the pivot.

Use these starting angles:

- 2x2 RED: 37 degrees
- 2x2 BLUE: 72 degrees
- 2x3 RED: 107 degrees
- 2x3 BLUE: 142 degrees

6. On each ray, place a funnel center point at `sweep_r`.
7. Around each funnel center point, sketch a catch rectangle.

The catch rectangle should be oriented like this:

- Wide direction is tangential to the chute arc
- Depth direction is radial from the servo pivot

Use:

```text
tangential width = funnel_mouth_w
radial depth = funnel_mouth_d
```

The servo error moves the chute exit along the arc, so the funnel must be wide in the tangential direction.

## Arc Spacing Check

Four servo positions are about 35 degrees apart. The center spacing between adjacent funnel centers is approximately:

```text
spacing = 2 * sweep_r * sin(17.5 degrees)
spacing is about 0.60 * sweep_r
```

Useful values:

- 65mm radius: about 39mm adjacent center spacing
- 75mm radius: about 45mm adjacent center spacing
- 83mm radius: about 50mm adjacent center spacing
- 95mm radius: about 57mm adjacent center spacing
- 100mm radius: about 60mm adjacent center spacing
- 110mm radius: about 66mm adjacent center spacing

What this means:

- 65mm to 75mm radius is too tight for 50mm to 60mm mouths unless bins are staggered.
- 95mm radius works well for a 50mm funnel mouth.
- 100mm to 110mm radius is better for a 60mm funnel mouth.

Start with:

```text
sweep_r = 95mm
funnel_mouth_w = 50mm
```

If there is room inside the 610mm x 610mm footprint, widen the funnel mouth toward 60mm.

## When Funnel Mouths Overlap

If adjacent catch rectangles overlap, fix the layout in this order:

1. Increase `sweep_r` toward 100mm or 110mm.
2. Reduce `funnel_mouth_w` from 60mm to 50mm.
3. Stagger alternating bins radially by 15mm to 25mm.
4. Slightly increase servo angle spacing if the real servo range allows it.
5. Shrink bin body size only if the footprint forces it.

Do not start by shrinking the funnels. The catch width is a reliability feature.

## Bin Body Tutorial

Model one bin first. After it is correct, copy it to the other three positions.

### Create the Basic Bin

1. Create a component named `Bin_2x2_RED`.
2. Sketch the external rectangle:

```text
external width = bin_w + 2 * wall
external depth = bin_d + 2 * wall
```

With the starting numbers:

```text
86mm x 86mm
```

3. Extrude the body upward:

```text
bin_h + floor_t = 63mm
```

4. Shell the top inward:

```text
wall = 3mm
```

5. Verify the internal cavity is:

```text
80mm x 80mm x 60mm
```

6. Add 1mm to 2mm fillets or chamfers to the top edges.
7. Keep the bottom flat so the bin sits cleanly in the frame or cassette tray.

### Add a Label Flat

The evaluator must identify bins without guessing.

1. Choose the front face.
2. Add a raised or recessed label panel:

```text
label_w = 65mm
label_h = 18mm
```

3. Add text:

```text
2x2 RED
```

4. Make the text large enough to read from arm's length.
5. Use raised or engraved letters deep enough to survive sanding and paint.

Repeat labels for:

```text
2x2 BLUE
2x3 RED
2x3 BLUE
```

## Funnel Tutorial

There are two acceptable funnel styles:

- Top-entry funnel
- Side-entry or side/top hybrid funnel

Choose based on the real Z stack. The chute exit must land inside the funnel catch window.

### Top-Entry Funnel

Use this if the chute exit is above the bin top.

Geometry:

- Funnel mouth is on top of the bin.
- Funnel mouth width is 50mm to 60mm tangentially.
- Funnel narrows toward a 30mm throat.
- Funnel transition distance is 15mm to 20mm.
- Brick drops into the open bin after the guide walls center it.

CAD steps:

1. Sketch the funnel mouth above the bin.
2. Align its center with the servo ray.
3. Make the wide direction tangent to the chute arc.
4. Sketch a smaller throat opening:

```text
30mm wide, centered under the mouth
```

5. Loft or extrude angled guide walls between mouth and throat.
6. Keep all inner corners smooth enough that studs cannot catch.
7. Add space for UHMW tape on sliding faces.

### Side-Entry or Side/Top Hybrid Funnel

Use this if the chute exit is near the upper side of the bin rather than clearly above it.

This is likely useful on the low-frame build because the conveyor, chute, and bin heights are close together.

Geometry:

- Funnel mouth faces the chute exit.
- Top of the bin remains open.
- The side mouth guides the brick inward and downward.
- The funnel does not block bin removal.

CAD steps:

1. On the chute-facing bin wall, sketch the side catch mouth.
2. Start with:

```text
mouth width = 50mm
mouth height = 18mm to 25mm
```

3. Center the mouth on the chute exit path.
4. Add angled side walls that guide to a 30mm throat.
5. Add a small sloped floor or lip if needed to keep the brick moving into the bin.
6. Keep the bin top open so the evaluator can see the sorted bricks.
7. Verify the brick cannot bounce back out through the side mouth.

Side-entry funnels must be tested with real bricks. They are useful, but the exit height and guide angle matter.

## Funnel Wall Rules

Use these rules for all funnel styles:

- No sharp internal ledges
- No vertical wall directly in the brick path
- No overhang that catches studs
- Funnel walls should angle inward at about 35 to 45 degrees
- Add 1mm to 2mm edge breaks on printed plastic corners
- Line sliding faces with UHMW tape before meaningful testing

The UHMW tape is physical material, not CAD decoration. Leave enough clearance for it.

## Servo Mount Tutorial

The servo mount must hold the pivot stable. Do not mount the servo only to a thin conveyor side plate.

### Servo Pocket

1. Measure the real servo body.
2. Create a component named `Servo_Mount`.
3. Model a rectangular pocket around the measured servo body.
4. Start from these approximate dimensions only if the real servo is not in front of you:

```text
body length = 40mm
body width = 19mm
body height = 43mm
```

5. Add clearance:

```text
0.3mm to 0.6mm per side for a printed pocket
```

6. Add screw tabs or clamp features that match the real servo ears.
7. Leave the servo output shaft vertical and centered on the CAD pivot.
8. Leave wire exit clearance away from the horn and chute sweep.

### Mount Attachment

The mount should tie into the base plate or a stiff frame member.

Good attachment options:

- printed cradle screwed to wood base
- printed cradle bolted to a thick wood riser
- pocket plus upper retaining strap
- printed locating pocket with wood screws through flanges

Bad attachment options:

- thin flexible plate only
- unsupported vertical wall
- mount attached only to conveyor side plate
- mount that blocks horn screws
- mount that traps the servo wire

### Servo Horn Adapter

The chute arm must connect to the servo horn without wobble.

1. Import or model the horn shape if available.
2. Use the real horn screw pattern.
3. Create a printed adapter plate that bolts through the horn.
4. Keep the adapter broad enough to resist twisting.
5. Keep the chute arm short and stiff.
6. Add ribs if needed rather than making the arm longer.

The servo horn adapter is a high-risk fit. Print a small horn adapter coupon before printing the full chute.

## Chute Tutorial

The chute is a rectangular trough that rotates with the servo.

### Chute Channel

Use:

```text
internal width = 30mm
internal height = 15mm
wall thickness = 3mm
```

The as-fed brick is 15.8mm wide, so 30mm gives room for yaw, real belt tracking, and handoff tolerance.

### Chute Slope

Start CAD at 40 degrees from horizontal.

Before final printing, test real bricks on a short UHMW-lined chute segment at:

```text
30 degrees
35 degrees
40 degrees
45 degrees
```

Choose the lowest angle that slides reliably without sticking.

### Entry and Exit Are Different

Do not copy the chute exit height to the chute entry.

- Chute entry follows the conveyor exit and exit lip.
- Chute exit follows chute length, chute angle, and funnel catch height.

Check both in CAD and then with real bricks.

### Chute Length

Keep the pivot-to-exit plan radius as short as practical. A shorter arm reduces servo error at the exit.

However, the funnels need enough spacing. Use the arc spacing check before locking the radius.

Start with:

```text
plan sweep radius = 95mm
```

Then adjust after checking:

- funnel overlap
- chute exit height
- frame clearance
- servo torque and arm flex
- bin removal path

## Z Stack Tutorial

The Z stack is the vertical relationship between:

- belt surface
- exit lip
- chute entry
- servo pivot
- chute exit
- funnel catch window
- bin top

Current working values:

```text
belt surface starts around Z=68mm from machine base
exit lip is about 5mm to 6mm above belt surface
servo pivot starts around Z=90mm
bin internal height is Z=0mm to Z=60mm
```

These values are provisional. The final geometry must be checked in the real assembly.

### Z Stack Workflow

1. Place the conveyor belt surface in machine-base coordinates.
2. Place the exit lip 5mm to 6mm above the belt surface.
3. Place the chute entry where the brick naturally leaves the exit lip.
4. Place the servo pivot at the working `servo_z`.
5. Model the chute slope.
6. Measure the chute exit height.
7. Place the funnel catch window around the chute exit.
8. Choose top-entry or side-entry funnel based on that height.

If the chute exit is too low for a top-entry funnel, do one of these:

- raise the pivot
- reduce the chute angle
- shorten or reshape the sloped chute path
- use a tested side-entry funnel

Do not hide a Z mismatch by making a tiny opening that only works in the CAD view.

## Connected Bin Cassette Tutorial

After one bin and one funnel work, make the final connected tray.

### Option A: Four-Bin Cassette

Use this if the printer bed and removal path allow it.

Features:

- four separate cavities
- shared walls between adjacent bins
- shared floor or rear spine
- front label strip
- optional finger grips
- bottom locating features that sit in the frame

CAD steps:

1. Copy the validated bin body to all four ray positions.
2. Trim or combine shared walls where bins touch.
3. Add a shared base plate under all four bins:

```text
base thickness = 3mm to 5mm
```

4. Add a rear spine if it improves stiffness.
5. Keep front faces clear for labels.
6. Check removal from the front or front side.
7. Check that the cassette does not hit the chute, servo horn, or wiring.

### Option B: Two Two-Bin Modules

Use this if a four-bin cassette is too large or hard to print.

Recommended split:

- Module 1: 2x2 RED and 2x2 BLUE
- Module 2: 2x3 RED and 2x3 BLUE

Benefits:

- easier print
- easier iteration
- still keeps adjacent bin spacing fixed
- simpler to remove from the machine

### Option C: Four Loose Bins in a Locating Tray

Use this if you need maximum serviceability.

The tray has four pockets or rails that locate separate bins. This keeps alignment repeatable while allowing one bin to be replaced without reprinting the others.

This is the safest final service design if time allows.

## Bin Placement With Staggering

If wide funnels overlap, stagger bins instead of shrinking everything.

Staggering means moving alternating bins slightly inward or outward along their servo rays.

Start with:

```text
Bin 1 radius = sweep_r
Bin 2 radius = sweep_r + 20mm
Bin 3 radius = sweep_r
Bin 4 radius = sweep_r + 20mm
```

Then check:

- adjacent funnel overlap
- chute exit still lands inside each mouth
- bin bodies do not collide
- footprint stays under 610mm x 610mm
- bins remain removable

Staggering often solves the layout without sacrificing catch width.

## What to Print First

Do not print the full four-bin cassette first.

Print in this order:

1. Servo pocket test
2. Servo horn adapter coupon
3. Short chute angle coupon
4. One chute arm and one funnel
5. One full bin with funnel
6. Two-bin module or full cassette

The one-bin test should prove:

- servo aligns to the target
- chute exit lands inside the funnel
- brick does not catch on the funnel
- brick does not bounce back out
- bin can be removed without disturbing the chute

Only after that should the full bin set be printed.

## CAD Checks Before Export

Check these before exporting STLs:

- Four servo rays exist and match the bin order.
- Chute exit overlaps each funnel mouth at its target angle.
- Adjacent funnel mouths do not unintentionally overlap.
- Bin bodies fit inside the 610mm x 610mm footprint.
- Bins are removable from the front or front side.
- Servo horn screws are accessible.
- Servo wire exits away from the chute sweep.
- Chute does not hit any bin wall during rotation.
- Funnel edges do not create stud catch points.
- Labels are visible from the front.
- UHMW tape surfaces have clearance.
- The Z stack has been checked against conveyor belt height.

## Physical Validation Tests

Run these before declaring the selector ready.

### Chute Slide Test

Use a short chute segment with UHMW tape installed.

Test at:

```text
30 degrees
35 degrees
40 degrees
45 degrees
```

Pass condition:

- brick slides without sticking
- brick does not accelerate so hard that it bounces out of the funnel
- studs and corners do not catch

### One-Funnel Test

Mount one funnel and bin at one servo position.

Run at least 10 real-brick drops.

Pass condition:

- 10 of 10 bricks enter the intended bin
- no brick catches on the chute or funnel
- no brick bounces out
- servo returns repeatably to the same target

### Four-Bin Forced Route Test

With all four bins installed, command each servo position manually.

Run:

```text
10 bricks per bin minimum
```

Pass condition:

- each bin receives 10 of 10 forced-route trials
- no adjacent bin receives a brick
- no brick lands between bins
- no brick touches servo wiring or horn

### Full Sorting Test

After sensing and firmware are integrated, run the full 24-brick set.

Pass condition:

- all 24 bricks land in the correct labeled bins
- final bin counts match the expected set
- no evaluator action is needed except manual one-at-a-time feed

Expected bin counts:

- 2x2 RED: 6
- 2x2 BLUE: 6
- 2x3 RED: 4
- 2x3 BLUE: 8

## Common Failure Fixes

### Brick misses the funnel

Fixes:

- widen the funnel mouth
- increase sweep radius
- adjust servo angle
- reduce chute exit speed
- stiffen the chute arm
- check servo horn slop

### Brick hits the funnel edge and bounces

Fixes:

- chamfer the catch edge
- make the funnel wall less vertical
- add UHMW tape
- reduce chute angle if the brick exits too fast
- raise or lower the funnel catch window

### Funnel mouths overlap

Fixes:

- increase sweep radius
- stagger bins radially
- narrow mouth from 60mm to 50mm
- use two-bin modules
- revise servo angle spacing

### Bin cannot be removed

Fixes:

- move removal direction to front side
- split into two-bin modules
- lower the rear wall where safe
- add finger grips
- move wiring away from bin path

### Servo mount flexes

Fixes:

- attach mount to thicker wood or base plate
- add ribs
- shorten unsupported plastic
- add a top strap over the servo
- avoid mounting only to thin conveyor side plates

## Recommended First Build

Use this exact first build unless the CAD layout proves it cannot fit:

```text
sweep radius: 95mm
funnel mouth: 50mm tangential width
funnel throat: 30mm
bin internal size: 80mm x 80mm x 60mm
servo pivot: Z=90mm starting point
chute angle: 40 degrees starting point
final bin style: connected removable cassette or two two-bin modules
prototype print: one bin plus one funnel
```

This is compact enough to stay realistic and forgiving enough to protect sorting accuracy.

## Final Export Checklist

Before exporting final parts:

- [ ] One-bin prototype tested with real bricks
- [ ] Chute slide angle chosen from UHMW test
- [ ] Servo pocket checked against real servo
- [ ] Horn adapter checked against real horn
- [ ] Funnel mouth overlap checked in top view
- [ ] Z stack checked in side view
- [ ] Bin removal path checked
- [ ] Bin labels added
- [ ] Footprint boundary visible in assembly screenshots
- [ ] Forced-route test plan ready for all four bins

If any of these are not true, export only the next prototype part, not the full final cassette.

## Reference Files

- `docs/ARCHITECTURE.md`
- `cad/MECHANICAL.md`
- `cad/DIMENSIONS.md`
- `docs/project/CAD_PLAN.md`
- `docs/project/SERVO_CHUTE_HANDOFF.md`
