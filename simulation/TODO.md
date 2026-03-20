# Simulator TODO

---

## Pending

**[TODO] Test suite for routing engine**

A dedicated agent should write `simulation/tests/simulator.test.js` (ES module,
run with `node tests/simulator.test.js` after adding `{"type":"module"}` to
`simulation/package.json`). Minimum test coverage needed:

- `coolingPctToRate(0) === 1.0`, `coolingPctToRate(50) === 0.5`, `coolingPctToRate(100) === 0.0`
- `decayHeat(1.0, 1, 0.5)` approx 0.5
- `generateSequence` with 'random' returns exactly 24 bricks with correct type counts
- `generateSequence` with 'worst_case_accuracy' starts with a plow-using brick
- `computeSimulation` at 800sps interleaved: 0 routing errors (same-plow re-fire is NOT an error)
- `computeSimulation` at 800sps worst_case: 0 routing errors (consecutive same-type fine at 800sps)
- `computeSimulation` at 1600sps worst_case_accuracy: routing errors present
- `computeSimulation` num_runs=2 with gap emits THERMAL_UPDATE events during gap
- `computeSimulation` num_runs=100 random: no crash, brickLog.length=2400
- Thermal misfire probability = 0 below warn, > 0 above warn, > 0.10 above danger
- Same-type fast-release: consecutive same-type bricks at 400sps release faster than normal interval

Root cause of the original false-positive HALT bug (now fixed): the old code used
`plowDeenergizeAt` to detect same-plow re-fires as conflicts. Same-plow re-fire is
NOT a routing error - the plow is already extended to the correct position. The real
conflict is when a brick passes through a DIFFERENT plow that is still extended.

---

## Completed

---

## Completed

**[DONE] Multi-run bin expected counts** - bin-exp divs now have IDs and are updated
dynamically by metrics.js. Counts scale with num_runs (e.g. 3 runs shows
"expected: 3x6 = 18"). Bin counters accumulate across runs. End-of-run match
detection uses num_runs-scaled expected values.

**[DONE] Physical run log replay** - Load CSV button in header opens a file picker.
Parses the firmware serial log format (brick, ts_enter_ms, ts_confirm_ms, transit_ms,
gap_us, size, color, type, plow, bin, ok, heat_sol1/2/3/step, thermal_state).
Drives the animation from actual timestamps, shows thermal state from real fire events,
flags bricks where actual transit differs from expected by more than 150ms (belt speed
drift detection) as DRIFT events in the timeline. Implemented in src/replay.js.

**[DONE] Simulator ERROR_HALT on solenoid conflict** - computeSimulation() now tracks
plowDeenergizeAt[1..3]. When a PLOW_FIRE event fires before the previous de-energize
completes, the simulation emits ERROR_HALT, stops immediately, and returns haltEvent.
The animation stops, shows a full-canvas red overlay with the exact brick/plow/timestamp,
and the Warnings tab reports the conflict. Implemented in src/simulation.js and
src/animation.js (drawErrorHaltOverlay in src/canvas.js).

**[DONE] Refactor into modular source** - simulator.html is now a ~76KB bundle generated
from ES modules in src/. Structure:

```
simulation/
  simulator.html    bundled output (committed, works by double-click)
  template.html     HTML shell with INLINE_CSS / INLINE_JS placeholders
  style.css         all styles
  build.sh          npx esbuild src/main.js -> dist/bundle.js -> simulator.html
  dist/bundle.js    esbuild IIFE output
  src/
    defaults.js     DEFAULTS + SECTIONS - only file that syncs with config.h
    thermal.js      decayHeat()
    simulation.js   generateSequence(), computeSimulation() with ERROR_HALT
    warnings.js     computeWarnings() - all warning checks
    canvas.js       drawBelt(), drawPlow(), drawAnimBrick(), drawErrorHaltOverlay()
    metrics.js      updateMetrics(), updateThermalUI(), showRunSummary()
    controls.js     buildControls(), slider handlers, resetSection()
    tabs.js         switchTab(), addEvent(), populateBrickTable()
    animation.js    shared state, animFrame(), processEvent(), startSim(), resetSim()
    replay.js       parseReplayCSV(), buildReplayEvents(), startReplay()
    main.js         init(), callback wiring, global window handlers
```

To rebuild simulator.html after changing source: `cd simulation && bash build.sh`
