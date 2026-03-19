# Judge Q&A Preparation

Practice answering these out loud before the competition. Have someone ask them cold. Know the answer before they finish the question.

---

**"Where is your feedback loop?"**

Point to the IR beams at each bin entrance. The controller knows the expected destination for every brick. After routing, it waits up to 500ms for that specific bin beam to break. If the beam does not break, the system halts immediately and displays the error with the brick number and expected bin. At the end of the run, all four bin totals are verified against expected counts. The display shows live confirmation as it happens.

---

**"Why IR break-beams instead of ultrasonic for size detection?"**

The two brick lengths differ by 7.9mm (about 5/16 of an inch). Ultrasonic sensors have noise margins at close range that exceed this difference. You cannot reliably distinguish two objects that close in size. IR break-beams are digital: the beam is either broken or it is not. Zero noise floor. The measurement is a timing interval between two digital events, which the microcontroller measures with microsecond resolution.

---

**"Why solenoids instead of servos for the routing plows?"**

A servo needs 100 to 150 milliseconds to sweep and return. At our throughput, bricks are spaced 140 milliseconds apart. A servo cannot finish its return stroke before the next brick arrives. Solenoids pre-set in 10 milliseconds while the brick is still traveling from the sensor to the plow. By the time the brick arrives, the plow is already in position. Actuation is entirely off the critical path.

---

**"Why a stepper motor for the brick feeder instead of something simpler?"**

The stepper counts its own steps, which means it counts its own rotations. One full rotation releases exactly one brick. This is mechanically guaranteed by the cam geometry. Between releases, the coils stay energized at reduced current, so the queue of bricks cannot push back against the cam and cause a double-release. No encoder needed. No sensor needed. The motor is the sensor.

---

**"Why a gravity chute? Why not a powered feed?"**

Fewer moving parts means fewer failure modes. Gravity is reliable. The stepper cam at the base provides precise metering without anything mechanical above it. The only thing that can go wrong in the chute is a jam, which is immediately visible and recoverable.

---

**"What does your display actually show?"**

As each brick is classified, its silhouette animates down the chute graphic in the correct color (red or blue) and the correct shape (2x2 or 2x3 aspect ratio). The corresponding bin counter flashes and increments. A sidebar shows brick number, size, color, and destination. A small bar shows actuator thermal state in real time. At the end of the run, the screen shows total time, per-bin totals versus expected, and whether all confirmations succeeded. It is a live window into the system's internal decisions, not a decoration.

---

**"How does it handle back-to-back runs?"**

The system tracks actuator heat state between runs using an exponential decay model. If heat approaches a threshold from repeated firing, the release rate automatically reduces to protect the hardware. The system manages itself. Heatsinks on all solenoids provide passive cooling between cycles. A judge can request a second run immediately and the system will either run normally or tell you it needs a moment. It will not silently degrade or fail.

---

**"What is your throughput?"**

Five bricks per second, limited by the stepper escapement cycle time. All sensing and routing happens while bricks are in transit: color classification takes under 3 milliseconds, solenoid pre-set takes 10 milliseconds, microcontroller logic takes microseconds. None of these are in the critical path. The belt and escapement determine throughput.

---

**"What if a brick goes to the wrong bin?"**

The bin confirmation beams detect physical arrival, not classification. A brick that is misclassified but still arrives somewhere will trigger a wrong beam or no beam, which halts the system. A brick that is correctly classified but mechanically fails to arrive (jam, falls off belt) triggers a timeout on the expected beam, which halts the system. Both failure modes result in an immediate halt with error display. The system does not continue past an error.

---

**"How do you know it is accurate?"**

We ran at least 10 full 24-brick runs during calibration and logged all results to CSV via serial output. The per-bin totals, accuracy rate, run time, and transit time measurements from those runs are in our engineering notebook. The system also measures its own belt speed during every run by timing brick transit and comparing to the calculated value.
