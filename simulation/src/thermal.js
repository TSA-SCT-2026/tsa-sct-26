// thermal.js - thermal model helpers

'use strict';

export function decayHeat(heat, elapsedSec, rate) {
  return heat * Math.pow(rate, elapsedSec);
}

// cooling_pct: 0 = no cooling (rate 1.0), 100 = instant (rate 0.0), 50 = halves/sec (rate 0.5)
export function coolingPctToRate(pct) {
  return (100 - pct) / 100;
}

// Returns misfire probability [0..1] for a solenoid at given heat level.
// 0 below warn, ramps to ~10% at danger, ~40% at heat=1.0
export function misfireProbability(heat, warnLevel, dangerLevel) {
  if (heat <= warnLevel) return 0;
  if (heat <= dangerLevel) {
    return (heat - warnLevel) / (dangerLevel - warnLevel) * 0.10;
  }
  return 0.10 + (heat - dangerLevel) / (1.0 - dangerLevel) * 0.30;
}
