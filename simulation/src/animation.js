// animation.js - shared state, animation loop, event processing

'use strict';

// Shared state object - all modules import this and read/write its properties
export const state = {
  params: null,
  animRunning: false,
  animPaused: false,
  animStartWallMs: null,
  animOffsetSimMs: 0,
  simData: null,
  animFrameId: null,
  liveThermal: {sol1:0, sol2:0, sol3:0, stepper:0},
  liveBinCounts: [0, 0, 0, 0],
  liveEventIdx: 0,
  animBricks: [],
  pendingFlashes: [],
  plowStates: [
    null,
    {phase:'idle', phaseStartSim:0, angle:0},
    {phase:'idle', phaseStartSim:0, angle:0},
    {phase:'idle', phaseStartSim:0, angle:0},
  ],
  speedMultiplier: 1,
  replayMode: false,
  haltOverlay: null, // {msg} when ERROR_HALT is active
};

// Callbacks registered from main.js
const cb = {
  updateWarnings: null,
  showRunSummary: null,
  updateThermalUI: null,
  setBinCount: null,
  flashBin: null,
  updateBinMatchState: null,
  drawBelt: null,
  drawErrorHaltOverlay: null,
  addEvent: null,
  populateBrickTable: null,
  setText: null,
};

export function registerCallbacks(cbs) {
  Object.assign(cb, cbs);
}

import { BRICK_COLOR } from './canvas.js';

// Reset animation UI state without recomputing sim
function resetAnimState() {
  state.liveThermal = {sol1:0, sol2:0, sol3:0, stepper:0};
  state.liveBinCounts = [0, 0, 0, 0];
  state.liveEventIdx = 0;
  state.animBricks = [];
  state.pendingFlashes = [];
  state.haltOverlay = null;
  state.plowStates = [
    null,
    {phase:'idle', phaseStartSim:0, angle:0},
    {phase:'idle', phaseStartSim:0, angle:0},
    {phase:'idle', phaseStartSim:0, angle:0},
  ];
}

// Used by both startSim and startReplay - launches the rAF loop after state.simData is set
export function _launchAnimLoop(updateWarnings, populateBrickTable) {
  resetAnimState();

  for (let i = 0; i < 4; i++) {
    if (cb.setBinCount) cb.setBinCount(i, 0);
    const box = document.getElementById('bin-' + i);
    if (box) box.className = 'bin-box';
  }
  const tlEl = document.getElementById('timeline-log');
  if (tlEl) tlEl.innerHTML = '';
  const blEl = document.getElementById('brick-log-body');
  if (blEl) blEl.innerHTML = '<tr><td colspan="9" style="color:var(--text3);text-align:center;padding:12px">Running...</td></tr>';
  const rsEl = document.getElementById('run-summary');
  if (rsEl) rsEl.style.display = 'none';

  if (updateWarnings) updateWarnings();
  if (populateBrickTable && state.simData) populateBrickTable(state.simData.brickLog);

  state.animRunning = true;
  state.animPaused = false;
  state.animStartWallMs = performance.now();
  state.animOffsetSimMs = 0;

  document.getElementById('runBtn').textContent = 'Pause';
  document.getElementById('resetBtn').style.display = 'inline-block';

  requestAnimationFrame(animFrame);
}

export function startSim(computeSimulation, params, updateWarnings, populateBrickTable) {
  if (state.animRunning) return;
  state.simData = computeSimulation(params);
  state.replayMode = false;
  _launchAnimLoop(updateWarnings, populateBrickTable);
}

export function resetSim(params, drawBeltFn) {
  state.animRunning = false;
  state.animPaused = false;
  state.replayMode = false;
  state.haltOverlay = null;
  if (state.animFrameId) cancelAnimationFrame(state.animFrameId);
  resetAnimState();

  for (let i = 0; i < 4; i++) {
    if (cb.setBinCount) cb.setBinCount(i, 0);
    const box = document.getElementById('bin-' + i);
    if (box) box.className = 'bin-box';
  }
  if (cb.updateThermalUI) cb.updateThermalUI();
  document.getElementById('runBtn').textContent = 'Run Simulation';
  document.getElementById('resetBtn').style.display = 'none';
  const rsEl = document.getElementById('run-summary');
  if (rsEl) rsEl.style.display = 'none';
  const tlEl = document.getElementById('timeline-log');
  if (tlEl) tlEl.innerHTML = '<span style="color:var(--text3);font-size:12px">Run a simulation to see events here.</span>';
  const blEl = document.getElementById('brick-log-body');
  if (blEl) blEl.innerHTML = '<tr><td colspan="9" style="color:var(--text3);text-align:center;padding:12px">No data yet.</td></tr>';

  const rlEl = document.getElementById('replay-label');
  if (rlEl) rlEl.style.display = 'none';

  const canvas = document.getElementById('belt-canvas');
  if (canvas) {
    const ctx = canvas.getContext('2d');
    if (drawBeltFn) drawBeltFn(ctx, canvas.width, canvas.height, 0);
  }
}

function animFrame(wallMs) {
  if (!state.animRunning) return;

  if (!state.animPaused) {
    const wallElapsed = wallMs - state.animStartWallMs;
    const simMs = state.animOffsetSimMs + wallElapsed * state.speedMultiplier;

    while (state.liveEventIdx < state.simData.events.length && state.simData.events[state.liveEventIdx].t <= simMs) {
      processEvent(state.simData.events[state.liveEventIdx], simMs);
      state.liveEventIdx++;
      if (state.haltOverlay) break;
    }

    updatePlowAngles(simMs);
    updateAnimBricks(simMs);

    const canvas = document.getElementById('belt-canvas');
    if (canvas) {
      const ctx = canvas.getContext('2d');
      if (cb.drawBelt) {
        cb.drawBelt(ctx, canvas.width, canvas.height, simMs);
      }
      if (state.haltOverlay && cb.drawErrorHaltOverlay) {
        cb.drawErrorHaltOverlay(ctx, canvas.width, canvas.height, state.haltOverlay.msg);
      }
    }

    if (cb.updateThermalUI) cb.updateThermalUI();

    if (state.haltOverlay) {
      state.animRunning = false;
      document.getElementById('runBtn').textContent = 'Run Simulation';
      document.getElementById('resetBtn').style.display = 'inline-block';
      return;
    }

    if (state.liveEventIdx >= state.simData.events.length) {
      state.animRunning = false;
      document.getElementById('runBtn').textContent = 'Run Simulation';
      if (cb.showRunSummary) cb.showRunSummary(state.simData);
      if (cb.updateBinMatchState) cb.updateBinMatchState();
    }
  }

  if (state.animRunning) {
    state.animFrameId = requestAnimationFrame(animFrame);
  }
}

function processEvent(ev, simMs) {
  const tStr = ev.t.toFixed(0) + 'ms';
  const p = state.params;

  switch (ev.type) {
    case 'BRICK_RELEASED': {
      if (cb.addEvent) cb.addEvent(tStr, 'RELEASED', `Brick #${ev.brickNum} (${ev.brickType}) at ${ev.sps}sps [${ev.thermalState}]`);
      if (ev.thermal) state.liveThermal = {...ev.thermal};
      if (cb.setText) {
        cb.setText('m-thermal-state', ev.thermalState);
        cb.setText('m-active-sps', ev.sps + ' sps');
      }
      const brickColor = BRICK_COLOR[ev.brickType] || '#888';
      const plowNum = p.plow_map[ev.brickType];
      state.animBricks.push({
        id: ev.brickNum,
        type: ev.brickType,
        spawnSim: ev.t,
        is2x3: ev.brickType.includes('2x3'),
        color: brickColor,
        plow: plowNum,
        binIdx: plowNum === 0 ? 3 : plowNum - 1,
        classified: false,
        classifySimMs: null,
        plowFireSimMs: null,
        binConfirmSimMs: null,
        done: false,
        flashBorder: 0,
      });
      break;
    }
    case 'BRICK_CLASSIFIED': {
      if (cb.addEvent) cb.addEvent(tStr, 'CLASSIFIED', `#${ev.brickNum}: ${ev.sizeResult} ${ev.colorResult} | gap=${ev.gapUs}us | ${ev.samples} samples`);
      const ab = state.animBricks.find(b => b.id === ev.brickNum);
      if (ab) { ab.classified = true; ab.classifySimMs = ev.t; ab.flashBorder = 200; }
      break;
    }
    case 'PLOW_FIRE': {
      if (cb.addEvent) cb.addEvent(tStr, 'PLOW_FIRE', `Plow ${ev.plow} fires for brick #${ev.brickNum}`);
      if (ev.thermal) state.liveThermal = {...ev.thermal};
      const ps = state.plowStates[ev.plow];
      if (ps) { ps.phase = 'fire'; ps.phaseStartSim = ev.t; ps.angle = 0; }
      const ab = state.animBricks.find(b => b.id === ev.brickNum);
      if (ab) ab.plowFireSimMs = ev.t;
      break;
    }
    case 'PLOW_HOLD': {
      if (cb.addEvent) cb.addEvent(tStr, 'PLOW_HOLD', `Plow ${ev.plow} holding (40% PWM)`);
      const ps = state.plowStates[ev.plow];
      if (ps) { ps.phase = 'hold'; ps.phaseStartSim = ev.t; }
      break;
    }
    case 'PLOW_RELEASE': {
      if (cb.addEvent) cb.addEvent(tStr, 'PLOW_RELEASE', `Plow ${ev.plow} de-energized, spring return`);
      const ps = state.plowStates[ev.plow];
      if (ps) { ps.phase = 'retract'; ps.phaseStartSim = ev.t; }
      break;
    }
    case 'PLOW_SKIP':
      if (cb.addEvent) cb.addEvent(tStr, 'PLOW_SKIP', ev.msg);
      break;
    case 'BIN_CONFIRM': {
      if (cb.addEvent) cb.addEvent(tStr, 'BIN_CONFIRM', `Brick #${ev.brickNum} -> Bin ${ev.binIdx + 1} (${['2x2 BLUE','2x2 RED','2x3 RED','2x3 BLUE'][ev.binIdx]})`);
      state.liveBinCounts[ev.binIdx]++;
      if (cb.setBinCount) cb.setBinCount(ev.binIdx, state.liveBinCounts[ev.binIdx]);
      if (cb.flashBin) cb.flashBin(ev.binIdx);
      const ab = state.animBricks.find(b => b.id === ev.brickNum);
      if (ab) ab.binConfirmSimMs = ev.t;
      break;
    }
    case 'RUN_COMPLETE':
      if (cb.addEvent) cb.addEvent(tStr, 'RUN_COMPLETE', `Run ${ev.run + 1} done. Bin counts: ${ev.binCounts.join(', ')}`);
      break;
    case 'INTER_RUN_GAP':
      if (cb.addEvent) cb.addEvent(tStr, 'INTER_RUN_GAP', `${ev.gapMs/1000}s cooldown between runs`);
      break;
    case 'ERROR_HALT': {
      if (cb.addEvent) cb.addEvent(tStr, 'ERROR_HALT', `Plow ${ev.plow} conflict on brick #${ev.brickNum}`);
      state.haltOverlay = {
        msg: `Plow ${ev.plow} conflict on brick #${ev.brickNum} at ${ev.t.toFixed(0)}ms`
      };
      break;
    }
    case 'DRIFT_DETECTED': {
      // Yellow timeline entry for belt drift
      const log = document.getElementById('timeline-log');
      if (log) {
        const row = document.createElement('div');
        row.className = 'event-row';
        row.innerHTML = `<span class="ev-t" style="color:var(--yellow)">${tStr}</span><span class="ev-type" style="color:var(--yellow)">DRIFT</span><span class="ev-detail" style="color:var(--yellow)">${ev.msg}</span>`;
        log.appendChild(row);
        if (log.scrollTop + log.clientHeight >= log.scrollHeight - 40) log.scrollTop = log.scrollHeight;
      }
      break;
    }
  }
}

function updatePlowAngles(simMs) {
  const p = state.params;
  for (let i = 1; i <= 3; i++) {
    const ps = state.plowStates[i];
    if (!ps) continue;
    const elapsed = simMs - ps.phaseStartSim;
    if (ps.phase === 'fire') {
      ps.angle = Math.min(35, elapsed / p.sol_full_ms * 35);
    } else if (ps.phase === 'hold') {
      ps.angle = 35;
    } else if (ps.phase === 'retract') {
      ps.angle = Math.max(0, 35 - elapsed / 20 * 35);
      if (ps.angle <= 0) { ps.phase = 'idle'; ps.angle = 0; }
    } else {
      ps.angle = 0;
    }
  }
}

function updateAnimBricks(simMs) {
  state.animBricks = state.animBricks.filter(b => !b.done);
  for (const b of state.animBricks) {
    if (b.flashBorder > 0) b.flashBorder -= 16;
  }
}

export function setSpeed(s) {
  if (state.animRunning && !state.animPaused) {
    const wallNow = performance.now();
    const wallElapsed = wallNow - state.animStartWallMs;
    state.animOffsetSimMs += wallElapsed * state.speedMultiplier;
    state.animStartWallMs = wallNow;
  }
  state.speedMultiplier = s;
  ['spd1','spd2','spd5','spd10'].forEach(id => {
    const el = document.getElementById(id);
    if (el) el.classList.remove('active');
  });
  const activeEl = document.getElementById('spd' + s);
  if (activeEl) activeEl.classList.add('active');
}
