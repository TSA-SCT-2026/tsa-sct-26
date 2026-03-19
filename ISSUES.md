# Open Issues

Unresolved technical questions, decisions, and logistics that need to be addressed before specific build phases. Not the same as risks (those are in ARCHITECTURE.md) -- these are specific open items with a resolution path.

---

## Before frame CAD (before April 2)

**Top-down footprint layout**
Draw a to-scale top-down view of the full system: belt, bins, chute footprint, frame edges. Verify everything fits within 610mm x 610mm. This is a 30-minute task. Do not start frame CAD without it. Bins diverting sideways are the main unknown -- two bins per side, each ~80mm wide x 100mm deep, need to confirm they fit alongside the belt run.

**Exact bin positions**
The exact distance from the sensing zone to each of the 4 bin entrance beams is needed for:
1. PI controller Option B (transit time / distance = speed)
2. Solenoid de-energize timing (currently estimated at 280ms, needs actual measurement)
3. Bin confirmation timeout (currently 500ms estimate, needs to be set from measured transit times)

Determine these in dry assembly. Log them. Update EMBEDDED.md and ELECTRICAL.md with actual values.

---

## During sensor validation (early April)

**Belt speed at operating voltage**
The TT gearmotor spec gives a no-load RPM. Actual belt speed under load at 5V with the selected PWM duty cycle is not known. Measure it during breadboard validation (run motor, manually time a brick over a known distance). If actual speed differs significantly from 200mm/s, recalculate: size detection threshold, solenoid de-energize timing, color dwell time, brick spacing.

**Belt friction with bricks**
Verify bricks do not slide sideways on the smooth GT2 belt surface during plow actuation. The plow tip pushes laterally against a moving brick -- if belt-to-brick friction is too low, the brick may spin rather than deflect cleanly. Test during first powered belt run. If sliding is a problem, lightly textured belt surface or slightly slowing the belt during plow contact are options.

**L298N PWM frequency**
Test at 1kHz, 5kHz, and 10kHz. Observe: motor response, audible noise, L298N temperature under sustained operation. Pick the frequency that gives smooth control with acceptable heat. Document the result.

**LiPo capacity under load**
Run 5 back-to-back full 24-brick runs and note voltage at start and end. Estimate how many runs a full charge supports. Competition may involve 3-5 judge demo requests in sequence. If one charge cannot cover a session, plan a charging break or verify the second LiPo can be swapped quickly.

---

## During integration (April 9-14)

**I2C stability under load**
If color sensor drops reads when belt motor and solenoids are active (EMI, ground noise), add 4.7k pullup resistors on SDA and SCL. Diagnose before assuming the sensor is broken.

**Timer allocation on ESP32**
Track which hardware timers are used as firmware is written. ESP32 has 4 hardware timers. Minimum needed: 1 for size detection, 1 for solenoid scheduling. The pulse counter peripheral handles encoder counting separately. Leave at least 1 timer unallocated as reserve.

---

## Competition power source (resolve before ordering second LiPo)

**AC outlet availability at the judging table**
The scoring does not require battery power. A bench supply at competition is more reliable than LiPo -- no voltage sag, no depletion across back-to-back demos, simpler two-rail isolation. But it requires AC power at the judging table, which is not guaranteed at TSA state/nationals venues.

Resolution path: contact event organizers or find a past competitor who can confirm whether outlets are available at individual judging stations. If yes, use a bench supply for competition and skip the second LiPo. If no or unknown, stick with LiPo.

Either way, use the bench supply for all development and calibration.

## Competition logistics (before April 29)

**Reload time**
Practice reloading 24 bricks from bins back into the chute in under 30 seconds. Judges may request a second immediate demo. The 20-second load estimate assumes smooth operation. Practice this as a deliberate drill, not an afterthought.

**Error recovery procedure**
If the system halts on ERROR_HALT during a judge demo: know exactly what to do. The procedure should be: identify the jammed brick, clear it, reset the system, reload as needed. Walk through this scenario at least twice before competition so it looks practiced, not panicked.

**Laptop for serial monitor**
Bring a laptop configured to open serial immediately. During demo, serial output shows internal state which can answer judge questions in real time ("what did it classify that brick as?"). Also useful for diagnosing any unexpected behavior.
