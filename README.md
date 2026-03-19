# tsa-sct-26

TSA System Control Technology 2026 -- Automated Warehouse Sorting and Conveyor System.

Sorts 24 LEGO bricks by size and color into 4 bins on a conveyor, automatically, in under 10 seconds. Competition: May 1 2026.

## Start here

Read ARCHITECTURE.md for the full system overview, design philosophy, pipeline, and build schedule.

## Repo structure

```
tsa-sct-26/
  ARCHITECTURE.md          master system overview -- read this first
  MECHANICAL.md            physical design, all measurements, fabrication concerns
  ELECTRICAL.md            power architecture, wiring, protection components
  EMBEDDED.md              state machine, sensor logic, actuator control, firmware checklist
  BOM.md                   parts list with ordering info (order today)
  ISSUES.md                open technical questions with resolution paths
  IDEAS.md                 design alternatives analysis and exploratory concepts
  NOTEBOOK.md              engineering notebook checklist and timeline

  cad/                     all printable parts, organized by subsystem
  firmware/                ESP32 embedded firmware (PlatformIO project)
  docs/
    competition/           official problem statement and judge Q&A prep
    engineering/           decision matrices, calibration procedures, performance data
    runs/                  CSV logs from calibration and reliability runs
```

## Competition requirements summary

- 2ft x 2ft maximum footprint
- 24 LEGO bricks (12x 2x2, 12x 2x3) sorted by size then color into 4 bins
- Minimum: 2 sensors, manual start/stop, programmable controller, feedback loop, motorized conveyor, 1 automated sorting mechanism
- Full requirements: docs/competition/PROBLEM_STATEMENT.md
