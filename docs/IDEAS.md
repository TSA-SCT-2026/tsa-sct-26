# Ideas and Design Exploration

Scratchpad for concepts that are exploratory, not yet architecture. Some of these informed current design decisions. Some may still be relevant if a subsystem needs to be reconsidered.

---

## Alternative diverter mechanisms (considered before committing to solenoid plow)

Every design was evaluated against the core constraint: sort 5 bricks per second, 200mm/s belt, 140ms inter-brick window.

**Solenoid plow: current design**
Pre-set before brick arrives, actuation off critical path, spring return is passive. The only mechanism that fully decouples actuation timing from belt speed. Chosen.

**Servo flap**
Weakness: 100-150ms sweep and return time. Eats the entire inter-brick window. Throughput drops to 2-3 bricks/second. Not viable at target speed.

**Solenoid puller / fast retractor**
Instead of pushing the arm out and spring-returning it, the solenoid holds the arm retracted (in the normal/clear position) and releases it to snap forward under spring power, then re-energizes to retract again. The spring does the divert, not the solenoid. This could be fast: spring snap is faster than solenoid pull. However it means the solenoid is energized most of the time (holding retracted), inverting the heat profile. Current design has solenoids de-energized by default, which is thermally better for 33% of bricks that take the default path with zero actuation. Puller concept is interesting but thermal trade-off goes the wrong direction for this brick distribution.

**Paddle wheel / continuous rotation**
Weakness: timing is mechanically coupled to belt speed, requires synchronization. One paddle per bin path, significant footprint. No selective control without a clutch. See ARCHITECTURE.md for full analysis.

**Pop-up gate (pin through belt)**
Weakness: GT2 belt cannot be slotted without structural failure. Requires a second mechanism to clear the brick. Physically stops bricks rather than deflecting them, which means bounce and jam risk.

**Track diverter (physical rail switches)**
Weakness: inertia cost on both brick and mechanism. Like a pop-up gate but larger footprint. Mechanically interesting but slower than solenoid plow.

**Conveyor -> solenoid queued tilting platform**
A short platform between belt segments that tilts to route the brick to one side. Weakness: cost of inertia (needs a lip to slow brick down before tilt), gravity-friction speed limits return, more moving mass than a plow arm. Complex to mount at belt speed.

**Diagonal conveyors that reverse to collect**
Multiple belts at angles to the main belt that can redirect by reversing direction. Weakness: inertia at belt transitions, potentially the fastest if conveyors run extremely fast, but the transition inertia cost may exceed what is saved. Footprint and complexity are significant. Worth researching if current design hits a throughput wall: specifically look at conveyor speed vs inertia cost equations before dismissing.

**Omnidirectional ball / pop-up perpendicular conveyor**
An omnidirectional wheel or roller array that activates to push bricks sideways off the main belt. Weakness: inertia, requires brick to be nearly stationary or moving slowly relative to the crossbelt. Not viable at 200mm/s on a 2ft platform.

---

## Puller vs pusher solenoid timing optimization

For any solenoid-actuated mechanism, consider which direction the solenoid pushes vs which direction is spring-loaded. The puller variant: spring loads the active position, solenoid holds the neutral position. This means the active movement is instant (spring snap) rather than solenoid-speed. Retraction is solenoid-speed rather than spring-speed.

For our plow: the spring returns the arm to the clear position (neutral). The solenoid pushes to the divert position. This means divert motion is solenoid speed (~10ms), clear motion is spring speed (15-20ms target). The divert is on the critical path (needs to happen before brick arrives). The clear is not critical (brick has already passed). Current design is already optimized: fast direction = solenoid, return direction = spring.

If a future design needs faster return (spring) and slower extension (solenoid), invert the geometry.

---

## Engineering notebook as a decision driver

The notebook should guide decisions, not just record them. Every significant design choice should be preceded by: define the constraints, list the options, derive the winning option mathematically from the constraints. The notebook documents that derivation, not just the conclusion.

Judges should be able to trace every dimension, every threshold, every timing value back to an equation with variables and constants written out. If you changed a value, the notebook explains why: what measurement or data caused the revision.

Charts for everything: speed vs accuracy curve, lever geometry diagram, timing diagram for the sensing zone, power budget table, thermal model decay curve. Equations in the margin. Constants labeled.

---

## Wall power vs battery

For calibration sessions, a bench supply is more stable than LiPo. However it must never be used for logged runs or demo.

If the competition venue allows a wall outlet, a 5V 3A regulated wall adapter would be more stable than LiPo for demo. TSA competitions typically do not guarantee outlet access at the judging table. Assume battery only for competition.

The bulk capacitor on the logic rail (1000uF, see ELECTRICAL.md) partially addresses supply stability by buffering transients regardless of power source.

---

## Possible additions if time permits

These are lower priority and should only be attempted after the core system is reliable and all 10 reliability runs are logged.

- **PI speed controller** (fully detailed in EMBEDDED.md): closed-loop belt speed using slotted disk encoder
- **Web server logging** (in EMBEDDED.md): live JSON event feed over WiFi during calibration
- **Slow demo mode**: second button halves belt speed and slows animations. Useful during judge explanations
- **Brick counter on display**: large text showing bricks sorted out of 24, visible from across the table
- **Break-beam at chute exit**: confirms escapement fired and brick left the chute before entering the sensing zone, one more feedback point
