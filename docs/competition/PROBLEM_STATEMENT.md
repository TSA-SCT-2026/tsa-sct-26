# Official Problem Statement -- TSA System Control Technology 2026

> Copied verbatim from the official TSA event document. Do not edit.

---

## System Control Technology
### Automated Warehouse Sorting & Conveyor System

---

## PROBLEM STATEMENT

Modern distribution centers rely on highly automated system control solutions to process thousands of packages per hour with speed, accuracy, and safety. Companies such as Amazon use advanced conveyor systems, sensors, and programmable controllers to identify, sort, and route packages to their correct destinations while minimizing errors and downtime. A new company is striving to outperform existing distribution centers and is seeking a design team to help them reach their goal.

---

## DESIGN BRIEF

Competitors are challenged to design, construct, and demonstrate a scaled automated warehouse sorting and conveyor system that simulates how large-scale fulfillment centers -- such as those operated by Amazon -- use system control principles to manage logistics operations efficiently. The system must detect and separate packages by characteristics, make control decisions, and activate mechanical components to route packages accurately and safely.

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

## Notes for the team

- The 2ft x 2ft footprint constraint is hard. Current design targets 610mm x 610mm which is exactly 2ft x 2ft. Verify every major component placement fits before committing to frame CAD.
- "Sorted by size into 2 containers, then by color" describes the outcome, not the mechanism. The system is not required to sort size first mechanically -- it just needs the right bricks in the right bins. Our single-pass approach (classify both attributes in one sensing zone, route directly to the correct bin) satisfies the requirement.
- "At least two sensors" is satisfied by the two IR break-beams for size alone. The color sensor and bin confirmation beams are additional. Exceeding this looks good.
- Manual start/stop is required. A button in the firmware is not optional.
