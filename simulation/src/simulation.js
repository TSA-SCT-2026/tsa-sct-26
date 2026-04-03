// simulation.js
// V6 chamber-drop discrete event model.

'use strict';

import { CATEGORY_TO_BIN } from './defaults.js';
import { coolingPctToRate, decayHeat } from './thermal.js';

const STATE = {
  IDLE: 'IDLE',
  FEED: 'FEED',
  APPROACH: 'APPROACH',
  SEATED: 'SEATED',
  SENSING: 'SENSING',
  INDEXED: 'INDEXED',
  RELEASED: 'RELEASED',
  CONFIRM: 'CONFIRM',
  RESET: 'RESET',
  COMPLETE: 'COMPLETE',
  ERROR_HALT: 'ERROR_HALT',
};

const TYPES = ['2x2_red', '2x2_blue', '2x3_blue', '2x3_red'];

function clamp01(v) {
  return Math.max(0, Math.min(1, v));
}

function categorySize(type) {
  return type.includes('2x3') ? '2x3' : '2x2';
}

function categoryColor(type) {
  return type.includes('red') ? 'RED' : 'BLUE';
}

function expectedCountsForRun(params) {
  return [
    params.counts['2x2_red'] || 0,
    params.counts['2x2_blue'] || 0,
    params.counts['2x3_blue'] || 0,
    params.counts['2x3_red'] || 0,
  ];
}

function sequenceInterleaved(rem, total) {
  const seq = [];
  while (seq.length < total) {
    let pushed = false;
    for (const t of TYPES) {
      if (rem[t] > 0) {
        seq.push(t);
        rem[t]--;
        pushed = true;
      }
    }
    if (!pushed) break;
  }
  return seq;
}

function sequenceWorstCase(rem) {
  const seq = [];
  for (const t of TYPES) {
    for (let i = 0; i < rem[t]; i++) seq.push(t);
  }
  return seq;
}

function sequenceDefaultFirst(rem) {
  const seq = [];
  for (let i = 0; i < rem['2x3_red']; i++) seq.push('2x3_red');
  rem['2x3_red'] = 0;
  for (const t of TYPES) {
    for (let i = 0; i < rem[t]; i++) seq.push(t);
  }
  return seq;
}

function sequenceWorstCaseAccuracy(rem, total) {
  const seq = [];
  const p = ['2x2_red', '2x2_blue', '2x3_blue'];
  let pi = 0;
  while (seq.length < total) {
    let appended = false;
    for (let k = 0; k < p.length; k++) {
      const t = p[(pi + k) % p.length];
      if (rem[t] > 0) {
        seq.push(t);
        rem[t]--;
        pi = (pi + k + 1) % p.length;
        appended = true;
        break;
      }
    }
    if (!appended) break;
    if (rem['2x3_red'] > 0 && seq.length < total) {
      seq.push('2x3_red');
      rem['2x3_red']--;
    }
  }
  for (const t of TYPES) {
    while (rem[t] > 0 && seq.length < total) {
      seq.push(t);
      rem[t]--;
    }
  }
  return seq;
}

export function generateSequence(params) {
  const rem = Object.fromEntries(TYPES.map((t) => [t, params.counts[t] || 0]));
  const total = TYPES.reduce((n, t) => n + rem[t], 0);

  if (params.sequence === 'worst_case') return sequenceWorstCase(rem);
  if (params.sequence === 'default_first') return sequenceDefaultFirst(rem);
  if (params.sequence === 'worst_case_accuracy') return sequenceWorstCaseAccuracy(rem, total);
  if (params.sequence === 'random') {
    const seq = sequenceWorstCase(rem);
    for (let i = seq.length - 1; i > 0; i--) {
      const j = Math.floor(Math.random() * (i + 1));
      [seq[i], seq[j]] = [seq[j], seq[i]];
    }
    return seq;
  }
  return sequenceInterleaved(rem, total);
}

function shortestStepDelta(current, target, revSteps) {
  let delta = target - current;
  if (delta > revSteps / 2) delta -= revSteps;
  if (delta < -revSteps / 2) delta += revSteps;
  return delta;
}

function thermalState(maxHeat, params) {
  if (maxHeat >= params.thermal_danger_level) return 'DANGER';
  if (maxHeat >= params.thermal_warn_level) return 'WARNING';
  return 'NORMAL';
}

function indexSpeedForState(stateLabel, params) {
  if (stateLabel === 'DANGER') return Math.max(120, Math.round(params.stepper_run_sps * 0.50));
  if (stateLabel === 'WARNING') return Math.max(180, Math.round(params.stepper_run_sps * 0.75));
  return params.stepper_run_sps;
}

function indexDurationMs(moveSteps, params, activeRunSps) {
  if (moveSteps <= 0) return 0;
  const accelSteps = Math.min(params.stepper_decel_steps, Math.floor(moveSteps / 2));
  const cruiseSteps = Math.max(0, moveSteps - accelSteps * 2);
  const v0 = Math.max(1, params.stepper_start_sps);
  const v1 = Math.max(v0 + 1, activeRunSps);
  const accelTime = (2 * accelSteps) / (v0 + v1);
  const cruiseTime = cruiseSteps / v1;
  return Math.round((accelTime * 2 + cruiseTime) * 1000);
}

function colorSenseDuration(params) {
  const targetMs = params.color_sample_count * params.color_integration_ms;
  return Math.min(targetMs, params.color_timeout_ms);
}

function addStateEvent(events, t, state, brickNum, detail = '') {
  events.push({ t, type: 'STATE_ENTER', state, brickNum, detail });
}

export function computeSimulation(params) {
  const events = [];
  const brickLog = [];
  const runSummary = [];

  const expectedPerRun = expectedCountsForRun(params);
  const rate = coolingPctToRate(params.thermal_cooling_pct);

  let totalErrors = 0;
  let totalHalts = 0;
  let totalThrottleBricks = 0;
  let totalThrottleDelayMs = 0;
  let overallCorrect = 0;
  let overallTotal = 0;
  let overallPeakHeat = 0;
  let thermalDangerReached = false;

  let globalT = 0;
  let globalBrickNum = 0;

  for (let run = 0; run < params.num_runs; run++) {
    const seq = generateSequence(params);
    const runBins = [0, 0, 0, 0];
    let runState = STATE.IDLE;
    let statePositionSteps = params.bin_steps[params.home_bin_index];
    let runStart = globalT;
    let haltEvent = null;
    let lastEventT = globalT;
    let runPeakHeat = 0;

    let thermal = { sol1: 0, sol2: 0, sol3: 0, stepper: 0 };

    const applyDecay = (nowT) => {
      const dt = Math.max(0, nowT - lastEventT) / 1000;
      thermal.sol1 = decayHeat(thermal.sol1, dt, rate);
      thermal.sol2 = decayHeat(thermal.sol2, dt, rate);
      thermal.sol3 = decayHeat(thermal.sol3, dt, rate);
      thermal.stepper = decayHeat(thermal.stepper, dt, rate);
      lastEventT = nowT;
    };

    addStateEvent(events, globalT, STATE.IDLE, 0, `run ${run + 1} ready`);

    for (let i = 0; i < seq.length; i++) {
      const type = seq[i];
      const sizeResult = categorySize(type);
      const colorResult = categoryColor(type);
      const targetBin = CATEGORY_TO_BIN[type];
      const targetSteps = params.bin_steps[targetBin];
      globalBrickNum++;
      overallTotal++;

      addStateEvent(events, globalT, STATE.FEED, globalBrickNum);
      runState = STATE.FEED;

      const feedMs = Math.round((params.transport_distance_mm / params.belt_target_mm_s) * 1000);
      let tEntry = globalT + feedMs;
      if (feedMs > params.feed_timeout_ms) {
        tEntry = globalT + params.feed_timeout_ms;
        addStateEvent(events, tEntry, STATE.ERROR_HALT, globalBrickNum, 'JAM_CHUTE');
        haltEvent = { t: tEntry, state: STATE.ERROR_HALT, code: 'JAM_CHUTE', brickNum: globalBrickNum };
        totalErrors++;
        totalHalts++;
        break;
      }

      events.push({
        t: tEntry,
        type: 'ENTRY_BEAM_TRIGGERED',
        brickNum: globalBrickNum,
        run,
        category: type,
      });

      addStateEvent(events, tEntry, STATE.APPROACH, globalBrickNum);
      runState = STATE.APPROACH;
      const tSeated = tEntry + params.entry_to_stop_ms;
      if (params.entry_to_stop_ms > params.approach_timeout_ms) {
        addStateEvent(events, tEntry + params.approach_timeout_ms, STATE.ERROR_HALT, globalBrickNum, 'JAM_APPROACH');
        haltEvent = { t: tEntry + params.approach_timeout_ms, state: STATE.ERROR_HALT, code: 'JAM_APPROACH', brickNum: globalBrickNum };
        totalErrors++;
        totalHalts++;
        break;
      }

      events.push({
        t: tSeated,
        type: 'BRICK_SEATED',
        brickNum: globalBrickNum,
        run,
      });

      addStateEvent(events, tSeated, STATE.SEATED, globalBrickNum);
      runState = STATE.SEATED;

      const tSensingStart = tSeated + params.settle_ms;
      addStateEvent(events, tSensingStart, STATE.SENSING, globalBrickNum);
      runState = STATE.SENSING;

      const senseMs = colorSenseDuration(params);
      const validSamples = Math.max(
        0,
        Math.min(params.color_sample_count, Math.floor(params.color_timeout_ms / params.color_integration_ms)),
      );
      const tSensingDone = tSensingStart + Math.max(params.size_read_ms, senseMs);

      if (validSamples < params.color_min_samples) {
        addStateEvent(events, tSensingDone, STATE.ERROR_HALT, globalBrickNum, 'SENSOR_FAULT');
        haltEvent = { t: tSensingDone, state: STATE.ERROR_HALT, code: 'SENSOR_FAULT', brickNum: globalBrickNum };
        totalErrors++;
        totalHalts++;
        break;
      }

      events.push({
        t: tSensingDone,
        type: 'BRICK_CLASSIFIED',
        brickNum: globalBrickNum,
        run,
        category: type,
        sizeResult,
        colorResult,
        samples: validSamples,
        targetBin,
      });

      applyDecay(tSensingDone);
      const preIndexMax = Math.max(thermal.sol1, thermal.sol2, thermal.sol3, thermal.stepper);
      const tState = thermalState(preIndexMax, params);
      if (tState !== 'NORMAL') totalThrottleBricks++;
      if (tState === 'DANGER') thermalDangerReached = true;
      const activeSps = indexSpeedForState(tState, params);
      const normalIndexMs = indexDurationMs(Math.abs(shortestStepDelta(statePositionSteps, targetSteps, params.stepper_steps_per_rev)), params, params.stepper_run_sps);

      addStateEvent(events, tSensingDone, STATE.INDEXED, globalBrickNum);
      runState = STATE.INDEXED;

      const moveDelta = shortestStepDelta(statePositionSteps, targetSteps, params.stepper_steps_per_rev);
      const moveSteps = Math.abs(moveDelta);
      const indexMs = indexDurationMs(moveSteps, params, activeSps);
      const indexEnd = tSensingDone + indexMs;
      events.push({
        t: tSensingDone,
        type: 'DISC_INDEX_START',
        brickNum: globalBrickNum,
        run,
        fromSteps: statePositionSteps,
        toSteps: targetSteps,
        moveSteps,
        indexMs,
        activeSps,
      });
      const thermalStepInc = params.thermal_heat_per_step * (moveSteps / params.stepper_steps_per_rev);
      thermal.stepper = clamp01(thermal.stepper + thermalStepInc);
      runPeakHeat = Math.max(runPeakHeat, thermal.stepper);
      overallPeakHeat = Math.max(overallPeakHeat, thermal.stepper);

      if (activeSps < params.stepper_run_sps) {
        totalThrottleDelayMs += Math.max(0, indexMs - normalIndexMs);
      }

      events.push({
        t: indexEnd,
        type: 'DISC_INDEXED',
        brickNum: globalBrickNum,
        run,
        targetBin,
        moveSteps,
        activeSps,
        thermalState: tState,
      });

      statePositionSteps = ((targetSteps % params.stepper_steps_per_rev) + params.stepper_steps_per_rev) % params.stepper_steps_per_rev;

      addStateEvent(events, indexEnd, STATE.RELEASED, globalBrickNum);
      runState = STATE.RELEASED;

      const releaseEnd = indexEnd + params.solenoid_on_ms;
      const settleDone = indexEnd + params.fall_settle_ms;
      applyDecay(indexEnd);
      thermal.sol1 = clamp01(thermal.sol1 + params.thermal_heat_per_sol);
      runPeakHeat = Math.max(runPeakHeat, thermal.sol1);
      overallPeakHeat = Math.max(overallPeakHeat, thermal.sol1);

      events.push({
        t: indexEnd,
        type: 'PLATFORM_RELEASED',
        brickNum: globalBrickNum,
        run,
        targetBin,
      });
      events.push({
        t: releaseEnd,
        type: 'SOLENOID_OFF',
        brickNum: globalBrickNum,
        run,
      });

      addStateEvent(events, settleDone, STATE.CONFIRM, globalBrickNum);
      runState = STATE.CONFIRM;

      const confirmAt = settleDone + params.bin_confirm_latency_ms;
      if (params.bin_confirm_latency_ms > params.bin_confirm_timeout_ms) {
        addStateEvent(events, settleDone + params.bin_confirm_timeout_ms, STATE.ERROR_HALT, globalBrickNum, 'MISS_BIN');
        haltEvent = { t: settleDone + params.bin_confirm_timeout_ms, state: STATE.ERROR_HALT, code: 'MISS_BIN', brickNum: globalBrickNum };
        totalErrors++;
        totalHalts++;
        break;
      }

      events.push({
        t: confirmAt,
        type: 'BIN_CONFIRM',
        brickNum: globalBrickNum,
        run,
        binIdx: targetBin,
        category: type,
        correct: true,
      });

      runBins[targetBin]++;
      overallCorrect++;

      addStateEvent(events, confirmAt, STATE.RESET, globalBrickNum);
      runState = STATE.RESET;

      const resetDone = confirmAt + Math.max(params.token_restore_ms, 1);
      events.push({
        t: resetDone,
        type: 'TOKEN_RESTORED',
        brickNum: globalBrickNum,
        run,
      });

      brickLog.push({
        num: globalBrickNum,
        run: run + 1,
        type,
        sizeResult,
        colorResult,
        samples: validSamples,
        targetBin: targetBin + 1,
        actualBin: targetBin + 1,
        binIdx: targetBin,
        indexSteps: moveSteps,
        indexMs,
        transitMs: Math.round(confirmAt - tEntry),
        totalBrickMs: Math.round(resetDone - globalT),
        thermalState: thermalState(Math.max(thermal.sol1, thermal.sol2, thermal.sol3, thermal.stepper), params),
        correct: true,
      });

      if ((i + 1) % params.rehome_period_bricks === 0) {
        events.push({
          t: resetDone + 2,
          type: 'REHOME_CHECK',
          brickNum: globalBrickNum,
          run,
          ok: true,
        });
      }

      globalT = resetDone;
    }

    const runEnd = globalT;
    runState = haltEvent ? STATE.ERROR_HALT : STATE.COMPLETE;
    addStateEvent(events, runEnd, runState, 0, haltEvent ? haltEvent.code : `run ${run + 1} complete`);
    events.push({
      t: runEnd,
      type: haltEvent ? 'RUN_HALTED' : 'RUN_COMPLETE',
      run,
      binCounts: [...runBins],
      totalMs: Math.max(0, runEnd - runStart),
      haltCode: haltEvent ? haltEvent.code : null,
    });

    const expected = expectedPerRun;
    const runCorrect = runBins.reduce((n, v, idx) => n + Math.min(v, expected[idx]), 0);
    const runTotal = runBins.reduce((n, v) => n + v, 0);
    runSummary.push({
      run: run + 1,
      halted: Boolean(haltEvent),
      haltCode: haltEvent ? haltEvent.code : null,
      totalMs: Math.max(0, runEnd - runStart),
      binCounts: [...runBins],
      expectedCounts: [...expected],
      accuracy: runTotal > 0 ? (runCorrect / runTotal) * 100 : 0,
      peakHeat: runPeakHeat,
    });

    if (run < params.num_runs - 1) {
      const gapStart = globalT;
      events.push({
        t: gapStart,
        type: 'INTER_RUN_GAP',
        run,
        gapMs: params.interrun_gap_ms,
      });
      const step = 500;
      for (let dt = step; dt <= params.interrun_gap_ms; dt += step) {
        events.push({
          t: gapStart + dt,
          type: 'THERMAL_UPDATE',
          run,
          thermal: {
            sol1: decayHeat(thermal.sol1, dt / 1000, rate),
            sol2: decayHeat(thermal.sol2, dt / 1000, rate),
            sol3: decayHeat(thermal.sol3, dt / 1000, rate),
            stepper: decayHeat(thermal.stepper, dt / 1000, rate),
          },
        });
      }
      globalT += params.interrun_gap_ms;
    }
  }

  events.sort((a, b) => a.t - b.t);

  const pureRunMs = runSummary.reduce((n, r) => n + r.totalMs, 0);
  const totalGapMs = Math.max(0, params.num_runs - 1) * params.interrun_gap_ms;
  const totalSimMs = pureRunMs + totalGapMs;
  const avgBps = pureRunMs > 0 ? overallTotal / (pureRunMs / 1000) : 0;
  const accuracy = overallTotal > 0 ? (overallCorrect / overallTotal) * 100 : 0;

  return {
    events,
    brickLog,
    runSummary,
    totalErrors,
    totalHalts,
    thermalDangerReached,
    peakHeat: overallPeakHeat,
    totalSimMs,
    pureRunMs,
    totalGapMs,
    avgBps,
    throttleCount: totalThrottleBricks,
    throttledMs: totalThrottleDelayMs,
    accuracy,
    haltEvent: null,
  };
}
