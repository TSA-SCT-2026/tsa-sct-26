// metrics.js - updateMetrics(), updateThermalUI(), showRunSummary()

'use strict';

import { state } from './animation.js';

function setText(id, text) {
  const el = document.getElementById(id);
  if (el) el.textContent = text;
}
function setTextClass(id, text, cls) {
  const el = document.getElementById(id);
  if (el) { el.textContent = text; el.className = 'mv ' + (cls || ''); }
}

export function updateMetrics() {
  const p = state.params;
  if (!p) return;

  const escRate = p.stepper_sps / p.stepper_steps_per_rev;
  const beltCap = p.belt_target_mm_s / p.brick_spacing_mm;
  const margin = (beltCap - escRate) / escRate * 100;
  const isBelt = beltCap < escRate;
  const estTime = p.total_bricks / escRate;

  setText('m-esc-rate', escRate.toFixed(2) + ' bricks/s');
  setText('m-belt-spd', p.belt_target_mm_s + ' mm/s');
  setTextClass('m-belt-cap', beltCap.toFixed(2) + ' bricks/s', isBelt ? 'r' : '');
  setTextClass('m-bottleneck', isBelt ? 'BELT' : 'ESCAPEMENT', isBelt ? 'r' : 'g');
  setTextClass('m-margin', (margin >= 0 ? '+' : '') + margin.toFixed(0) + '%', margin < 0 ? 'r' : margin < 20 ? 'y' : 'g');
  setText('m-runtime', '~' + estTime.toFixed(1) + 's');

  const dwell2x2 = p.brick_2x2_mm / p.belt_target_mm_s * 1000;
  const samples = Math.floor(dwell2x2 / p.color_integration_ms);
  const sigWin = p.beam_gap_mm / p.belt_target_mm_s * 1000;
  const tmarg = p.size_timeout_ms - sigWin;

  setText('m-dwell', dwell2x2.toFixed(1) + ' ms');
  setTextClass('m-samples', samples + ' samples', samples < 4 ? 'r' : samples < 8 ? 'y' : 'g');
  setText('m-sig-win', '0 - ' + sigWin.toFixed(0) + ' ms');
  setText('m-timeout', p.size_timeout_ms + ' ms');
  setTextClass('m-tmarg', tmarg.toFixed(0) + ' ms', tmarg < 0 ? 'r' : tmarg < 20 ? 'y' : 'g');

  for (let i = 1; i <= 4; i++) {
    const wEl = document.getElementById('tw' + i);
    const dEl = document.getElementById('td' + i);
    if (wEl) wEl.style.left = (p.thermal_warn_level * 100) + '%';
    if (dEl) dEl.style.left = (p.thermal_danger_level * 100) + '%';
  }

  if (!state.animRunning) {
    setText('m-active-sps', p.stepper_sps + ' sps');
    setText('m-thermal-state', 'NORMAL');
  }

  // Update bin expected counts based on num_runs
  const BASE_EXPECTED = [6, 6, 4, 8];
  const numRuns = p.num_runs || 1;
  for (let i = 0; i < 4; i++) {
    const expEl = document.getElementById('bin-exp-' + i);
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

export function updateThermalUI() {
  const p = state.params;
  if (!p) return;
  const liveThermal = state.liveThermal;
  const bars = [
    ['sol1', 'th-sol1-bar', 'th-sol1-val'],
    ['sol2', 'th-sol2-bar', 'th-sol2-val'],
    ['sol3', 'th-sol3-bar', 'th-sol3-val'],
    ['stepper', 'th-step-bar', 'th-step-val'],
  ];
  const maxHeat = Math.max(liveThermal.sol1, liveThermal.sol2, liveThermal.sol3, liveThermal.stepper);
  for (const [key, barId, valId] of bars) {
    const h = liveThermal[key] || 0;
    const bar = document.getElementById(barId);
    const val = document.getElementById(valId);
    if (bar) {
      bar.style.width = (h * 100) + '%';
      bar.style.background = h > p.thermal_danger_level ? 'var(--red)' : h > p.thermal_warn_level ? 'var(--yellow)' : 'var(--green)';
    }
    if (val) val.textContent = h.toFixed(2);
  }
  if (!state.animRunning) {
    const stateEl = document.getElementById('m-thermal-state');
    if (stateEl) {
      const thermalState = maxHeat > p.thermal_danger_level ? 'DANGER' : maxHeat > p.thermal_warn_level ? 'WARNING' : 'NORMAL';
      stateEl.textContent = thermalState;
      stateEl.className = 'mv ' + (thermalState === 'DANGER' ? 'r' : thermalState === 'WARNING' ? 'y' : 'g');
    }
  }
}

export function showRunSummary(sd) {
  const p = state.params;
  document.getElementById('run-summary').style.display = 'block';
  const totalSec = sd.totalSimMs / 1000;
  setText('m-total-time', totalSec.toFixed(2) + 's');
  const totalBricks = sd.brickLog.length;
  setText('m-bricks-sorted', totalBricks + ' / ' + (p.total_bricks * p.num_runs));
  setText('m-accuracy', '100% (sim always classifies correctly)');
  setTextClass('m-peak-heat', sd.peakHeat.toFixed(3), sd.peakHeat > p.thermal_danger_level ? 'r' : sd.peakHeat > p.thermal_warn_level ? 'y' : 'g');
}

export function setBinCount(idx, count) {
  state.liveBinCounts[idx] = count;
  const el = document.getElementById('bin-cnt-' + idx);
  if (el) el.textContent = count;
}

export function flashBin(idx) {
  const box = document.getElementById('bin-' + idx);
  if (!box) return;
  box.classList.add('flash');
  setTimeout(() => box.classList.remove('flash'), 120);
}

export function updateBinMatchState() {
  const p = state.params;
  if (!p) return;
  const BASE_EXPECTED = [6, 6, 4, 8];
  const numRuns = p.num_runs || 1;
  for (let i = 0; i < 4; i++) {
    const exp = BASE_EXPECTED[i] * numRuns;
    const box = document.getElementById('bin-' + i);
    if (box) {
      box.className = 'bin-box ' + (state.liveBinCounts[i] === exp ? 'match' : 'over');
    }
  }
}
