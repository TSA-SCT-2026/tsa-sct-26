# Rejected Alternatives

This file is a branch list of ideas that were considered and not selected for the active production path.
Keep it short and honest. Expand a branch only when there is a real decision history to record.

## Full designs

- Continuous conveyor with singulator and fixed downstream diverters
  - Singulator branch: escapement
  - Singulator branch: cam gate
  - Singulator branch: Geneva style indexer
  - Singulator branch: star wheel
  - Singulator branch: dual pin feed
  - Routing branch: stationary flipper array
  - Routing branch: short-travel diverter gate
  - Reason it was not selected: the feed still becomes the bottleneck and the singulation risk is too high for the clutched LEGO queue

- Solenoid flap routing
  - Flap branch: direct support flap
  - Flap branch: spring biased flap
  - Flap branch: flap with guide gate
  - Flap branch: flap with sensor assisted reset
  - Reason it was not selected: the flap has too much overlap risk and the reset path is harder to make deterministic than support removal

- Two-stage lane split
  - First split branch: size then color
  - First split branch: color then size
  - Routing branch: separate lanes for each size
  - Routing branch: separate lanes for each color
  - Reason it was not selected: it increases packaging pressure inside the footprint and adds hardware without removing the main reset bottleneck

- Direct-drive conveyor
  - Roller branch: motor shaft clamp
  - Roller branch: printed motor bore
  - Roller branch: unsupported roller shaft
  - Reason it was not selected: ratio changes are too limited and the production packaging story is weaker than the supported shaft timing-belt stage

- Downstream fixed diverter after trapdoor
  - Gate branch: short-travel flap
  - Gate branch: pop-up tab
  - Gate branch: fixed diverter wall
  - Reason it is not selected now: the selector chute remains the current active path, but the evidence gate can reopen this branch if timing or reliability demands it

## Subdesign notes

- Singulation ideas were rejected because they move the hard problem upstream and add a second source of jam risk.
- Routing ideas were compared by motion distance, reset time, packaging, and how easy a first-time operator can understand them.
- Conveyor support ideas were compared by ratio flexibility, shaft loading, and how much the motor mounting choice constrains the rest of the frame.
