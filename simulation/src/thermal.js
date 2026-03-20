// thermal.js - thermal model helpers

'use strict';

export function decayHeat(heat, elapsedSec, rate) {
  return heat * Math.pow(rate, elapsedSec);
}
