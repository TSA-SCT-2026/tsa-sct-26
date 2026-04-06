# Engineering Documentation

Competition documentation goes here. Judges score this heavily. Build it from logged run data, not from memory.
Current execution priority is CAD-first validation, so begin with mechanical decision matrices and release-gate test evidence.

Notebook source of truth lives in `docs/notebook/` and run CSV data lives in `docs/runs/`.

## Short terms

- Selector chute: the 4-index routing mechanism that indexes to the target bin
- Chamber pitch: the feed distance needed to hand off the next brick after the chamber clears
- Selector-ready: the routing path is aligned and locked, so release is allowed
- Reset confirmation: the release mechanism is back in a safe state, so the next feed is allowed

## Recommended notebook file set

Keep the notebook authored in this directory with one file per section:

- `decision_matrices.md`
- `rejected_alternatives.md`
- `calibration_records.md`
- `equations_and_models.md`
- `failure_mode_analysis.md`
- `performance_summary.md`
- `iteration_log.md`

## Decision matrices

Each decision matrix documents: what options were considered, what was chosen, and why each alternative was rejected with a specific technical reason. This is the format judges look for. The rationale already exists in the architecture files. Pull it into structured tables here.

Subsystems that need matrices:
- Size sensor (IR break-beam vs ultrasonic vs ToF vs camera)
- Color sensor (TCS3200 GY-31 vs alternative color modules vs camera vs discrete photodiodes)
- Release actuation (servo-swept support vs direct cam vs coil pullback)
- Selector mechanism (4-index selector chute vs linear diverter vs hybrid gate)
- Conveyor drive topology (active off-axis timing-belt stage, supported-shaft layout, and rejected alternatives)

## Rejected alternatives

Keep `rejected_alternatives.md` as a short branch list. Use bullet sections for full designs first, then subdesigns inside each branch.

Include at least these branches:
- Continuous conveyor with singulator and fixed downstream diverters
- Two-stage lane split with size first then color
- Solenoid flap routing after release
- Direct-drive conveyor with unsupported motor shaft
- Rotary selector replacement concepts that were examined and rejected

Each branch should record the subdesigns that were considered under it, such as singulator style, gate style, shaft support style, or routing layout.

## Calibration procedures

Write these after hardware exists. The procedure must specify: what hardware configuration is required (shroud installed, demo bricks, belt running), what to measure, how to determine the threshold, and how to verify it. Calibration done without the final hardware configuration is not valid.

## Performance data

Run at least 10 full 24-brick runs during reliability testing. Log all of them to CSV via serial output in `docs/runs/`. Summarize in a table: run number, time, per-bin totals, accuracy, any errors. This table is the core of the performance section.

Make sure the selector gate study is represented in the notebook with modeled and measured routing latency, adjacent-move time, worst-case move time, homing penalty, and the decision rule that keeps the selector chute active unless the evidence says it cannot win.

## Failure mode analysis

For each potential failure: what is it, how likely, how is it detected, what does the system do. Format as a table. Include both detected failures (jam, release-actuator failure, sensor miss) and undetected failures (misclassification that lands in a bin) with explanation of why the latter is acceptable.
