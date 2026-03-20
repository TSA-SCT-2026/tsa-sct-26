// simulation.js - discrete event engine: generateSequence(), computeSimulation()

'use strict';

import { decayHeat, coolingPctToRate, misfireProbability } from './thermal.js';

// Plow spring return time after de-energize (ms)
const RETRACT_MS = 20;

export function generateSequence(p) {
  const types = ['2x2_blue', '2x2_red', '2x3_red', '2x3_blue'];
  const seq = [];

  if (p.sequence === 'worst_case' || p.sequence === 'worst_case_thermal') {
    // Group all bricks by type to maximize heat accumulation per solenoid
    for (const t of types) for (let i = 0; i < p.counts[t]; i++) seq.push(t);

  } else if (p.sequence === 'worst_case_accuracy') {
    // Alternate each plow-using brick with a default brick to maximize cross-plow conflicts
    // at high sps. After defaults run out, dump remaining plow bricks consecutively.
    const plowTypes = ['2x2_blue', '2x2_red', '2x3_red'];
    const rem = Object.fromEntries(types.map(t => [t, p.counts[t]]));
    let pi = 0;
    while (true) {
      let added = false;
      for (let k = 0; k < plowTypes.length; k++) {
        const t = plowTypes[(pi + k) % plowTypes.length];
        if (rem[t] > 0) {
          seq.push(t); rem[t]--;
          pi = (pi + k + 1) % plowTypes.length;
          added = true; break;
        }
      }
      if (!added || seq.length >= p.total_bricks) break;
      if (rem['2x3_blue'] > 0) { seq.push('2x3_blue'); rem['2x3_blue']--; }
      if (seq.length >= p.total_bricks) break;
    }
    for (const t of types) while (rem[t] > 0 && seq.length < p.total_bricks) { seq.push(t); rem[t]--; }

  } else if (p.sequence === 'default_last') {
    // Legacy alias: plow bricks first, default last
    for (const t of ['2x2_blue', '2x2_red', '2x3_red']) for (let i = 0; i < p.counts[t]; i++) seq.push(t);
    for (let i = 0; i < p.counts['2x3_blue']; i++) seq.push('2x3_blue');

  } else if (p.sequence === 'default_first') {
    // Default path bricks first, plow bricks last
    for (let i = 0; i < p.counts['2x3_blue']; i++) seq.push('2x3_blue');
    for (const t of ['2x2_blue', '2x2_red', '2x3_red']) for (let i = 0; i < p.counts[t]; i++) seq.push(t);

  } else if (p.sequence === 'random') {
    // Fisher-Yates shuffle - each run gets its own independent order
    for (const t of types) for (let i = 0; i < p.counts[t]; i++) seq.push(t);
    for (let i = seq.length - 1; i > 0; i--) {
      const j = Math.floor(Math.random() * (i + 1));
      [seq[i], seq[j]] = [seq[j], seq[i]];
    }

  } else {
    // interleaved: round-robin by type
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

// Compute the time (ms) a brick arrives at plow `plowIdx` (1-3) after T_classified.
// Plow 1 is at sol_lead_ms from sensing. Plows 2 and 3 are proportionally further
// based on bin_distances_mm (distance from sensing zone to each bin entrance).
function plowArrivalMs(T_classified, plowIdx, p) {
  const extraMm = p.bin_distances_mm[plowIdx - 1] - p.bin_distances_mm[0];
  return T_classified + p.sol_lead_ms + (extraMm / p.belt_target_mm_s) * 1000;
}

// computeSimulation returns a full event log and per-brick statistics.
// Routing is physically modeled: bricks trace through plows in order, and the first
// extended plow catches them regardless of intent. No ERROR_HALT - routing errors
// are logged and the run continues.
export function computeSimulation(p) {
  const DIST_CHUTE_TO_SENSOR_MM = 100;

  const rate = coolingPctToRate(p.thermal_cooling_pct);
  const normalReleaseInterval = (p.stepper_steps_per_rev / p.stepper_sps) * 1000;
  // Minimum release interval when same-type fast-release is active (belt spacing limit)
  const minSameTypeInterval = (p.brick_spacing_mm / p.belt_target_mm_s) * 1000;

  let thermal = {sol1:0, sol2:0, sol3:0, stepper:0};
  let lastReleaseT = 0;
  let binCounts = [0, 0, 0, 0];
  let thermalDangerReached = false;
  let peakHeat = 0;

  const allBrickLog = [];
  const allEvents = [];
  let globalBrickNum = 0;

  // plowExtendedUntil[n]: sim time when plow n finishes retracting (fully idle)
  const plowExtendedUntil = [0, 0, 0, 0];

  let throttleCount = 0;
  let throttledMs = 0;
  let pureRunMs = 0;

  // Error tracking
  let totalErrors = 0;
  const errorCauses = { plow_conflict: 0, thermal_misfire: 0, sol_retract_early: 0 };

  const THERMAL_UPDATE_INTERVAL_MS = 500;

  for (let run = 0; run < p.num_runs; run++) {
    const seq = generateSequence(p);

    if (run > 0) {
      const gapStartT = lastReleaseT;
      allEvents.push({ t: gapStartT, type: 'INTER_RUN_GAP', run: run - 1, gapMs: p.interrun_gap_ms });

      // Emit smooth THERMAL_UPDATE events during the cooldown gap
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

      const gapSec = p.interrun_gap_ms / 1000;
      thermal.sol1    = decayHeat(thermal.sol1,    gapSec, rate);
      thermal.sol2    = decayHeat(thermal.sol2,    gapSec, rate);
      thermal.sol3    = decayHeat(thermal.sol3,    gapSec, rate);
      thermal.stepper = decayHeat(thermal.stepper, gapSec, rate);
      lastReleaseT += p.interrun_gap_ms;
    }

    let runStartT = 0;
    let runLastConfirmT = 0;
    let prevBrickType = null;

    for (let i = 0; i < seq.length; i++) {
      const brickType = seq[i];
      globalBrickNum++;

      // Thermal state determines release speed
      const maxHeat = Math.max(thermal.sol1, thermal.sol2, thermal.sol3, thermal.stepper);
      let sps = p.stepper_sps;
      let thermalState = 'NORMAL';
      if (maxHeat > p.thermal_danger_level) { sps = 267; thermalState = 'DANGER'; thermalDangerReached = true; }
      else if (maxHeat > p.thermal_warn_level) { sps = 533; thermalState = 'WARNING'; }

      let releaseInterval = (p.stepper_steps_per_rev / sps) * 1000;

      // If same type as previous AND plow is still extended, fire immediately (belt-spacing limited).
      // The plow is already in position - no solenoid setup needed.
      const plowNumForType = p.plow_map[brickType];
      const prevPlowNum = prevBrickType ? p.plow_map[prevBrickType] : -1;
      if (
        brickType === prevBrickType &&
        plowNumForType > 0 &&
        (lastReleaseT + minSameTypeInterval + plowNumForType > 0) // always true, just for clarity
      ) {
        // Estimate whether plow will still be extended when this brick arrives - check against
        // the minimum-interval T_classified to see if plow is still up
        const estT_classified_min = lastReleaseT + minSameTypeInterval +
          (DIST_CHUTE_TO_SENSOR_MM / p.belt_target_mm_s) * 1000 +
          (p.brick_2x3_mm / p.belt_target_mm_s) * 1000;
        if (estT_classified_min < plowExtendedUntil[plowNumForType]) {
          // Plow still extended - use fast release
          releaseInterval = Math.max(minSameTypeInterval, releaseInterval);
          // If belt-spacing interval is less than thermal-throttled interval, still respect throttle
          if (minSameTypeInterval < releaseInterval) {
            releaseInterval = minSameTypeInterval;
          }
        }
      }

      // Track throttle delay vs normal
      if (thermalState !== 'NORMAL') {
        throttleCount++;
        throttledMs += releaseInterval - normalReleaseInterval;
      }

      const T_release = lastReleaseT + releaseInterval;
      if (i === 0) runStartT = T_release;

      const dtSec = releaseInterval / 1000;
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

      // --- Thermal misfire check ---
      // Check BEFORE updating heat so the heat at fire time is used
      let hasMisfire = false;
      if (plowNum > 0) {
        const solKey = 'sol' + plowNum;
        const dt2 = (T_classified - T_release) / 1000;
        const heatAtFire = decayHeat(thermal[solKey], dt2, rate);
        const prob = misfireProbability(heatAtFire, p.thermal_warn_level, p.thermal_danger_level);
        hasMisfire = Math.random() < prob;
      }

      // --- Physical routing: trace brick through plows in order ---
      // Each brick physically passes plow 1, then 2, then 3.
      // The first extended plow deflects it to that bin regardless of intent.
      let actualBin = 3; // default: passes all plows
      let routingCorrect = true;
      let errorCause = null;

      for (let px = 1; px <= 3; px++) {
        const arrT = plowArrivalMs(T_classified, px, p);
        const plowExtended = arrT < plowExtendedUntil[px];

        if (px === plowNum && !hasMisfire) {
          // This brick's intended plow - it fires. Check if it'll catch the brick.
          // After we fire (handled below), plowExtendedUntil[px] will be updated.
          // The plow is extended from T_classified to T_classified + sol_deenergize_ms + RETRACT_MS.
          const newExtUntil = T_classified + p.sol_deenergize_ms + RETRACT_MS;
          if (arrT < newExtUntil) {
            // Correct deflection
            actualBin = px - 1;
            // But if a PREVIOUS brick's plow (px' < px) is also extended, that one wins
            // (already handled above in loop - we break on first extended plow, so we only
            // reach px === plowNum if no earlier plow was extended)
          } else {
            // sol_deenergize_ms too short - plow retracts before brick arrives
            routingCorrect = false;
            errorCause = 'sol_retract_early';
            actualBin = 3; // brick passes through
          }
          break;
        } else if (plowExtended) {
          // A wrong plow (not this brick's intended one) is extended
          actualBin = px - 1;
          if (px !== plowNum) {
            routingCorrect = false;
            if (hasMisfire && px > plowNum) {
              errorCause = 'thermal_misfire'; // brick passed through its own misfired plow, caught by later one
            } else {
              errorCause = 'plow_conflict';
            }
          }
          break;
        }
        // If px === plowNum and hasMisfire: plow didn't extend, brick continues past it
      }

      // If plowNum > 0 and we reached actualBin===3 without break: either misfire sent
      // brick to default path, or no plow caught it
      if (plowNum > 0 && actualBin === 3) {
        if (hasMisfire) {
          routingCorrect = false;
          errorCause = 'thermal_misfire';
        }
        // sol_retract_early already handled above; default stays
      }

      // --- Update plow state after routing determination ---
      if (plowNum > 0) {
        if (!hasMisfire) {
          const solKey = 'sol' + plowNum;
          const dt2 = (T_classified - T_release) / 1000;
          thermal[solKey] = decayHeat(thermal[solKey], dt2, rate);
          thermal[solKey] = Math.min(thermal[solKey] + p.thermal_heat_per_sol, 1.0);
          peakHeat = Math.max(peakHeat, thermal[solKey]);

          // Reset de-energize timer (handles same-plow re-fire cleanly)
          plowExtendedUntil[plowNum] = T_classified + p.sol_deenergize_ms + RETRACT_MS;

          allEvents.push({ t: T_classified, type: 'PLOW_FIRE', plow: plowNum, brickNum: globalBrickNum, thermal: {...thermal} });
          allEvents.push({ t: T_classified + p.sol_full_ms, type: 'PLOW_HOLD', plow: plowNum, brickNum: globalBrickNum });
          allEvents.push({ t: T_classified + p.sol_deenergize_ms, type: 'PLOW_RELEASE', plow: plowNum, brickNum: globalBrickNum });
        } else {
          // Misfire: solenoid attempted but heat-degraded stroke insufficient
          allEvents.push({
            t: T_classified, type: 'PLOW_MISFIRE', plow: plowNum, brickNum: globalBrickNum,
            msg: `Plow ${plowNum} misfire (thermal heat too high) on brick #${globalBrickNum}`
          });
        }
      } else {
        allEvents.push({ t: T_classified, type: 'PLOW_SKIP', brickNum: globalBrickNum, msg: 'default path - no solenoid' });
      }

      // --- Routing error event ---
      if (!routingCorrect) {
        totalErrors++;
        errorCauses[errorCause] = (errorCauses[errorCause] || 0) + 1;
        allEvents.push({
          t: T_classified,
          type: 'ROUTING_ERROR',
          brickNum: globalBrickNum,
          cause: errorCause,
          expectedBin: plowNum === 0 ? 3 : plowNum - 1,
          actualBin,
          msg: `Brick #${globalBrickNum} (${brickType}) misrouted to bin ${actualBin + 1} [${errorCause}]`
        });
      }

      const T_confirm = T_sense + (p.bin_distances_mm[actualBin] / p.belt_target_mm_s) * 1000;
      allEvents.push({ t: T_confirm, type: 'BIN_CONFIRM', brickNum: globalBrickNum, binIdx: actualBin, brickType, correct: routingCorrect });

      runLastConfirmT = T_confirm;
      binCounts[actualBin]++;
      const transitMs = Math.round(T_confirm - T_release);

      allBrickLog.push({
        num: globalBrickNum, run: run + 1, type: brickType,
        sizeResult: is2x3 ? '2x3' : '2x2',
        colorResult: brickType.includes('red') ? 'RED' : 'BLUE',
        samples, plow: plowNum, binIdx: actualBin, transitMs, thermalState,
        correct: routingCorrect, errorCause: errorCause || null
      });

      lastReleaseT = T_release;
      prevBrickType = brickType;
    }

    if (seq.length > 0) pureRunMs += runLastConfirmT - runStartT;

    allEvents.push({ t: lastReleaseT + 2000, type: 'RUN_COMPLETE', run, binCounts: [...binCounts], totalMs: lastReleaseT });
    binCounts = [0, 0, 0, 0];
  }

  allEvents.sort((a, b) => a.t - b.t);

  const totalSimMs = allEvents[allEvents.length - 1]?.t || 0;
  const totalGapMs = (p.num_runs - 1) * p.interrun_gap_ms;
  const avgBps = pureRunMs > 0 ? (allBrickLog.length / (pureRunMs / 1000)) : 0;
  const accuracy = allBrickLog.length > 0
    ? (allBrickLog.filter(b => b.correct).length / allBrickLog.length) * 100
    : 100;

  return {
    events: allEvents, brickLog: allBrickLog, thermalDangerReached, peakHeat,
    totalSimMs, haltEvent: null,
    throttleCount, throttledMs, pureRunMs, totalGapMs, avgBps,
    totalErrors, errorCauses, accuracy
  };
}
