// controls.js - buildControls(), onSlider(), setParam(), setParamSlider(), findCtrl(), resetSection()

'use strict';

import { DEFAULTS, SECTIONS } from './defaults.js';
import { state } from './animation.js';

let _updateMetrics = null;
let _updateWarnings = null;

export function registerControlCallbacks(cbs) {
  _updateMetrics = cbs.updateMetrics;
  _updateWarnings = cbs.updateWarnings;
}

export function buildControls(params) {
  const panel = document.getElementById('controls');
  if (!panel) return;
  panel.innerHTML = '';

  for (const sec of SECTIONS) {
    const div = document.createElement('div');
    div.className = 'section';

    const hdr = document.createElement('div');
    hdr.className = 'sec-hdr';
    hdr.innerHTML = `<span class="sec-title">${sec.label}</span><span class="chev">&#9660;</span>`;
    const body = document.createElement('div');
    body.className = 'sec-body';
    hdr.onclick = () => {
      hdr.classList.toggle('closed');
      body.classList.toggle('hidden');
    };
    div.appendChild(hdr);

    if (sec.id === 'run') {
      const gapSec = (params.interrun_gap_ms / 1000).toFixed(1);
      body.innerHTML = `
        <div class="ctrl">
          <div class="ctrl-top"><span class="ctrl-name">Number of runs</span></div>
          <div class="ctrl-desc">Simulate back-to-back runs to see thermal accumulation.</div>
          <input type="number" id="ctrl-num_runs" min="1" max="100" value="${params.num_runs}"
            style="margin-top:4px" onchange="window._setParam('num_runs', Math.max(1,parseInt(this.value)||1))">
        </div>
        <div class="ctrl">
          <div class="ctrl-top"><span class="ctrl-name">Brick sequence</span></div>
          <div class="ctrl-desc">Order bricks are processed through the chamber. Affects selector indexing stress and thermal profile.</div>
          <select id="ctrl-sequence" onchange="window._setParam('sequence', this.value)">
            <option value="interleaved">Interleaved round robin</option>
            <option value="worst_case">Grouped by category (thermal stress)</option>
            <option value="worst_case_accuracy">Alternate category blocks</option>
            <option value="default_first">2x3 red first (home bin first)</option>
            <option value="random">Random (shuffled independently each run)</option>
          </select>
        </div>
        <div class="ctrl">
          <div class="ctrl-top">
            <span class="ctrl-name">Inter-run gap</span>
            <input type="number" class="ctrl-val-input" id="ctrl-val-interrun_gap_ms"
              value="${gapSec}" min="0" max="60" step="0.5"
              onchange="window._setInterrunGapSec(parseFloat(this.value))">
          </div>
          <div class="ctrl-desc">Cooldown between runs (0-60s). Thermal bars animate the decay smoothly.</div>
          <input type="range" id="ctrl-interrun_gap_ms" min="0" max="60000" step="500"
            value="${params.interrun_gap_ms}"
            oninput="window._setParamSlider('interrun_gap_ms', this.value, 'ctrl-val-interrun_gap_ms', 1000, 's')">
        </div>`;
      // Restore selected value after innerHTML assignment
      body.querySelector('#ctrl-sequence').value = params.sequence;
    } else {
      for (const ctrl of sec.controls) {
        const scale = ctrl.scale || 1;
        const raw = params[ctrl.id];
        const display = raw * scale;
        const dec = ctrl.dec ?? 0;

        const cdiv = document.createElement('div');
        cdiv.className = 'ctrl';
        cdiv.innerHTML = `
          <div class="ctrl-top">
            <span class="ctrl-name">${ctrl.label}${ctrl.unit ? ' <span style="color:var(--text3);font-size:10px">(${ctrl.unit})</span>' : ''}</span>
            <input type="number" class="ctrl-val-input" id="cv-${ctrl.id}"
              value="${display.toFixed(dec)}"
              min="${ctrl.min * scale}" max="${ctrl.max * scale}" step="${ctrl.step * scale}"
              ${ctrl.locked ? 'disabled' : ''}
              onchange="window._onSliderInput('${ctrl.id}', parseFloat(this.value))">
          </div>
          <input type="range"
            id="ctrl-${ctrl.id}"
            min="${ctrl.min * scale}" max="${ctrl.max * scale}" step="${ctrl.step * scale}"
            value="${display}"
            ${ctrl.locked ? 'disabled' : ''}
            oninput="window._onSlider('${ctrl.id}', this.value)">
          <div class="ctrl-desc">${ctrl.desc}</div>`;
        body.appendChild(cdiv);
      }

      const resetBtn = document.createElement('button');
      resetBtn.className = 'btn sec-reset';
      resetBtn.textContent = 'Reset ' + sec.label + ' to defaults';
      resetBtn.onclick = () => resetSection(sec, params);
      body.appendChild(resetBtn);
    }

    div.appendChild(body);
    panel.appendChild(div);
  }
}

export function onSlider(id, rawVal, params) {
  const ctrl = findCtrl(id);
  const scale = ctrl ? (ctrl.scale || 1) : 1;
  const dec = ctrl ? (ctrl.dec ?? 0) : 0;
  params[id] = parseFloat(rawVal) / scale;
  const inputEl = document.getElementById('cv-' + id);
  if (inputEl) inputEl.value = parseFloat(rawVal).toFixed(dec);
  if (_updateMetrics) _updateMetrics();
  if (_updateWarnings) _updateWarnings();
}

export function onSliderInput(id, displayVal, params) {
  const ctrl = findCtrl(id);
  if (!ctrl || ctrl.locked) return;
  const scale = ctrl.scale || 1;
  const dec = ctrl.dec ?? 0;
  const minV = ctrl.min * scale;
  const maxV = ctrl.max * scale;
  const clamped = Math.max(minV, Math.min(maxV, isNaN(displayVal) ? minV : displayVal));
  params[id] = clamped / scale;
  const sliderEl = document.getElementById('ctrl-' + id);
  if (sliderEl) sliderEl.value = clamped;
  const inputEl = document.getElementById('cv-' + id);
  if (inputEl) inputEl.value = clamped.toFixed(dec);
  if (_updateMetrics) _updateMetrics();
  if (_updateWarnings) _updateWarnings();
}

export function setParam(id, val, params) {
  params[id] = val;
  if (_updateMetrics) _updateMetrics();
  if (_updateWarnings) _updateWarnings();
}

export function setParamSlider(id, rawVal, valElId, divisor, unit, params) {
  params[id] = parseFloat(rawVal);
  const display = (parseFloat(rawVal) / divisor).toFixed(1);
  const valEl = document.getElementById(valElId);
  if (valEl) valEl.value = display;
  if (_updateMetrics) _updateMetrics();
}

export function setInterrunGapSec(sec, params) {
  const ms = Math.max(0, Math.min(60000, (isNaN(sec) ? 0 : sec) * 1000));
  params.interrun_gap_ms = ms;
  const sliderEl = document.getElementById('ctrl-interrun_gap_ms');
  if (sliderEl) sliderEl.value = ms;
  const valEl = document.getElementById('ctrl-val-interrun_gap_ms');
  if (valEl) valEl.value = (ms / 1000).toFixed(1);
  if (_updateMetrics) _updateMetrics();
}

export function findCtrl(id) {
  for (const sec of SECTIONS) {
    for (const c of (sec.controls || [])) {
      if (c.id === id) return c;
    }
  }
  return null;
}

export function resetSection(sec, params) {
  for (const ctrl of (sec.controls || [])) {
    if (ctrl.locked) continue;
    params[ctrl.id] = DEFAULTS[ctrl.id];
    const scale = ctrl.scale || 1;
    const dec = ctrl.dec ?? 0;
    const el = document.getElementById('ctrl-' + ctrl.id);
    if (el) el.value = DEFAULTS[ctrl.id] * scale;
    const inputEl = document.getElementById('cv-' + ctrl.id);
    if (inputEl) inputEl.value = (DEFAULTS[ctrl.id] * scale).toFixed(dec);
  }
  if (_updateMetrics) _updateMetrics();
  if (_updateWarnings) _updateWarnings();
}
