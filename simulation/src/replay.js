// replay.js - parseReplayCSV(), buildReplayEvents(), startReplay()

'use strict';

import { state, _launchAnimLoop } from './animation.js';
import { BRICK_COLOR } from './canvas.js';

const SIZE_TIMEOUT_MS_FALLBACK = 150;

// Parse firmware serial log CSV into structured rows
export function parseReplayCSV(csvText) {
  const lines = csvText.trim().split('\n').map(l => l.trim()).filter(l => l.length > 0);
  if (lines.length < 2) return null;

  const headers = lines[0].split(',').map(h => h.trim());
  if (!headers.includes('brick') || !headers.includes('ts_enter_ms')) return null;

  const rows = [];
  for (let i = 1; i < lines.length; i++) {
    const vals = lines[i].split(',');
    if (vals.length < 16) continue;
    const row = {};
    for (let j = 0; j < headers.length && j < vals.length; j++) {
      row[headers[j]] = vals[j] ? vals[j].trim() : '';
    }
    // Parse numeric fields
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

// Build synthetic event list from CSV rows matching computeSimulation() output format
export function buildReplayEvents(rows, params) {
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

    const brickType = row.type || '2x3_blue';
    const is2x3 = brickType.includes('2x3');
    // Bin field is 1-based in CSV
    const binIdx = Math.max(0, Math.min(3, row.bin - 1));

    const thermal = {
      sol1: row.heat_sol1,
      sol2: row.heat_sol2,
      sol3: row.heat_sol3,
      stepper: row.heat_step,
    };
    const maxHeat = Math.max(thermal.sol1, thermal.sol2, thermal.sol3, thermal.stepper);
    peakHeat = Math.max(peakHeat, maxHeat);
    if (row.thermal_state === 'DANGER') thermalDangerReached = true;

    const sps = row.thermal_state === 'DANGER' ? 267 : row.thermal_state === 'WARNING' ? 533 : (params.stepper_sps || 800);

    // BRICK_RELEASED
    events.push({
      t: T_enter,
      type: 'BRICK_RELEASED',
      brickNum,
      brickType,
      run: 0,
      sps,
      thermalState: row.thermal_state || 'NORMAL',
      thermal: {...thermal},
    });

    // BRICK_AT_SENSOR (same time as release in replay - sensor is at start)
    events.push({ t: T_enter, type: 'BRICK_AT_SENSOR', brickNum, brickType });

    // BRICK_CLASSIFIED
    let T_classified;
    if (is2x3 && row.gap_us > 0) {
      T_classified = T_enter + row.gap_us / 1000;
    } else {
      T_classified = T_enter + SIZE_TIMEOUT_MS_FALLBACK;
    }

    events.push({
      t: T_classified,
      type: 'BRICK_CLASSIFIED',
      brickNum,
      brickType,
      sizeResult: row.size || (is2x3 ? '2x3' : '2x2'),
      colorResult: row.color || (brickType.includes('red') ? 'RED' : 'BLUE'),
      gapUs: row.gap_us || 0,
      samples: row.valid_samples || 0,
    });

    // Plow events
    const plowNum = row.plow;
    if (plowNum > 0) {
      const solFull = params.sol_full_ms || 20;
      const solDeenergize = params.sol_deenergize_ms || 280;
      events.push({ t: T_classified, type: 'PLOW_FIRE', plow: plowNum, brickNum, thermal: {...thermal} });
      events.push({ t: T_classified + solFull, type: 'PLOW_HOLD', plow: plowNum, brickNum });
      events.push({ t: T_classified + solDeenergize, type: 'PLOW_RELEASE', plow: plowNum, brickNum });
    } else {
      events.push({ t: T_classified, type: 'PLOW_SKIP', brickNum, msg: 'default path - no solenoid' });
    }

    // BIN_CONFIRM
    events.push({ t: row.ts_confirm_ms, type: 'BIN_CONFIRM', brickNum, binIdx, brickType });
    binCounts[binIdx]++;

    // Drift detection: compare actual vs expected transit
    const binDist = (params.bin_distances_mm || [250, 280, 310, 360])[binIdx];
    const beltSpeed = params.belt_target_mm_s || 200;
    const expectedTransit = Math.round(binDist / beltSpeed * 1000);
    const actualTransit = row.transit_ms;

    if (Math.abs(actualTransit - expectedTransit) > 150) {
      const delta = actualTransit - expectedTransit;
      events.push({
        t: row.ts_confirm_ms,
        type: 'DRIFT_DETECTED',
        brickNum,
        msg: `Brick #${brickNum}: actual transit ${actualTransit}ms vs expected ${expectedTransit}ms (delta ${delta > 0 ? '+' : ''}${delta}ms) - belt speed drift detected`,
      });
    }

    brickLog.push({
      num: brickNum,
      run: 1,
      type: brickType,
      sizeResult: row.size || (is2x3 ? '2x3' : '2x2'),
      colorResult: row.color || (brickType.includes('red') ? 'RED' : 'BLUE'),
      samples: row.valid_samples || 0,
      plow: plowNum,
      binIdx,
      transitMs: Math.round(actualTransit),
      thermalState: row.thermal_state || 'NORMAL',
      correct: row.ok === 1,
    });
  }

  const lastConfirm = rows.reduce((m, r) => Math.max(m, r.ts_confirm_ms), 0);
  events.push({ t: lastConfirm + 500, type: 'RUN_COMPLETE', run: 0, binCounts: [...binCounts], totalMs: lastConfirm });

  events.sort((a, b) => a.t - b.t);

  const totalSimMs = events[events.length - 1]?.t || 0;
  return { events, brickLog, thermalDangerReached, peakHeat, totalSimMs, haltEvent: null };
}

export function startReplay(csvText, params, updateWarnings, populateBrickTable) {
  const rows = parseReplayCSV(csvText);
  if (!rows) {
    alert('Could not parse CSV. Make sure it is a firmware serial log with the correct header.');
    return;
  }

  const replayData = buildReplayEvents(rows, params);
  if (!replayData) {
    alert('No valid brick rows found in CSV.');
    return;
  }

  state.replayMode = true;
  state.simData = replayData;

  const rlEl = document.getElementById('replay-label');
  if (rlEl) rlEl.style.display = 'inline-block';

  _launchAnimLoop(updateWarnings, populateBrickTable);
}
