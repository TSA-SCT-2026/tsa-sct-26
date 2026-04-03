// animation.js

'use strict';

const BIN_MAP = {
  '2x2_red': 0,
  '2x2_blue': 1,
  '2x3_blue': 2,
  '2x3_red': 3,
};

export const state = {
  params: null,
  animRunning: false,
  animPaused: false,
  animStartWallMs: 0,
  animOffsetSimMs: 0,
  simData: null,
  animFrameId: null,
  speedMultiplier: 1,
  replayMode: false,

  liveThermal: { sol1: 0, sol2: 0, sol3: 0, stepper: 0 },
  liveBinCounts: [0, 0, 0, 0],
  liveEventIdx: 0,
  animBricks: [],
  sceneState: {
    running: false,
    leverAngle: 0,
    platformAngle: 0,
    discAngleDeg: 225,
    discTargetDeg: 225,
    highlightBrick: null,
  },
  haltOverlay: null,
};

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

function resetAnimState() {
  state.liveThermal = { sol1: 0, sol2: 0, sol3: 0, stepper: 0 };
  state.liveBinCounts = [0, 0, 0, 0];
  state.liveEventIdx = 0;
  state.animBricks = [];
  state.haltOverlay = null;
  state.sceneState = {
    running: false,
    leverAngle: 0,
    platformAngle: 0,
    discAngleDeg: 225,
    discTargetDeg: 225,
    highlightBrick: null,
  };
}

function resetUiForRun() {
  const timeline = document.getElementById('timeline-log');
  if (timeline) timeline.innerHTML = '';
  const body = document.getElementById('brick-log-body');
  if (body) body.innerHTML = '<tr><td colspan="11" style="color:var(--text3);text-align:center;padding:12px">Running...</td></tr>';
  const summary = document.getElementById('run-summary');
  if (summary) summary.style.display = 'none';
  for (let i = 0; i < 4; i++) {
    cb.setBinCount && cb.setBinCount(i, 0);
    const box = document.getElementById(`bin-${i}`);
    if (box) box.className = 'bin-box';
  }
}

export function _launchAnimLoop(updateWarnings, populateBrickTable) {
  resetAnimState();
  resetUiForRun();
  if (updateWarnings) updateWarnings();
  if (populateBrickTable && state.simData) populateBrickTable(state.simData.brickLog);

  state.animRunning = true;
  state.animPaused = false;
  state.animStartWallMs = performance.now();
  state.animOffsetSimMs = 0;
  state.sceneState.running = true;

  const runBtn = document.getElementById('runBtn');
  if (runBtn) runBtn.textContent = 'Pause';
  const resetBtn = document.getElementById('resetBtn');
  if (resetBtn) resetBtn.style.display = 'inline-block';

  requestAnimationFrame(animFrame);
}

export function startSim(computeSimulation, params, updateWarnings, populateBrickTable) {
  if (state.animRunning) return;
  state.simData = computeSimulation(params);
  state.replayMode = false;
  _launchAnimLoop(updateWarnings, populateBrickTable);
}

export function runInstant(computeSimulationFn, params, updateWarnings, populateBrickTable) {
  if (state.animRunning) return;
  state.simData = computeSimulationFn(params);
  state.replayMode = false;
  resetAnimState();
  resetUiForRun();

  for (const ev of state.simData.events) {
    processEvent(ev, ev.t);
  }
  state.sceneState.running = false;
  cb.updateThermalUI && cb.updateThermalUI();
  cb.showRunSummary && cb.showRunSummary(state.simData);
  cb.updateBinMatchState && cb.updateBinMatchState();
  populateBrickTable && populateBrickTable(state.simData.brickLog);
  updateWarnings && updateWarnings();

  const runBtn = document.getElementById('runBtn');
  if (runBtn) runBtn.textContent = 'Run Simulation';
  const resetBtn = document.getElementById('resetBtn');
  if (resetBtn) resetBtn.style.display = 'inline-block';
}

export function resetSim(params, drawBeltFn) {
  state.animRunning = false;
  state.animPaused = false;
  state.replayMode = false;
  if (state.animFrameId) cancelAnimationFrame(state.animFrameId);
  resetAnimState();

  for (let i = 0; i < 4; i++) {
    cb.setBinCount && cb.setBinCount(i, 0);
    const box = document.getElementById(`bin-${i}`);
    if (box) box.className = 'bin-box';
  }

  cb.updateThermalUI && cb.updateThermalUI();
  const runBtn = document.getElementById('runBtn');
  if (runBtn) runBtn.textContent = 'Run Simulation';
  const resetBtn = document.getElementById('resetBtn');
  if (resetBtn) resetBtn.style.display = 'none';

  const summary = document.getElementById('run-summary');
  if (summary) summary.style.display = 'none';
  const timeline = document.getElementById('timeline-log');
  if (timeline) timeline.innerHTML = '<span style="color:var(--text3);font-size:12px">Run a simulation to see events here.</span>';
  const body = document.getElementById('brick-log-body');
  if (body) body.innerHTML = '<tr><td colspan="11" style="color:var(--text3);text-align:center;padding:12px">No data yet.</td></tr>';
  const replay = document.getElementById('replay-label');
  if (replay) replay.style.display = 'none';

  const canvas = document.getElementById('belt-canvas');
  if (canvas) {
    const ctx = canvas.getContext('2d');
    drawBeltFn && drawBeltFn(ctx, canvas.width, canvas.height, 0);
  }
}

function logEvent(type, detail, t) {
  if (!cb.addEvent) return;
  cb.addEvent(`${Math.round(t)}ms`, type, detail);
}

function updateThermalFromState() {
  const p = state.params;
  if (!p) return;
  const peak = Math.max(state.liveThermal.sol1, state.liveThermal.sol2, state.liveThermal.sol3, state.liveThermal.stepper);
  const thermalLabel = peak >= p.thermal_danger_level ? 'DANGER' : peak >= p.thermal_warn_level ? 'WARNING' : 'NORMAL';
  if (cb.setText) cb.setText('m-thermal-state', thermalLabel);
}

function moveBrickTo(binIdx, brickNum) {
  const existing = state.animBricks.find((b) => b.id === brickNum);
  if (!existing) return;
  const centers = [
    [0.64, 0.26],
    [0.76, 0.38],
    [0.76, 0.74],
    [0.62, 0.86],
  ];
  existing.targetBinIdx = binIdx;
  existing.tx = centers[binIdx][0];
  existing.ty = centers[binIdx][1];
}

function processEvent(ev, simMs) {
  switch (ev.type) {
    case 'STATE_ENTER': {
      if (ev.state === 'ERROR_HALT') {
        state.haltOverlay = `Brick #${ev.brickNum} ${ev.detail || ''}`.trim();
      }
      if (ev.state === 'RELEASED') {
        state.sceneState.leverAngle = 22;
        state.sceneState.platformAngle = 18;
      } else if (ev.state === 'RESET') {
        state.sceneState.leverAngle = 0;
        state.sceneState.platformAngle = 0;
      }
      logEvent('STATE', `${ev.state}${ev.detail ? ` (${ev.detail})` : ''}`, ev.t);
      break;
    }

    case 'ENTRY_BEAM_TRIGGERED': {
      const y = 0.54;
      state.animBricks.push({
        id: ev.brickNum,
        type: ev.category,
        sizeResult: ev.category.includes('2x3') ? '2x3' : '2x2',
        x: 0.10,
        y,
        tx: 0.24,
        ty: y,
        targetBinIdx: BIN_MAP[ev.category],
        done: false,
      });
      state.sceneState.highlightBrick = ev.brickNum;
      logEvent('ENTRY', `Brick #${ev.brickNum} entered chamber`, ev.t);
      break;
    }

    case 'BRICK_SEATED':
      logEvent('SEATED', `Brick #${ev.brickNum} seated`, ev.t);
      break;

    case 'BRICK_CLASSIFIED':
      logEvent('CLASSIFIED', `#${ev.brickNum}: ${ev.sizeResult} ${ev.colorResult} -> Bin ${ev.targetBin + 1}`, ev.t);
      break;

    case 'DISC_INDEX_START':
      state.liveThermal.stepper = Math.min(1, state.liveThermal.stepper + (state.params.thermal_heat_per_step || 0.05));
      updateThermalFromState();
      logEvent('INDEX_START', `#${ev.brickNum}: ${ev.moveSteps} steps @ ${ev.activeSps} sps`, ev.t);
      break;

    case 'DISC_INDEXED': {
      const degMap = [315, 45, 135, 225];
      state.sceneState.discTargetDeg = degMap[ev.targetBin];
      state.sceneState.discAngleDeg = degMap[ev.targetBin];
      if (ev.thermalState && cb.setText) {
        cb.setText('m-active-sps', `${ev.activeSps} steps/s`);
      }
      logEvent('INDEXED', `Disc -> Bin ${ev.targetBin + 1}`, ev.t);
      break;
    }

    case 'PLATFORM_RELEASED':
      state.liveThermal.sol1 = Math.min(1, state.liveThermal.sol1 + (state.params.thermal_heat_per_sol || 0.15));
      updateThermalFromState();
      state.sceneState.leverAngle = 28;
      state.sceneState.platformAngle = 38;
      logEvent('RELEASE', `Brick #${ev.brickNum} released`, ev.t);
      break;

    case 'SOLENOID_OFF':
      state.sceneState.leverAngle = 8;
      logEvent('SOL_OFF', `Brick #${ev.brickNum} solenoid off`, ev.t);
      break;

    case 'BIN_CONFIRM':
      state.liveBinCounts[ev.binIdx]++;
      cb.setBinCount && cb.setBinCount(ev.binIdx, state.liveBinCounts[ev.binIdx]);
      cb.flashBin && cb.flashBin(ev.binIdx);
      moveBrickTo(ev.binIdx, ev.brickNum);
      logEvent('BIN_CONFIRM', `#${ev.brickNum} -> ${ev.binIdx + 1}${ev.correct ? ' OK' : ' FAIL'}`, ev.t);
      break;

    case 'REHOME_CHECK':
      logEvent('REHOME', `#${ev.brickNum} home check ${ev.ok ? 'OK' : 'FAIL'}`, ev.t);
      break;

    case 'THERMAL_UPDATE':
      if (ev.thermal) state.liveThermal = { ...ev.thermal };
      break;

    case 'INTER_RUN_GAP':
      logEvent('GAP', `Run ${ev.run + 1} -> next run gap ${Math.round(ev.gapMs / 1000)}s`, ev.t);
      break;

    case 'RUN_COMPLETE':
      logEvent('RUN_COMPLETE', `Run ${ev.run + 1} done`, ev.t);
      break;

    case 'RUN_HALTED':
      logEvent('RUN_HALTED', `Run ${ev.run + 1} halted (${ev.haltCode})`, ev.t);
      break;
  }
}

function updateBrickAnimations() {
  for (const b of state.animBricks) {
    if (b.done) continue;
    const dx = b.tx - b.x;
    const dy = b.ty - b.y;
    b.x += dx * 0.15;
    b.y += dy * 0.15;
    if (Math.abs(dx) < 0.002 && Math.abs(dy) < 0.002 && b.targetBinIdx != null) {
      b.done = true;
    }
  }
}

function render(simMs) {
  const canvas = document.getElementById('belt-canvas');
  if (!canvas) return;
  const ctx = canvas.getContext('2d');

  const mappedBricks = state.animBricks.map((b) => ({
    ...b,
    x: b.x * canvas.width,
    y: b.y * canvas.height,
  }));

  cb.drawBelt && cb.drawBelt(ctx, canvas.width, canvas.height, simMs, state.params, state.liveBinCounts, state.sceneState, mappedBricks, state.replayMode);
  if (state.haltOverlay && cb.drawErrorHaltOverlay) {
    cb.drawErrorHaltOverlay(ctx, canvas.width, canvas.height, state.haltOverlay);
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
    }

    updateBrickAnimations();
    cb.updateThermalUI && cb.updateThermalUI();
    render(simMs);

    if (state.liveEventIdx >= state.simData.events.length) {
      state.animRunning = false;
      state.sceneState.running = false;
      const runBtn = document.getElementById('runBtn');
      if (runBtn) runBtn.textContent = 'Run Simulation';
      cb.showRunSummary && cb.showRunSummary(state.simData);
      cb.updateBinMatchState && cb.updateBinMatchState();
    }
  }

  if (state.animRunning) {
    state.animFrameId = requestAnimationFrame(animFrame);
  }
}

export function setSpeed(multiplier) {
  if (state.animRunning && !state.animPaused) {
    const now = performance.now();
    const wallElapsed = now - state.animStartWallMs;
    state.animOffsetSimMs += wallElapsed * state.speedMultiplier;
    state.animStartWallMs = now;
  }
  state.speedMultiplier = multiplier;
  ['spd1', 'spd2', 'spd5', 'spd10', 'spdInstant'].forEach((id) => {
    const el = document.getElementById(id);
    if (el) el.classList.remove('active');
  });
  const selected = multiplier === 1 ? 'spd1' : multiplier === 2 ? 'spd2' : multiplier === 5 ? 'spd5' : 'spd10';
  const active = document.getElementById(selected);
  if (active) active.classList.add('active');
}
