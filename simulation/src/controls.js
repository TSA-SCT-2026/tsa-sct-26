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
      body.innerHTML = `
        <div class="ctrl">
          <div class="ctrl-top"><span class="ctrl-name">Number of runs</span></div>
          <div class="ctrl-desc">Simulate back-to-back runs to see thermal accumulation across runs.</div>
          <input type="number" id="ctrl-num_runs" min="1" max="10" value="${params.num_runs}"
            style="margin-top:4px" onchange="window._setParam('num_runs', parseInt(this.value)||1)">
        </div>
        <div class="ctrl">
          <div class="ctrl-top"><span class="ctrl-name">Brick sequence</span></div>
          <div class="ctrl-desc">Order bricks are released. Worst-case concentrates solenoid fires for maximum thermal stress. Random shuffles each run independently.</div>
          <select id="ctrl-sequence" onchange="window._setParam('sequence', this.value)">
            <option value="interleaved">Default order (interleaved round-robin)</option>
            <option value="worst_case">Worst-case: plow bricks first</option>
            <option value="default_first">Worst-case: default-path bricks first</option>
            <option value="random">Random (shuffled each run)</option>
          </select>
        </div>
        <div class="ctrl">
          <div class="ctrl-top"><span class="ctrl-name">Inter-run gap</span><span class="ctrl-val">${params.interrun_gap_ms/1000}s</span></div>
          <div class="ctrl-desc">Cooldown time between runs (reload time). Thermal model keeps decaying.</div>
          <input type="range" id="ctrl-interrun_gap_ms" min="1000" max="30000" step="1000"
            value="${params.interrun_gap_ms}"
            oninput="window._setParamSlider('interrun_gap_ms', this.value, 'ctrl-val-interrun_gap_ms', 1000, 's')">
        </div>`;
    } else {
      for (const ctrl of sec.controls) {
        const scale = ctrl.scale || 1;
        const raw = params[ctrl.id];
        const display = Math.round(raw * scale * 100) / 100;
        const dec = ctrl.dec || 0;
        const valStr = display.toFixed(dec) + (ctrl.unit ? ' ' + ctrl.unit : '');

        const cdiv = document.createElement('div');
        cdiv.className = 'ctrl';
        cdiv.innerHTML = `
          <div class="ctrl-top">
            <span class="ctrl-name">${ctrl.label}</span>
            <span class="ctrl-val" id="cv-${ctrl.id}">${valStr}</span>
          </div>
          <input type="range"
            id="ctrl-${ctrl.id}"
            min="${ctrl.min}" max="${ctrl.max}" step="${ctrl.step || 1}"
            value="${raw * scale}"
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
  const dec = ctrl ? (ctrl.dec || 0) : 0;
  const unit = ctrl ? (ctrl.unit || '') : '';
  const val = parseFloat(rawVal) / scale;
  params[id] = val;
  const display = (parseFloat(rawVal)).toFixed(dec) + (unit ? ' ' + unit : '');
  const el = document.getElementById('cv-' + id);
  if (el) el.textContent = display;
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
  const el = document.getElementById('cv-' + id);
  if (el) el.textContent = (parseFloat(rawVal) / divisor).toFixed(0) + unit;
  const valEl = document.getElementById(valElId);
  if (valEl) valEl.textContent = (parseFloat(rawVal) / divisor).toFixed(0) + unit;
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
    const dec = ctrl.dec || 0;
    const unit = ctrl.unit || '';
    const el = document.getElementById('ctrl-' + ctrl.id);
    if (el) el.value = DEFAULTS[ctrl.id] * scale;
    const velEl = document.getElementById('cv-' + ctrl.id);
    if (velEl) velEl.textContent = (DEFAULTS[ctrl.id] * scale).toFixed(dec) + (unit ? ' ' + unit : '');
  }
  if (_updateMetrics) _updateMetrics();
  if (_updateWarnings) _updateWarnings();
}
