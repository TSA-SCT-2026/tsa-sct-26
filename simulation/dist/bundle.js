"use strict";
(() => {
  // src/defaults.js
  var DEFAULTS = {
    // Escapement
    stepper_sps: 800,
    stepper_steps_per_rev: 200,
    // Belt
    belt_target_mm_s: 200,
    motor_noload_rpm: 600,
    motor_load_factor: 0.65,
    // Sensing
    beam_gap_mm: 19,
    size_timeout_ms: 150,
    color_integration_ms: 2,
    // Solenoid
    sol_full_ms: 20,
    sol_hold_pct: 40,
    sol_deenergize_ms: 280,
    sol_lead_ms: 150,
    // Thermal
    thermal_heat_per_sol: 0.15,
    thermal_heat_per_step: 0.05,
    thermal_decay_rate: 0.5,
    thermal_warn_level: 0.6,
    thermal_danger_level: 0.85,
    // Geometry (from MECHANICAL.md / EMBEDDED.md)
    brick_2x2_mm: 15.8,
    brick_2x3_mm: 23.7,
    brick_spacing_mm: 28,
    total_bricks: 24,
    // Bin distances from sensing zone (estimates - update from CAD)
    bin_distances_mm: [250, 280, 310, 360],
    // Brick counts by type (bin order: 2x2_blue, 2x2_red, 2x3_red, 2x3_blue)
    counts: { "2x2_blue": 6, "2x2_red": 6, "2x3_red": 4, "2x3_blue": 8 },
    // Plow assignment: 0 = default path (no fire)
    plow_map: { "2x2_blue": 1, "2x2_red": 2, "2x3_red": 3, "2x3_blue": 0 },
    // Run settings
    num_runs: 1,
    sequence: "interleaved",
    // 'interleaved' | 'worst_case' | 'default_last'
    interrun_gap_ms: 5e3
  };
  var SECTIONS = [
    {
      id: "escapement",
      label: "Escapement",
      controls: [
        {
          id: "stepper_sps",
          label: "Release speed",
          unit: "sps",
          min: 200,
          max: 2e3,
          step: 50,
          desc: "How fast the stepper spins to release one brick. Higher = more bricks/sec but fewer color samples."
        },
        {
          id: "stepper_steps_per_rev",
          label: "Steps per revolution",
          unit: "steps",
          min: 200,
          max: 200,
          step: 1,
          locked: true,
          desc: "Full-step NEMA 11 (1.8 deg/step). Fixed - shown for reference only."
        }
      ]
    },
    {
      id: "belt",
      label: "Belt",
      controls: [
        {
          id: "belt_target_mm_s",
          label: "Belt speed",
          unit: "mm/s",
          min: 50,
          max: 400,
          step: 10,
          desc: "How fast bricks travel from sensor to bins. Must stay ahead of the escapement rate."
        },
        {
          id: "motor_load_factor",
          label: "Motor load factor",
          unit: "%",
          min: 40,
          max: 95,
          step: 1,
          desc: "How much the motor slows under real load vs no-load spec. 65% is a realistic starting estimate.",
          scale: 100
        },
        {
          id: "motor_noload_rpm",
          label: "Motor no-load RPM",
          unit: "RPM",
          min: 100,
          max: 1e3,
          step: 50,
          desc: "JGB37-520 at 6V is rated 600 RPM no-load. Used to compute belt speed at load."
        }
      ]
    },
    {
      id: "sensing",
      label: "Sensing Zone",
      controls: [
        {
          id: "beam_gap_mm",
          label: "Beam spacing",
          unit: "mm",
          min: 10,
          max: 30,
          step: 1,
          desc: "Physical distance between the two size detection beams. Fixed in CAD."
        },
        {
          id: "size_timeout_ms",
          label: "Size timeout",
          unit: "ms",
          min: 50,
          max: 300,
          step: 5,
          desc: "If beam 2 does not break within this window the brick is classified 2x2. Must be above 95ms at 200mm/s."
        },
        {
          id: "color_integration_ms",
          label: "Color integration time",
          unit: "ms",
          min: 2,
          max: 10,
          step: 1,
          desc: "How long the color sensor takes per reading. TCS34725 minimum is ~2.4ms. Longer = fewer total samples."
        }
      ]
    },
    {
      id: "solenoid",
      label: "Solenoid Timing",
      controls: [
        {
          id: "sol_full_ms",
          label: "Full power duration",
          unit: "ms",
          min: 5,
          max: 50,
          step: 5,
          desc: "How long the solenoid gets full voltage to reach full extension. Too short = incomplete stroke."
        },
        {
          id: "sol_hold_pct",
          label: "Hold duty cycle",
          unit: "%",
          min: 20,
          max: 100,
          step: 5,
          desc: "PWM level to hold plow in position after full extension. 40% holds against spring, reduces heat."
        },
        {
          id: "sol_deenergize_ms",
          label: "De-energize time",
          unit: "ms",
          min: 100,
          max: 500,
          step: 10,
          desc: "Total time solenoid stays on before turning off. Must be long enough for brick to fully clear plow."
        },
        {
          id: "sol_lead_ms",
          label: "Sensor-to-plow travel",
          unit: "ms",
          min: 50,
          max: 300,
          step: 10,
          desc: "How long it takes a brick to travel from sensing zone to plow 1. Solenoid fires immediately on classification."
        }
      ]
    },
    {
      id: "thermal",
      label: "Thermal Model",
      controls: [
        {
          id: "thermal_heat_per_sol",
          label: "Heat per solenoid fire",
          unit: "",
          min: 0.01,
          max: 0.5,
          step: 0.01,
          dec: 2,
          desc: "How much heat accumulates each time a plow fires. Tune after measuring real solenoid temperature."
        },
        {
          id: "thermal_heat_per_step",
          label: "Heat per stepper release",
          unit: "",
          min: 0.01,
          max: 0.2,
          step: 0.01,
          dec: 2,
          desc: "Heat added each time the escapement releases a brick."
        },
        {
          id: "thermal_decay_rate",
          label: "Cooling rate (per sec)",
          unit: "",
          min: 0.1,
          max: 2,
          step: 0.1,
          dec: 1,
          desc: "How fast heat dissipates when actuators are idle. 0.5 = heat halves every second."
        },
        {
          id: "thermal_warn_level",
          label: "Warning threshold",
          unit: "",
          min: 0.3,
          max: 0.9,
          step: 0.05,
          dec: 2,
          desc: "Heat level at which escapement slows to WARNING rate (~2.7 bricks/sec)."
        },
        {
          id: "thermal_danger_level",
          label: "Danger threshold",
          unit: "",
          min: 0.5,
          max: 1,
          step: 0.05,
          dec: 2,
          desc: "Heat level at which escapement slows to DANGER rate (~1.3 bricks/sec)."
        }
      ]
    },
    {
      id: "run",
      label: "Run Settings",
      controls: []
      // special: handled as selects/inputs
    }
  ];

  // src/thermal.js
  function decayHeat(heat, elapsedSec, rate) {
    return heat * Math.pow(rate, elapsedSec);
  }

  // src/simulation.js
  function generateSequence(p) {
    const types = ["2x2_blue", "2x2_red", "2x3_red", "2x3_blue"];
    const seq = [];
    if (p.sequence === "worst_case") {
      for (const t of types) for (let i = 0; i < p.counts[t]; i++) seq.push(t);
    } else if (p.sequence === "default_last") {
      for (const t of ["2x2_blue", "2x2_red", "2x3_red"]) for (let i = 0; i < p.counts[t]; i++) seq.push(t);
      for (let i = 0; i < p.counts["2x3_blue"]; i++) seq.push("2x3_blue");
    } else {
      const rem = Object.fromEntries(types.map((t) => [t, p.counts[t]]));
      let safety = 200;
      while (seq.length < p.total_bricks && safety-- > 0) {
        for (const t of types) {
          if (rem[t] > 0 && seq.length < p.total_bricks) {
            seq.push(t);
            rem[t]--;
          }
        }
      }
    }
    return seq;
  }
  function computeSimulation(p) {
    const DIST_CHUTE_TO_SENSOR_MM = 100;
    let thermal = { sol1: 0, sol2: 0, sol3: 0, stepper: 0 };
    let lastReleaseT = 0;
    let binCounts = [0, 0, 0, 0];
    let thermalDangerReached = false;
    let peakHeat = 0;
    const allBrickLog = [];
    const allEvents = [];
    let globalBrickNum = 0;
    const plowDeenergizeAt = [0, 0, 0, 0];
    let haltEvent = null;
    for (let run = 0; run < p.num_runs; run++) {
      const seq = generateSequence(p);
      if (run > 0) {
        const gapSec = p.interrun_gap_ms / 1e3;
        thermal.sol1 = decayHeat(thermal.sol1, gapSec, p.thermal_decay_rate);
        thermal.sol2 = decayHeat(thermal.sol2, gapSec, p.thermal_decay_rate);
        thermal.sol3 = decayHeat(thermal.sol3, gapSec, p.thermal_decay_rate);
        thermal.stepper = decayHeat(thermal.stepper, gapSec, p.thermal_decay_rate);
        lastReleaseT += p.interrun_gap_ms;
        allEvents.push({ t: lastReleaseT - p.interrun_gap_ms, type: "INTER_RUN_GAP", run: run - 1, gapMs: p.interrun_gap_ms });
      }
      for (let i = 0; i < seq.length; i++) {
        const brickType = seq[i];
        globalBrickNum++;
        const maxHeat = Math.max(thermal.sol1, thermal.sol2, thermal.sol3, thermal.stepper);
        let sps = p.stepper_sps;
        let thermalState = "NORMAL";
        if (maxHeat > p.thermal_danger_level) {
          sps = 267;
          thermalState = "DANGER";
          thermalDangerReached = true;
        } else if (maxHeat > p.thermal_warn_level) {
          sps = 533;
          thermalState = "WARNING";
        }
        const releaseInterval = p.stepper_steps_per_rev / sps * 1e3;
        const T_release = lastReleaseT + releaseInterval;
        const dtSec = (T_release - lastReleaseT) / 1e3;
        thermal.sol1 = decayHeat(thermal.sol1, dtSec, p.thermal_decay_rate);
        thermal.sol2 = decayHeat(thermal.sol2, dtSec, p.thermal_decay_rate);
        thermal.sol3 = decayHeat(thermal.sol3, dtSec, p.thermal_decay_rate);
        thermal.stepper = decayHeat(thermal.stepper, dtSec, p.thermal_decay_rate);
        thermal.stepper = Math.min(thermal.stepper + p.thermal_heat_per_step, 1);
        peakHeat = Math.max(peakHeat, thermal.stepper);
        allEvents.push({
          t: T_release,
          type: "BRICK_RELEASED",
          brickNum: globalBrickNum,
          brickType,
          run,
          sps,
          thermalState,
          thermal: { ...thermal }
        });
        const T_sense = T_release + DIST_CHUTE_TO_SENSOR_MM / p.belt_target_mm_s * 1e3;
        allEvents.push({ t: T_sense, type: "BRICK_AT_SENSOR", brickNum: globalBrickNum, brickType });
        const brickLen = brickType.includes("2x3") ? p.brick_2x3_mm : p.brick_2x2_mm;
        const is2x3 = brickType.includes("2x3");
        let sizeResolveMs, gapUs;
        if (is2x3) {
          gapUs = Math.round(p.beam_gap_mm / p.belt_target_mm_s * 1e6);
          sizeResolveMs = p.beam_gap_mm / p.belt_target_mm_s * 1e3;
        } else {
          gapUs = 0;
          sizeResolveMs = p.size_timeout_ms;
        }
        const dwellMs = brickLen / p.belt_target_mm_s * 1e3;
        const samples = Math.max(1, Math.floor(dwellMs / p.color_integration_ms));
        const T_classified = T_sense + Math.max(sizeResolveMs, dwellMs);
        allEvents.push({
          t: T_classified,
          type: "BRICK_CLASSIFIED",
          brickNum: globalBrickNum,
          brickType,
          sizeResult: is2x3 ? "2x3" : "2x2",
          colorResult: brickType.includes("red") ? "RED" : "BLUE",
          gapUs,
          samples
        });
        const plowNum = p.plow_map[brickType];
        const binIdx = plowNum === 0 ? 3 : plowNum - 1;
        if (plowNum > 0) {
          if (T_classified < plowDeenergizeAt[plowNum]) {
            haltEvent = { brickNum: globalBrickNum, plow: plowNum, simMs: T_classified };
            allEvents.push({
              t: T_classified,
              type: "ERROR_HALT",
              brickNum: globalBrickNum,
              plow: plowNum,
              msg: `Plow ${plowNum} still energized (until ${plowDeenergizeAt[plowNum].toFixed(0)}ms) when brick #${globalBrickNum} fires at ${T_classified.toFixed(0)}ms`
            });
            allEvents.sort((a, b) => a.t - b.t);
            const totalSimMs2 = allEvents[allEvents.length - 1]?.t || 0;
            return { events: allEvents, brickLog: allBrickLog, thermalDangerReached, peakHeat, totalSimMs: totalSimMs2, haltEvent };
          }
          const solKey = "sol" + plowNum;
          const dt2 = (T_classified - T_release) / 1e3;
          thermal[solKey] = decayHeat(thermal[solKey], dt2, p.thermal_decay_rate);
          thermal[solKey] = Math.min(thermal[solKey] + p.thermal_heat_per_sol, 1);
          peakHeat = Math.max(peakHeat, thermal[solKey]);
          plowDeenergizeAt[plowNum] = T_classified + p.sol_deenergize_ms;
          allEvents.push({ t: T_classified, type: "PLOW_FIRE", plow: plowNum, brickNum: globalBrickNum, thermal: { ...thermal } });
          allEvents.push({ t: T_classified + p.sol_full_ms, type: "PLOW_HOLD", plow: plowNum, brickNum: globalBrickNum });
          allEvents.push({ t: T_classified + p.sol_deenergize_ms, type: "PLOW_RELEASE", plow: plowNum, brickNum: globalBrickNum });
        } else {
          allEvents.push({ t: T_classified, type: "PLOW_SKIP", brickNum: globalBrickNum, msg: "default path - no solenoid" });
        }
        const binDist = p.bin_distances_mm[binIdx];
        const T_confirm = T_sense + binDist / p.belt_target_mm_s * 1e3;
        allEvents.push({ t: T_confirm, type: "BIN_CONFIRM", brickNum: globalBrickNum, binIdx, brickType });
        binCounts[binIdx]++;
        const transitMs = Math.round(T_confirm - T_release);
        allBrickLog.push({
          num: globalBrickNum,
          run: run + 1,
          type: brickType,
          sizeResult: is2x3 ? "2x3" : "2x2",
          colorResult: brickType.includes("red") ? "RED" : "BLUE",
          samples,
          plow: plowNum,
          binIdx,
          transitMs,
          thermalState,
          correct: true
        });
        lastReleaseT = T_release;
      }
      allEvents.push({ t: lastReleaseT + 2e3, type: "RUN_COMPLETE", run, binCounts: [...binCounts], totalMs: lastReleaseT });
      binCounts = [0, 0, 0, 0];
    }
    allEvents.sort((a, b) => a.t - b.t);
    const totalSimMs = allEvents[allEvents.length - 1]?.t || 0;
    return { events: allEvents, brickLog: allBrickLog, thermalDangerReached, peakHeat, totalSimMs, haltEvent: null };
  }

  // src/warnings.js
  function computeWarnings(p, simData) {
    const warnings = [];
    const escRate = p.stepper_sps / p.stepper_steps_per_rev;
    const beltCap = p.belt_target_mm_s / p.brick_spacing_mm;
    if (beltCap < escRate) {
      warnings.push({
        level: "e",
        msg: `Belt is the bottleneck: belt capacity (${beltCap.toFixed(2)} bricks/s) is below escapement rate (${escRate.toFixed(2)} bricks/s). Bricks will pile up and jam.`
      });
    }
    const dwell2x2 = p.brick_2x2_mm / p.belt_target_mm_s * 1e3;
    const samples = Math.floor(dwell2x2 / p.color_integration_ms);
    if (samples < 4) {
      warnings.push({ level: "e", msg: `Only ${samples} color samples for a 2x2 brick. Minimum for reliable classification is 8. Reduce belt speed or color integration time.` });
    } else if (samples < 8) {
      warnings.push({ level: "w", msg: `Only ${samples} color samples for a 2x2 brick. Classification may be unreliable. Target is 8+ samples.` });
    }
    const sigWin = p.beam_gap_mm / p.belt_target_mm_s * 1e3;
    const tmarg = p.size_timeout_ms - sigWin;
    if (tmarg < 0) {
      warnings.push({ level: "e", msg: `Size timeout (${p.size_timeout_ms}ms) is BELOW the 2x3 signal window (${sigWin.toFixed(0)}ms). 2x3 bricks will be misclassified as 2x2.` });
    } else if (tmarg < 20) {
      warnings.push({ level: "w", msg: `Size timeout margin is only ${tmarg.toFixed(0)}ms (timeout: ${p.size_timeout_ms}ms, 2x3 signal: ${sigWin.toFixed(0)}ms). Recommend at least 20ms margin.` });
    }
    const brickClearTime = p.sol_lead_ms + p.brick_2x3_mm / p.belt_target_mm_s * 1e3;
    if (p.sol_deenergize_ms < brickClearTime) {
      warnings.push({ level: "e", msg: `Solenoid de-energizes at ${p.sol_deenergize_ms}ms but a 2x3 brick needs ${brickClearTime.toFixed(0)}ms to fully clear the plow (lead ${p.sol_lead_ms}ms + transit ${(p.brick_2x3_mm / p.belt_target_mm_s * 1e3).toFixed(0)}ms). Plow will retract under the brick.` });
    } else if (p.sol_deenergize_ms < brickClearTime + 5) {
      warnings.push({ level: "w", msg: `Solenoid de-energize margin is only ${(p.sol_deenergize_ms - brickClearTime).toFixed(0)}ms. Tighter than 5ms risks plow retracting before brick fully clears.` });
    }
    const releaseInterval = p.stepper_steps_per_rev / p.stepper_sps * 1e3;
    const seqForWarning = generateSequence(p);
    const lastFireBrickIdx = {};
    let minSamePlowGap = Infinity;
    for (let i = 0; i < seqForWarning.length; i++) {
      const pl = p.plow_map[seqForWarning[i]];
      if (pl > 0) {
        if (lastFireBrickIdx[pl] !== void 0) {
          const gap = (i - lastFireBrickIdx[pl]) * releaseInterval;
          if (gap < minSamePlowGap) minSamePlowGap = gap;
        }
        lastFireBrickIdx[pl] = i;
      }
    }
    if (isFinite(minSamePlowGap) && minSamePlowGap < p.sol_deenergize_ms) {
      warnings.push({ level: "w", msg: `Plow overlap: with the current sequence, plow fires again after ${minSamePlowGap.toFixed(0)}ms but solenoid is still energized for ${p.sol_deenergize_ms}ms. This will cause ERROR_HALT in simulation. Different plows are independent - only a problem if the same plow fires twice. Change sequence or reduce sps to widen the gap.` });
    }
    if (warnings.length === 0) {
      warnings.push({ level: "ok", msg: "All parameters within acceptable ranges. No issues detected at these settings." });
    }
    if (simData && simData.thermalDangerReached) {
      warnings.push({ level: "w", msg: "Thermal DANGER level was reached during the simulated run. System would throttle to 1.3 bricks/sec." });
    }
    if (simData && simData.haltEvent) {
      warnings.push({ level: "e", msg: `Simulation halted: ERROR_HALT on brick #${simData.haltEvent.brickNum} - Plow ${simData.haltEvent.plow} conflict at ${simData.haltEvent.simMs.toFixed(0)}ms.` });
    }
    return warnings;
  }

  // src/animation.js
  var state = {
    params: null,
    animRunning: false,
    animPaused: false,
    animStartWallMs: null,
    animOffsetSimMs: 0,
    simData: null,
    animFrameId: null,
    liveThermal: { sol1: 0, sol2: 0, sol3: 0, stepper: 0 },
    liveBinCounts: [0, 0, 0, 0],
    liveEventIdx: 0,
    animBricks: [],
    pendingFlashes: [],
    plowStates: [
      null,
      { phase: "idle", phaseStartSim: 0, angle: 0 },
      { phase: "idle", phaseStartSim: 0, angle: 0 },
      { phase: "idle", phaseStartSim: 0, angle: 0 }
    ],
    speedMultiplier: 1,
    replayMode: false,
    haltOverlay: null
    // {msg} when ERROR_HALT is active
  };
  var cb = {
    updateWarnings: null,
    showRunSummary: null,
    updateThermalUI: null,
    setBinCount: null,
    flashBin: null,
    updateBinMatchState: null,
    drawBelt: null,
    drawErrorHaltOverlay: null,
    addEvent: null,
    populateBrickTable: null,
    setText: null
  };
  function registerCallbacks(cbs) {
    Object.assign(cb, cbs);
  }
  function resetAnimState() {
    state.liveThermal = { sol1: 0, sol2: 0, sol3: 0, stepper: 0 };
    state.liveBinCounts = [0, 0, 0, 0];
    state.liveEventIdx = 0;
    state.animBricks = [];
    state.pendingFlashes = [];
    state.haltOverlay = null;
    state.plowStates = [
      null,
      { phase: "idle", phaseStartSim: 0, angle: 0 },
      { phase: "idle", phaseStartSim: 0, angle: 0 },
      { phase: "idle", phaseStartSim: 0, angle: 0 }
    ];
  }
  function _launchAnimLoop(updateWarnings2, populateBrickTable2) {
    resetAnimState();
    for (let i = 0; i < 4; i++) {
      if (cb.setBinCount) cb.setBinCount(i, 0);
      const box = document.getElementById("bin-" + i);
      if (box) box.className = "bin-box";
    }
    const tlEl = document.getElementById("timeline-log");
    if (tlEl) tlEl.innerHTML = "";
    const blEl = document.getElementById("brick-log-body");
    if (blEl) blEl.innerHTML = '<tr><td colspan="9" style="color:var(--text3);text-align:center;padding:12px">Running...</td></tr>';
    const rsEl = document.getElementById("run-summary");
    if (rsEl) rsEl.style.display = "none";
    if (updateWarnings2) updateWarnings2();
    if (populateBrickTable2 && state.simData) populateBrickTable2(state.simData.brickLog);
    state.animRunning = true;
    state.animPaused = false;
    state.animStartWallMs = performance.now();
    state.animOffsetSimMs = 0;
    document.getElementById("runBtn").textContent = "Pause";
    document.getElementById("resetBtn").style.display = "inline-block";
    requestAnimationFrame(animFrame);
  }
  function startSim(computeSimulation2, params2, updateWarnings2, populateBrickTable2) {
    if (state.animRunning) return;
    state.simData = computeSimulation2(params2);
    state.replayMode = false;
    _launchAnimLoop(updateWarnings2, populateBrickTable2);
  }
  function resetSim(params2, drawBeltFn) {
    state.animRunning = false;
    state.animPaused = false;
    state.replayMode = false;
    state.haltOverlay = null;
    if (state.animFrameId) cancelAnimationFrame(state.animFrameId);
    resetAnimState();
    for (let i = 0; i < 4; i++) {
      if (cb.setBinCount) cb.setBinCount(i, 0);
      const box = document.getElementById("bin-" + i);
      if (box) box.className = "bin-box";
    }
    if (cb.updateThermalUI) cb.updateThermalUI();
    document.getElementById("runBtn").textContent = "Run Simulation";
    document.getElementById("resetBtn").style.display = "none";
    const rsEl = document.getElementById("run-summary");
    if (rsEl) rsEl.style.display = "none";
    const tlEl = document.getElementById("timeline-log");
    if (tlEl) tlEl.innerHTML = '<span style="color:var(--text3);font-size:12px">Run a simulation to see events here.</span>';
    const blEl = document.getElementById("brick-log-body");
    if (blEl) blEl.innerHTML = '<tr><td colspan="9" style="color:var(--text3);text-align:center;padding:12px">No data yet.</td></tr>';
    const rlEl = document.getElementById("replay-label");
    if (rlEl) rlEl.style.display = "none";
    const canvas = document.getElementById("belt-canvas");
    if (canvas) {
      const ctx = canvas.getContext("2d");
      if (drawBeltFn) drawBeltFn(ctx, canvas.width, canvas.height, 0);
    }
  }
  function animFrame(wallMs) {
    if (!state.animRunning) return;
    if (!state.animPaused) {
      const wallElapsed = wallMs - state.animStartWallMs;
      const simMs = state.animOffsetSimMs + wallElapsed * state.speedMultiplier;
      while (state.liveEventIdx < state.simData.events.length && state.simData.events[state.liveEventIdx].t <= simMs) {
        processEvent(state.simData.events[state.liveEventIdx], simMs);
        state.liveEventIdx++;
        if (state.haltOverlay) break;
      }
      updatePlowAngles(simMs);
      updateAnimBricks(simMs);
      const canvas = document.getElementById("belt-canvas");
      if (canvas) {
        const ctx = canvas.getContext("2d");
        if (cb.drawBelt) {
          cb.drawBelt(ctx, canvas.width, canvas.height, simMs);
        }
        if (state.haltOverlay && cb.drawErrorHaltOverlay) {
          cb.drawErrorHaltOverlay(ctx, canvas.width, canvas.height, state.haltOverlay.msg);
        }
      }
      if (cb.updateThermalUI) cb.updateThermalUI();
      if (state.haltOverlay) {
        state.animRunning = false;
        document.getElementById("runBtn").textContent = "Run Simulation";
        document.getElementById("resetBtn").style.display = "inline-block";
        return;
      }
      if (state.liveEventIdx >= state.simData.events.length) {
        state.animRunning = false;
        document.getElementById("runBtn").textContent = "Run Simulation";
        if (cb.showRunSummary) cb.showRunSummary(state.simData);
        if (cb.updateBinMatchState) cb.updateBinMatchState();
      }
    }
    if (state.animRunning) {
      state.animFrameId = requestAnimationFrame(animFrame);
    }
  }
  function processEvent(ev, simMs) {
    const tStr = ev.t.toFixed(0) + "ms";
    const p = state.params;
    switch (ev.type) {
      case "BRICK_RELEASED": {
        if (cb.addEvent) cb.addEvent(tStr, "RELEASED", `Brick #${ev.brickNum} (${ev.brickType}) at ${ev.sps}sps [${ev.thermalState}]`);
        if (ev.thermal) state.liveThermal = { ...ev.thermal };
        if (cb.setText) {
          cb.setText("m-thermal-state", ev.thermalState);
          cb.setText("m-active-sps", ev.sps + " sps");
        }
        const brickColor = BRICK_COLOR[ev.brickType] || "#888";
        const plowNum = p.plow_map[ev.brickType];
        state.animBricks.push({
          id: ev.brickNum,
          type: ev.brickType,
          spawnSim: ev.t,
          is2x3: ev.brickType.includes("2x3"),
          color: brickColor,
          plow: plowNum,
          binIdx: plowNum === 0 ? 3 : plowNum - 1,
          classified: false,
          classifySimMs: null,
          plowFireSimMs: null,
          binConfirmSimMs: null,
          done: false,
          flashBorder: 0
        });
        break;
      }
      case "BRICK_CLASSIFIED": {
        if (cb.addEvent) cb.addEvent(tStr, "CLASSIFIED", `#${ev.brickNum}: ${ev.sizeResult} ${ev.colorResult} | gap=${ev.gapUs}us | ${ev.samples} samples`);
        const ab = state.animBricks.find((b) => b.id === ev.brickNum);
        if (ab) {
          ab.classified = true;
          ab.classifySimMs = ev.t;
          ab.flashBorder = 200;
        }
        break;
      }
      case "PLOW_FIRE": {
        if (cb.addEvent) cb.addEvent(tStr, "PLOW_FIRE", `Plow ${ev.plow} fires for brick #${ev.brickNum}`);
        if (ev.thermal) state.liveThermal = { ...ev.thermal };
        const ps = state.plowStates[ev.plow];
        if (ps) {
          ps.phase = "fire";
          ps.phaseStartSim = ev.t;
          ps.angle = 0;
        }
        const ab = state.animBricks.find((b) => b.id === ev.brickNum);
        if (ab) ab.plowFireSimMs = ev.t;
        break;
      }
      case "PLOW_HOLD": {
        if (cb.addEvent) cb.addEvent(tStr, "PLOW_HOLD", `Plow ${ev.plow} holding (40% PWM)`);
        const ps = state.plowStates[ev.plow];
        if (ps) {
          ps.phase = "hold";
          ps.phaseStartSim = ev.t;
        }
        break;
      }
      case "PLOW_RELEASE": {
        if (cb.addEvent) cb.addEvent(tStr, "PLOW_RELEASE", `Plow ${ev.plow} de-energized, spring return`);
        const ps = state.plowStates[ev.plow];
        if (ps) {
          ps.phase = "retract";
          ps.phaseStartSim = ev.t;
        }
        break;
      }
      case "PLOW_SKIP":
        if (cb.addEvent) cb.addEvent(tStr, "PLOW_SKIP", ev.msg);
        break;
      case "BIN_CONFIRM": {
        if (cb.addEvent) cb.addEvent(tStr, "BIN_CONFIRM", `Brick #${ev.brickNum} -> Bin ${ev.binIdx + 1} (${["2x2 BLUE", "2x2 RED", "2x3 RED", "2x3 BLUE"][ev.binIdx]})`);
        state.liveBinCounts[ev.binIdx]++;
        if (cb.setBinCount) cb.setBinCount(ev.binIdx, state.liveBinCounts[ev.binIdx]);
        if (cb.flashBin) cb.flashBin(ev.binIdx);
        const ab = state.animBricks.find((b) => b.id === ev.brickNum);
        if (ab) ab.binConfirmSimMs = ev.t;
        break;
      }
      case "RUN_COMPLETE":
        if (cb.addEvent) cb.addEvent(tStr, "RUN_COMPLETE", `Run ${ev.run + 1} done. Bin counts: ${ev.binCounts.join(", ")}`);
        break;
      case "INTER_RUN_GAP":
        if (cb.addEvent) cb.addEvent(tStr, "INTER_RUN_GAP", `${ev.gapMs / 1e3}s cooldown between runs`);
        break;
      case "ERROR_HALT": {
        if (cb.addEvent) cb.addEvent(tStr, "ERROR_HALT", `Plow ${ev.plow} conflict on brick #${ev.brickNum}`);
        state.haltOverlay = {
          msg: `Plow ${ev.plow} conflict on brick #${ev.brickNum} at ${ev.t.toFixed(0)}ms`
        };
        break;
      }
      case "DRIFT_DETECTED": {
        const log = document.getElementById("timeline-log");
        if (log) {
          const row = document.createElement("div");
          row.className = "event-row";
          row.innerHTML = `<span class="ev-t" style="color:var(--yellow)">${tStr}</span><span class="ev-type" style="color:var(--yellow)">DRIFT</span><span class="ev-detail" style="color:var(--yellow)">${ev.msg}</span>`;
          log.appendChild(row);
          if (log.scrollTop + log.clientHeight >= log.scrollHeight - 40) log.scrollTop = log.scrollHeight;
        }
        break;
      }
    }
  }
  function updatePlowAngles(simMs) {
    const p = state.params;
    for (let i = 1; i <= 3; i++) {
      const ps = state.plowStates[i];
      if (!ps) continue;
      const elapsed = simMs - ps.phaseStartSim;
      if (ps.phase === "fire") {
        ps.angle = Math.min(35, elapsed / p.sol_full_ms * 35);
      } else if (ps.phase === "hold") {
        ps.angle = 35;
      } else if (ps.phase === "retract") {
        ps.angle = Math.max(0, 35 - elapsed / 20 * 35);
        if (ps.angle <= 0) {
          ps.phase = "idle";
          ps.angle = 0;
        }
      } else {
        ps.angle = 0;
      }
    }
  }
  function updateAnimBricks(simMs) {
    state.animBricks = state.animBricks.filter((b) => !b.done);
    for (const b of state.animBricks) {
      if (b.flashBorder > 0) b.flashBorder -= 16;
    }
  }
  function setSpeed(s) {
    if (state.animRunning && !state.animPaused) {
      const wallNow = performance.now();
      const wallElapsed = wallNow - state.animStartWallMs;
      state.animOffsetSimMs += wallElapsed * state.speedMultiplier;
      state.animStartWallMs = wallNow;
    }
    state.speedMultiplier = s;
    ["spd1", "spd2", "spd5", "spd10"].forEach((id) => {
      const el = document.getElementById(id);
      if (el) el.classList.remove("active");
    });
    const activeEl = document.getElementById("spd" + s);
    if (activeEl) activeEl.classList.add("active");
  }

  // src/canvas.js
  var CY = 120;
  var BT = 100;
  var BB = 140;
  var FX = {
    chute: 0.03,
    taperStart: 0.07,
    taperEnd: 0.17,
    beam1: 0.24,
    beam2: 0.28,
    color: 0.26,
    bufferEnd: 0.35,
    plow1: 0.42,
    plow2: 0.57,
    plow3: 0.72,
    end: 0.9
  };
  var BIN_Y = [30, 210, 30, CY];
  var PLOW_DIR = [null, "up", "down", "up"];
  var PLOW_FX = [null, FX.plow1, FX.plow2, FX.plow3];
  var BRICK_COLOR = { "2x2_blue": "#185FA5", "2x2_red": "#E24B4A", "2x3_red": "#E24B4A", "2x3_blue": "#185FA5" };
  function drawLabel(ctx, cx, cy, text, size, color) {
    ctx.fillStyle = color;
    ctx.font = `${size}px system-ui`;
    ctx.textAlign = "center";
    ctx.fillText(text, cx, cy);
  }
  function drawHatched(ctx, x, y, w, h, bg, line) {
    ctx.fillStyle = bg;
    ctx.fillRect(x, y, w, h);
    ctx.save();
    ctx.beginPath();
    ctx.rect(x, y, w, h);
    ctx.clip();
    ctx.strokeStyle = line;
    ctx.lineWidth = 1;
    for (let i = -h; i < w + h; i += 8) {
      ctx.beginPath();
      ctx.moveTo(x + i, y);
      ctx.lineTo(x + i + h, y + h);
      ctx.stroke();
    }
    ctx.restore();
    ctx.strokeStyle = "#555";
    ctx.lineWidth = 1;
    ctx.strokeRect(x, y, w, h);
  }
  function drawBinBox(ctx, bx, by, bw, bh, label, binIdx, count, numRuns) {
    const BASE_EXPECTED = [6, 6, 4, 8];
    const runs = numRuns || 1;
    const exp = BASE_EXPECTED[binIdx] * runs;
    const isMatch = count === exp && !state.animRunning;
    ctx.fillStyle = isMatch ? "rgba(74,222,128,0.1)" : "rgba(50,50,60,0.8)";
    ctx.strokeStyle = isMatch ? "#4ade80" : "#555";
    ctx.lineWidth = 1;
    ctx.beginPath();
    ctx.roundRect(bx, by, bw, bh, 3);
    ctx.fill();
    ctx.stroke();
    ctx.fillStyle = "#9ca3af";
    ctx.font = "7px system-ui";
    ctx.textAlign = "center";
    ctx.fillText(label, bx + bw / 2, by + 10);
    ctx.fillStyle = "#f9fafb";
    ctx.font = "bold 12px monospace";
    ctx.fillText(count, bx + bw / 2, by + bh - 7);
  }
  function drawPlow(ctx, plowNum, W, H, simMs, liveBinCounts, plowStates, params2, numRuns) {
    const ps = plowStates[plowNum];
    if (!ps) return;
    const bx = W * PLOW_FX[plowNum];
    const angle = ps.angle;
    const dir = PLOW_DIR[plowNum];
    let armColor = "#555";
    if (ps.phase === "fire") armColor = "#fb923c";
    else if (ps.phase === "hold") armColor = "#fbbf24";
    else if (ps.phase === "retract") armColor = "#888";
    const rad = angle * Math.PI / 180;
    const armLength = (BB - BT) * 0.85;
    ctx.save();
    if (dir === "up") {
      ctx.translate(bx, BB);
      ctx.rotate(-rad);
      ctx.strokeStyle = armColor;
      ctx.lineWidth = 3;
      ctx.beginPath();
      ctx.moveTo(0, 0);
      ctx.lineTo(-armLength * Math.sin(rad * 0.3 + 0.1), -armLength);
      ctx.stroke();
      ctx.fillStyle = armColor;
      ctx.beginPath();
      ctx.arc(-armLength * Math.sin(rad * 0.3 + 0.1), -armLength, 3, 0, Math.PI * 2);
      ctx.fill();
    } else {
      ctx.translate(bx, BT);
      ctx.rotate(rad);
      ctx.strokeStyle = armColor;
      ctx.lineWidth = 3;
      ctx.beginPath();
      ctx.moveTo(0, 0);
      ctx.lineTo(-armLength * Math.sin(rad * 0.3 + 0.1), armLength);
      ctx.stroke();
      ctx.fillStyle = armColor;
      ctx.beginPath();
      ctx.arc(-armLength * Math.sin(rad * 0.3 + 0.1), armLength, 3, 0, Math.PI * 2);
      ctx.fill();
    }
    ctx.restore();
    const binIdx = plowNum - 1;
    const binLabels = ["2x2 BLUE", "2x2 RED", "2x3 RED"];
    const byOffset = dir === "up" ? BT - 50 : BB + 14;
    const boxH = 30;
    drawBinBox(ctx, bx - 30, byOffset - (dir === "up" ? boxH : 0), 60, boxH, binLabels[binIdx], binIdx, liveBinCounts[binIdx], numRuns);
    const lbl = ["P1", "P2", "P3"][plowNum - 1];
    drawLabel(ctx, bx, dir === "up" ? BB + 10 : BT - 10, lbl, 8, armColor === "#555" ? "#555" : armColor);
  }
  function drawAnimBrick(ctx, b, simMs, beltPxPerMs, W, H, params2) {
    const elapsed = simMs - b.spawnSim;
    if (elapsed < 0) return;
    const pxPerMm = W * (FX.end - FX.chute) / 500;
    const beltMm = elapsed * params2.belt_target_mm_s / 1e3;
    const chuteX = W * FX.chute;
    const brickW = b.is2x3 ? 15 : 10;
    const brickH = 10;
    const plowMmPositions = [null, 210, 285, 360];
    const endMm = 480;
    const plowMm = b.plow > 0 ? plowMmPositions[b.plow] : endMm;
    const deflectDist = 55;
    if (beltMm > plowMm + deflectDist) {
      b.done = true;
      return;
    }
    let drawX, drawY;
    if (beltMm < plowMm) {
      drawX = chuteX + beltMm * pxPerMm;
      drawY = CY;
      const beam1Mm = 116;
      if (b.flashBorder > 0 && beltMm > beam1Mm) {
        ctx.strokeStyle = "#ffffff";
        ctx.lineWidth = 2;
        ctx.strokeRect(drawX - brickW / 2 - 1, drawY - brickH / 2 - 1, brickW + 2, brickH + 2);
      }
    } else {
      const t = Math.min((beltMm - plowMm) / deflectDist, 1);
      drawX = chuteX + (plowMm + t * 15) * pxPerMm;
      const targetY = BIN_Y[b.binIdx];
      drawY = CY + (targetY - CY) * t;
      ctx.globalAlpha = 1 - t * 0.5;
    }
    ctx.fillStyle = b.color;
    ctx.fillRect(drawX - brickW / 2, drawY - brickH / 2, brickW, brickH);
    ctx.fillStyle = "rgba(255,255,255,0.25)";
    if (b.is2x3) {
      ctx.beginPath();
      ctx.arc(drawX - 4, drawY, 1.5, 0, Math.PI * 2);
      ctx.fill();
      ctx.beginPath();
      ctx.arc(drawX, drawY, 1.5, 0, Math.PI * 2);
      ctx.fill();
      ctx.beginPath();
      ctx.arc(drawX + 4, drawY, 1.5, 0, Math.PI * 2);
      ctx.fill();
    } else {
      ctx.beginPath();
      ctx.arc(drawX - 2, drawY, 1.5, 0, Math.PI * 2);
      ctx.fill();
      ctx.beginPath();
      ctx.arc(drawX + 2, drawY, 1.5, 0, Math.PI * 2);
      ctx.fill();
    }
    ctx.globalAlpha = 1;
  }
  function drawErrorHaltOverlay(ctx, W, H, msg) {
    ctx.fillStyle = "rgba(0,0,0,0.65)";
    ctx.fillRect(0, 0, W, H);
    const bannerH = 60;
    const bannerY = H / 2 - bannerH / 2;
    ctx.fillStyle = "rgba(220,38,38,0.92)";
    ctx.fillRect(0, bannerY, W, bannerH);
    ctx.strokeStyle = "#f87171";
    ctx.lineWidth = 2;
    ctx.strokeRect(0, bannerY, W, bannerH);
    ctx.fillStyle = "#fff";
    ctx.font = "bold 16px system-ui";
    ctx.textAlign = "center";
    ctx.fillText("ERROR_HALT", W / 2, bannerY + 22);
    ctx.font = "11px monospace";
    ctx.fillText(msg, W / 2, bannerY + 44);
  }
  function drawBelt(ctx, W, H, simMs, params2, liveBinCounts, plowStates, animBricks, replayMode) {
    const p = params2;
    const pxPerMm = W * (FX.end - FX.chute) / 500;
    const beltPxPerMs = p.belt_target_mm_s * pxPerMm / 1e3;
    const x = (f) => W * f;
    ctx.fillStyle = "#141414";
    ctx.fillRect(0, 0, W, H);
    ctx.fillStyle = "#222";
    ctx.fillRect(x(FX.chute), BT, x(FX.end) - x(FX.chute), BB - BT);
    ctx.strokeStyle = "#555";
    ctx.lineWidth = 1.5;
    ctx.beginPath();
    ctx.moveTo(x(FX.chute), BT);
    ctx.lineTo(x(FX.end), BT);
    ctx.stroke();
    ctx.beginPath();
    ctx.moveTo(x(FX.chute), BB);
    ctx.lineTo(x(FX.end), BB);
    ctx.stroke();
    ctx.strokeStyle = "#2a2a2a";
    ctx.lineWidth = 1;
    const ridgeSpacing = 20;
    const ridgeOffset = simMs * beltPxPerMs % ridgeSpacing;
    for (let rx = x(FX.chute) + ridgeOffset - ridgeSpacing; rx < x(FX.end); rx += ridgeSpacing) {
      if (rx < x(FX.chute)) continue;
      ctx.beginPath();
      ctx.moveTo(rx, BT + 2);
      ctx.lineTo(rx, BB - 2);
      ctx.stroke();
    }
    ctx.strokeStyle = "#666";
    ctx.lineWidth = 2;
    const taperY1 = BT - 18, taperY2 = BT;
    ctx.beginPath();
    ctx.moveTo(x(FX.taperStart), taperY1);
    ctx.lineTo(x(FX.taperEnd), taperY2);
    ctx.stroke();
    ctx.beginPath();
    ctx.moveTo(x(FX.taperStart), BB + 18);
    ctx.lineTo(x(FX.taperEnd), BB);
    ctx.stroke();
    drawLabel(ctx, x(FX.taperStart) + (x(FX.taperEnd) - x(FX.taperStart)) / 2, BT - 24, "TAPER", 9, "#666");
    ctx.strokeStyle = "#555";
    ctx.lineWidth = 1;
    const chuteW = x(FX.taperStart) - x(FX.chute);
    const chuteH = 60;
    drawHatched(ctx, x(FX.chute), CY - chuteH / 2, chuteW, chuteH, "#333", "#444");
    drawLabel(ctx, x(FX.chute) + chuteW / 2, CY + 28, "CHUTE", 8, "#888");
    const senseX = x(FX.beam1) - 4;
    const senseW = x(FX.beam2) + 4 - senseX;
    ctx.fillStyle = "rgba(26,80,120,0.25)";
    ctx.fillRect(senseX, BT, senseW, BB - BT);
    ctx.strokeStyle = "#1a6080";
    ctx.lineWidth = 1;
    ctx.strokeRect(senseX, BT, senseW, BB - BT);
    ctx.strokeStyle = "#ff6666";
    ctx.lineWidth = 1.5;
    ctx.setLineDash([3, 2]);
    ctx.beginPath();
    ctx.moveTo(x(FX.beam1), BT - 8);
    ctx.lineTo(x(FX.beam1), BB + 8);
    ctx.stroke();
    ctx.setLineDash([]);
    drawLabel(ctx, x(FX.beam1), BT - 14, "B1", 8, "#ff6666");
    ctx.strokeStyle = "#ff9944";
    ctx.lineWidth = 1.5;
    ctx.setLineDash([3, 2]);
    ctx.beginPath();
    ctx.moveTo(x(FX.beam2), BT - 8);
    ctx.lineTo(x(FX.beam2), BB + 8);
    ctx.stroke();
    ctx.setLineDash([]);
    drawLabel(ctx, x(FX.beam2), BT - 14, "B2", 8, "#ff9944");
    const gapPx = x(FX.beam2) - x(FX.beam1);
    ctx.strokeStyle = "#888";
    ctx.lineWidth = 1;
    ctx.beginPath();
    ctx.moveTo(x(FX.beam1), BB + 14);
    ctx.lineTo(x(FX.beam2), BB + 14);
    ctx.stroke();
    drawLabel(ctx, x(FX.beam1) + gapPx / 2, BB + 22, `${p.beam_gap_mm}mm`, 8, "#888");
    ctx.fillStyle = "#4fc3f7";
    ctx.beginPath();
    ctx.arc(x(FX.color), BT - 10, 5, 0, Math.PI * 2);
    ctx.fill();
    drawLabel(ctx, x(FX.color), BT - 22, "COLOR", 7, "#4fc3f7");
    drawLabel(ctx, senseX + senseW / 2, CY, "SIZE + COLOR", 8, "#4fc3f7");
    ctx.strokeStyle = "#444";
    ctx.lineWidth = 1;
    ctx.setLineDash([4, 3]);
    ctx.beginPath();
    ctx.moveTo(x(FX.beam2), CY);
    ctx.lineTo(x(FX.plow1) - 2, CY);
    ctx.stroke();
    ctx.setLineDash([]);
    drawLabel(ctx, x(FX.beam2) + (x(FX.plow1) - x(FX.beam2)) / 2, CY - 14, `lead: ${p.sol_lead_ms}ms`, 8, "#555");
    const numRuns = p.num_runs || 1;
    drawPlow(ctx, 1, W, H, simMs, liveBinCounts, plowStates, p, numRuns);
    drawPlow(ctx, 2, W, H, simMs, liveBinCounts, plowStates, p, numRuns);
    drawPlow(ctx, 3, W, H, simMs, liveBinCounts, plowStates, p, numRuns);
    drawBinBox(ctx, x(FX.end) + 4, CY - 18, 50, 36, "2x3 BLUE", 3, liveBinCounts[3], numRuns);
    for (const b of animBricks) {
      drawAnimBrick(ctx, b, simMs, beltPxPerMs, W, H, p);
    }
    if (replayMode) {
      ctx.fillStyle = "rgba(96,165,250,0.18)";
      ctx.fillRect(W - 100, 6, 94, 18);
      ctx.fillStyle = "#60a5fa";
      ctx.font = "bold 10px system-ui";
      ctx.textAlign = "center";
      ctx.fillText("REPLAY MODE", W - 53, 19);
    }
  }

  // src/metrics.js
  function setText(id, text) {
    const el = document.getElementById(id);
    if (el) el.textContent = text;
  }
  function setTextClass(id, text, cls) {
    const el = document.getElementById(id);
    if (el) {
      el.textContent = text;
      el.className = "mv " + (cls || "");
    }
  }
  function updateMetrics() {
    const p = state.params;
    if (!p) return;
    const escRate = p.stepper_sps / p.stepper_steps_per_rev;
    const beltCap = p.belt_target_mm_s / p.brick_spacing_mm;
    const margin = (beltCap - escRate) / escRate * 100;
    const isBelt = beltCap < escRate;
    const estTime = p.total_bricks / escRate;
    setText("m-esc-rate", escRate.toFixed(2) + " bricks/s");
    setText("m-belt-spd", p.belt_target_mm_s + " mm/s");
    setTextClass("m-belt-cap", beltCap.toFixed(2) + " bricks/s", isBelt ? "r" : "");
    setTextClass("m-bottleneck", isBelt ? "BELT" : "ESCAPEMENT", isBelt ? "r" : "g");
    setTextClass("m-margin", (margin >= 0 ? "+" : "") + margin.toFixed(0) + "%", margin < 0 ? "r" : margin < 20 ? "y" : "g");
    setText("m-runtime", "~" + estTime.toFixed(1) + "s");
    const dwell2x2 = p.brick_2x2_mm / p.belt_target_mm_s * 1e3;
    const samples = Math.floor(dwell2x2 / p.color_integration_ms);
    const sigWin = p.beam_gap_mm / p.belt_target_mm_s * 1e3;
    const tmarg = p.size_timeout_ms - sigWin;
    setText("m-dwell", dwell2x2.toFixed(1) + " ms");
    setTextClass("m-samples", samples + " samples", samples < 4 ? "r" : samples < 8 ? "y" : "g");
    setText("m-sig-win", "0 - " + sigWin.toFixed(0) + " ms");
    setText("m-timeout", p.size_timeout_ms + " ms");
    setTextClass("m-tmarg", tmarg.toFixed(0) + " ms", tmarg < 0 ? "r" : tmarg < 20 ? "y" : "g");
    for (let i = 1; i <= 4; i++) {
      const wEl = document.getElementById("tw" + i);
      const dEl = document.getElementById("td" + i);
      if (wEl) wEl.style.left = p.thermal_warn_level * 100 + "%";
      if (dEl) dEl.style.left = p.thermal_danger_level * 100 + "%";
    }
    if (!state.animRunning) {
      setText("m-active-sps", p.stepper_sps + " sps");
      setText("m-thermal-state", "NORMAL");
    }
    const BASE_EXPECTED = [6, 6, 4, 8];
    const numRuns = p.num_runs || 1;
    for (let i = 0; i < 4; i++) {
      const expEl = document.getElementById("bin-exp-" + i);
      if (expEl) {
        const baseExp = BASE_EXPECTED[i];
        if (numRuns > 1) {
          expEl.textContent = `expected: ${numRuns}x${baseExp} = ${numRuns * baseExp}`;
        } else {
          expEl.textContent = `expected: ${baseExp}`;
        }
      }
    }
  }
  function updateThermalUI() {
    const p = state.params;
    if (!p) return;
    const liveThermal = state.liveThermal;
    const bars = [
      ["sol1", "th-sol1-bar", "th-sol1-val"],
      ["sol2", "th-sol2-bar", "th-sol2-val"],
      ["sol3", "th-sol3-bar", "th-sol3-val"],
      ["stepper", "th-step-bar", "th-step-val"]
    ];
    const maxHeat = Math.max(liveThermal.sol1, liveThermal.sol2, liveThermal.sol3, liveThermal.stepper);
    for (const [key, barId, valId] of bars) {
      const h = liveThermal[key] || 0;
      const bar = document.getElementById(barId);
      const val = document.getElementById(valId);
      if (bar) {
        bar.style.width = h * 100 + "%";
        bar.style.background = h > p.thermal_danger_level ? "var(--red)" : h > p.thermal_warn_level ? "var(--yellow)" : "var(--green)";
      }
      if (val) val.textContent = h.toFixed(2);
    }
    if (!state.animRunning) {
      const stateEl = document.getElementById("m-thermal-state");
      if (stateEl) {
        const thermalState = maxHeat > p.thermal_danger_level ? "DANGER" : maxHeat > p.thermal_warn_level ? "WARNING" : "NORMAL";
        stateEl.textContent = thermalState;
        stateEl.className = "mv " + (thermalState === "DANGER" ? "r" : thermalState === "WARNING" ? "y" : "g");
      }
    }
  }
  function showRunSummary(sd) {
    const p = state.params;
    document.getElementById("run-summary").style.display = "block";
    const totalSec = sd.totalSimMs / 1e3;
    setText("m-total-time", totalSec.toFixed(2) + "s");
    const totalBricks = sd.brickLog.length;
    setText("m-bricks-sorted", totalBricks + " / " + p.total_bricks * p.num_runs);
    setText("m-accuracy", "100% (sim always classifies correctly)");
    setTextClass("m-peak-heat", sd.peakHeat.toFixed(3), sd.peakHeat > p.thermal_danger_level ? "r" : sd.peakHeat > p.thermal_warn_level ? "y" : "g");
  }
  function setBinCount(idx, count) {
    state.liveBinCounts[idx] = count;
    const el = document.getElementById("bin-cnt-" + idx);
    if (el) el.textContent = count;
  }
  function flashBin(idx) {
    const box = document.getElementById("bin-" + idx);
    if (!box) return;
    box.classList.add("flash");
    setTimeout(() => box.classList.remove("flash"), 120);
  }
  function updateBinMatchState() {
    const p = state.params;
    if (!p) return;
    const BASE_EXPECTED = [6, 6, 4, 8];
    const numRuns = p.num_runs || 1;
    for (let i = 0; i < 4; i++) {
      const exp = BASE_EXPECTED[i] * numRuns;
      const box = document.getElementById("bin-" + i);
      if (box) {
        box.className = "bin-box " + (state.liveBinCounts[i] === exp ? "match" : "over");
      }
    }
  }

  // src/controls.js
  var _updateMetrics = null;
  var _updateWarnings = null;
  function registerControlCallbacks(cbs) {
    _updateMetrics = cbs.updateMetrics;
    _updateWarnings = cbs.updateWarnings;
  }
  function buildControls(params2) {
    const panel = document.getElementById("controls");
    if (!panel) return;
    panel.innerHTML = "";
    for (const sec of SECTIONS) {
      const div = document.createElement("div");
      div.className = "section";
      const hdr = document.createElement("div");
      hdr.className = "sec-hdr";
      hdr.innerHTML = `<span class="sec-title">${sec.label}</span><span class="chev">&#9660;</span>`;
      const body = document.createElement("div");
      body.className = "sec-body";
      hdr.onclick = () => {
        hdr.classList.toggle("closed");
        body.classList.toggle("hidden");
      };
      div.appendChild(hdr);
      if (sec.id === "run") {
        body.innerHTML = `
        <div class="ctrl">
          <div class="ctrl-top"><span class="ctrl-name">Number of runs</span></div>
          <div class="ctrl-desc">Simulate back-to-back runs to see thermal accumulation across runs.</div>
          <input type="number" id="ctrl-num_runs" min="1" max="10" value="${params2.num_runs}"
            style="margin-top:4px" onchange="window._setParam('num_runs', parseInt(this.value)||1)">
        </div>
        <div class="ctrl">
          <div class="ctrl-top"><span class="ctrl-name">Brick sequence</span></div>
          <div class="ctrl-desc">Order bricks are released. Worst-case concentrates solenoid fires together for maximum thermal stress.</div>
          <select id="ctrl-sequence" onchange="window._setParam('sequence', this.value)">
            <option value="interleaved">Interleaved (round-robin by type)</option>
            <option value="worst_case">Worst-case (all plow bricks first)</option>
            <option value="default_last">Default-last (default-path bricks last)</option>
          </select>
        </div>
        <div class="ctrl">
          <div class="ctrl-top"><span class="ctrl-name">Inter-run gap</span><span class="ctrl-val">${params2.interrun_gap_ms / 1e3}s</span></div>
          <div class="ctrl-desc">Cooldown time between runs (reload time). Thermal model keeps decaying.</div>
          <input type="range" id="ctrl-interrun_gap_ms" min="1000" max="30000" step="1000"
            value="${params2.interrun_gap_ms}"
            oninput="window._setParamSlider('interrun_gap_ms', this.value, 'ctrl-val-interrun_gap_ms', 1000, 's')">
        </div>`;
      } else {
        for (const ctrl of sec.controls) {
          const scale = ctrl.scale || 1;
          const raw = params2[ctrl.id];
          const display = Math.round(raw * scale * 100) / 100;
          const dec = ctrl.dec || 0;
          const valStr = display.toFixed(dec) + (ctrl.unit ? " " + ctrl.unit : "");
          const cdiv = document.createElement("div");
          cdiv.className = "ctrl";
          cdiv.innerHTML = `
          <div class="ctrl-top">
            <span class="ctrl-name">${ctrl.label}</span>
            <span class="ctrl-val" id="cv-${ctrl.id}">${valStr}</span>
          </div>
          <input type="range"
            id="ctrl-${ctrl.id}"
            min="${ctrl.min}" max="${ctrl.max}" step="${ctrl.step || 1}"
            value="${raw * scale}"
            ${ctrl.locked ? "disabled" : ""}
            oninput="window._onSlider('${ctrl.id}', this.value)">
          <div class="ctrl-desc">${ctrl.desc}</div>`;
          body.appendChild(cdiv);
        }
        const resetBtn = document.createElement("button");
        resetBtn.className = "btn sec-reset";
        resetBtn.textContent = "Reset " + sec.label + " to defaults";
        resetBtn.onclick = () => resetSection(sec, params2);
        body.appendChild(resetBtn);
      }
      div.appendChild(body);
      panel.appendChild(div);
    }
  }
  function onSlider(id, rawVal, params2) {
    const ctrl = findCtrl(id);
    const scale = ctrl ? ctrl.scale || 1 : 1;
    const dec = ctrl ? ctrl.dec || 0 : 0;
    const unit = ctrl ? ctrl.unit || "" : "";
    const val = parseFloat(rawVal) / scale;
    params2[id] = val;
    const display = parseFloat(rawVal).toFixed(dec) + (unit ? " " + unit : "");
    const el = document.getElementById("cv-" + id);
    if (el) el.textContent = display;
    if (_updateMetrics) _updateMetrics();
    if (_updateWarnings) _updateWarnings();
  }
  function setParam(id, val, params2) {
    params2[id] = val;
    if (_updateMetrics) _updateMetrics();
    if (_updateWarnings) _updateWarnings();
  }
  function setParamSlider(id, rawVal, valElId, divisor, unit, params2) {
    params2[id] = parseFloat(rawVal);
    const el = document.getElementById("cv-" + id);
    if (el) el.textContent = (parseFloat(rawVal) / divisor).toFixed(0) + unit;
    const valEl = document.getElementById(valElId);
    if (valEl) valEl.textContent = (parseFloat(rawVal) / divisor).toFixed(0) + unit;
    if (_updateMetrics) _updateMetrics();
  }
  function findCtrl(id) {
    for (const sec of SECTIONS) {
      for (const c of sec.controls || []) {
        if (c.id === id) return c;
      }
    }
    return null;
  }
  function resetSection(sec, params2) {
    for (const ctrl of sec.controls || []) {
      if (ctrl.locked) continue;
      params2[ctrl.id] = DEFAULTS[ctrl.id];
      const scale = ctrl.scale || 1;
      const dec = ctrl.dec || 0;
      const unit = ctrl.unit || "";
      const el = document.getElementById("ctrl-" + ctrl.id);
      if (el) el.value = DEFAULTS[ctrl.id] * scale;
      const velEl = document.getElementById("cv-" + ctrl.id);
      if (velEl) velEl.textContent = (DEFAULTS[ctrl.id] * scale).toFixed(dec) + (unit ? " " + unit : "");
    }
    if (_updateMetrics) _updateMetrics();
    if (_updateWarnings) _updateWarnings();
  }

  // src/tabs.js
  function switchTab(tab) {
    document.querySelectorAll(".tab").forEach((el, i) => {
      const ids = ["timeline", "bricks", "warnings"];
      el.classList.toggle("active", ids[i] === tab);
    });
    document.querySelectorAll(".tab-panel").forEach((el) => el.classList.remove("active"));
    const panel = document.getElementById("panel-" + tab);
    if (panel) panel.classList.add("active");
  }
  function addEvent(t, type, detail) {
    const log = document.getElementById("timeline-log");
    if (!log) return;
    const row = document.createElement("div");
    row.className = "event-row";
    row.innerHTML = `<span class="ev-t">${t}</span><span class="ev-type">${type}</span><span class="ev-detail">${detail}</span>`;
    log.appendChild(row);
    if (log.scrollTop + log.clientHeight >= log.scrollHeight - 40) {
      log.scrollTop = log.scrollHeight;
    }
  }
  function populateBrickTable(brickLog) {
    const body = document.getElementById("brick-log-body");
    if (!body) return;
    const thermalColors = { NORMAL: "", WARNING: "y", DANGER: "r" };
    body.innerHTML = brickLog.map((b) => {
      const tc = thermalColors[b.thermalState] || "";
      const typeLabel = b.type.replace("_", " ").toUpperCase();
      return `<tr>
      <td>${b.num}</td>
      <td>${typeLabel}</td>
      <td>${b.sizeResult}</td>
      <td>${b.colorResult}</td>
      <td><span style="color:${b.samples < 4 ? "var(--red)" : b.samples < 8 ? "var(--yellow)" : "var(--green)"}">${b.samples}</span></td>
      <td>${b.plow === 0 ? "none" : b.plow}</td>
      <td>${b.binIdx + 1}</td>
      <td>${b.transitMs}</td>
      <td><span style="color:${tc === "r" ? "var(--red)" : tc === "y" ? "var(--yellow)" : "var(--green)"}">${b.thermalState}</span></td>
    </tr>`;
    }).join("");
  }
  function updateWarningsUI(warnings) {
    const container = document.getElementById("warnings-content");
    if (!container) return;
    container.innerHTML = warnings.map(
      (w) => `<div class="warn-row ${w.level}">${w.msg}</div>`
    ).join("");
  }

  // src/replay.js
  var SIZE_TIMEOUT_MS_FALLBACK = 150;
  function parseReplayCSV(csvText) {
    const lines = csvText.trim().split("\n").map((l) => l.trim()).filter((l) => l.length > 0);
    if (lines.length < 2) return null;
    const headers = lines[0].split(",").map((h) => h.trim());
    if (!headers.includes("brick") || !headers.includes("ts_enter_ms")) return null;
    const rows = [];
    for (let i = 1; i < lines.length; i++) {
      const vals = lines[i].split(",");
      if (vals.length < 16) continue;
      const row = {};
      for (let j = 0; j < headers.length && j < vals.length; j++) {
        row[headers[j]] = vals[j] ? vals[j].trim() : "";
      }
      row.brick = parseInt(row.brick) || 0;
      row.ts_enter_ms = parseFloat(row.ts_enter_ms) || 0;
      row.ts_confirm_ms = parseFloat(row.ts_confirm_ms) || 0;
      row.transit_ms = parseFloat(row.transit_ms) || 0;
      row.gap_us = parseFloat(row.gap_us) || 0;
      row.valid_samples = parseInt(row.valid_samples) || 0;
      row.plow = parseInt(row.plow) || 0;
      row.bin = parseInt(row.bin) || 0;
      row.ok = parseInt(row.ok) || 0;
      row.heat_sol1 = parseFloat(row.heat_sol1) || 0;
      row.heat_sol2 = parseFloat(row.heat_sol2) || 0;
      row.heat_sol3 = parseFloat(row.heat_sol3) || 0;
      row.heat_step = parseFloat(row.heat_step) || 0;
      rows.push(row);
    }
    return rows.length > 0 ? rows : null;
  }
  function buildReplayEvents(rows, params2) {
    if (!rows || rows.length === 0) return null;
    const events = [];
    const brickLog = [];
    let thermalDangerReached = false;
    let peakHeat = 0;
    const binCounts = [0, 0, 0, 0];
    for (let i = 0; i < rows.length; i++) {
      const row = rows[i];
      const brickNum = row.brick;
      const T_enter = row.ts_enter_ms;
      const brickType = row.type || "2x3_blue";
      const is2x3 = brickType.includes("2x3");
      const binIdx = Math.max(0, Math.min(3, row.bin - 1));
      const thermal = {
        sol1: row.heat_sol1,
        sol2: row.heat_sol2,
        sol3: row.heat_sol3,
        stepper: row.heat_step
      };
      const maxHeat = Math.max(thermal.sol1, thermal.sol2, thermal.sol3, thermal.stepper);
      peakHeat = Math.max(peakHeat, maxHeat);
      if (row.thermal_state === "DANGER") thermalDangerReached = true;
      const sps = row.thermal_state === "DANGER" ? 267 : row.thermal_state === "WARNING" ? 533 : params2.stepper_sps || 800;
      events.push({
        t: T_enter,
        type: "BRICK_RELEASED",
        brickNum,
        brickType,
        run: 0,
        sps,
        thermalState: row.thermal_state || "NORMAL",
        thermal: { ...thermal }
      });
      events.push({ t: T_enter, type: "BRICK_AT_SENSOR", brickNum, brickType });
      let T_classified;
      if (is2x3 && row.gap_us > 0) {
        T_classified = T_enter + row.gap_us / 1e3;
      } else {
        T_classified = T_enter + SIZE_TIMEOUT_MS_FALLBACK;
      }
      events.push({
        t: T_classified,
        type: "BRICK_CLASSIFIED",
        brickNum,
        brickType,
        sizeResult: row.size || (is2x3 ? "2x3" : "2x2"),
        colorResult: row.color || (brickType.includes("red") ? "RED" : "BLUE"),
        gapUs: row.gap_us || 0,
        samples: row.valid_samples || 0
      });
      const plowNum = row.plow;
      if (plowNum > 0) {
        const solFull = params2.sol_full_ms || 20;
        const solDeenergize = params2.sol_deenergize_ms || 280;
        events.push({ t: T_classified, type: "PLOW_FIRE", plow: plowNum, brickNum, thermal: { ...thermal } });
        events.push({ t: T_classified + solFull, type: "PLOW_HOLD", plow: plowNum, brickNum });
        events.push({ t: T_classified + solDeenergize, type: "PLOW_RELEASE", plow: plowNum, brickNum });
      } else {
        events.push({ t: T_classified, type: "PLOW_SKIP", brickNum, msg: "default path - no solenoid" });
      }
      events.push({ t: row.ts_confirm_ms, type: "BIN_CONFIRM", brickNum, binIdx, brickType });
      binCounts[binIdx]++;
      const binDist = (params2.bin_distances_mm || [250, 280, 310, 360])[binIdx];
      const beltSpeed = params2.belt_target_mm_s || 200;
      const expectedTransit = Math.round(binDist / beltSpeed * 1e3);
      const actualTransit = row.transit_ms;
      if (Math.abs(actualTransit - expectedTransit) > 150) {
        const delta = actualTransit - expectedTransit;
        events.push({
          t: row.ts_confirm_ms,
          type: "DRIFT_DETECTED",
          brickNum,
          msg: `Brick #${brickNum}: actual transit ${actualTransit}ms vs expected ${expectedTransit}ms (delta ${delta > 0 ? "+" : ""}${delta}ms) - belt speed drift detected`
        });
      }
      brickLog.push({
        num: brickNum,
        run: 1,
        type: brickType,
        sizeResult: row.size || (is2x3 ? "2x3" : "2x2"),
        colorResult: row.color || (brickType.includes("red") ? "RED" : "BLUE"),
        samples: row.valid_samples || 0,
        plow: plowNum,
        binIdx,
        transitMs: Math.round(actualTransit),
        thermalState: row.thermal_state || "NORMAL",
        correct: row.ok === 1
      });
    }
    const lastConfirm = rows.reduce((m, r) => Math.max(m, r.ts_confirm_ms), 0);
    events.push({ t: lastConfirm + 500, type: "RUN_COMPLETE", run: 0, binCounts: [...binCounts], totalMs: lastConfirm });
    events.sort((a, b) => a.t - b.t);
    const totalSimMs = events[events.length - 1]?.t || 0;
    return { events, brickLog, thermalDangerReached, peakHeat, totalSimMs, haltEvent: null };
  }
  function startReplay(csvText, params2, updateWarnings2, populateBrickTable2) {
    const rows = parseReplayCSV(csvText);
    if (!rows) {
      alert("Could not parse CSV. Make sure it is a firmware serial log with the correct header.");
      return;
    }
    const replayData = buildReplayEvents(rows, params2);
    if (!replayData) {
      alert("No valid brick rows found in CSV.");
      return;
    }
    state.replayMode = true;
    state.simData = replayData;
    const rlEl = document.getElementById("replay-label");
    if (rlEl) rlEl.style.display = "inline-block";
    _launchAnimLoop(updateWarnings2, populateBrickTable2);
  }

  // src/main.js
  state.params = JSON.parse(JSON.stringify(DEFAULTS));
  var params = state.params;
  function setText2(id, text) {
    const el = document.getElementById(id);
    if (el) el.textContent = text;
  }
  function updateWarnings() {
    const warnings = computeWarnings(params, state.simData);
    updateWarningsUI(warnings);
  }
  function doDrawBelt(ctx, W, H, simMs) {
    drawBelt(ctx, W, H, simMs, params, state.liveBinCounts, state.plowStates, state.animBricks, state.replayMode);
  }
  function doDrawErrorHaltOverlay(ctx, W, H, msg) {
    drawErrorHaltOverlay(ctx, W, H, msg);
  }
  registerCallbacks({
    updateWarnings,
    showRunSummary,
    updateThermalUI,
    setBinCount,
    flashBin,
    updateBinMatchState,
    drawBelt: doDrawBelt,
    drawErrorHaltOverlay: doDrawErrorHaltOverlay,
    addEvent,
    populateBrickTable,
    setText: setText2
  });
  registerControlCallbacks({
    updateMetrics,
    updateWarnings
  });
  window._onSlider = (id, val) => onSlider(id, val, params);
  window._setParam = (id, val) => setParam(id, val, params);
  window._setParamSlider = (id, rawVal, valElId, divisor, unit) => setParamSlider(id, rawVal, valElId, divisor, unit, params);
  window.switchTab = switchTab;
  window.setSpeed = (s) => setSpeed(s);
  window.handleRunBtn = function() {
    if (state.animRunning) {
      state.animPaused = !state.animPaused;
      document.getElementById("runBtn").textContent = state.animPaused ? "Resume" : "Pause";
      return;
    }
    startSim(computeSimulation, params, updateWarnings, populateBrickTable);
  };
  window.resetSim = function() {
    resetSim(params, doDrawBelt);
  };
  function resizeCanvas() {
    const canvas = document.getElementById("belt-canvas");
    const wrap = document.getElementById("belt-wrap");
    if (!canvas || !wrap) return;
    canvas.width = wrap.clientWidth;
    canvas.height = 240;
    if (!state.animRunning) {
      const ctx = canvas.getContext("2d");
      doDrawBelt(ctx, canvas.width, canvas.height, 0);
    }
  }
  function handleLoadCSV() {
    const input = document.createElement("input");
    input.type = "file";
    input.accept = ".csv,text/csv,text/plain";
    input.onchange = (e) => {
      const file = e.target.files[0];
      if (!file) return;
      const reader = new FileReader();
      reader.onload = (ev) => {
        if (state.animRunning) {
          resetSim(params, doDrawBelt);
        }
        startReplay(ev.target.result, params, updateWarnings, populateBrickTable);
      };
      reader.readAsText(file);
    };
    input.click();
  }
  function init() {
    buildControls(params);
    updateMetrics();
    updateWarnings();
    resizeCanvas();
    window.addEventListener("resize", resizeCanvas);
    const loadCsvBtn = document.getElementById("loadCsvBtn");
    if (loadCsvBtn) loadCsvBtn.addEventListener("click", handleLoadCSV);
  }
  init();
})();
