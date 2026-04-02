# Ideas and Design Exploration

Scratchpad for concepts that are exploratory, not yet architecture. Some of these informed current design decisions. Some may still be relevant if a subsystem needs to be reconsidered.

---

## Belt mechanism selection

There are exactly two coherent belt strategies for this application:

1. **Toothed belt on toothed pulleys** - positive drive, speed mathematically deterministic.
   Requires printed GT2 pulleys with accurate 0.75mm tooth profiles, belt splice with tooth
   alignment. Fabrication risk is non-zero but manageable (droftarts generator is proven).

2. **Flat belt on smooth rollers** - friction drive, needs speed feedback. Best transport
   surface, simplest fabrication (smooth cylinders, flat splice). Hall sensor + PI loop
   closes the speed determinism gap at the cost of ~30 lines of firmware.

"Toothed belt on smooth rollers" is not a coherent strategy: GT2 teeth on a smooth PLA
roller contact only at tooth tips (discrete, deformable neoprene pads with air gaps).
The Euler belt equation requires continuous surface contact. Effective mu is lower than
a flat belt on the same roller. This approach has the downside of friction drive (slip)
without the upside of positive drive (teeth engaging pulleys). Rejected.

**Current design: Strategy 2 (flat neoprene belt, Hall PID)**
Neoprene rubber strip (~25mm wide) on smooth printed rollers. Motor D-shaft rigid coupling
to drive roller (zero slip at that joint). A3144 Hall sensor + 2 magnets on idler roller
feed a PI loop that corrects speed drift from voltage sag and micro-slip. Transport surface
is smooth neoprene - better grip on ABS bricks than GT2 backing.

**Fallback if friction test fails (day-1):**
Add heat-shrink sleeve to drive roller, then rough with 120-grit. If still failing: GT2
hybrid (6mm GT2 belt connects motor to roller shaft with positive drive, neoprene belt
wraps rollers for transport). This bounds all slip to the roller-belt interface.

Rejected immediately: gravity rollers (LEGO bottom geometry catches), linear pusher
(serial: 70+ sec for 24 bricks), O-ring cord (line contact, co-planarity problems).

---

## Alternative diverter mechanisms (considered during design)

Every design was evaluated against the core constraint: sort at 200mm/s belt, 179ms timing window.

**Direct solenoid pusher: current design**
Solenoid rod pushes a face plate directly into the brick. Spring return passive. 40ms on-time.
No lever arm, no pivot. Simpler and more reliable than lever designs. Chosen.

**Solenoid plow with lever arm (considered, rejected)**
Lever amplifies short solenoid stroke to longer plow tip travel. More parts, more failure modes.
Direct pusher achieves adequate travel without a lever. Rejected in favor of direct push.

**Servo flap**
Weakness: 100-150ms sweep and return time. Eats the entire inter-brick window. Throughput drops to 2-3 bricks/second. Not viable at target speed.

**Solenoid puller / fast retractor**
Spring does the divert, solenoid holds retracted (neutral) and releases to snap. Spring snap is faster than solenoid pull. Rejected: solenoid is energized most of the time, inverting the heat profile. Current design has solenoids off by default (thermally better for the 33% default-path bricks).

**Paddle wheel / continuous rotation**
Weakness: timing is mechanically coupled to belt speed, requires synchronization. One paddle per bin path, significant footprint. No selective control without a clutch. See ARCHITECTURE.md for full analysis.

**Pop-up gate (pin through belt)**
Weakness: GT2 belt cannot be slotted without structural failure. Requires a second mechanism to clear the brick. Physically stops bricks rather than deflecting them, which means bounce and jam risk.

**Track diverter (physical rail switches)**
Weakness: inertia cost on both brick and mechanism. Like a pop-up gate but larger footprint. Mechanically interesting but slower than solenoid plow.

**Conveyor -> solenoid queued tilting platform**
A short platform between belt segments that tilts to route the brick to one side. Weakness: cost of inertia (needs a lip to slow brick down before tilt), gravity-friction speed limits return, more moving mass than a plow arm. Complex to mount at belt speed.

**Diagonal conveyors that reverse to collect**
Multiple angled belts that redirect by reversing. Weakness: inertia at belt transitions, significant footprint and complexity. Worth revisiting only if current design hits a throughput wall.

**Omnidirectional ball / pop-up perpendicular conveyor**
An omnidirectional wheel or roller array that activates to push bricks sideways off the main belt. Weakness: inertia, requires brick to be nearly stationary or moving slowly relative to the crossbelt. Not viable at 200mm/s on a 2ft platform.

---

## Puller vs pusher solenoid timing optimization

For any solenoid-actuated mechanism, consider which direction is solenoid-driven vs spring-loaded. The puller variant: spring loads the active position, solenoid holds the neutral position. Active movement is instant (spring snap); retraction is solenoid-speed.

For the direct pusher: the solenoid fires the push (fast, ~10ms), spring returns the rod to rest (passive). The push is on the critical path (must reach the brick in the right window). The return is not critical (brick has already been deflected). This is already optimized: fast direction = solenoid, return direction = spring.

If a future design needs faster return and slower extension, invert the geometry.

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
