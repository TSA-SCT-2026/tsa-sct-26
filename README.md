# tsa-sct-26

TSA System Control Technology 2026: automated LEGO brick sorter for nationals on May 1, 2026.

The active production architecture sorts 24 LEGO bricks by size and color into 4 bins using:
- Preloaded 24-brick compressed queue
- One-brick isolation chamber
- Static sensing in the chamber
- NEMA17 conveyor feed axis
- Off-axis toothed timing-belt stage to a supported smooth drive roller
- 4-index selector chute under the trapdoor, active for now
- Event-gated control with physical truth checks before each release and restart

Goal: first place at nationals.

## Start here

Read these in order:
1. `docs/ARCHITECTURE.md`
2. The subsystem doc for the area you are touching
3. `docs/project/GLOSSARY.md` if any high-level architecture term is unclear

## Active system model

Do not model the machine as one full belt traverse per brick.

The active throughput model is:
- A compressed queue is already staged at the chute exit
- Only one brick may occupy the chamber
- The next brick advances by chamber pitch, not by full conveyor length
- Static sensing, selector readiness, drop, bin confirm, and platform-level reset form the control loop
- Safe overlap is allowed only when physical state confirms it

The likely production bottleneck is selector motion plus reset latency, not conveyor transport length.

## Selector status

The 4-index selector chute is the active routing design.

It is not permanently locked. The repo now treats it as an evidence-gated choice:
- Keep the selector chute unless measured or modeled routing cost shows it cannot support a sub-10-second winning design with margin
- Evaluate selector cost using official NEMA11 motion references plus real bench data from the actual mechanism

## Repo structure

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

  cad/
    MECHANICAL.md
    DIMENSIONS.md
    README.md
    frame/rollers/

  firmware/
    EMBEDDED.md
    README.md
    src/

  wiring/
    ELECTRICAL.md

  simulation/
  reference/
```

## Development notes

- `simulation/` is a separate sandbox and should not drive mechanical truth by itself.
- `reference/` is not part of the active implementation surface.
- Numeric geometry and derived timing assumptions should trace back to `cad/DIMENSIONS.md`.
- Judges score reliability, clarity, and documentation heavily. Operator-facing UX is not cosmetic.

## Competition constraints

- Footprint must stay within 610mm x 610mm
- Evaluator operates the system alone with written instructions
- Primary competition flow is: load 24 bricks, press start, machine sorts
- LiPo power is the only meaningful run condition
