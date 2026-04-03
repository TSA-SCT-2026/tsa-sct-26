# tsa-sct-26

TSA System Control Technology 2026: Automated Warehouse Sorting and Conveyor System.

Sorts 24 LEGO bricks by size and color into 4 bins on a conveyor, automatically, in under 10 seconds. Competition: May 1 2026.

## Start here

Read `docs/ARCHITECTURE.md` for the full system overview, design philosophy, pipeline, and build schedule.

## System overview

Current design uses a **chamber isolation architecture**:
- Narrow 20mm belt conveyor with side rails (brick isolation)
- Bricks oriented widthwise across conveyor, with length along travel for stable fit and deterministic size sensing
- Class 3 lever trapdoor (8mm solenoid stroke amplified to about 30mm tip travel)
- Stepper-indexed 4-position rotating chute selector beneath trap
- Four collection bins at stationary positions

This design eliminates lateral diverter complexity and achieves deterministic per-brick isolation through sequential chamber gating.

## Repo structure

```
tsa-sct-26/
  cad/                     all printable parts, organized by subsystem
    MECHANICAL.md          physical design and fabrication notes
    DIMENSIONS.md          critical geometry and tolerances
  firmware/                ESP32 embedded firmware (PlatformIO project)
    EMBEDDED.md            firmware architecture and specification
  wiring/
    ELECTRICAL.md          power architecture, wiring, protection components
  simulation/              browser-based system simulator (no server needed)
    simulator.html         open this directly - interactive parameter tuning
    CONFIG_REFERENCE.md    explanation of every parameter, timing math, warnings
  docs/
    ARCHITECTURE.md        master architecture and phase plan
    BOM.md                 parts list with ordering info
    CALIBRATION.md         calibration process and targets
    CHECKLIST.md           build and verification checklist
    TEST_PROTOCOL.md       pass/fail protocol
    COMPETITION_INFO.md    competition references and constraints
    notebook/
      README.md            engineering notebook guidance
  SHORT_TODO.md            immediate session task list
```

## Development tools

Open `simulation/simulator.html` in a browser for an interactive system simulator. No server needed. Defaults match `firmware/src/config.h`. Controls let you tune every parameter and see the effect on throughput, sensing, and thermal behavior before touching hardware. See `simulation/CONFIG_REFERENCE.md` for a full explanation of every parameter.

When firmware config values change, update `simulation/src/defaults.js` in the same change so simulator tuning remains aligned.

## Competition requirements summary

- 2ft x 2ft maximum footprint
- 24 LEGO bricks (12x 2x2, 12x 2x3) sorted by size then color into 4 bins
- Minimum: 2 sensors, manual start/stop, programmable controller, feedback loop, motorized conveyor, 1 automated sorting mechanism
- Full requirements: `docs/COMPETITION_INFO.md`
