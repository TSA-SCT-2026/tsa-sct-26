# Engineering Documentation

Competition documentation goes here. Judges score this heavily. Build it from logged run data, not from memory.

## What goes here

```
docs/engineering/
  decision_matrices/    one file per subsystem -- sensor choice, actuator choice, escapement choice
  calibration/          calibration procedures for size threshold and color threshold
  equations/            all math: lever geometry, belt speed, brick spacing, thermal model
  failure_modes/        failure mode analysis table
```

## Decision matrices

Each decision matrix documents: what options were considered, what was chosen, and why each alternative was rejected with a specific technical reason. This is the format judges look for. The rationale already exists in the architecture files -- pull it into structured tables here.

Subsystems that need matrices:
- Size sensor (IR break-beam vs ultrasonic vs ToF vs camera)
- Color sensor (TCS34725 vs camera vs discrete photodiodes)
- Plow actuation (solenoid vs servo vs paddle wheel vs pop-up gate)
- Escapement mechanism (stepper cam vs dual-pin solenoid vs Geneva drive vs rotary servo)

## Calibration procedures

Write these after hardware exists. The procedure must specify: what hardware configuration is required (shroud installed, demo bricks, belt running), what to measure, how to determine the threshold, and how to verify it. Calibration done without the final hardware configuration is not valid.

## Performance data

Run at least 10 full 24-brick runs during reliability testing. Log all of them to CSV via serial output. Summarize in a table: run number, time, per-bin totals, accuracy, any errors. This table is the core of the performance section.

## Failure mode analysis

For each potential failure: what is it, how likely, how is it detected, what does the system do. Format as a table. Include both detected failures (jam, solenoid misfire, sensor miss) and undetected failures (misclassification that lands in a bin) with explanation of why the latter is acceptable.
