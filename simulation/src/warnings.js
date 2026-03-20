// warnings.js - all warning checks, returns structured warning list

'use strict';

import { generateSequence } from './simulation.js';

export function computeWarnings(p, simData) {
  const warnings = [];

  const escRate = p.stepper_sps / p.stepper_steps_per_rev;
  const beltCap = p.belt_target_mm_s / p.brick_spacing_mm;
  if (beltCap < escRate) {
    warnings.push({
      level: 'e',
      msg: `Belt is the bottleneck: belt capacity (${beltCap.toFixed(2)} bricks/s) is below ` +
           `escapement rate (${escRate.toFixed(2)} bricks/s). Bricks will pile up and jam.`
    });
  }

  const dwell2x2 = p.brick_2x2_mm / p.belt_target_mm_s * 1000;
  const samples = Math.floor(dwell2x2 / p.color_integration_ms);
  if (samples < 4) {
    warnings.push({ level:'e', msg:`Only ${samples} color samples for a 2x2 brick. Minimum for reliable classification is 8. Reduce belt speed or color integration time.` });
  } else if (samples < 8) {
    warnings.push({ level:'w', msg:`Only ${samples} color samples for a 2x2 brick. Classification may be unreliable. Target is 8+ samples.` });
  }

  const sigWin = p.beam_gap_mm / p.belt_target_mm_s * 1000;
  const tmarg = p.size_timeout_ms - sigWin;
  if (tmarg < 0) {
    warnings.push({ level:'e', msg:`Size timeout (${p.size_timeout_ms}ms) is BELOW the 2x3 signal window (${sigWin.toFixed(0)}ms). 2x3 bricks will be misclassified as 2x2.` });
  } else if (tmarg < 20) {
    warnings.push({ level:'w', msg:`Size timeout margin is only ${tmarg.toFixed(0)}ms (timeout: ${p.size_timeout_ms}ms, 2x3 signal: ${sigWin.toFixed(0)}ms). Recommend at least 20ms margin.` });
  }

  const brickClearTime = p.sol_lead_ms + (p.brick_2x3_mm / p.belt_target_mm_s * 1000);
  if (p.sol_deenergize_ms < brickClearTime) {
    warnings.push({ level:'e', msg:`Solenoid de-energizes at ${p.sol_deenergize_ms}ms but a 2x3 brick needs ${brickClearTime.toFixed(0)}ms to fully clear the plow (lead ${p.sol_lead_ms}ms + transit ${(p.brick_2x3_mm/p.belt_target_mm_s*1000).toFixed(0)}ms). Plow will retract under the brick.` });
  } else if (p.sol_deenergize_ms < brickClearTime + 5) {
    warnings.push({ level:'w', msg:`Solenoid de-energize margin is only ${(p.sol_deenergize_ms - brickClearTime).toFixed(0)}ms. Tighter than 5ms risks plow retracting before brick fully clears.` });
  }

  // Plow overlap: check if same plow fires again before de-energizing.
  // When this happens, the simulation will halt with ERROR_HALT.
  const releaseInterval = (p.stepper_steps_per_rev / p.stepper_sps) * 1000;
  const seqForWarning = generateSequence(p);
  const lastFireBrickIdx = {};
  let minSamePlowGap = Infinity;
  for (let i = 0; i < seqForWarning.length; i++) {
    const pl = p.plow_map[seqForWarning[i]];
    if (pl > 0) {
      if (lastFireBrickIdx[pl] !== undefined) {
        const gap = (i - lastFireBrickIdx[pl]) * releaseInterval;
        if (gap < minSamePlowGap) minSamePlowGap = gap;
      }
      lastFireBrickIdx[pl] = i;
    }
  }
  if (isFinite(minSamePlowGap) && minSamePlowGap < p.sol_deenergize_ms) {
    warnings.push({ level:'w', msg:`Plow overlap: with the current sequence, plow fires again after ${minSamePlowGap.toFixed(0)}ms but solenoid is still energized for ${p.sol_deenergize_ms}ms. This will cause ERROR_HALT in simulation. Different plows are independent - only a problem if the same plow fires twice. Change sequence or reduce sps to widen the gap.` });
  }

  if (warnings.length === 0) {
    warnings.push({ level:'ok', msg:'All parameters within acceptable ranges. No issues detected at these settings.' });
  }

  if (simData && simData.thermalDangerReached) {
    warnings.push({ level:'w', msg:'Thermal DANGER level was reached during the simulated run. System would throttle to 1.3 bricks/sec.' });
  }

  if (simData && simData.haltEvent) {
    warnings.push({ level:'e', msg:`Simulation halted: ERROR_HALT on brick #${simData.haltEvent.brickNum} - Plow ${simData.haltEvent.plow} conflict at ${simData.haltEvent.simMs.toFixed(0)}ms.` });
  }

  return warnings;
}
