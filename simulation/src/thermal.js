// thermal.js - thermal model helpers

'use strict';

export function decayHeat(heat, elapsedSec, rate) {
  return heat * Math.pow(rate, elapsedSec);
}

// coolingPctToRate converts the 0-100 integer cooling percent to a decay rate.
// 0% = no cooling (rate 1.0, heat never decays).
// 50% = heat halves every second (rate 0.5, matches old default thermal_decay_rate).
// 100% = instant cooling (rate 0.0, heat drops to zero immediately).
export function coolingPctToRate(pct) {
  return (100 - pct) / 100;
}
