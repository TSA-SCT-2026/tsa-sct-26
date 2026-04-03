# Brick Sorter Simulator

Chamber-drop simulator for TSA SCT 2026.

This simulator is for real system development, not game physics: it focuses on tuning firmware-facing timing, thermal behavior, run sequencing, and troubleshooting with replayed logs.

---

## Quick start

Open `simulator.html` in a browser and click **Run Simulation**.

No server or install is required for use.

---

## What this models

- State flow: `IDLE -> FEED -> APPROACH -> SEATED -> SENSING -> INDEXED -> RELEASED -> CONFIRM -> RESET`
- Chamber-static sensing timing
- Selector disc indexing time from steps and SPS
- Trapdoor release and confirmation windows
- Thermal accumulation and cooldown across multi-run sessions
- Replay of run logs from CSV

## What this does not model

- Detailed rigid-body or spring-force dynamics
- Sensor noise drift or optical artifacts
- Electrical transients and power sag

---

## Build after source changes

If you edit `src/`, `template.html`, or `style.css`:

```bash
cd simulation
bash build.sh
```

This rebuilds `dist/bundle.js` and regenerates `simulator.html`.

---

## Run tests

```bash
cd simulation
node tests/simulator.test.js
```

`package.json` includes `"type": "module"` so tests run directly on Node 18+.

---

## CSV replay

Use **Load CSV** in the simulator header.

Accepted shapes:

1. Full enter and confirm timestamps: `brick, ts_enter_ms, ts_confirm_ms, ...`
2. Single timestamp rows: `brick, ts_ms, ...` or `brick, timestamp_ms, ...`

Recommended fields for better replay fidelity:

`brick, category|type, size, color, bin, ok, samples|valid_samples`

If timestamps are partial, the simulator fills missing phases using current timing parameters.

---

## Files

```
simulation/
  simulator.html
  template.html
  style.css
  build.sh
  package.json
  dist/
    bundle.js
  src/
    defaults.js
    thermal.js
    simulation.js
    warnings.js
    canvas.js
    metrics.js
    controls.js
    tabs.js
    animation.js
    replay.js
    main.js
  tests/
    simulator.test.js
  TODO.md
```

---

## Keeping in sync with firmware

Mirror firmware constants in `src/defaults.js` when `firmware/src/config.h` changes, then rebuild.

Checklist step for every config-affecting change:

1. Update `firmware/src/config.h`.
2. Update matching fields in `simulation/src/defaults.js` in the same change.
3. Rebuild with `bash build.sh`.
4. Run `node tests/simulator.test.js`.

Planned enhancement is tracked in `simulation/TODO.md`: optional "Use firmware config" import mode to generate simulation defaults from firmware config automatically.

Current source of truth for architecture behavior is:

- `docs/ARCHITECTURE.md`
- `firmware/EMBEDDED.md`
- `cad/MECHANICAL.md`
