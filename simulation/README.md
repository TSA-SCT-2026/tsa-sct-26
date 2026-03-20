# Brick Sorter Simulator

A discrete-event simulator for the TSA SCT 2026 brick sorting system.
Models escapement timing, belt transport, solenoid routing, and thermal
accumulation across back-to-back runs.

---

## Quick start

No install required. Open `simulator.html` in any browser and click
**Run Simulation**. That is it.

```
simulation/
  simulator.html    self-contained, works by double-click
```

`simulator.html` is a fully bundled single file with no external dependencies.
It works offline, on any OS, from any path. No Node, no server, no build step
needed just to run it.

---

## Rebuilding after source changes

The bundle is pre-built and committed. You only need to rebuild if you change
files under `src/`, `style.css`, or `template.html`.

Requirements: Node.js 18+ with npx available.

```bash
cd simulation
bash build.sh
```

This runs `npx esbuild src/main.js --bundle --format=iife` into `dist/bundle.js`,
then inlines `style.css` and `dist/bundle.js` into `template.html` to produce
`simulator.html`. The output file is self-contained and committed to the repo.

Dependencies are pulled automatically by npx on first run:

- `esbuild` (bundler, no runtime dependency in the output)

No `package.json` is needed to build. No `npm install` is needed. esbuild is
invoked directly via npx.

---

## Source layout

```
simulation/
  simulator.html      bundled output (committed)
  template.html       HTML shell with INLINE_CSS / INLINE_JS placeholders
  style.css           all styles
  build.sh            build script
  dist/bundle.js      esbuild IIFE output (intermediate, committed)
  src/
    defaults.js       DEFAULTS + SECTIONS (syncs with firmware/src/config.h)
    thermal.js        decayHeat(), coolingPctToRate(), misfireProbability()
    simulation.js     generateSequence(), computeSimulation()
    warnings.js       computeWarnings()
    canvas.js         belt/plow/brick/overlay drawing
    metrics.js        updateMetrics(), showRunSummary(), bin counts
    controls.js       buildControls(), slider handlers
    tabs.js           switchTab(), timeline log, per-brick table
    animation.js      animation loop, event processing, startSim(), resetSim()
    replay.js         CSV replay from firmware serial log
    main.js           init(), callback wiring, window globals
  tests/
    simulator.test.js routing engine test suite (run with node, ES module)
  CONFIG_REFERENCE.md  parameter documentation and tuning guide
  TODO.md              pending work items
```

---

## Running the test suite

```bash
cd simulation
node tests/simulator.test.js
```

Requires Node.js 18+ (native ES module support, no transpile step). The test
file imports directly from `src/` and exercises the routing engine, thermal
model, and sequence generators without the browser layer.

To enable `{"type":"module"}` resolution (required to run the tests):

```bash
echo '{"type":"module"}' > simulation/package.json
```

This file is not committed because it is only needed for running tests from
the command line. The browser build works without it.

---

## Keeping simulator in sync with firmware

All default values live in `src/defaults.js`. When you change a value in
`firmware/src/config.h`, update the matching entry in `DEFAULTS` in
`defaults.js` and rebuild.

The `SECTIONS` array in `defaults.js` controls what the controls panel shows
and the order. Adding a new parameter: add it to both `DEFAULTS` and `SECTIONS`,
then rebuild.

See `CONFIG_REFERENCE.md` for a full description of every parameter.

---

## Loading a real run log (CSV replay)

Click **Load CSV** in the header and select a `.csv` file exported from the
firmware serial log. The simulator parses the log and replays the animation
from real timestamps.

Expected CSV columns (firmware serial format):
```
brick, ts_enter_ms, ts_confirm_ms, transit_ms, gap_us, size, color, type,
plow, bin, ok, heat_sol1, heat_sol2, heat_sol3, heat_step, thermal_state
```

The replay view highlights bricks where actual transit time differs from
expected by more than 150ms as DRIFT events in the timeline. These flag
belt speed drift during a physical run.
