# tsa-sct-26

TSA System Control Technology 2026: LEGO brick sorter for states on May 2, 2026.

The active states architecture sorts 24 LEGO bricks by size and color into 4 bins using:
- Manual one-at-a-time feed onto a conveyor
- NEMA17-driven conveyor
- Conveyor-mounted size and color sensing station near the feed end
- Break-beam timing size sensor with two pairs in the sensing shroud
- TCS3200/GY-31 color sensor with shroud
- MG995/MG996-class servo rotary chute selector
- Four labeled bins under the chute sweep
- Wood frame from available materials, with printed brackets where useful

Goal: first place at states.

The previous chamber, release-gate, compressed-queue, NEMA11 selector design is archived under `_archive/previous-chamber-release-design-2026-04/` for later nationals work.

## Start Here

Read these in order:
1. `docs/ARCHITECTURE.md`
2. The subsystem doc for the area you are touching
3. `docs/project/OPEN_DECISIONS.md` for unresolved states-build choices
4. `docs/project/GLOSSARY.md` if a term is unclear

## Active System Model

Do not model the states build as a chamber sorter.

The active flow is:
- Operator places one brick on the conveyor
- Conveyor carries it through the sensing station
- Size and color classification choose the target bin
- Servo chute points at the target bin
- Brick exits the belt, slides down the chute, and lands in the bin
- System returns to READY for the next brick

Optional feed automation comes only after this manual-feed machine sorts reliably.

## Repo Structure

```text
tsa-sct-26/
  SHORT_TODO.md
  COMMIT_GUIDE.log

  docs/
    ARCHITECTURE.md
    README.md
    build/
    project/
    notebook/
    runs/
    datasheet/

  cad/
    MECHANICAL.md
    DIMENSIONS.md
    README.md

  firmware/
    EMBEDDED.md
    README.md
    src/

  wiring/
    ELECTRICAL.md

  reference/
```

## Development Notes

- Exact geometry and derived timing assumptions should trace to `cad/DIMENSIONS.md` only when source-backed. Draft CAD values belong as variables, ranges, or test coefficients.
- `docs/project/BOM.xlsx` remains the purchase source of truth.
- Judges score reliability, clarity, and documentation heavily.
- Operator-facing UX is not cosmetic.

## Competition Constraints

- Footprint must stay within 610mm x 610mm
- Evaluator operates the system alone with written instructions
- Primary states flow is manual one-at-a-time feed until a reliable base sorter exists
- LiPo power is the only meaningful run condition
