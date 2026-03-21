# Diverter Mechanism Decision

Open design question requiring a final decision before ordering.
This document is written for a fresh AI instance with no prior conversation bias.
Recommend a mechanism. Justify it against the scoring rubric and constraints.

---

## Project context

TSA System Control Technology nationals, May 1 2026.
A fully automated conveyor sorts 24 LEGO bricks by size (2x2 vs 2x3) and color
(red vs blue) into 4 bins. Evaluated by a judge who operates it alone using
written instructions. No team present during evaluation.

Scoring: 100 points total.
- Inventor's Log: 20 points
- Solution to Problem: 60 points (reliability, accuracy, engineering quality,
  documentation of decisions with rejected alternatives)
- Programming Structure: 20 points

Speed is not explicitly in the rubric. It is assumed to be a tiebreaker only.
A system sorting 24 bricks in 8 seconds with 100% accuracy likely scores the
same as one doing it in 6 seconds with 100% accuracy. Do not optimize speed
at the cost of reliability or accuracy.

## Hard constraints

- Footprint: 610mm x 610mm maximum
- Power: 2S LiPo 7.4V, two 5V rails from LM2596 buck converters
- Microcontroller: ESP32
- Timeline: parts arrive ~April 1, competition May 1, ~4 weeks to build
- Team: 2 people, one focused on embedded firmware, one on CAD/mechanical
- CAD person availability: uncertain this week, reason for open decision

## Brick distribution (affects diverter duty cycle)

- 2x3 blue: 8 bricks (33%) - DEFAULT path, no actuator fires
- 2x2 blue: 6 bricks (25%)
- 2x2 red:  6 bricks (25%)
- 2x3 red:  4 bricks (17%)

Total actuations per run: 16 (the 8 default-path bricks require nothing).

## Belt and sensing architecture (decided, not open)

- Single straight belt, wide flat rubber surface (~60-80mm wide) on printed rollers
- Separate narrow GT2 belt loop on motor shaft for encoder/speed measurement only
- JGB37-520 6V 600RPM gearmotor drives the belt
- L298N PWM speed control, PI controller for closed-loop speed
- Belt target speed: 200mm/s
- Escapement: NEMA 11 stepper + TMC2209, cam disk releases one brick per revolution
- Starting escapement rate: 800 sps = 4 bricks/sec, ~9 second run
- Size detection: two Adafruit IR break-beams 19mm apart, hardware timer measures
  gap between beam breaks. 2x3 (23.7mm) breaks both beams, 2x2 (15.8mm) does not.
- Color detection: TCS34725 sampling continuously from beam 1 break, averaged over
  brick dwell window (~79ms at 200mm/s = ~32 samples). Black belt filter discards
  non-brick readings.
- Bin confirmation: one IR break-beam per bin entrance, 500ms timeout triggers
  ERROR_HALT if brick does not arrive

## Timing window available for diverter

Brick travels from sensing zone to first divert point in approximately 150ms.
Solenoid/servo must be in position before brick arrives.
After brick passes, actuator must return to neutral before next brick arrives.

At 800 sps (4 bricks/sec): 250ms between brick releases.
At 1000 sps (5 bricks/sec): 200ms between brick releases.
At 1200 sps (6 bricks/sec): 167ms between brick releases.

The diverter must complete extension + return within the inter-brick window
minus any firmware scheduling jitter (assume ~10ms jitter budget).

## Parts already ordered (in cart, not yet placed)

Solenoids: JF-0530B 5V x4 (already in cart)
Springs: torsion spring assortment (already in cart)
No servos ordered yet (MG996R ~$3-4 each, could be added tonight)
No additional motors ordered yet

## Firmware state

State machine, classifier, router, thermal model, logger, test harness all
written and tested in simulation. Firmware is modular. The routing decision
(which plow/actuator fires for which brick type) is a lookup table.
Changing from solenoid to servo requires: different GPIO output pattern,
different timing constants, no state machine changes. Estimated 30-60 minutes
of firmware work to adapt to any mechanism.
Firmware is not a constraint on this decision.

---

## Candidate mechanisms

### Option 1: Solenoid plow with lever arm

A solenoid pushes a 3D printed lever arm (8mm input, 24mm output, ratio 3.0)
which rotates a plow blade into the brick path. Spring returns the arm to neutral.
Three plows at different positions along the belt for three of the four brick types.
Default path (most common brick type) fires no solenoid.

Actuation time: ~10ms to full extension.
Return time: ~20-30ms (spring).
Total occupied time per actuation: ~50ms.
Margin at 800 sps: 200ms.
Margin at 1000 sps: 150ms.
Margin at 1200 sps: 117ms.

Parts needed: solenoids (in cart), torsion springs (in cart), M2 pivot hardware,
printed lever arm and plow.

CAD required: lever arm with pivot hole and solenoid attachment point (simple part),
plow blade, mounting bracket. Estimated complexity: low. A competent CAD person
should complete in 2-4 hours.

Teammate concern: the pivot hinge design is undecided. Concern about lever arm
being flimsy or dislodging. Concern about opening in channel wall for lever sweep.

Mitigation: M2 bolt through a printed bracket with brass tube bushing at pivot.
Lever arm printed with layer lines perpendicular to load direction. Channel wall
has a slot sized to the lever sweep arc. If the arm breaks, it is a 10-minute
reprint. Force margin against brick load is approximately 94x.

Documentation story: strong. Force margin calculation, lever ratio derivation,
spring selection methodology, solenoid comparison to servo (timing math), thermal
management (PWM hold, heatsink). All of this is engineering decision material
that judges score.

### Option 2: Servo (MG996R)

A servo arm sweeps into the brick path to divert it. Spring return not needed,
servo holds position. Three servos for three divert paths.

MG996R specs: metal gears, 0.17s per 60 degrees at 6V.
For 90 degree sweep (enough to divert brick across 25mm belt): ~0.25s.
Return sweep: another 0.25s.
Total occupied time: ~500ms.

Margin at 800 sps (250ms inter-brick): NEGATIVE. Servo cannot complete extension
and return before next brick arrives at 800 sps.

This means servo constrains maximum escapement rate to approximately:
1 / (0.5s per actuation) = 2 bricks/sec from a servo-limited channel.
At that rate 24 bricks takes 12+ seconds.

Note: not all bricks require actuation. 33% take the default path. But the
worst-case plow (6 actuations at 25% frequency) still sees bricks every 250ms
at 800 sps, which the servo cannot handle.

With a fast servo (0.1s per 60 degrees): total occupied time ~330ms. Still
exceeds the 250ms inter-brick window at 800 sps. Margin appears only if
escapement is slowed to 600 sps or below (3 bricks/sec, ~14 second run).

Parts needed: MG996R x4 (~$12-16 total), not currently in cart.

CAD required: servo horn attachment to plow arm, mounting bracket. Similar
complexity to solenoid plow. Slightly simpler because no pivot mechanism needed,
servo provides its own rotation.

Documentation story: weak. Servo is the obvious default choice. Every other
team will have considered servos. Choosing servos because they are simpler is
not a strong engineering decision. Choosing servos with documented tradeoff
analysis (we accepted a slower run in exchange for build simplicity) is better
but still not as strong as the solenoid story.

### Option 3: Wide-face direct solenoid pusher (no lever)

A solenoid pushes directly laterally (no lever amplification) with a wide flat
face plate attached to the solenoid rod. Face plate is ~20-25mm wide to catch
slightly yawed bricks.

JF-0530B stroke: ~10mm. Belt is 25mm wide. A 10mm push with a 20mm face plate
may not fully clear a 15.8mm brick off a 25mm belt - the brick may remain
partially on the belt and not fall cleanly into the bin.

If the channel is designed so the bin entrance is only 10mm lateral travel away,
this could work. Requires careful channel geometry: the bin opening must be
positioned so a 10mm push delivers the brick fully into it.

Actuation time: same as solenoid plow (~10ms). Return time: spring, ~20-30ms.
Margin: same as option 1.

Parts needed: solenoids (in cart), springs (in cart), face plate (simple flat
printed piece), no lever or pivot needed.

CAD required: simpler than option 1. Face plate is a rectangle. No pivot.
Channel geometry needs careful design around the 10mm stroke constraint.

Teammate concern: this directly addresses the lever/pivot concern while keeping
solenoid timing advantages. The tradeoff is less tip travel (10mm vs 30mm with
lever), which constrains channel geometry.

Documentation story: moderate. Simpler mechanism, less engineering complexity
to document. Can still document the stroke vs geometry tradeoff and the face
width decision.

### Option 4: Rotary bin selector

A single 4-chamber rotating disk at the belt exit routes bricks by rotating to
align the correct chamber before the brick arrives. One motor drives the disk.
No plows, no lateral diverters, belt is completely clear.

Timing requirement: after classification, disk must rotate up to 270 degrees
(worst case), settle, and be confirmed in position before brick arrives.
At 200mm/s over ~200mm sensor-to-exit distance: 1000ms available for disk movement.
This is the most generous timing window of all options.

However: system is serial. Cannot release next brick until disk has settled and
confirmed. Brick release rate is limited by disk rotation time, not escapement.
With a fast servo driving the disk (0.1s per 60 degrees): 270 degree rotation
= 0.45s + settling = ~600ms minimum between bricks = 1.7 bricks/sec maximum.
24 bricks = ~14 seconds minimum plus overhead.

Parts needed: one additional servo or DC motor with encoder for disk, disk printed
in 4 chambers. Not currently in cart.

CAD required: 4-chamber disk, motor mount, encoder for position confirmation.
Moderate complexity.

Documentation story: interesting mechanism with a clear engineering story
(eliminate lateral forces on brick, simplify belt channel, trade throughput
for reliability). Unusual enough to stand out. Weak on throughput.

### Option 5: Parallel belt system

Two belts side by side. A simple 2-way diverter at the chute exit routes each
brick onto one of the two belts. Each belt has its own 2-bin routing at its end
(one plow per belt, or just two bins at the end of each belt with a 2-way choice).

Diverter at the chute exit only needs to make a 2-way decision, which any
mechanism (even servo) can do reliably at full speed (servo only needs 90 degree
sweep for 2-way, occupies ~250ms, exactly at the margin for 800 sps).

Complexity cost: two belt motors, two speed controllers, two timing systems,
belt alignment across two parallel frames, twice the perfboard wiring. Firmware
needs to track which belt each brick is on. CAD significantly more complex.

Parts needed: second JGB37, second L298N, second GT2 timing loop, second wide
belt. Approximately $15-20 additional cost.

Documentation story: interesting architectural choice with documented reasoning.
The 2-way diverter simplification is a real engineering argument.

---

## Decision criteria ranked by scoring impact

1. Does it work reliably for 24 bricks repeatedly? (reliability = majority of 60pts)
2. Can it be built and calibrated in 4 weeks by a 2-person team? (schedule)
3. Does it generate strong engineering documentation? (decision matrices, calculations)
4. Does it run within the 10-second target? (speed, tiebreaker only)
5. Does it stand out from other teams? (minor factor, judges score engineering not novelty)

## What Opus is being asked to decide

Given the above, recommend one mechanism. State clearly:
- Which option
- Why it scores best against the 5 criteria above
- What the critical build risk is and how to mitigate it
- What the team should do in the next 48 hours

Do not recommend the solenoid plow just because firmware was written for it.
Firmware adaptation takes 30-60 minutes and is not a constraint.
Do not recommend any option just because it is novel.
Recommend the option most likely to result in first place given real constraints.
