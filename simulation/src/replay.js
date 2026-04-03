// replay.js

'use strict';

import { state, _launchAnimLoop } from './animation.js';

const REQUIRED = ['brick'];
const ALT_TIME_FIELDS = [
  ['ts_ms'],
  ['timestamp_ms'],
  ['ts_enter_ms', 'ts_confirm_ms'],
];

function parseNumber(v, fallback = 0) {
  const n = Number(v);
  return Number.isFinite(n) ? n : fallback;
}

function normalizeHeader(h) {
  return String(h || '').trim().toLowerCase();
}

export function parseReplayCSV(csvText) {
  if (!csvText || typeof csvText !== 'string') return null;
  const lines = csvText.split(/\r?\n/).map((l) => l.trim()).filter((l) => l.length > 0 && !l.startsWith('#'));
  if (lines.length < 2) return null;

  const headers = lines[0].split(',').map(normalizeHeader);
  for (const req of REQUIRED) {
    if (!headers.includes(req)) return null;
  }

  const hasTimeShape = ALT_TIME_FIELDS.some((shape) => shape.every((key) => headers.includes(key)));
  if (!hasTimeShape) return null;

  const rows = [];
  for (let i = 1; i < lines.length; i++) {
    const vals = lines[i].split(',');
    const row = {};
    headers.forEach((h, idx) => { row[h] = (vals[idx] || '').trim(); });
    row.brick = parseNumber(row.brick, i);
    row.bin = parseNumber(row.bin, 0);
    row.ok = parseNumber(row.ok, 1);
    row.samples = parseNumber(row.samples || row.valid_samples, 8);
    row.category = row.category || row.type || '';
    row.size = row.size || (row.category.includes('2x3') ? '2x3' : '2x2');
    row.color = row.color || (row.category.includes('red') ? 'RED' : 'BLUE');
    row.ts_enter_ms = parseNumber(row.ts_enter_ms, 0);
    row.ts_confirm_ms = parseNumber(row.ts_confirm_ms, 0);
    row.ts_ms = parseNumber(row.ts_ms || row.timestamp_ms, 0);
    rows.push(row);
  }
  return rows.length > 0 ? rows : null;
}

function replayRowToEvents(row, params, run = 0) {
  const events = [];
  const t0 = row.ts_enter_ms || row.ts_ms;
  const category = row.category || row.type || '2x3_red';
  const binIdxRaw = row.bin > 0 ? row.bin - 1 : (category.includes('2x2_red') ? 0 : category.includes('2x2_blue') ? 1 : category.includes('2x3_blue') ? 2 : 3);
  const binIdx = Math.max(0, Math.min(3, binIdxRaw));
  const classifyAt = t0 + params.settle_ms + Math.min(params.color_timeout_ms, params.color_sample_count * params.color_integration_ms);
  const indexDone = classifyAt + Math.round((params.stepper_steps_per_rev / 4 / params.stepper_run_sps) * 1000);
  const confirmAt = row.ts_confirm_ms || (indexDone + params.fall_settle_ms + params.bin_confirm_latency_ms);

  events.push({ t: t0, type: 'STATE_ENTER', state: 'FEED', brickNum: row.brick, detail: 'replay' });
  events.push({ t: t0, type: 'ENTRY_BEAM_TRIGGERED', brickNum: row.brick, run, category });
  events.push({ t: t0 + params.entry_to_stop_ms, type: 'BRICK_SEATED', brickNum: row.brick, run });
  events.push({ t: classifyAt, type: 'BRICK_CLASSIFIED', brickNum: row.brick, run, category, sizeResult: row.size, colorResult: row.color, samples: row.samples, targetBin: binIdx });
  events.push({ t: classifyAt, type: 'DISC_INDEX_START', brickNum: row.brick, run, fromSteps: 0, toSteps: params.bin_steps[binIdx], moveSteps: 0, indexMs: 0, activeSps: params.stepper_run_sps });
  events.push({ t: indexDone, type: 'DISC_INDEXED', brickNum: row.brick, run, targetBin: binIdx, moveSteps: 0, activeSps: params.stepper_run_sps, thermalState: 'NORMAL' });
  events.push({ t: indexDone, type: 'PLATFORM_RELEASED', brickNum: row.brick, run, targetBin: binIdx });
  events.push({ t: indexDone + params.solenoid_on_ms, type: 'SOLENOID_OFF', brickNum: row.brick, run });
  events.push({ t: confirmAt, type: 'BIN_CONFIRM', brickNum: row.brick, run, binIdx, category, correct: row.ok === 1 });

  return events;
}

export function buildReplayEvents(rows, params) {
  if (!rows || rows.length === 0) return null;

  const events = [];
  const brickLog = [];
  const binCounts = [0, 0, 0, 0];

  for (const row of rows) {
    const ev = replayRowToEvents(row, params, 0);
    events.push(...ev);
    const binIdx = ev.find((e) => e.type === 'BIN_CONFIRM').binIdx;
    binCounts[binIdx]++;
    brickLog.push({
      num: row.brick,
      run: 1,
      type: row.category || row.type || '2x3_red',
      sizeResult: row.size,
      colorResult: row.color,
      samples: row.samples,
      targetBin: binIdx + 1,
      actualBin: binIdx + 1,
      binIdx,
      indexMs: 0,
      transitMs: Math.round((row.ts_confirm_ms || 0) - (row.ts_enter_ms || 0)),
      thermalState: (row.thermal_state || 'NORMAL').toUpperCase(),
      correct: row.ok === 1,
    });
  }

  events.sort((a, b) => a.t - b.t);
  const end = events[events.length - 1]?.t || 0;
  events.push({ t: end + 1, type: 'RUN_COMPLETE', run: 0, binCounts: [...binCounts], totalMs: end });

  return {
    events,
    brickLog,
    runSummary: [],
    totalErrors: 0,
    totalHalts: 0,
    thermalDangerReached: false,
    peakHeat: 0,
    totalSimMs: end,
    pureRunMs: end,
    totalGapMs: 0,
    avgBps: end > 0 ? rows.length / (end / 1000) : 0,
    throttleCount: 0,
    throttledMs: 0,
    accuracy: rows.length > 0 ? (rows.filter((r) => r.ok === 1).length / rows.length) * 100 : 100,
    haltEvent: null,
  };
}

export function startReplay(csvText, params, updateWarnings, populateBrickTable) {
  const rows = parseReplayCSV(csvText);
  if (!rows) {
    alert('Could not parse CSV. Include at least brick and timestamp columns.');
    return;
  }

  const simData = buildReplayEvents(rows, params);
  if (!simData) {
    alert('No valid rows found in CSV.');
    return;
  }

  state.replayMode = true;
  state.simData = simData;
  const badge = document.getElementById('replay-label');
  if (badge) badge.style.display = 'inline-block';
  _launchAnimLoop(updateWarnings, populateBrickTable);
}
