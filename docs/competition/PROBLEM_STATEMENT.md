# Official Problem Statement: TSA System Control Technology 2026

> Copied verbatim from the official TSA event document. Do not edit.

---

## System Control Technology
### Automated Warehouse Sorting & Conveyor System

---

## PROBLEM STATEMENT

Modern distribution centers rely on highly automated system control solutions to process thousands of packages per hour with speed, accuracy, and safety. Companies such as Amazon use advanced conveyor systems, sensors, and programmable controllers to identify, sort, and route packages to their correct destinations while minimizing errors and downtime. A new company is striving to outperform existing distribution centers and is seeking a design team to help them reach their goal.

---

## DESIGN BRIEF

Competitors are challenged to design, construct, and demonstrate a scaled automated warehouse sorting and conveyor system that simulates how large-scale fulfillment centers (such as those operated by Amazon) use system control principles to manage logistics operations efficiently. The system must detect and separate packages by characteristics, make control decisions, and activate mechanical components to route packages accurately and safely.

- Your model working system should be no larger than 2ft x 2ft
- 24 packages will be sorted by size and color
- Packages should be sorted efficiently and promptly

**Total packages needed for demonstration:**

There are 24 packages that need to be sorted efficiently. Packages must first be sorted by size into 2 containers, and then separated again by color for a total of 4 individual containers.

| Brick | Part # | Dimensions (approx) | Count | Color split |
|-------|--------|---------------------|-------|-------------|
| 2x2 | #3003 | 0.6in x 0.6in x 0.5in (1.5cm x 1.5cm x 1.2cm) | 12 | 6 blue, 6 red |
| 2x3 | #3002 | 0.9in x 0.6in x 0.5in (2.3cm x 1.5cm x 1.25cm) | 12 | 8 blue, 4 red |

---

## SYSTEM REQUIREMENTS

The system must include:

**Input Components**
- At least two (2) sensors used to detect package characteristics (size, color, weight, or presence)
- A manual start/stop control

**Processing / Control**
- A programmable control device (PLC, microcontroller, or equivalent)
- Control logic that determines package routing based on sensor input
- A feedback loop that confirms successful sorting or identifies errors

**Output Components**
- A motorized conveyor system
- At least one (1) automated sorting mechanism (diverter, gate, arm, or ramp)

---

## Scoring breakdown

Evaluation is based on 100 points:
- **Inventor's Log: 20 points** - the engineering notebook. Shows design process, decisions derived from data, iteration history. Must be brought to conference day.
- **Solution to Problem: 60 points** - the working device. Accuracy, reliability, sensor integration, feedback loop, sorting mechanism quality, speed as a factor within this category.
- **Programming Structure: 20 points** - code quality. Clean architecture, logical organization, readable structure. This is explicitly scored. The firmware module structure matters.

## Event format: critical

**No interview. No on-site work. No team presence during evaluation.**

Teams have 15 minutes to set up the completed device and leave written operator instructions. Evaluators operate the device independently using those instructions. Top three announced at awards.

Implications for design and documentation:
- The device must work from a cold start operated by a stranger following written instructions. The team is not present to intervene.
- The display must communicate system state unambiguously without any context. "SORT COMPLETE", "ERROR", "READY" must be readable and obvious.
- Error recovery (jam, halt) must be possible by following the written instructions alone. Design the reset procedure to be simple.
- The operator instructions sheet is a required deliverable. It does not exist yet. See docs/competition/ for where to put it.
- The 15-minute setup constraint means the system should require no calibration on-site. Everything is tuned and locked before conference day.

## Notes for the team

- The 2ft x 2ft footprint constraint is hard. Current design targets 610mm x 610mm which is exactly 2ft x 2ft. Verify every major component placement fits before committing to frame CAD.
- "Sorted by size into 2 containers, then by color" describes the outcome, not the mechanism. The system is not required to sort size first mechanically. It just needs the right bricks in the right bins. Our single-pass approach (classify both attributes in one sensing zone, route directly to the correct bin) satisfies the requirement.
- "At least two sensors" is satisfied by the two IR break-beams for size alone. The color sensor and bin confirmation beams are additional. Exceeding this looks good.
- Manual start/stop is required. A button in the firmware is not optional.
- No school or individual names on the project. State Conference ID only on all parts (device, log, documentation).
