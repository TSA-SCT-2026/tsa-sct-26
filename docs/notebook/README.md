# Engineering Documentation

Competition documentation goes here. Judges score this heavily. Build it from logged run data, not from memory.

Current execution priority is CAD-first validation for the simplified states build: conveyor handoff, servo chute selector, sensing station, and operator-facing clarity.

Notebook source of truth lives in `docs/notebook/` and run CSV data lives in `docs/runs/`.

## Short Terms

- Servo rotary chute selector: the MG995/MG996-class servo mechanism that points the chute at one of four bins
- Sensing station: the conveyor-mounted size and color sensing area
- Handoff: the transition from belt exit into the chute
- Route ready: the chute is at the calibrated target angle
- Full-run log: a CSV record of a 24-brick run

## Recommended Notebook File Set

Keep the notebook authored in this directory with one file per section:

- `decision_matrices.md`
- `rejected_alternatives.md`
- `calibration_records.md`
- `equations_and_models.md`
- `failure_mode_analysis.md`
- `performance_summary.md`
- `iteration_log.md`

## Decision Matrices

Each decision matrix documents what options were considered, what was chosen, and why each alternative was rejected with a specific technical reason.

Subsystems that need matrices:
- Architecture pivot: archived chamber release design vs simplified states conveyor and servo chute
- Break-beam size sensing rationale and rejected alternatives
- Color sensor and shroud approach
- Conveyor CAD source
- Frame material: wood vs 3D printed vs mixed
- Servo chute selector geometry
- Optional feed chute after manual one-at-a-time feed works

## Rejected Alternatives

Keep `rejected_alternatives.md` as a short branch list. Include:
- Archived chamber and release-gate sorter
- Automated compressed queue for states
- NEMA11 selector for states
- 2020 extrusion frame for states
- Downloaded NEMA17 conveyor as the first states path
- Two-stage size-then-color physical lane split

## Calibration Procedures

Write these after hardware exists. The procedure must specify hardware configuration, what to measure, how to determine the threshold, and how to verify it.

Color calibration done without the final shroud installed is not valid.

## Performance Data

Run at least 10 full 24-brick runs during reliability testing. Log all of them to CSV via serial output in `docs/runs/`.

Summarize:
- run number
- total time
- per-bin totals
- accuracy
- any errors
- corrective action after each failure

## Failure Mode Analysis

For each potential failure, document what it is, how likely it is, how it is detected, and what the system does.

Include:
- missed chute entry
- brick stuck in chute
- wrong servo angle
- size misclassification
- color misclassification
- belt stall or belt walking
- evaluator placement error
