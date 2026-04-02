# Developer Notes

This is a TSA System Control Technology nationals project. Competition: May 1 2026. The system sorts 24 LEGO bricks by size and color into 4 bins, automatically, in under 10 seconds, on a conveyor.

The goal is first place at nationals. Not participation. Not top five. First. Assume that level of ambition in every decision.

Scoring is 100 points: Inventor's Log (20), Solution to Problem (60), Programming Structure (20). The dominant category is the device working correctly. Speed matters inside the 60-point solution category but reliability, accuracy, documentation, and code quality together outweigh raw throughput. Do not sacrifice reliability for speed.

The event has no interview and no team presence during evaluation. An evaluator who has never seen the system operates it alone using written instructions. Every design decision that affects whether a stranger can run this correctly is a scoring decision.

## How to work on this project

Assume nothing. Every design decision is open to revision if a better argument exists. Question the obvious. Chase simplicity: the best solution usually does more with less. If you see a simpler path to the same outcome, say so. If you see a flaw in the current design, say so.

That said, read the rationale before challenging a decision. Most choices have been thought through. The architecture files explain not just what was chosen but what was rejected and why. Engage with the reasoning, not just the conclusion.

## Repo structure

```
tsa-sct-26/
  ARCHITECTURE.md          master system overview - read this first
  AGENTS.md                this file

  firmware/                ESP32 firmware (PlatformIO)
    AGENTS.md              firmware-specific agent notes
  cad/                     printable parts by subsystem
  docs/
    MECHANICAL.md          physical design and fabrication
    ELECTRICAL.md          wiring and power
    EMBEDDED.md            firmware architecture and specification
    BOM.md                 parts list
    ISSUES.md              open technical questions
    IDEAS.md               design alternatives and exploratory concepts
    NOTEBOOK.md            engineering notebook checklist and timeline
    SHORT_TODO.md          immediate action items

    competition/           competition info, operator instructions
    engineering/           decision matrices, calibration, performance data
    runs/                  CSV logs from calibration and reliability runs
```

Read ARCHITECTURE.md for a full system overview. Then read the subsystem doc (`firmware/AGENTS.md`, `docs/MECHANICAL.md`, `docs/ELECTRICAL.md`, or `docs/EMBEDDED.md`) relevant to what you are doing.

## Physical UX is scored

The evaluator operates alone. The physical design must communicate everything without a human to explain it. This directly affects the Solution to Problem score.

Specific things that matter:
- 3D printed chute entrance guide showing brick orientation (studs up, length along travel axis)
- Labeled start button. Printed or engraved text - evaluator should not have to guess
- Display states: READY, SORTING, SORT COMPLETE, ERROR. Large enough to read from arm's length
- Bin labels on each bin: "2x2 RED", "2x2 BLUE", "2x3 BLUE", "2x3 RED"
- Cable management. Routed cleanly looks engineered

Physical presentation is part of what separates first from third.

## Writing conventions

No em dashes and no double dashes (--) anywhere in docs or comments. Reword the sentence instead. A colon or a single dash is fine when needed, but usually a reword is cleaner. This applies to all markdown files and inline code comments.

## Git commits

Most commits are one line. Anything reasonably substantial gets a second line after a blank line for context. Three lines is rare. Write for why and what, not how. Assume the reader can read the diff.

```
add size detection interrupt handler
add color sensor averaging over brick dwell window
fix solenoid de-energize timing calculation
add thermal model with WARNING and DANGER thresholds

update plow lever geometry to ratio 3.0
needed 25mm tip travel, solenoid stroke is 10mm
```

Bad: "updated some files", "fixed bug", "WIP", "changes"

## Documentation

Judges score documentation heavily. Decision matrices (what was considered, what was chosen, why alternatives were rejected) matter as much as the system itself. Every architectural decision in these files has a documented rationale. Keep it that way.

Serial output during calibration runs is the engineering notebook data source. Log it to CSV. Do not manually record run results.

Decision matrices belong in `docs/engineering/decision_matrices/`. Calibration procedures belong in `docs/engineering/calibration/`. Run logs belong in `docs/runs/`.

## Key warnings

- The chute transition piece is the highest risk item. Nothing else should be printed until it is validated with real bricks.
- Never suggest demoing or calibrating on a bench supply. LiPo only for any run that matters.
- The color sensor must be calibrated with the shroud installed. Open-air calibration is meaningless.
- Flyback diodes on every solenoid, bulk capacitor on the stepper driver motor power input. Both are non-negotiable. Do not generate wiring instructions that omit these.
- Footprint is hard-constrained at 610mm x 610mm (2ft x 2ft). Flag any design that risks exceeding this.
- Programming Structure is 20 of 100 scored points. Code module separation, naming, and readability are evaluated. Write code a judge can follow on first read.
