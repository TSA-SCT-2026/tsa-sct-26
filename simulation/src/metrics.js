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

  // Accuracy with color coding
  const accuracy = sd.accuracy != null ? sd.accuracy : 100;
  const correctCount = sd.brickLog.filter(b => b.correct !== false).length;
  const accStr = accuracy.toFixed(1) + '% (' + correctCount + '/' + totalBricks + ' correct)';
  setTextClass('m-accuracy', accStr, accuracy < 90 ? 'r' : accuracy < 100 ? 'y' : 'g');

  setTextClass('m-peak-heat', sd.peakHeat.toFixed(3), sd.peakHeat > p.thermal_danger_level ? 'r' : sd.peakHeat > p.thermal_warn_level ? 'y' : 'g');

  // Avg throughput: bricks per second of pure sorting time (excludes inter-run gaps)
  const pureRunSec = (sd.pureRunMs || 0) / 1000;
  const avgBps = pureRunSec > 0 ? totalBricks / pureRunSec : 0;
  setText('m-avg-bps', avgBps.toFixed(2) + ' bricks/s');

  // Throttle stats
  const tc = sd.throttleCount || 0;
  setTextClass('m-throttle-count', tc + ' bricks', tc > 0 ? 'y' : 'g');
  const throttledSec = ((sd.throttledMs || 0) / 1000).toFixed(2);
  setText('m-throttled-ms', throttledSec + 's');

  // Routing errors breakdown
  const errs = sd.errorCauses || {};
  const totalErrs = sd.totalErrors || 0;
  const errParts = [];
  if (errs.plow_conflict)   errParts.push(`${errs.plow_conflict} plow conflicts`);
  if (errs.thermal_misfire) errParts.push(`${errs.thermal_misfire} thermal misfires`);
  if (errs.sol_retract_early) errParts.push(`${errs.sol_retract_early} retract-too-early`);
  const errEl = document.getElementById('m-error-detail');
  if (errEl) {
    if (totalErrs === 0) {
      errEl.textContent = 'none';
      errEl.className = 'mv g';
    } else {
      errEl.textContent = errParts.join(', ');
      errEl.className = 'mv r';
    }
  }

  // Diagnosis line
  const diagEl = document.getElementById('m-diagnosis');
  if (diagEl) {
    const lines = [];
    if (errs.plow_conflict > 0) {
      const conflictSps = Math.floor(p.stepper_steps_per_rev / p.sol_deenergize_ms * 1000);
      lines.push(`Plow conflicts: reduce sps below ~${conflictSps} or increase de-energize time`);
    }
    if (errs.thermal_misfire > 0) {
      lines.push(`Thermal misfires: heat exceeded danger threshold - reduce heat per fire or increase cooling`);
    }
    if (errs.sol_retract_early > 0) {
      lines.push(`Retract-too-early: sol_deenergize_ms too short for belt speed - increase to >${Math.ceil(p.sol_lead_ms + p.brick_2x3_mm / p.belt_target_mm_s * 1000)}ms`);
    }
    if (tc > 0) {
      lines.push(`Thermal throttling added ${((sd.throttledMs||0)/1000).toFixed(1)}s to run time`);
    }
    diagEl.textContent = lines.length > 0 ? lines.join(' | ') : '';
    diagEl.style.display = lines.length > 0 ? 'block' : 'none';
  }
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
