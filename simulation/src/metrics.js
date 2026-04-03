// metrics.js

'use strict';

import { state } from './animation.js';
import { BIN_LABELS } from './defaults.js';

function setText(id, text) {
  const el = document.getElementById(id);
  if (el) el.textContent = text;
}

function setTextClass(id, text, cls) {
  const el = document.getElementById(id);
  if (!el) return;
  el.textContent = text;
  el.className = cls ? `mv ${cls}` : 'mv';
}

function nominalCycleMs(params) {
  const feed = (params.transport_distance_mm / params.belt_target_mm_s) * 1000;
  const color = Math.min(params.color_timeout_ms, params.color_sample_count * params.color_integration_ms);
  const index = Math.round((params.stepper_steps_per_rev / 4 / params.stepper_run_sps) * 1000);
  return (
    feed +
    params.entry_to_stop_ms +
    params.settle_ms +
    Math.max(params.size_read_ms, color) +
    index +
    params.fall_settle_ms +
    params.bin_confirm_latency_ms +
    params.token_restore_ms
  );
}

export function updateMetrics() {
  const p = state.params;
  if (!p) return;

  const feedMs = (p.transport_distance_mm / p.belt_target_mm_s) * 1000;
  const colorMs = Math.min(p.color_timeout_ms, p.color_sample_count * p.color_integration_ms);
  const samplesByTimeout = Math.floor(p.color_timeout_ms / p.color_integration_ms);
  const indexQuarter = Math.round((p.stepper_steps_per_rev / 4 / p.stepper_run_sps) * 1000);
  const cycleMs = nominalCycleMs(p);
  const bps = cycleMs > 0 ? 1000 / cycleMs : 0;
  const runMs = cycleMs * p.total_bricks;

  setText('m-esc-rate', `${bps.toFixed(2)} bricks/s`);
  setText('m-belt-spd', `${p.belt_target_mm_s} mm/s`);
  setText('m-belt-cap', `${(1000 / feedMs).toFixed(2)} entries/s`);
  setText('m-bottleneck', feedMs > indexQuarter ? 'FEED' : 'INDEX');
  setText('m-margin', `${Math.round(p.feed_timeout_ms - feedMs)} ms`);
  setText('m-runtime', `~${(runMs / 1000).toFixed(1)}s`);

  setText('m-dwell', `${colorMs.toFixed(0)} ms`);
  setTextClass(
    'm-samples',
    `${samplesByTimeout} samples`,
    samplesByTimeout < p.color_min_samples ? 'r' : samplesByTimeout < p.color_sample_count ? 'y' : 'g',
  );
  setText('m-sig-win', `${p.size_read_ms} ms`);
  setText('m-timeout', `${p.color_timeout_ms} ms`);
  setTextClass('m-tmarg', `${Math.max(0, p.color_timeout_ms - colorMs)} ms`, p.color_timeout_ms < colorMs ? 'r' : 'g');

  for (let i = 1; i <= 4; i++) {
    const warn = document.getElementById(`tw${i}`);
    const danger = document.getElementById(`td${i}`);
    if (warn) warn.style.left = `${p.thermal_warn_level * 100}%`;
    if (danger) danger.style.left = `${p.thermal_danger_level * 100}%`;
  }

  if (!state.animRunning) {
    setText('m-active-sps', `${p.stepper_run_sps} steps/s`);
    setTextClass('m-thermal-state', 'NORMAL', 'g');
  }

  const expected = [
    p.counts['2x2_red'],
    p.counts['2x2_blue'],
    p.counts['2x3_blue'],
    p.counts['2x3_red'],
  ];
  const runs = p.num_runs || 1;
  for (let i = 0; i < 4; i++) {
    const exp = document.getElementById(`bin-exp-${i}`);
    if (exp) exp.textContent = runs > 1 ? `expected: ${runs}x${expected[i]} = ${runs * expected[i]}` : `expected: ${expected[i]}`;
  }
}

export function updateThermalUI() {
  const p = state.params;
  if (!p) return;
  const t = state.liveThermal;
  const keys = [
    ['sol1', 'th-sol1-bar', 'th-sol1-val'],
    ['sol2', 'th-sol2-bar', 'th-sol2-val'],
    ['sol3', 'th-sol3-bar', 'th-sol3-val'],
    ['stepper', 'th-step-bar', 'th-step-val'],
  ];
  let peak = 0;
  for (const [key, barId, valId] of keys) {
    const h = t[key] || 0;
    peak = Math.max(peak, h);
    const bar = document.getElementById(barId);
    const val = document.getElementById(valId);
    if (bar) {
      bar.style.width = `${Math.round(h * 100)}%`;
      bar.style.background = h >= p.thermal_danger_level ? 'var(--red)' : h >= p.thermal_warn_level ? 'var(--yellow)' : 'var(--green)';
    }
    if (val) val.textContent = h.toFixed(2);
  }

  const ts = peak >= p.thermal_danger_level ? 'DANGER' : peak >= p.thermal_warn_level ? 'WARNING' : 'NORMAL';
  setTextClass('m-thermal-state', ts, ts === 'DANGER' ? 'r' : ts === 'WARNING' ? 'y' : 'g');
}

export function showRunSummary(simData) {
  const p = state.params;
  const summary = document.getElementById('run-summary');
  if (!summary || !p) return;
  summary.style.display = 'block';

  const expectedTotal = p.total_bricks * p.num_runs;
  const sorted = simData.brickLog.length;
  setText('m-total-time', `${(simData.totalSimMs / 1000).toFixed(2)}s`);
  setText('m-bricks-sorted', `${sorted} / ${expectedTotal}`);

  setTextClass('m-accuracy', `${simData.accuracy.toFixed(1)}%`, simData.accuracy < 100 ? 'y' : 'g');
  setTextClass(
    'm-peak-heat',
    simData.peakHeat.toFixed(3),
    simData.peakHeat >= p.thermal_danger_level ? 'r' : simData.peakHeat >= p.thermal_warn_level ? 'y' : 'g',
  );
  setText('m-avg-bps', `${simData.avgBps.toFixed(2)} bricks/s`);
  setTextClass('m-throttle-count', `${simData.throttleCount} bricks`, simData.throttleCount > 0 ? 'y' : 'g');
  setText('m-throttled-ms', `${(simData.throttledMs / 1000).toFixed(2)}s`);

  const haltText = simData.totalHalts > 0 ? `${simData.totalHalts} halts` : 'none';
  setTextClass('m-error-detail', haltText, simData.totalHalts > 0 ? 'r' : 'g');

  const diag = document.getElementById('m-diagnosis');
  if (diag) {
    const notes = [];
    if (simData.totalHalts > 0) notes.push('One or more bricks entered ERROR_HALT');
    if (simData.thermalDangerReached) notes.push('Thermal danger threshold reached');
    if (simData.throttleCount > 0) notes.push('Indexing was thermally throttled');
    if (simData.accuracy === 100 && simData.totalHalts === 0) notes.push('Run remained deterministic with full accuracy');
    diag.textContent = notes.join(' | ');
    diag.style.display = notes.length > 0 ? 'block' : 'none';
  }
}

export function setBinCount(idx, count) {
  state.liveBinCounts[idx] = count;
  const el = document.getElementById(`bin-cnt-${idx}`);
  if (el) el.textContent = String(count);
}

export function flashBin(idx) {
  const box = document.getElementById(`bin-${idx}`);
  if (!box) return;
  box.classList.add('flash');
  setTimeout(() => box.classList.remove('flash'), 120);
}

export function updateBinMatchState() {
  const p = state.params;
  if (!p) return;
  const expected = [
    p.counts['2x2_red'] * p.num_runs,
    p.counts['2x2_blue'] * p.num_runs,
    p.counts['2x3_blue'] * p.num_runs,
    p.counts['2x3_red'] * p.num_runs,
  ];
  for (let i = 0; i < 4; i++) {
    const box = document.getElementById(`bin-${i}`);
    if (!box) continue;
    box.className = `bin-box ${state.liveBinCounts[i] === expected[i] ? 'match' : 'over'}`;
    const label = box.querySelector('.bin-name');
    if (label && label.textContent !== BIN_LABELS[i]) label.textContent = BIN_LABELS[i];
  }
}
