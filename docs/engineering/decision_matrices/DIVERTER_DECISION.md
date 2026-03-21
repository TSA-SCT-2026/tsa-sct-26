# Diverter Mechanism Decision - Unbiased Design Review

## Instructions for the AI reading this document

You are being asked to make a hardware design recommendation for a competition
project. Before you read anything else, internalize these rules completely:

1. You have zero prior context on this project. There is no prior conversation.
   There is no established design. Ignore any prior assumptions entirely. Even the information given to you below IS NOW LAW. You can disagree with anything, change it. Its just suggestions to help you.

2. There is no "current design." Every option below is equally valid. Nothing
   has been decided. No firmware, no CAD, no ordered parts constrain this
   decision. All of that can be changed in hours. Treat every option as if it
   were being proposed fresh today with no sunk cost.

3. Do not favor complexity or simplicity as a value in itself. Do not favor
   novelty or familiarity. Do not favor options that sound more "engineered."
   Favor the option most likely to result in first place.

4. Ease of building is NOT a meaningful constraint for this team. If an option
   is physically possible with a 2-person team in 4 weeks, it is on the table.
   The team is willing to do difficult work to win. Do not eliminate options
   because they seem hard. Eliminate them only if they are genuinely infeasible
   within the constraints or if they score worse.

5. Every option in this document was proposed seriously. None of them are
   obviously wrong. Evaluate each one on actual merits against actual scoring
   criteria. If an option you or I might instinctively dismiss 

6. You are not limited to the options listed. If you see a better design that
   combines elements of multiple options, or a modification to one of the
   options that makes it significantly better, propose it. Your job is to find
   the highest-scoring solution, not to pick from a menu.

7. Your output must include:
   - A clear final ranking of ALL options from most likely to win first place
     to least likely, with a one-line reason for each ranking position
   - A clear single winner with full justification
   - A decision tree: if the winner is infeasible for any reason, what is the
     fallback, and the fallback after that
   - What to order tonight that is not already available
   - What the team does in the first 48 hours

8. You must not stay undecided. You must produce a final ranking and a winner.
   If two options are genuinely close, say so but still pick one and explain
   why that one rather than the other given the tiebreaker criteria.

9. Do not reference any prior conversation, any prior design history, or any
   assumption about what this team has already decided. Reason entirely from
   the constraints and options in this document.

---

## What this system does

Sorts 24 LEGO bricks by size (2x2 vs 2x3) and color (red vs blue) into 4 bins,
automatically, on a conveyor, in a single run. Operated by a judge who has never
seen the system, following written instructions only. No team present.

Brick dimensions:
- 2x2: 15.8mm x 15.8mm x 11.4mm, square footprint
- 2x3: 23.7mm x 15.8mm x 11.4mm, longer
- All bricks weigh approximately 2-4 grams

Brick distribution per run:
- 2x3 blue: 8 (33%)
- 2x2 blue: 6 (25%)
- 2x2 red: 6 (25%)
- 2x3 red: 4 (17%)

---

## Scoring rubric (100 points total)

- Inventor's Log: 20 points (documentation, engineering notebook)
- Solution to Problem: 60 points (does it work, reliability, accuracy,
  engineering quality, decision documentation with rejected alternatives)
- Programming Structure: 20 points (code organization, readability)

Speed is NOT explicitly in the rubric. We assume that a faster sorter will stand out, making them think it is a better engineered design, but it is only an assumption

A 12-second reliable sort may score identically to an 8-second reliable sort, or the 8 second could score better. If they think the speed was good engineering, I believe they will reward it. Thats why I want to run the belt fast and escapement fast.

---

## Hard constraints

- Footprint: 610mm x 610mm maximum, cannot exceed this
- Power: LiPo battery
- Microcontroller: ESP32
- Competition date: May 1 2026
- Parts order deadline: tonight (March 21 2026)
- Parts arrival: approximately April 1 2026
- Build and calibration window: April 1 to April 27 (approximately 4 weeks)
- Team size: 2 people
  - Person A: embedded firmware focus, can write firmware for any mechanism
    in approximately 30-60 minutes once design is decided
  - Person B: CAD/mechanical focus, availability this week is uncertain,
    which is part of why this decision is open
- Budget: approximately $130-150 total

---

## Parts already in cart (these are sunk costs, ignore them as a constraint)

The following are already in the cart but should NOT bias the decision.
If the right answer requires different parts, order them or remove parts. All parts are aliexpress except high accuracy like LiPo, the break beams used to measure brick size, etc. These are listed only
so you know what is available without additional shopping:

- JF-0530B solenoids 5V x4 (~$1.10 each)
- Torsion spring assortment
- NEMA 11 stepper + TMC2209 (escapement)
- JGB37-520 6V 600RPM gearmotor (belt drive)
- L298N motor driver
- TCS34725 color sensor
- IR break-beams x6 pairs
- LM2596 buck converters x2
- GT2 timing belt + pulleys (could get more depending on the method, not ordered yet)
- Wide flat rubber belt (for brick transport surface, we havent added this to cart yet, how wide? it depends on the method, we could also get multiple depending on the method)
- ESP32, caps, wire, perfboard
- Wires, solder, perf board, other misc

Parts NOT in cart that could be added tonight at low cost:
- MG996R servo: ~$3-4 each (metal gear, 0.17s per 60 degrees at 6V)
- Additional small DC gearmotors: ~$3-6 each
- Additional belt/roller materials: ~$3-5
- Additional L298N or MX1508 drivers: ~$1-2 each
- Adafruit break beams for accurate size measurement
- LiPo(s)
- LiPo charger
- Anything else you want

---

## Sensing and classification (decided, not open, included for context)

Size classification: two IR break-beams 19mm apart along the belt axis.
Hardware timer measures gap between beam 1 and beam 2 breaking.
- 2x3 brick (23.7mm): longer than 19mm gap, breaks both beams, gap ~95ms at 200mm/s
- 2x2 brick (15.8mm): shorter than 19mm gap, beam 2 never breaks (timeout)
This is binary, hardware-timed, essentially immune to noise.

Color classification: TCS34725 RGB sensor facing down through a shroud.
Samples continuously from beam 1 break, ~32 samples per brick at 200mm/s.
Averages all valid samples, classifies by R/(R+G+B) ratio.
Red LEGO has strong red spectral signature, blue LEGO has strong blue signature.
Separation is large and reliable.

Both sensors output classification result approximately 150ms after brick enters
the sensing zone. This is the time available for the diverter to pre-set before
the brick arrives.

---

## Belt and transport (decided, not open, included for context)

Single straight belt. Wide flat rubber surface (~60-80mm wide) on 3D printed
rollers. Belt moves at approximately 200mm/s. Bricks ride on the belt surface
and travel from chute end to bin end in approximately 1.5 seconds.

The belt is the ONLY transport mechanism. All bricks travel the full belt length.
Diversion happens along the belt or at the end of the belt.

---

## The open question: how do we get bricks into 4 different bins?

Below are ALL proposed options. Evaluate each one.

---

### Option A: Solenoid plow with lever arm

Three solenoid-actuated lever arms at three positions along the belt.
Each lever arm has a pivot, a solenoid input arm (8mm), and a plow output arm
(24mm), giving 3:1 mechanical advantage.
Solenoid stroke 10mm becomes 30mm tip travel at the plow face.
30mm tip travel pushes a brick fully across the 25mm belt and into a side bin.
Spring returns the arm to neutral after each actuation.
Fourth bin (most common brick type, 33%) takes the default path with no actuation.

Actuation: solenoid energizes, lever sweeps plow into path in ~10ms.
Return: spring, ~20-30ms.
Total cycle time per actuation: ~50ms.

Three plows at different positions means three decisions happen in parallel as
bricks travel down the belt. Brick 3 can be at plow 1 while brick 4 is being
classified. The decisions are off the critical path.

At 4 bricks/sec (250ms between releases): 200ms margin after 50ms actuation.
At 6 bricks/sec (167ms between releases): 117ms margin.
At 8 bricks/sec (125ms between releases): 75ms margin.

Open questions / risks:
- Pivot hinge design undecided. Options: M2 bolt through printed bracket with
  brass tube bushing, or small ball bearing pressed into the arm.
- CAD person concern: lever arm might be flimsy, pivot might dislodge.
- Channel wall needs a slot for the lever sweep arc.
- Force margin against brick is approximately 94x (solenoid force >> required).

---

### Option B: Servo plow (direct sweep, no lever)

Same layout as Option A but with MG996R servos instead of solenoids and levers.
Servo horn directly pushes a plow arm into the brick path.
No lever, no spring, no pivot. Servo holds position, returns on command.

MG996R: 0.17s per 60 degrees at 6V. For 90-degree sweep: ~0.25s.
Return sweep: ~0.25s. Total cycle time: ~500ms.

At 4 bricks/sec (250ms between releases): cycle time EXCEEDS inter-brick window.
The servo cannot complete extension and return before the next brick arrives at
the same plow position.

This means the escapement must be slowed to keep inter-brick windows above 500ms:
500ms window = 2 bricks/sec = 24 bricks in ~12+ seconds.

Three servos in parallel still helps (different plows handle different bricks)
but the worst-case plow (2x2 blue, 6 bricks at 25% frequency) sees a brick
every 4th release. At 4 bricks/sec that is one 2x2-blue brick every second,
which is within the servo's recovery time. The constraint is only binding if
the same plow must fire twice within its cycle time.

At 4 bricks/sec with the interleaved brick sequence, no single plow fires more
than once per ~750ms. So servos MAY be viable at 4 bricks/sec if the firmware
sequences releases to avoid back-to-back same-plow firings.

At higher speeds (6+ bricks/sec), same-plow collision becomes likely.

Parts: MG996R x4, not currently in cart, ~$12-16 to add.
CAD: simpler than option A (no pivot, no lever ratio calculation).
Firmware: trivial, PWM position control.

---

### Option C: Wide-face direct solenoid push (no lever)

Solenoid pushes directly sideways with a wide flat face plate (~20-25mm wide)
attached directly to the solenoid rod. No lever amplification.

JF-0530B stroke: approximately 10mm.
Belt width: ~60-80mm.
Bin entrance must be positioned so a 10mm push delivers brick fully into it.
This constrains channel geometry: bin opening must be within 10mm of brick edge.

Face plate width 20-25mm catches slightly yawed bricks.
Actuation and return timing: identical to option A (solenoid + spring).
Tip travel is 10mm instead of 30mm.

CAD: simplest of all solenoid options. No pivot, no lever.
The constraint is channel geometry: bins must be adjacent to belt, not offset.
If channel geometry can accommodate this, it eliminates the pivot concern entirely.

Risk: 10mm stroke may be insufficient if the belt is wide. Depends on exact
channel layout. Needs to be verified in CAD before committing.

---

### Option D: Rotary bin selector (disk at belt exit)

A single 4-chamber rotating disk positioned at the belt exit point.
Bricks fall off the end of the belt and into whichever chamber the disk has
rotated to present. One motor drives the disk.
Belt has no diverters at all. Clean straight run.

Timing: brick exits belt approximately 1.5 seconds after release.
Classification result is available at ~150ms after brick enters sensing zone.
Time available for disk rotation: ~1.35 seconds. This is generous.

But the system is serial: cannot release the next brick until the disk has
rotated, settled, and confirmed position. Otherwise two bricks arrive at the
exit while the disk is mid-rotation.

With a servo driving the disk: 270-degree worst-case rotation at 0.17s/60deg
= ~0.75s + settling ~0.25s = ~1s between bricks minimum.
24 bricks at 1 brick/sec = 24 seconds minimum, plus overhead.

With a stepper driving the disk: faster rotation possible but requires
acceleration ramping firmware to stop accurately at 4 positions under varying
load (brick weight shifts as chambers fill). Position encoder needed.

Bin confirmation: one IR sensor per chamber opening.
CAD: 4-chamber disk, motor mount, position sensing.
Parts: one additional motor + driver, not currently in cart.

---

### Option E: Glued blocks on main conveyor belt as brick separators, with lateral solenoid pushers

Physical dividers glued onto the wide flat belt surface create individual cells,
one brick per cell. This eliminates timing-based brick spacing: the belt geometry
handles it mechanically.

With bricks pre-separated into cells, a lateral pusher (solenoid or servo) fires
when each cell aligns with the correct bin opening. The pusher does not need to
work against a moving brick - the brick is already in a defined location.
Timing becomes positional rather than velocity-based.

Pusher timing: fire when encoder confirms the correct cell is at the bin opening.
This is deterministic and does not depend on belt speed accuracy.

Belt construction concern: divider blocks are glued onto a rubber belt that
flexes around the rollers at each end. The glue joint is stressed in peel on
every rotation. Even low loads cause peel failure over repeated cycles at the
roller radius. This is a material failure mode, not a load failure mode.

Alternative attachment: if dividers are NOT glued but instead are part of a
printed or modular belt surface (e.g., laser-cut flexible sheet with integrated
walls, or 3D printed articulated belt sections), the peel problem disappears.
Custom belt construction is a significant fabrication challenge.

Risk assessment: if belt divider fabrication is reliable, this is a very clean
design. The positional triggering eliminates the tight timing window problem
entirely. The question is entirely about whether the dividers can be attached
durably.

---

### Option F: Zig-zag multi-belt layout

Instead of one straight belt with diverters, multiple shorter belt segments
arranged in a zig-zag or branching pattern. Each branch decision point uses
a simple 2-way diverter, which is easier to execute reliably than a 4-way.

Example layout:
- Main belt feeds a 2-way diverter (left or right)
- Left branch leads to a 2-way diverter (bin 1 or bin 2)
- Right branch leads to a 2-way diverter (bin 3 or bin 4)
Total diverters: 3, all 2-way only.

A 2-way servo diverter is much more viable than a 4-way: only 90-degree sweep
needed, and the inter-brick timing concern is reduced because each diverter only
handles a fraction of total bricks.

The first diverter handles all 24 bricks (must be fast).
Each second-tier diverter handles 12 bricks (half the frequency, more time).

Footprint concern: zig-zag layout must fit within 610x610mm. With a 400mm main
belt and two side branches, this may be tight. Needs a top-down layout sketch
before committing.

CAD complexity: significantly higher than single-belt options. Multiple belt
frames, multiple motors, multiple speed controllers, alignment between segments.
Bricks must transfer cleanly from one belt segment to the next without tumbling.
Transition points between belt segments are a new failure mode.

Parts: 2-3 additional small gearmotors, additional motor drivers, additional
belts and rollers. Approximately $20-30 additional cost.

---

### Option G: Parallel dual-belt with first-stage 2-way diverter

Two belts running side by side. A single 2-way diverter at the chute exit
routes each brick onto belt A or belt B. Belt A leads to bins 1 and 2 (with
a plow or pusher). Belt B leads to bins 3 and 4 (with a plow or pusher).

The first-stage diverter only makes a 2-way decision (much easier than 4-way).
Each belt's plow only makes a 2-way decision (even easier).

Inter-brick timing at the first diverter: still needs to fire and return before
next brick arrives (same constraint as before, but for a 2-way decision only).
A servo doing a 45-degree sweep: ~0.12s. Return: ~0.12s. Cycle: ~250ms.
This exactly matches the 4 bricks/sec inter-brick window. Marginal.

Footprint: two belts side by side, each ~80mm wide plus frame, total ~200mm
across. Plus the main chute and two sets of bins. Needs layout verification.

Parts: second JGB37 motor, second L298N, second belt+rollers, additional plows.
Approximately $15-25 additional cost.

CAD complexity: moderate to high. Two complete belt frames plus the diverter.

---

## What Opus is being asked to produce

Read all options above fully before forming any opinion.

Then produce the following, in this exact order:

### 1. Full ranking (all options, best to worst for winning first place)
One line per option. Format: "Rank N: [Option name] - [one sentence reason]"
Include any hybrid or modified option you think is better than what is listed.

### 2. Winner
State the winner clearly. Full justification covering:
- Why it scores best against the scoring rubric
- Why each other option scores lower (specific reason per option)
- The single biggest risk with the winner and exactly how to mitigate it

### 3. Decision tree
If the winner turns out to be infeasible after trying, what is the fallback.
If that fallback is also infeasible, what is the fallback after that.
Be specific about what "infeasible" means for each - what specific failure
condition triggers the fallback.

### 4. What to order tonight
Specific parts not in the available parts list that are needed for the winner.
Name, quantity, approximate cost, and why each is needed.

### 5. First 48 hours
Concrete tasks for Person A (firmware) and Person B (CAD/mechanical) in the
first 48 hours after this decision is made. Be specific - not "start the CAD"
but "CAD the lever arm: 24mm output arm, 8mm input arm, 3mm pivot hole, print
in this orientation." That level of specificity.

Hard constraint on your output: you must not be undecided. You must produce a
ranked list and a winner. If options are close, break the tie and explain why, but allow a decision tree to adapt to team constraints.
