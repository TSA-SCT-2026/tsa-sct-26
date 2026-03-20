# Simulator TODO

Open items for future simulator development. None of these block using the current
version - the simulator is fully functional for parameter tuning and pre-build analysis.

---

## Bugs

**Multi-run bin expected counts are wrong**
The four bin boxes in the metrics panel always show "expected: 6 / 6 / 4 / 8" (single-run
targets). When num_runs > 1, the expected values should scale: e.g., 3 runs = 18 / 18 / 12 / 24.
The bin count display and end-of-run match/mismatch coloring are also confusing across runs
because liveBinCounts accumulates without resetting between runs.

Fix: multiply expected counts by num_runs in the metrics display. Decide whether bin counters
reset per-run or accumulate - the latter is more useful for thermal stress testing. If accumulating,
show "N / expected NxM" format.

---

## Features

**Physical run log replay**
Import a serial log CSV from a real run and replay it in the simulator. The serial log already
captures every event needed: brick number, size classification, color classification, destination
bin, confirmation result, transit time, timestamp.

Replay would:
- Drive the animation from actual timestamps rather than simulated timing
- Run the thermal model forward on real fire events, producing estimated heat curves
- Flag discrepancies between expected and actual transit times (belt speed drift detection)
- Let you scrub through the run to see system state at any point

Particularly useful after calibration runs: load the CSV, see where the thermal model says
WARNING should have triggered, compare against whether you observed throttling.

**Simulator ERROR_HALT on solenoid conflict**
Currently the sim warns when same-plow firing interval is shorter than sol_deenergize_ms, but
continues running. A FAIL mode would: detect when a plow's fire event fires while that plow
is still in FULL or HOLD phase, stop the simulation, display ERROR_HALT on the belt canvas,
and log the exact brick and plow that caused the conflict.

This directly mirrors what happens physically: the plow never retracted, the next brick hits
an already-extended arm and jams. Extremely useful for finding max sustainable sps before
plow conflicts become inevitable. Run worst_case sequence and crank up sps until it fails.

---

## Code quality

**Refactor from single HTML into modular source**
The current simulator.html is one file (~1650 lines). It works but is hard to extend cleanly.
A proper structure with separation of concerns:

```
simulation/
  simulator.html          thin shell: layout only, loads modules
  src/
    defaults.js           DEFAULTS object - the only file that syncs with config.h
    simulation.js         discrete event engine: computeSimulation(), generateSequence()
    thermal.js            thermal model: decayHeat(), updateThermal()
    warnings.js           all warning checks, returns structured warning list
    animation.js          requestAnimationFrame loop, event processing, anim brick state
    canvas.js             all canvas drawing: belt, bricks, plows, bin boxes, labels
    metrics.js            live metrics panel updates
    controls.js           control section builder, slider handlers, reset logic
    tabs.js               tab switching, timeline log, per-brick table
  style.css               extracted from the <style> block
```

Benefits: each concern is independently editable and testable. Adding log replay or failure
mode touches simulation.js and animation.js without touching drawing code. Adding a new
warning touches only warnings.js. The defaults.js file becomes the canonical sync point
with config.h and is trivially diffable.

Use ES modules (import/export). The file can still open by double-click if served via a
simple local server, or use a minimal bundler (esbuild, single command) to produce a
deployable simulator.html for the repo. Keep the bundled output committed so it still works
by double-click without any tooling.

This is a good project for after calibration when the simulator parameters are locked in
and you want to add replay or failure mode without fighting a 1650-line monolith.
