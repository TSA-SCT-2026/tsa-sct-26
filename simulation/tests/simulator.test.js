import assert from 'node:assert/strict';

import {
  CATEGORY_TO_BIN,
  DEFAULTS,
  EXPECTED_BINS,
  FULL_RUN_SEQUENCE,
  SPEED_MODES,
  cloneDefaults,
  classifyFromSense,
  computeSimulation,
  generateSequence,
  handleHarnessCommand,
  makeSenseResult,
  selectorRouteReadyMs,
} from '../src/model.js';

function sum(values) {
  return values.reduce((total, value) => total + value, 0);
}

function countByCategory(sequence) {
  const counts = {};
  for (const category of sequence) counts[category] = (counts[category] || 0) + 1;
  return counts;
}

{
  const params = cloneDefaults();
  const seq = generateSequence(params);
  assert.equal(seq.length, 24);
  assert.deepEqual(seq, FULL_RUN_SEQUENCE);
  assert.deepEqual(countByCategory(seq), {
    '2x2_RED': 6,
    '2x2_BLUE': 6,
    '2x3_BLUE': 8,
    '2x3_RED': 4,
  });
}

{
  const params = cloneDefaults();
  const red = makeSenseResult('2x2_RED', 1, 0, params);
  const blue = makeSenseResult('2x3_BLUE', 2, 0, params);
  assert.equal(classifyFromSense(red, params), '2x2_RED');
  assert.equal(classifyFromSense(blue, params), '2x3_BLUE');
  assert.ok(red.beam.bEnterMs > red.beam.aEnterMs);
  assert.ok(red.beam.bExitMs > red.beam.bEnterMs);
  assert.ok(red.beltSpeedAvgMms > 0);
}

{
  const params = cloneDefaults();
  params.beltSubtraction = true;
  const sense = makeSenseResult('2x3_RED', 3, 100, params);
  assert.ok(sense.rawRedHz > sense.redNetHz);
  assert.ok(sense.rawBlueHz > sense.blueNetHz);
  assert.ok(sense.redRatio >= params.colorThreshold);
}

{
  const params = cloneDefaults();
  params.beamBToServoMm = 140;
  params.entrySpacingMs = 620;
  const result = computeSimulation(params);
  assert.equal(result.halt, null);
  assert.equal(result.confirmed, params.totalBricks);
  assert.deepEqual(result.bins, EXPECTED_BINS);
  assert.equal(result.countsMatch, true);
  assert.equal(Math.round(result.accuracy), 100);
  assert.ok(result.maxInFlight >= 1);
}

{
  const params = cloneDefaults();
  params.entrySpacingMs = 260;
  params.beamBToServoMm = 140;
  const result = computeSimulation(params);
  assert.equal(result.halt, null);
  assert.ok(result.maxInFlight > 1);
}

{
  const params = cloneDefaults();
  params.beamBToServoMm = 50;
  params.entrySpacingMs = 180;
  const result = computeSimulation(params);
  assert.ok(result.halt);
  assert.equal(result.halt.code, 'POSITION_DRIFT');
}

{
  const params = cloneDefaults();
  params.logMode = 'csv';
  params.beamBToServoMm = 140;
  const result = computeSimulation(params);
  assert.ok(result.csv[0].startsWith('event,ts_ms,brick'));
  assert.ok(result.csv.some((line) => line.startsWith('run_complete,')));
}

{
  const params = cloneDefaults();
  let output = handleHarnessCommand('speedmode aggressive', params, null);
  assert.ok(output.includes('aggressive'));
  assert.equal(params.speedMode, 'aggressive');
  output = handleHarnessCommand('speedmode', params, null);
  assert.ok(output.includes(String(SPEED_MODES.aggressive.fastSps)));
  output = handleHarnessCommand('test route 4', params, null);
  assert.ok(output.includes(String(selectorRouteReadyMs(params))));
  output = handleHarnessCommand('status', params, computeSimulation({ ...params, beamBToServoMm: 140 }));
  assert.ok(output.includes('confirmed=24/24'));
}

{
  assert.equal(CATEGORY_TO_BIN['2x2_RED'], 1);
  assert.equal(CATEGORY_TO_BIN['2x2_BLUE'], 4);
  assert.equal(CATEGORY_TO_BIN['2x3_BLUE'], 3);
  assert.equal(CATEGORY_TO_BIN['2x3_RED'], 2);
  assert.equal(sum(EXPECTED_BINS), DEFAULTS.totalBricks);
}

console.log('simulation tests passed');
