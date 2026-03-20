# Simulator TODO

All items from this list have been implemented. File kept for reference.

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
