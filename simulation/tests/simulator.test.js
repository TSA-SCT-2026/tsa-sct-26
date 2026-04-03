import assert from 'node:assert/strict';

import { DEFAULTS } from '../src/defaults.js';
import { coolingPctToRate, decayHeat, misfireProbability } from '../src/thermal.js';
import { computeSimulation, generateSequence } from '../src/simulation.js';

function cloneDefaults() {
  return JSON.parse(JSON.stringify(DEFAULTS));
}

function countTypes(seq) {
  const counts = { '2x2_red': 0, '2x2_blue': 0, '2x3_blue': 0, '2x3_red': 0 };
  for (const t of seq) counts[t] = (counts[t] || 0) + 1;
  return counts;
}

function hasEvent(sd, type) {
  return sd.events.some((e) => e.type === type);
}

// Thermal utilities
assert.equal(coolingPctToRate(0), 1.0);
assert.equal(coolingPctToRate(50), 0.5);
assert.equal(coolingPctToRate(100), 0.0);
assert.ok(Math.abs(decayHeat(1.0, 1, 0.5) - 0.5) < 0.0001);

// Sequence generation
{
  const p = cloneDefaults();
  p.sequence = 'random';
  const seq = generateSequence(p);
  assert.equal(seq.length, p.total_bricks);
  assert.deepEqual(countTypes(seq), p.counts);
}

{
  const p = cloneDefaults();
  p.sequence = 'worst_case_accuracy';
  const seq = generateSequence(p);
  assert.ok(seq.length > 0);
  assert.notEqual(seq[0], '2x3_red');
}

// Core simulation checks
{
  const p = cloneDefaults();
  p.sequence = 'interleaved';
  const sd = computeSimulation(p);
  assert.equal(sd.brickLog.length, p.total_bricks);
  assert.equal(sd.totalHalts, 0);
  assert.equal(sd.accuracy, 100);
  assert.ok(hasEvent(sd, 'RUN_COMPLETE'));
}

{
  const p = cloneDefaults();
  p.sequence = 'worst_case';
  const sd = computeSimulation(p);
  assert.equal(sd.totalHalts, 0);
  assert.equal(sd.accuracy, 100);
}

{
  const p = cloneDefaults();
  p.sequence = 'interleaved';
  p.num_runs = 2;
  p.interrun_gap_ms = 2000;
  const sd = computeSimulation(p);
  assert.equal(sd.brickLog.length, p.total_bricks * p.num_runs);
  assert.ok(hasEvent(sd, 'THERMAL_UPDATE'));
}

{
  const p = cloneDefaults();
  p.sequence = 'random';
  p.num_runs = 100;
  p.interrun_gap_ms = 0;
  const sd = computeSimulation(p);
  assert.equal(sd.brickLog.length, p.total_bricks * p.num_runs);
}

// Thermal misfire utility behavior
{
  const p = cloneDefaults();
  const below = misfireProbability(p.thermal_warn_level - 0.01, p.thermal_warn_level, p.thermal_danger_level);
  const aboveWarn = misfireProbability(p.thermal_warn_level + 0.01, p.thermal_warn_level, p.thermal_danger_level);
  const aboveDanger = misfireProbability(p.thermal_danger_level + 0.05, p.thermal_warn_level, p.thermal_danger_level);
  assert.equal(below, 0);
  assert.ok(aboveWarn > 0);
  assert.ok(aboveDanger > 0.10);
}

// Faster indexing should reduce runtime
{
  const slow = cloneDefaults();
  slow.stepper_run_sps = 400;
  const fast = cloneDefaults();
  fast.stepper_run_sps = 1600;
  const a = computeSimulation(slow);
  const b = computeSimulation(fast);
  assert.ok(b.totalSimMs < a.totalSimMs);
}

console.log('simulation tests passed');
