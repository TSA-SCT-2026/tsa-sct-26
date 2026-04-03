# Engineering Documentation

Competition documentation goes here. Judges score this heavily. Build it from logged run data, not from memory.
Current execution priority is CAD-first validation, so begin with mechanical decision matrices and trapdoor test evidence.

Notebook source of truth lives in `docs/notebook/` and run CSV data lives in `docs/runs/`.

## Recommended notebook file set

Keep the notebook authored in this directory with one file per section:

- `decision_matrices.md`
- `calibration_records.md`
- `equations_and_models.md`
- `failure_mode_analysis.md`
- `performance_summary.md`
- `iteration_log.md`

## Decision matrices

Each decision matrix documents: what options were considered, what was chosen, and why each alternative was rejected with a specific technical reason. This is the format judges look for. The rationale already exists in the architecture files. Pull it into structured tables here.

Subsystems that need matrices:
- Size sensor (IR break-beam vs ultrasonic vs ToF vs camera)
- Color sensor (TCS34725 vs camera vs discrete photodiodes)
- Trapdoor release actuation (class 3 lever with solenoid vs direct plunger vs sliding support)
- Selector mechanism (rotary selector disc vs linear diverter vs hybrid gate)

## Calibration procedures

Write these after hardware exists. The procedure must specify: what hardware configuration is required (shroud installed, demo bricks, belt running), what to measure, how to determine the threshold, and how to verify it. Calibration done without the final hardware configuration is not valid.

## Performance data

Run at least 10 full 24-brick runs during reliability testing. Log all of them to CSV via serial output in `docs/runs/`. Summarize in a table: run number, time, per-bin totals, accuracy, any errors. This table is the core of the performance section.

## Failure mode analysis

For each potential failure: what is it, how likely, how is it detected, what does the system do. Format as a table. Include both detected failures (jam, solenoid misfire, sensor miss) and undetected failures (misclassification that lands in a bin) with explanation of why the latter is acceptable.
