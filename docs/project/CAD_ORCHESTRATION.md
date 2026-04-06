# CAD Orchestration Plan

Date frozen: April 6, 2026

Purpose:
- Lock the last system-level decisions that block CAD.
- Produce one exhaustive buy list with no hidden gaps.
- Define the exact work from now through April 10, 2026 so CAD can proceed without architecture drift.
- Capture datasheet cleanup work so received-hardware truth becomes CAD-ready instead of half-remembered. 

Use this file as the working handoff and execution plan.
Use `docs/project/BOM.md` as the compact buying summary.
Use `docs/project/BOM.xlsx` as the only truth for what is already ordered.

## Current frozen architecture

This is the active production direction:
- Bricks run long-side-across in the chamber and conveyor. For a 2x3 brick, `23.7mm` is across the conveyor and `15.8mm` is along travel.
- Active conveyor strip is `25mm x 3mm` neoprene.
- Feed chute stays parallel to the final chamber orientation in the first CAD pass. No orientation-swap transition in the first pass.
- Start gate is active from the start. It lives above the transition in the tall chute section.
- Start gate actuator is the owned small hobby servo first. The heavier servo is backup only.
- Conveyor timing stage baseline is `20T / 20T` with one optional `16T` experiment pulley.
- Conveyor drive uses a supported `5mm` shaft with `MR85ZZ` bearings and clamp collars.
- Belt support is an integrated printed flat support path in the first CAD pass.
- Interior low-friction liner is adhesive-backed UHMW tape.
- Chamber seat truth is a required stop-wall micro-switch.
- Release gate is a `0530`-class solenoid driving a retracting support.
- No dedicated release-reset switch in the initial build, but the first CAD pass must reserve a release-return flag and micro-switch mounting provision.
- Selector remains the active routing path.
- Selector drive is NEMA11 plus a rigid `5mm` flange-mount hub plus a metal bracket.
- Selector home truth is a required micro-switch.
- Buy one IO expander board now and reserve room for it in the electronics layout. Keep its exact usage flexible until the final IO map is frozen.

## Current system story

The intended run sequence is:
1. Evaluator loads all 24 bricks into the vertical chute.
2. Start gate holds the full queue off the transition before the run.
3. Evaluator presses the labeled start button.
4. Start gate opens and stays open for the run.
5. Conveyor advances one brick into the one-brick chamber.
6. Chamber seat switch confirms the brick is fully seated.
7. Size beams and color sensor read while the brick is static.
8. Selector indexes to the correct routed position.
9. Release support retracts by solenoid pulse.
10. Brick drops through the selector chute to the correct bin.
11. Reset truth is satisfied and the conveyor advances the next brick.

The architecture stays simple because:
- The start gate removes queue preload from the transition.
- The chamber handles one brick at a time by geometry, not by clever timing.
- The release actuator only removes support, then gravity does the work.
- The selector rotates directly instead of converting motion through extra linkage.

## Final buy list

This is the exhaustive buy list that should exist after the next order and hardware-store trip.

### Amazon: buy now

Keep the current cart items:
- UHMW tape: `KitchenRaku UHMW tape`
- Selector hub: `Befenybay 2PCS Black Rigid Flange Shaft Coupling ... for 5mm Shaft Pair with M3 Thread`
- Optional timing experiment pulley: `Saiper GT2 16 Teeth 5mm Bore for 6mm Belt`
- Selector bracket: `STEPPERONLINE ST-M3`
- Baseline timing pulleys: `WINSINN GT2 20 Teeth 5mm Bore for 6mm Belt`
- Start button: `STARELO 12mm momentary push button`
- Power switch: `Twidec round rocker on/off switch`
- Supported drive shaft: `uxcell 5mm x 200mm hardened linear shaft`
- Shaft collars: `PATIKIL 5mm double split clamp collars`
- Long timing belt candidate: `uxcell 208-2GT-6 belts`
- Short timing belt candidate: `200mm 2GT 6mm belts`

Add these missing Amazon items:
- One small micro-switch pack for selector home and chamber seat truth, with extras as spares
- One IO expander board, preferably a simple I2C GPIO expander module

### Hardware store: buy now

- M3 screws, nuts, and washers for:
  - selector hub mounting
  - selector home switch mount
  - chamber seat switch mount
  - release support prototype hardware
  - general prototype fastener stock
- One M5 bolt, washers, and locknut for the idler axle
- Small rubber pad or bumper for the chamber stop wall
- Rubber contact cement or another proven neoprene splice adhesive for closing the transport belt loop
- Base plate stock if `610mm x 610mm x 6mm` sheet material is not already available from school stock or scrap
- Optional anti-slip feet only if they do not complicate packaging

### Already owned or already ordered

- NEMA17 stepper `17HS4401S`
- NEMA11 stepper
- TMC2209 drivers
- `0530` solenoids
- MR85ZZ bearings
- 608ZZ bearings as spare stock only
- Break-beam sets
- TCS3200 GY-31 color sensor modules
- Small hobby servo for start gate
- MG995 or MG996R class servo as backup only
- ESP32 dev board
- Waveshare 2 inch LCD
- LiPo battery and charger
- LM2596 buck converters
- 4700uF bulk capacitors
- 1N4007 diodes
- IRLZ44N MOSFETs
- Spring assortment
- Wire
- solder
- inline fuse holder
- cable sleeve and cable ties
- perfboard
- PLA filament
- neoprene strips in both `15mm x 3mm` and `25mm x 3mm`

### Do not buy now

- Additional bearings
- Separate metal belt bed stock
- Separate timing-belt tensioner hardware
- New start-gate servo
- Release-gate servo
- Additional conveyor ratio hardware beyond the optional `16T`
- AliExpress polish items

### Notes on the final buy list

- The current cart is good and should be kept.
- The only missing Amazon items are the micro-switch pack and the IO expander board.
- The local hardware gap is M3 hardware, M5 idler hardware, the chamber stop pad, neoprene splice adhesive, and base plate stock if school stock is not already assigned.
- The `608ZZ` bearings are not part of the active system. Keep them as spare stock and do not design around them.

## Subsystem-by-subsystem part justification

### 1. Start gate

Purpose:
- Hold the full queue off the transition before the run.
- Reduce wedge risk in the highest-risk feed region.

Chosen concept:
- Side-sweep rotary gate above the transition.
- Flush or near-flush retract when open.
- Parallel chute orientation in the first CAD pass.

Parts:
- Owned small hobby servo
- Printed servo bracket
- Printed gate arm
- Servo horn and horn screw
- M3 or small servo-mount screws

Why this is the simplest correct choice:
- Moves only twice per run
- Keeps moving hardware out of the low-clearance throat
- Prevents the queue from preloading the transition
- Lets the tight throat stay geometric instead of actuator-dependent

Risk note:
- If the small servo stalls under full queue load, switch to the heavier servo without changing the gate concept.

### 2. Timing belt to roller conveyor

Purpose:
- Advance bricks into the chamber with controllable speed and supported shaft geometry.

Chosen concept:
- NEMA17 to GT2 timing belt to supported `5mm` shaft to smooth printed drive roller to neoprene strip.

Parts:
- NEMA17 stepper
- `20T` motor pulley
- `20T` drive pulley
- Optional `16T` experiment pulley, not required for the first prototype
- `100T` belt candidate
- `104T` belt candidate
- `5mm x 200mm` shaft
- `2x` clamp collars
- Printed drive roller
- Printed idler roller
- `MR85ZZ` bearings
- M5 idler bolt, washers, locknut
- M3 clamp screws and nuts
- `25mm x 3mm` neoprene strip
- Printed flat support path
- UHMW tape on brick-contact faces

Why this is the simplest correct choice:
- `20T / 20T` is common, easy to source, and easy to understand
- Supported shaft avoids loading the motor shaft directly
- Optional `16T` gives a mild speed-up path later without redesigning the whole stage
- Printed support path is simpler than adding separate bed stock before proof is needed

What remains runtime-tunable:
- Final installed belt choice: `100T` or `104T`
- Final motor slot position
- Final conveyor speed inside the chosen hardware envelope
- Whether the optional `16T` ever gets installed after baseline data exists

### 3. Chamber and sensing

Purpose:
- Enforce one-brick occupancy
- Stop the brick repeatably
- Measure size and color while static

Chosen concept:
- One-brick chamber keyed to long-side-across geometry
- Required stop-wall micro-switch for seat truth
- Break-beam size sensing
- TCS3200 with installed shroud for color sensing

Parts:
- Printed chamber body
- Stop-wall rubber pad
- Stop-wall micro-switch
- Break-beam sensors and mounts
- TCS3200 module and mount
- Printed sensor shroud
- M3 mounting hardware

Why this is the simplest correct choice:
- Seat truth is physical, not guessed from time
- Break-beams and color sensor read while the chamber is static
- The chamber does the hard work of singulation by geometry

Warning:
- Chamber geometry is still the main remaining CAD derivation. That is the highest-value design work left before full prototype lock.

### 4. Release gate

Purpose:
- Remove support from exactly one seated brick so it falls cleanly.

Chosen concept:
- Solenoid-driven retracting support
- No hinged platform
- No dedicated release-reset switch in the initial build
- Reserve a release-return flag and micro-switch mount so physical reset truth can be instrumented without redesign

Parts:
- `0530` solenoid
- Printed support finger or retracting ledge
- Printed solenoid mount or guide
- Light return spring
- MOSFET driver
- Flyback diode
- Small hardware for guide or pivot as needed
- Optional use of one spare micro-switch from the switch pack if physical reset truth can not be proven another way

Why this is the simplest correct choice:
- Solenoid is faster than a servo for per-brick release
- The actuator only removes support and then gravity handles the drop
- Avoids the added complexity of a full moving floor

Warning:
- Do not assume the full closed support span equals the minimum clean withdrawal distance. That must be measured from prototypes.
- Do not call the release subsystem production-ready until `RESET_CONFIRMED` comes from real physical support-return truth.

### 5. Selector chute

Purpose:
- Route the dropped brick into one of four bins.

Chosen concept:
- Directly indexed 4-position selector chute
- NEMA11 rotates the chute through a rigid flange hub
- Required selector home micro-switch

Parts:
- NEMA11 stepper
- ST-M3 metal bracket
- Rigid `5mm` flange-mount hub with M3 face mounting
- Printed selector chute
- Selector home micro-switch
- Small screws for hub-to-chute mount
- Small screws for switch and bracket mount

Why this is the simplest correct choice:
- No extra shaft required
- No printed press-fit bore pretending to be a coupling
- Direct indexing is easier to reason about than a compound linkage
- Home switch gives deterministic truth after power-up and after faults

### 6. Operator controls and electronics

Purpose:
- Make the system easy for an evaluator to operate and safe to power.

Chosen concept:
- Separate labeled start button and power switch
- Buy the IO expander now and reserve room for it even if the first wiring draft delays using it

Parts:
- Real momentary start button
- Real on/off power switch
- ESP32
- LCD
- Battery
- Charger
- Buck converter
- Fuse holder
- Bulk capacitor
- drivers, MOSFETs, diodes
- optional IO expander board

Why this is the simplest correct choice:
- Evaluator-facing controls are obvious
- The IO expander removes late-stage pin-pressure risk without forcing a firmware-first design decision today

## Optimization plan integration

Source: `docs/OPTIMIZE_PLAN.log`

### Keep as written

- Restart feed on `RESET_CONFIRMED`, not on bin arrival
- Split release timing into `release_to_reset_ms` and `release_to_chute_clear_ms`
- Keep selector evidence gate active until real motion data proves the selector is good enough
- Start timing-stage runtime work from `20T / 20T`
- Model steady-state throughput from chamber pitch, not full-conveyor travel

### Keep, but narrow the interpretation

- `CHUTE_CLEAR` remains an optimization truth, but not a mechanical feature to overbuild in the first CAD pass
- `BIN_CONFIRMED` is for scoring and fault detection, not feed restart
- Selector radius reduction is good only if outlet separation and routing robustness stay intact
- Release support span optimization is good only if it still gives repeatable support under queue load
- Belt-speed formulas are planning tools, not proof

### Drop or defer for now

- Reopening the `25mm` strip decision: dropped
- Reopening upstream singulation complexity: dropped
- Freezing `16T / 20T` as architecture before runtime data: dropped
- Forcing overlap-heavy control assumptions into the base build before data: deferred

## Datasheet library audit

Status:
- The datasheet tree is not CAD-ready yet.
- The worst gaps are the solenoid, color sensor, break-beam, and TMC2209 notes.
- The closest to usable are NEMA17, NEMA11, and the fan, but even those still miss key CAD fields.

### What must be added before April 10

Add one structured truth block to every active part folder:
- Exact purchased name
- Spreadsheet trace if applicable
- Received status
- Source of dimensions
- Date checked
- CAD-ready yes or no

Per-part additions:

Break beam:
- Remove contradictory electrical claims
- Add body `L x W x H`
- Add hole size and hole-center location if present
- Add emitter and receiver lead/polarity table
- Add verified operating voltage and current
- Add cable length, cable exit, and connector style

Color sensor:
- Confirm exact module identity from the received board
- Add board `X`, `Y`, and thickness
- Add mounting-hole diameter and pitch
- Add header height
- Add sensor aperture center relative to holes
- Add LED and shroud keepout
- Add measured operating standoff for calibration

NEMA11:
- Confirm exact model from the label
- Add face size
- Add pilot boss diameter and height
- Add 4-hole pattern and thread depth
- Add shaft diameter, flat, and tolerance
- Add wire-order or coil-pair mapping

NEMA17:
- Add 31mm hole pattern and M3 depth
- Add pilot boss diameter
- Make the `5mm` shaft truth explicit and separate it from the `4.5mm` flat-related dimension
- Add connector type and coil pin order
- Add rear connector keepout

Solenoid:
- Confirm real coil voltage
- Add coil resistance or current draw
- Add mounting-hole pattern
- Add lead exit side and lead length
- Add rest and full-stroke plunger envelope
- Add duty-cycle or thermal note if known
- Add flyback-diode requirement note

Fan:
- Add hole pitch
- Add airflow direction
- Add connector type and polarity
- Add cable exit side
- Resolve the bearing-type contradiction

TMC2209:
- Add board `X` and `Y`
- Add heatsink height
- Add board pin orientation
- Add current-set method for the actual module family
- Add chosen microstep baseline
- Add mandatory bulk-capacitor note
- Add cooling note

Add missing active part notes:
- `MR85ZZ`
- `608ZZ`
- Ordered `5mm` shaft
- Ordered `5mm` collars
- Ordered `GT2` pulleys
- Ordered `GT2` belts

Cleanup work:
- Rename generic asset files like `image.png`, `specs.png`, `schematic.png`, `steps.png`, and `wiring.png`
- Remove generic marketplace filler from README notes
- Keep project-specific MCU wiring out of datasheet notes unless it is part of the part truth

## Exact execution plan

### April 6, 2026: lock the system and place orders

Human:
1. Place the Amazon order from the final buy list.
2. Add the missing micro-switch pack and IO expander board to the current cart.
3. Make the hardware-store run for M3 hardware, M5 idler hardware, the stop-wall rubber pad, neoprene splice adhesive, and base plate stock if school stock is not already assigned.
4. Save the exact ordered product names or screenshots for:
   - pulleys
   - belts
   - shaft
   - collars
   - selector hub
   - bracket
   - switch kit
   - UHMW tape
5. Put those saved order references into the datasheet library or a staging note for later cleanup.

Sub-agent lane:
1. Normalize `BOM.md`, `OPEN_DECISIONS.md`, `ARCHITECTURE.md`, `MECHANICAL.md`, and `DIMENSIONS.md` around the frozen decisions.
2. Produce one final gap audit against the physical system story, not the firmware placeholder state.
3. Draft the part-truth README templates for the datasheet folders.

### April 6 and April 7, 2026: finish CAD-ready truth capture

Human:
1. Verify the received-part notes for:
   - NEMA17
   - NEMA11
   - solenoid
   - break beam
   - color sensor
   - fan
   - TMC2209
2. For each one, add the missing CAD fields listed above.
3. Add notes for `MR85ZZ` and `608ZZ`.
4. Remove contradictory lines from the weakest notes instead of keeping both values around.

Sub-agent lane:
1. Refine the human’s raw datasheet notes into the standard structured format.
2. Flag any remaining missing datum that would block mounting or envelope CAD.
3. Rename low-traceability files in the datasheet tree.

### April 7 and April 8, 2026: first CAD pass on the critical path

Human and CAD owner:
1. Start with the chute and start-gate geometry.
2. Keep the chute parallel to the final chamber orientation.
3. Design the start gate as a side-sweep paddle above the transition.
4. Build the chamber around the long-side-across rule.
5. Reserve the stop-wall switch mount in the chamber.
6. Reserve the color-sensor shroud envelope from the beginning.
7. Build the release support envelope around the solenoid family, but keep the actual retracting support geometry prototype-friendly.

Sub-agent lane:
1. Derive the chamber and sensing geometry from the long-side-across brick rule.
2. Cross-check dimensions against the active docs so no old `15mm`-channel logic leaks back in.
3. Review CAD notes for hidden interface conflicts before parts are printed.

### April 8 and April 9, 2026: timing stage and selector packaging

Human and CAD owner:
1. Package the `20T / 20T` timing stage around the supported shaft.
2. Include slot travel that can absorb both `100T` and `104T` belt candidates.
3. Use an integrated printed flat support path for the belt.
4. Add UHMW tape surfaces where bricks contact printed walls.
5. Package the selector bracket, hub, and home-switch mount.
6. Keep the selector chute body light and compact, but do not shrink it past clean routing geometry.

Sub-agent lane:
1. Re-check the timing-stage geometry against the ordered shaft, collars, and pulley envelope.
2. Re-check selector hub and bracket assumptions against the chosen part style.
3. Audit the prototype print order for blockers.

### April 9 and April 10, 2026: prototype package ready to print and test

Human and CAD owner:
1. Finish the print-ready versions of:
   - chute transition with start gate
   - chamber shell
   - release support prototype
   - drive roller and idler
   - motor mount and shaft supports
   - selector chute and selector bracket interfaces
2. Export STL or print-ready artifacts.
3. Prepare the print-and-test order.

Required print-and-test order for April 10:
1. Chute transition and start-gate parts
2. Shaft-fit and bearing-fit coupons
3. Drive roller and idler
4. Chamber shell with seat switch mount
5. Release support prototype
6. Selector chute and selector hub interface
7. Larger frame pieces only after the high-risk parts pass

Definition of success by April 10:
- The BOM is ordered and gap-free for the first prototype build.
- The architecture docs no longer conflict with the chosen system.
- Datasheet notes are CAD-useful for the active parts.
- Prototype CAD exists for every critical subsystem.
- The remaining open work is prototype-derived geometry refinement and runtime tuning, not hidden hardware surprises.
- April 10 starts as a print-and-test day, not another architecture day.

## Human work versus sub-agent work

### Human-only work

- Order parts
- Verify received hardware and take measurements
- Decide whether a contradictory datasheet value is actually wrong
- Make calls on physical fit when a photo or raw part is the truth source
- Own final CAD shape decisions

### Best sub-agent work

- Repo-wide doc consistency sweeps
- Datasheet note cleanup and normalization
- Dimension cross-checking across docs
- BOM gap audits
- Independent architecture audits using only the system flow and proposed parts
- CAD checklist generation and print-order review

### Avoid delegating

- Physical measurements
- Purchase approval
- Final geometry judgment when a real part in hand is the deciding factor

## Rejected and deferred alternatives that matter

Rejected for the first CAD pass:
- Perpendicular loading chute with orientation swap at the transition
- Upstream escapement or star-wheel singulation
- Hinged release platform
- Separate metal belt bed
- Defaulting to the heavy servo for the start gate
- Driving the selector from a printed bore

Deferred, not rejected:
- `16T / 20T` timing experiment after runtime data
- Selector overlap timing optimization after chute-clear data
- Reopening downstream routing alternatives if selector timing disappoints

## Warnings and rules

- The chute transition remains the highest-risk print. Prototype it first.
- Do not let `15mm` conveyor logic sneak back into any active doc or CAD file.
- Do not treat PTFE thread-seal tape as a wear surface.
- Do not use tiny PCB tact switches as evaluator-facing controls.
- Do not let the selector home switch drift into "nice to have" status. It is required.
- Do not build the release gate around guessed travel. Measure clean withdrawal once prototypes exist.
- Do not reopen timing-stage ratio work until the base architecture is physically running and logged.
- Do not let placeholder firmware drive hardware purchases or geometry.

## What I would have done earlier

- I would have created `OPEN_DECISIONS.md` sooner so stale alternatives stopped crowding the main architecture docs.
- I would have frozen the long-side-across rule and `25mm` strip width sooner because they drive too much downstream geometry to leave ambiguous.
- I would have treated the datasheet library as CAD infrastructure from day one instead of optional documentation.

## Final checkpoint

If the steps above are followed, then by April 10, 2026:
- the system architecture is frozen enough for productive first-pass prototype CAD
- all missing hardware is ordered or assigned to a hardware-store trip
- the datasheet library is useful for packaging and mounting
- the first prototype print sequence is already prioritized
- the work left is print, fit, refine, and lock the still-provisional geometry from real results instead of reopening random system-level ideas
