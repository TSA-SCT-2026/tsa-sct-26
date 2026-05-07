'use strict';

export const CATEGORIES = ['2x2_RED', '2x2_BLUE', '2x3_BLUE', '2x3_RED'];

export const BIN_LABELS = ['2x2 RED', '2x2 BLUE', '2x3 BLUE', '2x3 RED'];

export const CATEGORY_TO_BIN = {
  '2x2_RED': 1,
  '2x2_BLUE': 2,
  '2x3_BLUE': 3,
  '2x3_RED': 4,
};

export const BIN_TO_CATEGORY = {
  1: '2x2_RED',
  2: '2x2_BLUE',
  3: '2x3_BLUE',
  4: '2x3_RED',
};

export const FULL_RUN_SEQUENCE = [
  '2x2_RED',
  '2x2_BLUE',
  '2x3_BLUE',
  '2x3_RED',
  '2x2_RED',
  '2x2_BLUE',
  '2x3_BLUE',
  '2x2_RED',
  '2x3_RED',
  '2x3_BLUE',
  '2x2_BLUE',
  '2x3_BLUE',
  '2x2_RED',
  '2x2_BLUE',
  '2x3_BLUE',
  '2x2_RED',
  '2x2_BLUE',
  '2x3_RED',
  '2x3_BLUE',
  '2x2_RED',
  '2x3_RED',
  '2x3_BLUE',
  '2x2_BLUE',
  '2x3_BLUE',
];

export const DEFAULTS = {
  totalBricks: 24,
  entrySpacingMs: 520,
  beltSpeedMms: 230,
  speedMode: 'balanced',
  sequence: 'firmware_fullrun',
  colorThreshold: 0.49,
  sizeThresholdMm: 18.0,
  beamSpacingMm: 40.436,
  beamBToServoMm: 50.0,
  colorSampleIntervalUs: 70000,
  colorPassMinSamples: 2,
  beltSubtraction: true,
  colorNoisePct: 1.5,
  sizeNoisePct: 1.0,
  missingBeamBRatePct: 0,
  servoSettleMs: 250,
  selectorJigglePulses: 4,
  selectorJiggleStepMs: 45,
  routeSwitchMarginMs: 15,
  spacingRecoveryMarginMs: 5,
  minSpacingSps: 1200,
  timedConfirmWindowMs: 20,
  handoffWindowMs: 1500,
  thermalHeatPerServoMove: 0.05,
  thermalDecayRate: 0.5,
  thermalWarnLevel: 0.60,
  thermalDangerLevel: 0.85,
  logMode: 'human',
};

export const SPEED_MODES = {
  balanced: { fastSps: 7000, slowSps: 3000 },
  aggressive: { fastSps: 8500, slowSps: 3500 },
  accuracy: { fastSps: 5000, slowSps: 2600 },
};

export const SERVO_ANGLES = {
  1: 45,
  2: 75,
  3: 110,
  4: 135,
};

export const EXPECTED_BINS = [6, 6, 8, 4];

const LENGTH_MM = {
  '2x2_RED': 16.0,
  '2x2_BLUE': 16.0,
  '2x3_BLUE': 24.0,
  '2x3_RED': 24.0,
};

const COLOR_TARGET = {
  '2x2_RED': 0.512,
  '2x3_RED': 0.512,
  '2x2_BLUE': 0.467,
  '2x3_BLUE': 0.467,
};

const BELT_BASELINE = {
  redHz: 110,
  blueHz: 112,
  clearHz: 160,
};

export function cloneDefaults() {
  return JSON.parse(JSON.stringify(DEFAULTS));
}

function clamp(v, min, max) {
  return Math.max(min, Math.min(max, v));
}

function round(v, places = 1) {
  const f = 10 ** places;
  return Math.round(v * f) / f;
}

function isRed(category) {
  return category.endsWith('RED');
}

function isTwoByThree(category) {
  return category.startsWith('2x3');
}

function pseudoNoise(index, salt, pct) {
  if (pct <= 0) return 0;
  const x = Math.sin((index + 1) * 12.9898 + salt * 78.233) * 43758.5453;
  return (x - Math.floor(x) - 0.5) * 2 * (pct / 100);
}

function expectedCounts() {
  return {
    '2x2_RED': 6,
    '2x2_BLUE': 6,
    '2x3_BLUE': 8,
    '2x3_RED': 4,
  };
}

export function generateSequence(params = DEFAULTS) {
  const mode = params.sequence || 'firmware_fullrun';
  if (mode === 'firmware_fullrun') return FULL_RUN_SEQUENCE.slice();

  const counts = expectedCounts();
  const seq = [];
  if (mode === 'grouped') {
    for (const category of CATEGORIES) {
      for (let i = 0; i < counts[category]; i++) seq.push(category);
    }
    return seq;
  }
  if (mode === 'red_then_blue') {
    for (const category of ['2x2_RED', '2x3_RED', '2x2_BLUE', '2x3_BLUE']) {
      for (let i = 0; i < counts[category]; i++) seq.push(category);
    }
    return seq;
  }
  if (mode === 'random_seeded') {
    const base = generateSequence({ ...params, sequence: 'grouped' });
    for (let i = base.length - 1; i > 0; i--) {
      const j = Math.floor(Math.abs(Math.sin(i * 41.17)) * (i + 1));
      [base[i], base[j]] = [base[j], base[i]];
    }
    return base;
  }

  while (seq.length < 24) {
    let pushed = false;
    for (const category of CATEGORIES) {
      if (counts[category] > 0) {
        seq.push(category);
        counts[category]--;
        pushed = true;
      }
    }
    if (!pushed) break;
  }
  return seq;
}

export function selectorRouteReadyMs(params = DEFAULTS) {
  const jiggleMs = (params.selectorJigglePulses + 1) * params.selectorJiggleStepMs;
  return params.servoSettleMs + jiggleMs;
}

export function classifyFromSense(sense, params = DEFAULTS) {
  const colorUsable = sense.sampleCount >= params.colorPassMinSamples &&
    sense.redRatio > 0.05 &&
    sense.redRatio < 0.95 &&
    sense.redNetHz + sense.blueNetHz >= 0.5;

  if (!sense.sizeValid || !colorUsable) return 'UNCERTAIN';

  const size = sense.measuredLengthAvgMm >= params.sizeThresholdMm ? '2x3' : '2x2';
  const color = sense.redRatio >= params.colorThreshold ? 'RED' : 'BLUE';
  return `${size}_${color}`;
}

export function makeSenseResult(category, brickNumber, detectedMs, params = DEFAULTS) {
  const speedNoise = pseudoNoise(brickNumber, 1, params.sizeNoisePct);
  const speedMms = Math.max(20, params.beltSpeedMms * (1 + speedNoise));
  const lengthNoise = pseudoNoise(brickNumber, 2, params.sizeNoisePct);
  const actualLength = LENGTH_MM[category] * (1 + lengthNoise);
  const missingBeamB = pseudoNoise(brickNumber, 3, 100) < (params.missingBeamBRatePct / 100) * 2 - 1;

  const aEnterMs = detectedMs;
  const leadMs = params.beamSpacingMm / speedMms * 1000;
  const trailSpeed = speedMms * (1 + pseudoNoise(brickNumber, 4, params.sizeNoisePct));
  const trailMs = params.beamSpacingMm / trailSpeed * 1000;
  const blockedAMs = actualLength / speedMms * 1000;
  const blockedBMs = actualLength / trailSpeed * 1000;
  const aExitMs = aEnterMs + blockedAMs;
  const bEnterMs = aEnterMs + leadMs;
  const bExitMs = bEnterMs + blockedBMs;

  const vLead = params.beamSpacingMm / (leadMs / 1000);
  const vTrail = missingBeamB ? 0 : params.beamSpacingMm / (trailMs / 1000);
  const vAvg = vTrail > 0 ? (vLead + vTrail) * 0.5 : vLead;
  const lenA = vAvg * (blockedAMs / 1000);
  const lenB = missingBeamB ? 0 : vAvg * (blockedBMs / 1000);
  const lenAvg = lenB > 0 ? (lenA + lenB) * 0.5 : lenA;
  const speedSkewPct = vTrail > 0 ? Math.abs(vLead - vTrail) / vAvg * 100 : 0;
  const lengthSkewPct = lenB > 0 ? Math.abs(lenA - lenB) / lenAvg * 100 : 0;

  const passMs = bExitMs - aEnterMs;
  const sampleCount = Math.max(0, Math.floor((passMs * 1000) / params.colorSampleIntervalUs));
  const targetRatio = COLOR_TARGET[category] + pseudoNoise(brickNumber, 5, params.colorNoisePct);
  const netSum = 980 + pseudoNoise(brickNumber, 6, 6) * 980;
  const redNet = clamp(targetRatio, 0.05, 0.95) * netSum;
  const blueNet = netSum - redNet;
  const rawRed = params.beltSubtraction ? BELT_BASELINE.redHz + redNet : redNet;
  const rawBlue = params.beltSubtraction ? BELT_BASELINE.blueHz + blueNet : blueNet;
  const rawClear = params.beltSubtraction ? BELT_BASELINE.clearHz + netSum * 0.62 : netSum * 0.62;
  const finalRedNet = params.beltSubtraction ? Math.max(0, rawRed - BELT_BASELINE.redHz) : rawRed;
  const finalBlueNet = params.beltSubtraction ? Math.max(0, rawBlue - BELT_BASELINE.blueHz) : rawBlue;
  const redRatio = finalRedNet + finalBlueNet > 0 ? finalRedNet / (finalRedNet + finalBlueNet) : 0;

  const sense = {
    isTwoByThree: lenAvg >= params.sizeThresholdMm,
    sizeValid: !missingBeamB,
    sizeSignal: Math.round(lenAvg),
    redRatio,
    rawRedHz: rawRed,
    rawBlueHz: rawBlue,
    rawClearHz: rawClear,
    redNetHz: finalRedNet,
    blueNetHz: finalBlueNet,
    beltSpeedLeadMms: vLead,
    beltSpeedTrailMms: vTrail,
    beltSpeedAvgMms: vAvg,
    measuredLengthAmm: lenA,
    measuredLengthBmm: lenB,
    measuredLengthAvgMm: lenAvg,
    speedSkewPct,
    lengthSkewPct,
    sampleCount,
    lowConfidence: false,
    guessed: false,
    retried: false,
    category: 'UNCERTAIN',
    beam: {
      aEnterMs,
      aExitMs,
      bEnterMs: missingBeamB ? null : bEnterMs,
      bExitMs: missingBeamB ? null : bExitMs,
    },
  };
  sense.category = classifyFromSense(sense, params);
  return sense;
}

function chooseProbabilityBin(sense, bins, params) {
  const sizeUsable = sense.measuredLengthAvgMm >= 8 && sense.measuredLengthAvgMm <= 35;
  const colorUsable = sense.sampleCount > 0 &&
    sense.rawRedHz > 0 &&
    sense.rawBlueHz > 0 &&
    sense.redRatio > 0.05 &&
    sense.redRatio < 0.95;
  const targetIsLarge = sense.measuredLengthAvgMm >= params.sizeThresholdMm;
  const targetIsRed = sense.redRatio >= params.colorThreshold;
  let bestBin = 1;
  let bestRemaining = -1;
  for (let bin = 1; bin <= 4; bin++) {
    const category = BIN_TO_CATEGORY[bin];
    if (sizeUsable && isTwoByThree(category) !== targetIsLarge) continue;
    if (colorUsable && isRed(category) !== targetIsRed) continue;
    const remaining = Math.max(0, EXPECTED_BINS[bin - 1] - bins[bin - 1]);
    if (remaining > bestRemaining) {
      bestRemaining = remaining;
      bestBin = bin;
    }
  }
  return bestBin;
}

function decayHeat(value, elapsedMs, params) {
  const seconds = Math.max(0, elapsedMs) / 1000;
  return value * Math.pow(params.thermalDecayRate, seconds);
}

function humanLine(ts, label, message) {
  return `[${String(Math.round(ts)).padStart(7, ' ')}ms] ${label.padEnd(14, ' ')}${message}`;
}

function csvRow(ts, eventName, brick, state, category, targetBin, actualBin, angle, samples, ok, error, message, heat) {
  return [
    eventName,
    Math.round(ts),
    brick || '',
    state || '',
    category || '',
    targetBin || '',
    actualBin || '',
    angle || '',
    samples || '',
    ok,
    error || '',
    round(heat, 3),
    message || '',
  ].join(',');
}

export function computeSimulation(inputParams = DEFAULTS) {
  const params = { ...cloneDefaults(), ...inputParams };
  const sequence = generateSequence(params).slice(0, params.totalBricks);
  const speedMode = SPEED_MODES[params.speedMode] || SPEED_MODES.balanced;
  const routeReadyMs = selectorRouteReadyMs(params);
  const beamAToServoMm = params.beamSpacingMm + params.beamBToServoMm;
  const bins = [0, 0, 0, 0];
  const brickLog = [];
  const events = [];
  const serial = [];
  const csv = ['event,ts_ms,brick,state,category,target_bin,actual_bin,servo_angle,samples,ok,error,heat,message'];
  const inFlight = [];
  const routeRecoveries = [];
  let heat = 0;
  let peakHeat = 0;
  let lastHeatMs = 0;
  let t = 0;
  let nextScheduledDetectMs = 0;
  let machineReadyMs = 0;
  let routeProtectedUntilMs = 0;
  let halt = null;

  function updateHeat(now) {
    heat = decayHeat(heat, now - lastHeatMs, params);
    lastHeatMs = now;
    peakHeat = Math.max(peakHeat, heat);
  }

  function logEvent(ts, type, detail) {
    events.push({ t: ts, type, detail });
  }

  function serialInfo(ts, msg) {
    if (params.logMode === 'csv') {
      csv.push(csvRow(ts, 'info', 0, '', '', 0, 0, 0, 0, '', '', msg, heat));
    } else {
      serial.push(humanLine(ts, 'INFO', msg));
    }
  }

  function confirmReady(now) {
    let write = 0;
    for (let i = 0; i < inFlight.length; i++) {
      const rec = inFlight[i];
      const confirmAt = rec.estimatedClearMs + params.timedConfirmWindowMs;
      if (confirmAt > now) {
        inFlight[write++] = rec;
        continue;
      }
      bins[rec.targetBin - 1]++;
      logEvent(confirmAt, 'BIN_CONFIRMED', `brick=${rec.brickNumber} bin=${rec.targetBin}`);
      if (params.logMode === 'csv') {
        csv.push(csvRow(confirmAt, 'handoff_done', rec.brickNumber, 'CONFIRM', '', rec.targetBin, 0, 0, 0, '1', '', '', heat));
        csv.push(csvRow(confirmAt, 'bin_confirm', rec.brickNumber, 'CONFIRM', '', rec.targetBin, rec.targetBin, 0, 0, '1', '', `transit_ms=${Math.round(confirmAt - rec.detectedMs)}`, heat));
      } else {
        serial.push(humanLine(confirmAt, 'HANDOFF', `brick=${rec.brickNumber} target_bin=${rec.targetBin}`));
        serial.push(humanLine(confirmAt, 'BIN_CONFIRM', `brick=${rec.brickNumber} expected=${rec.targetBin} actual=${rec.targetBin} ok=yes`));
      }
      const row = brickLog.find((b) => b.brickNumber === rec.brickNumber);
      if (row) {
        row.confirmMs = confirmAt;
        row.actualBin = rec.targetBin;
        row.ok = true;
      }
    }
    inFlight.length = write;
  }

  serialInfo(0, 'state machine ready for servo chute sorter');
  serialInfo(0, `conveyor: mode=${params.speedMode} fast_sps=${speedMode.fastSps} slow_sps=${speedMode.slowSps}`);
  logEvent(0, 'RUN_STARTED', '24 brick expected set');

  for (let i = 0; i < sequence.length; i++) {
    const brickNumber = i + 1;
    const physicalDetectMs = nextScheduledDetectMs;
    const detectedMs = Math.max(physicalDetectMs, machineReadyMs);
    t = detectedMs;
    confirmReady(t);
    updateHeat(t);

    const categoryTruth = sequence[i];
    const sense = makeSenseResult(categoryTruth, brickNumber, detectedMs, params);
    const senseDoneMs = sense.beam.bExitMs ?? detectedMs + 3500;
    t = senseDoneMs;
    updateHeat(t);

    let classified = sense.category;
    let targetBin = CATEGORY_TO_BIN[classified];
    if (!targetBin) {
      targetBin = chooseProbabilityBin(sense, bins, params);
      classified = BIN_TO_CATEGORY[targetBin];
      sense.category = classified;
      sense.lowConfidence = true;
      sense.guessed = true;
      serialInfo(t, `sense: best guess brick=${brickNumber} bin=${targetBin} retried=0`);
    }

    const servoAngle = SERVO_ANGLES[targetBin];
    const speedMms = sense.beltSpeedAvgMms || params.beltSpeedMms;
    const lengthMm = sense.measuredLengthAvgMm || LENGTH_MM[classified];
    let estimatedCommitMs = detectedMs + beamAToServoMm / speedMms * 1000;
    let clearTimeMs = lengthMm / speedMms * 1000;
    let estimatedClearMs = estimatedCommitMs + clearTimeMs;
    const earliestRouteMs = routeProtectedUntilMs > 0 ? routeProtectedUntilMs + params.routeSwitchMarginMs : 0;
    const latestRouteMs = estimatedCommitMs > routeReadyMs + params.routeSwitchMarginMs
      ? estimatedCommitMs - (routeReadyMs + params.routeSwitchMarginMs)
      : 0;

    let effectiveLatestRouteMs = latestRouteMs;
    let routeAtMs = effectiveLatestRouteMs;
    if (latestRouteMs === 0 || latestRouteMs < earliestRouteMs) {
      routeAtMs = Math.max(earliestRouteMs, t);
      const desiredCommitMs = routeAtMs + routeReadyMs + params.routeSwitchMarginMs + params.spacingRecoveryMarginMs;
      const elapsedMs = Math.max(0, t - detectedMs);
      const traveledMm = speedMms * elapsedMs / 1000;
      const remainingDistanceMm = Math.max(0, beamAToServoMm - traveledMm);
      const availableMs = Math.max(0, desiredCommitMs - t);
      const requiredSpeedMms = availableMs > 0 ? remainingDistanceMm / (availableMs / 1000) : 0;
      let requiredSps = speedMms > 0 ? Math.round((requiredSpeedMms / speedMms) * speedMode.slowSps) : speedMode.slowSps;
      requiredSps = Math.min(requiredSps, speedMode.slowSps);
      if (requiredSps < params.minSpacingSps || remainingDistanceMm <= 0) {
        halt = {
          t,
          brickNumber,
          code: 'POSITION_DRIFT',
          message: `route spacing unrecoverable remaining=${round(remainingDistanceMm)} req_sps=${requiredSps}`,
        };
        logEvent(t, 'ERROR_HALT', halt.message);
        break;
      }
      estimatedCommitMs = desiredCommitMs;
      clearTimeMs = requiredSpeedMms > 0 ? lengthMm / requiredSpeedMms * 1000 : clearTimeMs;
      estimatedClearMs = estimatedCommitMs + clearTimeMs;
      effectiveLatestRouteMs = routeAtMs;
      routeRecoveries.push({ brickNumber, routeAtMs, requiredSps, remainingDistanceMm });
      serialInfo(t, `route: spacing recovery brick=${brickNumber} req_sps=${requiredSps} remaining=${round(remainingDistanceMm)} route_at=${Math.round(routeAtMs)} commit_ms=${Math.round(estimatedCommitMs)}`);
    }

    if (t > effectiveLatestRouteMs) {
      halt = {
        t,
        brickNumber,
        code: 'POSITION_DRIFT',
        message: `route late now=${Math.round(t)} latest=${Math.round(effectiveLatestRouteMs)}`,
      };
      logEvent(t, 'ERROR_HALT', halt.message);
      break;
    }

    routeAtMs = Math.max(effectiveLatestRouteMs, earliestRouteMs);
    if (t < routeAtMs) {
      serialInfo(t, `route: timed wait=${Math.round(routeAtMs - t)}ms brick=${brickNumber}`);
      t = routeAtMs;
      updateHeat(t);
    }

    updateHeat(t);
    heat += params.thermalHeatPerServoMove * (1 + params.selectorJigglePulses);
    peakHeat = Math.max(peakHeat, heat);
    const routeReadyAt = t + routeReadyMs;
    machineReadyMs = routeReadyAt;
    routeProtectedUntilMs = estimatedClearMs;
    inFlight.push({
      brickNumber,
      targetBin,
      servoAngle,
      detectedMs,
      routeReadyMs: routeReadyAt,
      estimatedCommitMs,
      estimatedClearMs,
      speedMms,
      lengthMm,
    });

    logEvent(detectedMs, 'SIZE_ENTRY_DETECTED', `brick=${brickNumber}`);
    logEvent(senseDoneMs, 'SENSING_DONE', `brick=${brickNumber} cat=${classified}`);
    logEvent(routeReadyAt, 'ROUTE_READY', `brick=${brickNumber} bin=${targetBin} angle=${servoAngle}`);
    if (params.logMode === 'csv') {
      csv.push(csvRow(senseDoneMs, 'classified', brickNumber, 'SENSING', classified, targetBin, 0, 0, sense.sampleCount, '1', '', '', heat));
      csv.push(csvRow(routeReadyAt, 'route_ready', brickNumber, 'ROUTING', '', targetBin, 0, servoAngle, 0, '1', '', `BIN${targetBin}_${classified}`, heat));
    } else {
      serial.push(humanLine(senseDoneMs, 'CLASSIFIED', `brick=${brickNumber} category=${classified} target_bin=${targetBin} samples=${sense.sampleCount}`));
      serial.push(humanLine(routeReadyAt, 'ROUTE_READY', `brick=${brickNumber} target_bin=${targetBin} angle=${servoAngle} ok=yes`));
    }

    brickLog.push({
      brickNumber,
      categoryTruth,
      category: classified,
      detectedMs,
      senseDoneMs,
      targetBin,
      actualBin: 0,
      servoAngle,
      routeAtMs: t,
      routeReadyMs: routeReadyAt,
      estimatedCommitMs,
      estimatedClearMs,
      confirmMs: 0,
      ok: false,
      sense,
      inFlightPeak: inFlight.length,
      lowConfidence: sense.lowConfidence,
    });

    nextScheduledDetectMs += params.entrySpacingMs;
  }

  t = Math.max(t, routeProtectedUntilMs + params.timedConfirmWindowMs);
  confirmReady(t + params.handoffWindowMs);
  updateHeat(t);

  const countsMatch = bins.every((count, i) => count === EXPECTED_BINS[i]);
  if (!halt && countsMatch && brickLog.length === params.totalBricks) {
    logEvent(t, 'RUN_COMPLETE', `counts=[${bins.join(',')}]`);
    if (params.logMode === 'csv') {
      csv.push(csvRow(t, 'run_complete', params.totalBricks, 'COMPLETE', '', 0, 0, 0, 0, '1', '', `total_ms=${Math.round(t)} counts=[${bins.join(',')}]`, heat));
    } else {
      serial.push('');
      serial.push('=== RUN COMPLETE ================================================');
      serial.push(`total_ms: ${Math.round(t)}`);
      serial.push(`bin1: ${bins[0]}/${EXPECTED_BINS[0]}`);
      serial.push(`bin2: ${bins[1]}/${EXPECTED_BINS[1]}`);
      serial.push(`bin3: ${bins[2]}/${EXPECTED_BINS[2]}`);
      serial.push(`bin4: ${bins[3]}/${EXPECTED_BINS[3]}`);
      serial.push(`count_check: ${countsMatch ? 'PASS' : 'FAIL'}`);
      serial.push('===============================================================');
    }
  }

  const correct = brickLog.filter((b) => b.actualBin === b.targetBin && b.actualBin > 0).length;
  const confirmed = bins.reduce((sum, n) => sum + n, 0);
  return {
    params,
    sequence,
    events: events.sort((a, b) => a.t - b.t),
    brickLog,
    bins,
    expectedBins: EXPECTED_BINS.slice(),
    totalMs: t,
    confirmed,
    correct,
    accuracy: confirmed > 0 ? correct / confirmed * 100 : 0,
    countsMatch,
    halt,
    peakHeat,
    thermalState: peakHeat >= params.thermalDangerLevel ? 'DANGER' : peakHeat >= params.thermalWarnLevel ? 'WARNING' : 'NORMAL',
    maxInFlight: Math.max(0, ...brickLog.map((b) => b.inFlightPeak || 0)),
    routeRecoveries,
    routeReadyMs,
    speedMode,
    serial: params.logMode === 'csv' ? csv : serial,
    csv,
  };
}

export function formatHarnessStatus(result) {
  const p = result.params;
  return `[status] state=${result.halt ? 'ERROR_HALT' : result.countsMatch ? 'COMPLETE' : 'FEED'} confirmed=${result.confirmed}/${p.totalBricks} issued=${result.brickLog.length} in_flight=0 bins=[${result.bins.join(',')}] token=yes thermal=${result.thermalState} speedmode=${p.speedMode} fast_sps=${result.speedMode.fastSps} slow_sps=${result.speedMode.slowSps}`;
}

export function handleHarnessCommand(command, params, lastResult) {
  const line = command.trim();
  if (line === 'status') return formatHarnessStatus(lastResult || computeSimulation(params));
  if (line === 'speedmode') {
    const mode = SPEED_MODES[params.speedMode] || SPEED_MODES.balanced;
    return `[harness] speedmode=${params.speedMode} fast_sps=${mode.fastSps} slow_sps=${mode.slowSps}`;
  }
  if (line.startsWith('speedmode ')) {
    const mode = line.slice('speedmode '.length);
    if (!SPEED_MODES[mode]) return '[harness] usage: speedmode [balanced|aggressive|accuracy]';
    params.speedMode = mode;
    const s = SPEED_MODES[mode];
    return `[harness] speedmode=${mode} fast_sps=${s.fastSps} slow_sps=${s.slowSps}`;
  }
  if (line === 'test fullrun') {
    const result = computeSimulation(params);
    return result.halt ? `[harness] fullrun aborted at brick ${result.halt.brickNumber}` : '[harness] fullrun completed';
  }
  if (line.startsWith('test route ')) {
    const bin = Number(line.slice('test route '.length));
    if (bin < 1 || bin > 4) return '[harness] usage: test route <1-4>';
    return `[harness] route ok in ${selectorRouteReadyMs(params)}ms angle=${SERVO_ANGLES[bin]}`;
  }
  if (line === 'cal show' || line === 'cal') {
    return `cal: color=${params.colorThreshold.toFixed(4)} size=${params.sizeThresholdMm.toFixed(2)}mm spacing=${params.beamSpacingMm.toFixed(2)}mm\ncal: belt ${params.beltSubtraction ? 'baseline enabled' : 'baseline disabled'}`;
  }
  if (line === 'log csv') {
    params.logMode = 'csv';
    return 'event,ts_ms,brick,state,category,target_bin,actual_bin,servo_angle,samples,ok,error,heat,message';
  }
  if (line === 'log human') {
    params.logMode = 'human';
    return '[logger] switched to HUMAN mode';
  }
  if (line === 'help') {
    return [
      'status',
      'test fullrun',
      'test route <1-4>',
      'speedmode [balanced|aggressive|accuracy]',
      'cal show',
      'log csv',
      'log human',
    ].join('\n');
  }
  return `[harness] unknown command: ${line}`;
}
