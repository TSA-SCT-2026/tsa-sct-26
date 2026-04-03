// warnings.js

'use strict';

import { CATEGORY_TO_BIN } from './defaults.js';
import { generateSequence } from './simulation.js';

function indexBudgetMs(params) {
  return Math.round((params.stepper_steps_per_rev / 2 / params.stepper_run_sps) * 1000);
}

export function computeWarnings(params, simData) {
  const warnings = [];

  const feedMs = (params.transport_distance_mm / params.belt_target_mm_s) * 1000;
  if (feedMs > params.feed_timeout_ms) {
    warnings.push({
      level: 'e',
      msg: `Feed time is ${feedMs.toFixed(0)}ms but FEED timeout is ${params.feed_timeout_ms}ms. JAM_CHUTE halt is expected.`,
    });
  } else if (feedMs > params.feed_timeout_ms * 0.8) {
    warnings.push({
      level: 'w',
      msg: `Feed timeout margin is narrow: ${Math.round(params.feed_timeout_ms - feedMs)}ms remaining.`,
    });
  }

  const colorWindowMs = params.color_sample_count * params.color_integration_ms;
  if (colorWindowMs > params.color_timeout_ms) {
    warnings.push({
      level: 'e',
      msg: `Color integration needs ${colorWindowMs}ms but timeout is ${params.color_timeout_ms}ms. SENSOR_FAULT is likely.`,
    });
  }

  const timeoutLimitedSamples = Math.floor(params.color_timeout_ms / params.color_integration_ms);
  if (timeoutLimitedSamples < params.color_min_samples) {
    warnings.push({
      level: 'e',
      msg: `Only ${timeoutLimitedSamples} valid samples fit in color timeout, below minimum ${params.color_min_samples}.`,
    });
  } else if (timeoutLimitedSamples < params.color_sample_count) {
    warnings.push({
      level: 'w',
      msg: `Configured ${params.color_sample_count} color samples but timeout supports ${timeoutLimitedSamples}.`,
    });
  }

  if (params.fall_settle_ms < params.solenoid_on_ms + 80) {
    warnings.push({
      level: 'e',
      msg: `Fall settle ${params.fall_settle_ms}ms is too short for reliable drop and return after ${params.solenoid_on_ms}ms pulse.`,
    });
  }

  if (params.bin_confirm_timeout_ms < params.bin_confirm_latency_ms) {
    warnings.push({
      level: 'e',
      msg: `Bin confirm timeout ${params.bin_confirm_timeout_ms}ms is below expected latency ${params.bin_confirm_latency_ms}ms.`,
    });
  }

  const seq = generateSequence(params);
  let statePos = params.bin_steps[params.home_bin_index];
  let maxMove = 0;
  for (const type of seq) {
    const target = params.bin_steps[CATEGORY_TO_BIN[type]];
    let d = Math.abs(target - statePos);
    if (d > params.stepper_steps_per_rev / 2) d = params.stepper_steps_per_rev - d;
    maxMove = Math.max(maxMove, d);
    statePos = target;
  }
  const worstMs = Math.round((maxMove / params.stepper_run_sps) * 1000);
  const budgetMs = indexBudgetMs(params);
  if (worstMs > budgetMs) {
    warnings.push({
      level: 'w',
      msg: `Worst index move is ${maxMove} steps (~${worstMs}ms). Review throughput against run target.`,
    });
  }

  if (simData) {
    if (simData.thermalDangerReached) {
      warnings.push({
        level: 'w',
        msg: 'Thermal danger threshold was reached in simulation. Index speed throttling was applied.',
      });
    }
    if ((simData.totalHalts || 0) > 0) {
      warnings.push({
        level: 'e',
        msg: `Simulation produced ${simData.totalHalts} ERROR_HALT events. Review timeline for root causes.`,
      });
    }
    if (simData.accuracy < 100) {
      warnings.push({
        level: 'e',
        msg: `Sorting accuracy dropped to ${simData.accuracy.toFixed(1)}%. Chamber model should remain deterministic.`,
      });
    }
  }

  if (warnings.length === 0) {
    warnings.push({
      level: 'ok',
      msg: 'No warnings detected for current settings.',
    });
  }
  return warnings;
}
