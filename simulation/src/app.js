import {
  BIN_LABELS,
  DEFAULTS,
  EXPECTED_BINS,
  SERVO_ANGLES,
  SPEED_MODES,
  cloneDefaults,
  computeSimulation,
  formatHarnessStatus,
  handleHarnessCommand,
  selectorRouteReadyMs,
} from './model.js';

'use strict';

const state = {
  params: cloneDefaults(),
  result: null,
};

const CONTROL_SECTIONS = [
  {
    title: 'Feed And Motion',
    controls: [
      ['entrySpacingMs', 'Entry spacing', 'ms', 180, 1200, 20],
      ['beltSpeedMms', 'Measured belt speed', 'mm/s', 60, 280, 5],
      ['beamBToServoMm', 'Beam B to servo', 'mm', 30, 180, 5],
    ],
  },
  {
    title: 'Sensing',
    controls: [
      ['colorThreshold', 'Red threshold', '', 0.42, 0.56, 0.002],
      ['sizeThresholdMm', '2x3 threshold', 'mm', 14, 24, 0.2],
      ['colorNoisePct', 'Color noise', '%', 0, 8, 0.5],
      ['sizeNoisePct', 'Size noise', '%', 0, 8, 0.5],
      ['missingBeamBRatePct', 'Miss beam B rate', '%', 0, 30, 1],
    ],
  },
  {
    title: 'Selector',
    controls: [
      ['servoSettleMs', 'Servo settle', 'ms', 120, 420, 10],
      ['selectorJigglePulses', 'Jiggle pulses', '', 0, 8, 1],
      ['routeSwitchMarginMs', 'Route margin', 'ms', 0, 80, 5],
    ],
  },
];

function $(id) {
  return document.getElementById(id);
}

function fmtMs(v) {
  return `${Math.round(v)} ms`;
}

function fmtPct(v) {
  return `${v.toFixed(1)}%`;
}

function controlValueText(key, unit) {
  const value = state.params[key];
  if (typeof value === 'number' && value % 1 !== 0) return `${value.toFixed(3).replace(/0+$/, '').replace(/\.$/, '')}${unit ? ` ${unit}` : ''}`;
  return `${value}${unit ? ` ${unit}` : ''}`;
}

function buildControls() {
  const root = $('controls');
  root.innerHTML = '';

  const modeWrap = document.createElement('div');
  modeWrap.className = 'control-group';
  modeWrap.innerHTML = `
    <h3>Firmware Profile</h3>
    <label class="field">
      <span>Speed mode</span>
      <select id="speedMode">
        <option value="balanced">balanced</option>
        <option value="aggressive">aggressive</option>
        <option value="accuracy">accuracy</option>
      </select>
    </label>
    <label class="field">
      <span>Sequence</span>
      <select id="sequence">
        <option value="firmware_fullrun">firmware fullrun</option>
        <option value="interleaved">interleaved</option>
        <option value="grouped">grouped</option>
        <option value="red_then_blue">red then blue</option>
        <option value="random_seeded">seeded random</option>
      </select>
    </label>
    <label class="check">
      <input id="beltSubtraction" type="checkbox" checked>
      <span>Belt-subtracted color</span>
    </label>
    <label class="check">
      <input id="logCsv" type="checkbox">
      <span>CSV serial log</span>
    </label>
  `;
  root.appendChild(modeWrap);

  for (const section of CONTROL_SECTIONS) {
    const wrap = document.createElement('div');
    wrap.className = 'control-group';
    wrap.innerHTML = `<h3>${section.title}</h3>`;
    for (const [key, label, unit, min, max, step] of section.controls) {
      const row = document.createElement('label');
      row.className = 'slider-row';
      row.innerHTML = `
        <span class="slider-label">${label}</span>
        <input id="${key}" type="range" min="${min}" max="${max}" step="${step}" value="${state.params[key]}">
        <span id="${key}-value" class="slider-value">${controlValueText(key, unit)}</span>
      `;
      wrap.appendChild(row);
    }
    root.appendChild(wrap);
  }

  $('speedMode').value = state.params.speedMode;
  $('sequence').value = state.params.sequence;

  root.addEventListener('input', (event) => {
    const target = event.target;
    if (!target.id) return;
    if (target.type === 'range') {
      state.params[target.id] = Number(target.value);
      const def = CONTROL_SECTIONS.flatMap((s) => s.controls).find((c) => c[0] === target.id);
      $(target.id + '-value').textContent = controlValueText(target.id, def ? def[2] : '');
      runAndRender();
    }
  });

  root.addEventListener('change', (event) => {
    const target = event.target;
    if (target.id === 'speedMode') state.params.speedMode = target.value;
    if (target.id === 'sequence') state.params.sequence = target.value;
    if (target.id === 'beltSubtraction') state.params.beltSubtraction = target.checked;
    if (target.id === 'logCsv') state.params.logMode = target.checked ? 'csv' : 'human';
    runAndRender();
  });
}

function metric(label, value, tone = '') {
  return `<div class="metric ${tone}"><span>${label}</span><strong>${value}</strong></div>`;
}

function renderMetrics(result) {
  const p = state.params;
  const speed = SPEED_MODES[p.speedMode];
  const routeMs = selectorRouteReadyMs(p);
  const counts = result.bins.map((n, i) => `${n}/${EXPECTED_BINS[i]}`).join('  ');
  const routeTone = result.halt ? 'bad' : result.routeRecoveries.length > 0 ? 'warn' : 'ok';
  $('metrics').innerHTML = [
    metric('State', result.halt ? `ERROR ${result.halt.code}` : result.countsMatch ? 'SORT COMPLETE' : 'SORTING', result.halt ? 'bad' : 'ok'),
    metric('Confirmed', `${result.confirmed}/${p.totalBricks}`),
    metric('Run Time', fmtMs(result.totalMs)),
    metric('Accuracy', fmtPct(result.accuracy), result.accuracy === 100 ? 'ok' : 'bad'),
    metric('Bins', counts, result.countsMatch ? 'ok' : 'warn'),
    metric('Max In Flight', result.maxInFlight),
    metric('Route Ready', fmtMs(routeMs), routeTone),
    metric('Recoveries', result.routeRecoveries.length, routeTone),
    metric('Peak Servo Heat', result.peakHeat.toFixed(2), result.thermalState === 'NORMAL' ? 'ok' : 'warn'),
    metric('Speed Mode', `${p.speedMode} ${speed.fastSps}/${speed.slowSps} SPS`),
  ].join('');
}

function renderBrickLog(result) {
  const rows = result.brickLog.map((b) => {
    const s = b.sense;
    const route = b.lowConfidence ? 'guess' : `${b.servoAngle} deg`;
    const resultText = b.ok ? 'OK' : result.halt && result.halt.brickNumber === b.brickNumber ? result.halt.code : 'pending';
    return `
      <tr class="${b.ok ? '' : 'row-warn'}">
        <td>${b.brickNumber}</td>
        <td>${b.category}</td>
        <td>${fmtMs(b.detectedMs)}</td>
        <td>${s.measuredLengthAvgMm.toFixed(2)} mm</td>
        <td>${s.beltSpeedAvgMms.toFixed(1)} mm/s</td>
        <td>${s.redRatio.toFixed(4)} (${s.sampleCount})</td>
        <td>${b.targetBin} ${BIN_LABELS[b.targetBin - 1]}</td>
        <td>${route}</td>
        <td>${b.confirmMs ? fmtMs(b.confirmMs) : ''}</td>
        <td>${resultText}</td>
      </tr>
    `;
  }).join('');
  $('brick-log').innerHTML = rows || '<tr><td colspan="10">No bricks processed.</td></tr>';
}

function eventAt(result, type, brickNumber) {
  return result.events.find((e) => e.type === type && e.detail.includes(`brick=${brickNumber}`));
}

function drawCanvas(result) {
  const canvas = $('sim-canvas');
  const ctx = canvas.getContext('2d');
  const w = canvas.width;
  const h = canvas.height;
  ctx.clearRect(0, 0, w, h);
  ctx.fillStyle = '#f7f9fb';
  ctx.fillRect(0, 0, w, h);

  const beltX = 58;
  const beltY = 170;
  const beltW = 670;
  const beltH = 72;
  const beamAX = 190;
  const beamBX = 330;
  const servoX = 640;
  const chuteX = 770;
  const binY = 66;

  ctx.fillStyle = '#343b45';
  ctx.fillRect(beltX, beltY, beltW, beltH);
  ctx.fillStyle = '#4d5662';
  for (let x = beltX + 18; x < beltX + beltW; x += 38) ctx.fillRect(x, beltY + 10, 18, beltH - 20);

  ctx.strokeStyle = '#0f766e';
  ctx.lineWidth = 3;
  ctx.beginPath();
  ctx.moveTo(beamAX, beltY - 28);
  ctx.lineTo(beamAX, beltY + beltH + 28);
  ctx.moveTo(beamBX, beltY - 28);
  ctx.lineTo(beamBX, beltY + beltH + 28);
  ctx.stroke();
  ctx.fillStyle = '#0f766e';
  ctx.font = '12px system-ui';
  ctx.fillText('Beam A', beamAX - 22, beltY - 36);
  ctx.fillText('Beam B', beamBX - 22, beltY - 36);

  ctx.fillStyle = '#111827';
  ctx.beginPath();
  ctx.arc(servoX, beltY + beltH / 2, 24, 0, Math.PI * 2);
  ctx.fill();
  ctx.fillStyle = '#ffffff';
  ctx.fillText('Servo', servoX - 16, beltY + beltH / 2 + 4);

  const bins = [
    [chuteX, binY, '#dc2626', '1 2x2 RED'],
    [chuteX + 104, binY, '#2563eb', '2 2x2 BLUE'],
    [chuteX, binY + 220, '#1d4ed8', '3 2x3 BLUE'],
    [chuteX + 104, binY + 220, '#b91c1c', '4 2x3 RED'],
  ];
  for (let i = 0; i < bins.length; i++) {
    const [x, y, color, label] = bins[i];
    ctx.fillStyle = '#ffffff';
    ctx.strokeStyle = color;
    ctx.lineWidth = 3;
    ctx.fillRect(x, y, 92, 82);
    ctx.strokeRect(x, y, 92, 82);
    ctx.fillStyle = color;
    ctx.font = '11px system-ui';
    ctx.fillText(label, x + 8, y + 22);
    ctx.font = '22px system-ui';
    ctx.fillText(String(result.bins[i]), x + 38, y + 55);
  }

  const visible = result.brickLog.slice(0, Math.min(result.brickLog.length, 24));
  const maxT = Math.max(1, result.totalMs);
  for (const b of visible) {
    const confirm = b.confirmMs || result.totalMs;
    const phase = Math.min(1, confirm / maxT);
    const x = beltX + 20 + phase * (beltW - 60);
    const y = beltY + 16 + (b.brickNumber % 3) * 12;
    ctx.fillStyle = b.category.endsWith('RED') ? '#ef4444' : '#2563eb';
    ctx.fillRect(x, y, b.category.startsWith('2x3') ? 34 : 24, 18);
    ctx.fillStyle = '#ffffff';
    ctx.font = '10px system-ui';
    ctx.fillText(String(b.brickNumber), x + 6, y + 13);
  }

  ctx.strokeStyle = result.halt ? '#b91c1c' : '#16a34a';
  ctx.lineWidth = 4;
  const lastRoute = result.brickLog.at(-1);
  const activeBin = lastRoute ? lastRoute.targetBin : 1;
  const angle = (SERVO_ANGLES[activeBin] - 90) * Math.PI / 180;
  ctx.beginPath();
  ctx.moveTo(servoX, beltY + beltH / 2);
  ctx.lineTo(servoX + Math.cos(angle) * 115, beltY + beltH / 2 + Math.sin(angle) * 115);
  ctx.stroke();

  ctx.fillStyle = result.halt ? '#991b1b' : '#14532d';
  ctx.font = '16px system-ui';
  ctx.fillText(result.halt ? `Halt: ${result.halt.message}` : 'Timed confirmation routes into expected bins', 58, 36);
}

function renderSerial(result) {
  $('harness-output').textContent = result.serial.slice(-24).join('\n') || formatHarnessStatus(result);
}

function runAndRender() {
  state.result = computeSimulation(state.params);
  renderMetrics(state.result);
  renderBrickLog(state.result);
  renderSerial(state.result);
  drawCanvas(state.result);
}

function reset() {
  state.params = cloneDefaults();
  buildControls();
  runAndRender();
}

function downloadCsv() {
  const result = state.result || computeSimulation({ ...state.params, logMode: 'csv' });
  const csv = result.csv.join('\n');
  const blob = new Blob([csv], { type: 'text/csv' });
  const url = URL.createObjectURL(blob);
  const a = document.createElement('a');
  a.href = url;
  a.download = 'simulation-run.csv';
  a.click();
  URL.revokeObjectURL(url);
}

function handleHarness() {
  const input = $('harness-input');
  const output = handleHarnessCommand(input.value, state.params, state.result);
  $('harness-output').textContent = output;
  if (input.value.startsWith('speedmode') || input.value.startsWith('log ')) {
    buildControls();
    runAndRender();
  }
}

function init() {
  buildControls();
  $('run-btn').addEventListener('click', runAndRender);
  $('reset-btn').addEventListener('click', reset);
  $('csv-btn').addEventListener('click', downloadCsv);
  $('harness-send').addEventListener('click', handleHarness);
  $('harness-input').addEventListener('keydown', (event) => {
    if (event.key === 'Enter') handleHarness();
  });
  runAndRender();
}

init();
