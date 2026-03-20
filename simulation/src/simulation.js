// simulation.js - discrete event engine: generateSequence(), computeSimulation()

'use strict';

import { decayHeat, coolingPctToRate } from './thermal.js';

export function generateSequence(p) {
  const types = ['2x2_blue', '2x2_red', '2x3_red', '2x3_blue'];
  const seq = [];
  if (p.sequence === 'worst_case') {
    for (const t of types) for (let i = 0; i < p.counts[t]; i++) seq.push(t);
  } else if (p.sequence === 'default_last') {
    // Legacy: plow bricks first, default path bricks last
    for (const t of ['2x2_blue', '2x2_red', '2x3_red']) for (let i = 0; i < p.counts[t]; i++) seq.push(t);
    for (let i = 0; i < p.counts['2x3_blue']; i++) seq.push('2x3_blue');
  } else if (p.sequence === 'random') {
    // All bricks in array then Fisher-Yates shuffle
    for (const t of types) for (let i = 0; i < p.counts[t]; i++) seq.push(t);
    for (let i = seq.length - 1; i > 0; i--) {
      const j = Math.floor(Math.random() * (i + 1));
      [seq[i], seq[j]] = [seq[j], seq[i]];
    }
  } else if (p.sequence === 'default_first') {
    // Default path (2x3_blue, no plow) first, then plow bricks last
    for (let i = 0; i < p.counts['2x3_blue']; i++) seq.push('2x3_blue');
    for (const t of ['2x2_blue', '2x2_red', '2x3_red']) for (let i = 0; i < p.counts[t]; i++) seq.push(t);
  } else {
    // interleaved: round-robin
    const rem = Object.fromEntries(types.map(t => [t, p.counts[t]]));
    let safety = 200;
    while (seq.length < p.total_bricks && safety-- > 0) {
      for (const t of types) {
        if (rem[t] > 0 && seq.length < p.total_bricks) { seq.push(t); rem[t]--; }
      }
    }
  }
  return seq;
}

// computeSimulation returns:
//   { events, brickLog, thermalDangerReached, peakHeat, totalSimMs, haltEvent,
//     throttleCount, throttledMs, pureRunMs, totalGapMs, avgBps }
// haltEvent is non-null if an ERROR_HALT occurred: { brickNum, plow, simMs }
export function computeSimulation(p) {
  const DIST_CHUTE_TO_SENSOR_MM = 100;

  // Compute cooling rate once from the percent param
  const rate = coolingPctToRate(p.thermal_cooling_pct);

  let thermal = {sol1:0, sol2:0, sol3:0, stepper:0};
  let lastReleaseT = 0;
  let binCounts = [0, 0, 0, 0];
  let thermalDangerReached = false;
  let peakHeat = 0;

  const allBrickLog = [];
  const allEvents = [];
  let globalBrickNum = 0;

  // Track when each plow de-energizes so we can detect conflicts
  // plowDeenergizeAt[1..3] = simMs when plow finishes de-energizing
  const plowDeenergizeAt = [0, 0, 0, 0];

  let haltEvent = null;

  // Throttle stats
  let throttleCount = 0;
  let throttledMs = 0;

  // Pure run time tracking (excluding inter-run gaps)
  let pureRunMs = 0;

  const THERMAL_UPDATE_INTERVAL_MS = 500;

  for (let run = 0; run < p.num_runs; run++) {
    const seq = generateSequence(p);

    if (run > 0) {
      const gapStartT = lastReleaseT;
      allEvents.push({ t: gapStartT, type: 'INTER_RUN_GAP', run: run - 1, gapMs: p.interrun_gap_ms });

      // Emit THERMAL_UPDATE events every 500ms during the gap so bars animate smoothly
      const thermalAtGapStart = {...thermal};
      for (let dt = THERMAL_UPDATE_INTERVAL_MS; dt <= p.interrun_gap_ms; dt += THERMAL_UPDATE_INTERVAL_MS) {
        const elapsed = dt / 1000;
        allEvents.push({
          t: gapStartT + dt,
          type: 'THERMAL_UPDATE',
          thermal: {
            sol1:    decayHeat(thermalAtGapStart.sol1,    elapsed, rate),
            sol2:    decayHeat(thermalAtGapStart.sol2,    elapsed, rate),
            sol3:    decayHeat(thermalAtGapStart.sol3,    elapsed, rate),
            stepper: decayHeat(thermalAtGapStart.stepper, elapsed, rate),
          }
        });
      }

      // Apply full gap decay before the next run starts
      const gapSec = p.interrun_gap_ms / 1000;
      thermal.sol1    = decayHeat(thermal.sol1,    gapSec, rate);
      thermal.sol2    = decayHeat(thermal.sol2,    gapSec, rate);
      thermal.sol3    = decayHeat(thermal.sol3,    gapSec, rate);
      thermal.stepper = decayHeat(thermal.stepper, gapSec, rate);
      lastReleaseT += p.interrun_gap_ms;
    }

    const normalReleaseInterval = (p.stepper_steps_per_rev / p.stepper_sps) * 1000;
    let runStartT = 0;
    let runLastConfirmT = 0;

    for (let i = 0; i < seq.length; i++) {
      const brickType = seq[i];
      globalBrickNum++;

      const maxHeat = Math.max(thermal.sol1, thermal.sol2, thermal.sol3, thermal.stepper);
      let sps = p.stepper_sps;
      let thermalState = 'NORMAL';
      if (maxHeat > p.thermal_danger_level) { sps = 267; thermalState = 'DANGER'; thermalDangerReached = true; }
      else if (maxHeat > p.thermal_warn_level) { sps = 533; thermalState = 'WARNING'; }

      const releaseInterval = (p.stepper_steps_per_rev / sps) * 1000;

      // Track throttle stats
      if (thermalState !== 'NORMAL') {
        throttleCount++;
        throttledMs += releaseInterval - normalReleaseInterval;
      }

      const T_release = lastReleaseT + releaseInterval;

      // Track run start time (first brick of each run)
      if (i === 0) runStartT = T_release;

      const dtSec = (T_release - lastReleaseT) / 1000;
      thermal.sol1    = decayHeat(thermal.sol1,    dtSec, rate);
      thermal.sol2    = decayHeat(thermal.sol2,    dtSec, rate);
      thermal.sol3    = decayHeat(thermal.sol3,    dtSec, rate);
      thermal.stepper = decayHeat(thermal.stepper, dtSec, rate);

      thermal.stepper = Math.min(thermal.stepper + p.thermal_heat_per_step, 1.0);
      peakHeat = Math.max(peakHeat, thermal.stepper);

      allEvents.push({
        t: T_release, type: 'BRICK_RELEASED', brickNum: globalBrickNum, brickType, run,
        sps, thermalState, thermal: {...thermal}
      });

      const T_sense = T_release + (DIST_CHUTE_TO_SENSOR_MM / p.belt_target_mm_s) * 1000;
      allEvents.push({ t: T_sense, type: 'BRICK_AT_SENSOR', brickNum: globalBrickNum, brickType });

      const brickLen = brickType.includes('2x3') ? p.brick_2x3_mm : p.brick_2x2_mm;
      const is2x3 = brickType.includes('2x3');
      let sizeResolveMs, gapUs;
      if (is2x3) {
        gapUs = Math.round((p.beam_gap_mm / p.belt_target_mm_s) * 1e6);
        sizeResolveMs = (p.beam_gap_mm / p.belt_target_mm_s) * 1000;
      } else {
        gapUs = 0;
        sizeResolveMs = p.size_timeout_ms;
      }

      const dwellMs = (brickLen / p.belt_target_mm_s) * 1000;
      const samples = Math.max(1, Math.floor(dwellMs / p.color_integration_ms));

      const T_classified = T_sense + Math.max(sizeResolveMs, dwellMs);
      allEvents.push({
        t: T_classified, type: 'BRICK_CLASSIFIED', brickNum: globalBrickNum, brickType,
        sizeResult: is2x3 ? '2x3' : '2x2',
        colorResult: brickType.includes('red') ? 'RED' : 'BLUE',
        gapUs, samples
      });

      const plowNum = p.plow_map[brickType];
      const binIdx = plowNum === 0 ? 3 : plowNum - 1;

      if (plowNum > 0) {
        // Check for ERROR_HALT: is the plow still energized?
        if (T_classified < plowDeenergizeAt[plowNum]) {
          // Plow conflict - emit ERROR_HALT and stop
          haltEvent = { brickNum: globalBrickNum, plow: plowNum, simMs: T_classified };
          allEvents.push({
            t: T_classified,
            type: 'ERROR_HALT',
            brickNum: globalBrickNum,
            plow: plowNum,
            msg: `Plow ${plowNum} still energized (until ${plowDeenergizeAt[plowNum].toFixed(0)}ms) when brick #${globalBrickNum} fires at ${T_classified.toFixed(0)}ms`
          });
          allEvents.sort((a, b) => a.t - b.t);
          const totalSimMs = allEvents[allEvents.length - 1]?.t || 0;
          return {
            events: allEvents, brickLog: allBrickLog, thermalDangerReached, peakHeat,
            totalSimMs, haltEvent,
            throttleCount, throttledMs, pureRunMs,
            totalGapMs: (p.num_runs - 1) * p.interrun_gap_ms,
            avgBps: 0
          };
        }

        const solKey = 'sol' + plowNum;
        const dt2 = (T_classified - T_release) / 1000;
        thermal[solKey] = decayHeat(thermal[solKey], dt2, rate);
        thermal[solKey] = Math.min(thermal[solKey] + p.thermal_heat_per_sol, 1.0);
        peakHeat = Math.max(peakHeat, thermal[solKey]);

        // Record when this plow will de-energize
        plowDeenergizeAt[plowNum] = T_classified + p.sol_deenergize_ms;

        allEvents.push({ t: T_classified, type: 'PLOW_FIRE', plow: plowNum, brickNum: globalBrickNum, thermal: {...thermal} });
        allEvents.push({ t: T_classified + p.sol_full_ms, type: 'PLOW_HOLD', plow: plowNum, brickNum: globalBrickNum });
        allEvents.push({ t: T_classified + p.sol_deenergize_ms, type: 'PLOW_RELEASE', plow: plowNum, brickNum: globalBrickNum });
      } else {
        allEvents.push({ t: T_classified, type: 'PLOW_SKIP', brickNum: globalBrickNum, msg: 'default path - no solenoid' });
      }

      const binDist = p.bin_distances_mm[binIdx];
      const T_confirm = T_sense + (binDist / p.belt_target_mm_s) * 1000;
      allEvents.push({ t: T_confirm, type: 'BIN_CONFIRM', brickNum: globalBrickNum, binIdx, brickType });

      runLastConfirmT = T_confirm;
      binCounts[binIdx]++;
      const transitMs = Math.round(T_confirm - T_release);

      allBrickLog.push({
        num: globalBrickNum, run: run + 1, type: brickType,
        sizeResult: is2x3 ? '2x3' : '2x2',
        colorResult: brickType.includes('red') ? 'RED' : 'BLUE',
        samples, plow: plowNum, binIdx, transitMs, thermalState, correct: true
      });

      lastReleaseT = T_release;
    }

    // Accumulate pure sorting time for this run (first release to last confirm)
    if (seq.length > 0) {
      pureRunMs += runLastConfirmT - runStartT;
    }

    allEvents.push({ t: lastReleaseT + 2000, type: 'RUN_COMPLETE', run, binCounts: [...binCounts], totalMs: lastReleaseT });
    binCounts = [0, 0, 0, 0];
  }

  allEvents.sort((a, b) => a.t - b.t);

  const totalSimMs = allEvents[allEvents.length - 1]?.t || 0;
  const totalGapMs = (p.num_runs - 1) * p.interrun_gap_ms;
  const avgBps = pureRunMs > 0 ? (allBrickLog.length / (pureRunMs / 1000)) : 0;

  return {
    events: allEvents, brickLog: allBrickLog, thermalDangerReached, peakHeat,
    totalSimMs, haltEvent: null,
    throttleCount, throttledMs, pureRunMs, totalGapMs, avgBps
  };
}
