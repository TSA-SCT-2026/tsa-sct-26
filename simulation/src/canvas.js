// canvas.js - all canvas drawing functions and layout constants

'use strict';

import { state } from './animation.js';

// ---- CANVAS LAYOUT CONSTANTS ----
export const CY = 120;    // belt center Y
export const BT = 100;    // belt top Y
export const BB = 140;    // belt bottom Y

// X positions as fractions [0..1] of canvas width
export const FX = {
  chute:       0.03,
  taperStart:  0.07,
  taperEnd:    0.17,
  beam1:       0.24,
  beam2:       0.28,
  color:       0.26,
  bufferEnd:   0.35,
  plow1:       0.42,
  plow2:       0.57,
  plow3:       0.72,
  end:         0.90,
};

// Bin Y positions (where bricks exit to)
export const BIN_Y = [30, 210, 30, CY];

// Which plows go up vs down
export const PLOW_DIR = [null, 'up', 'down', 'up'];

// Plow X fractions: plow 1,2,3
export const PLOW_FX = [null, FX.plow1, FX.plow2, FX.plow3];

// Map brick type to bin index (0-based)
export const BRICK_TO_BIN = {'2x2_blue': 0, '2x2_red': 1, '2x3_red': 2, '2x3_blue': 3};

// Brick colors
export const BRICK_COLOR = {'2x2_blue': '#185FA5', '2x2_red': '#E24B4A', '2x3_red': '#E24B4A', '2x3_blue': '#185FA5'};

// ---- DRAWING FUNCTIONS ----

export function drawLabel(ctx, cx, cy, text, size, color) {
  ctx.fillStyle = color;
  ctx.font = `${size}px system-ui`;
  ctx.textAlign = 'center';
  ctx.fillText(text, cx, cy);
}

export function drawHatched(ctx, x, y, w, h, bg, line) {
  ctx.fillStyle = bg;
  ctx.fillRect(x, y, w, h);
  ctx.save();
  ctx.beginPath(); ctx.rect(x, y, w, h); ctx.clip();
  ctx.strokeStyle = line;
  ctx.lineWidth = 1;
  for (let i = -h; i < w + h; i += 8) {
    ctx.beginPath();
    ctx.moveTo(x + i, y);
    ctx.lineTo(x + i + h, y + h);
    ctx.stroke();
  }
  ctx.restore();
  ctx.strokeStyle = '#555';
  ctx.lineWidth = 1;
  ctx.strokeRect(x, y, w, h);
}

export function drawBinBox(ctx, bx, by, bw, bh, label, binIdx, count, numRuns) {
  const BASE_EXPECTED = [6, 6, 4, 8];
  const runs = numRuns || 1;
  const exp = BASE_EXPECTED[binIdx] * runs;
  const isMatch = count === exp && !state.animRunning;
  ctx.fillStyle = isMatch ? 'rgba(74,222,128,0.1)' : 'rgba(50,50,60,0.8)';
  ctx.strokeStyle = isMatch ? '#4ade80' : '#555';
  ctx.lineWidth = 1;
  ctx.beginPath();
  ctx.roundRect(bx, by, bw, bh, 3);
  ctx.fill();
  ctx.stroke();
  ctx.fillStyle = '#9ca3af';
  ctx.font = '7px system-ui';
  ctx.textAlign = 'center';
  ctx.fillText(label, bx + bw / 2, by + 10);
  ctx.fillStyle = '#f9fafb';
  ctx.font = 'bold 12px monospace';
  ctx.fillText(count, bx + bw / 2, by + bh - 7);
}

export function drawPlow(ctx, plowNum, W, H, simMs, liveBinCounts, plowStates, params, numRuns) {
  const ps = plowStates[plowNum];
  if (!ps) return;
  const bx = W * PLOW_FX[plowNum];
  const angle = ps.angle;
  const dir = PLOW_DIR[plowNum];

  let armColor = '#555';
  if (ps.phase === 'fire') armColor = '#fb923c';
  else if (ps.phase === 'hold') armColor = '#fbbf24';
  else if (ps.phase === 'retract') armColor = '#888';

  const rad = angle * Math.PI / 180;
  const armLength = (BB - BT) * 0.85;

  ctx.save();
  if (dir === 'up') {
    ctx.translate(bx, BB);
    ctx.rotate(-rad);
    ctx.strokeStyle = armColor;
    ctx.lineWidth = 3;
    ctx.beginPath(); ctx.moveTo(0, 0); ctx.lineTo(-armLength * Math.sin(rad * 0.3 + 0.1), -armLength); ctx.stroke();
    ctx.fillStyle = armColor;
    ctx.beginPath(); ctx.arc(-armLength * Math.sin(rad * 0.3 + 0.1), -armLength, 3, 0, Math.PI * 2); ctx.fill();
  } else {
    ctx.translate(bx, BT);
    ctx.rotate(rad);
    ctx.strokeStyle = armColor;
    ctx.lineWidth = 3;
    ctx.beginPath(); ctx.moveTo(0, 0); ctx.lineTo(-armLength * Math.sin(rad * 0.3 + 0.1), armLength); ctx.stroke();
    ctx.fillStyle = armColor;
    ctx.beginPath(); ctx.arc(-armLength * Math.sin(rad * 0.3 + 0.1), armLength, 3, 0, Math.PI * 2); ctx.fill();
  }
  ctx.restore();

  const binIdx = plowNum - 1;
  const binLabels = ['2x2 BLUE', '2x2 RED', '2x3 RED'];
  const byOffset = dir === 'up' ? BT - 50 : BB + 14;
  const boxH = 30;
  drawBinBox(ctx, bx - 30, byOffset - (dir === 'up' ? boxH : 0), 60, boxH, binLabels[binIdx], binIdx, liveBinCounts[binIdx], numRuns);

  const lbl = ['P1', 'P2', 'P3'][plowNum - 1];
  drawLabel(ctx, bx, dir === 'up' ? BB + 10 : BT - 10, lbl, 8, armColor === '#555' ? '#555' : armColor);
}

export function drawAnimBrick(ctx, b, simMs, beltPxPerMs, W, H, params) {
  const elapsed = simMs - b.spawnSim;
  if (elapsed < 0) return;

  const pxPerMm = W * (FX.end - FX.chute) / 500;
  const beltMm = elapsed * params.belt_target_mm_s / 1000;
  const chuteX = W * FX.chute;

  const brickW = b.is2x3 ? 15 : 10;
  const brickH = 10;

  const plowMmPositions = [null, 210, 285, 360];
  const endMm = 480;

  const plowMm = b.plow > 0 ? plowMmPositions[b.plow] : endMm;
  const deflectDist = 55;

  if (beltMm > plowMm + deflectDist) {
    b.done = true;
    return;
  }

  let drawX, drawY;

  if (beltMm < plowMm) {
    drawX = chuteX + beltMm * pxPerMm;
    drawY = CY;

    const beam1Mm = 116;
    if (b.flashBorder > 0 && beltMm > beam1Mm) {
      ctx.strokeStyle = '#ffffff';
      ctx.lineWidth = 2;
      ctx.strokeRect(drawX - brickW / 2 - 1, drawY - brickH / 2 - 1, brickW + 2, brickH + 2);
    }
  } else {
    const t = Math.min((beltMm - plowMm) / deflectDist, 1.0);
    drawX = chuteX + (plowMm + t * 15) * pxPerMm;
    const targetY = BIN_Y[b.binIdx];
    drawY = CY + (targetY - CY) * t;
    ctx.globalAlpha = 1.0 - t * 0.5;
  }

  ctx.fillStyle = b.color;
  ctx.fillRect(drawX - brickW / 2, drawY - brickH / 2, brickW, brickH);

  ctx.fillStyle = 'rgba(255,255,255,0.25)';
  if (b.is2x3) {
    ctx.beginPath(); ctx.arc(drawX - 4, drawY, 1.5, 0, Math.PI * 2); ctx.fill();
    ctx.beginPath(); ctx.arc(drawX, drawY, 1.5, 0, Math.PI * 2); ctx.fill();
    ctx.beginPath(); ctx.arc(drawX + 4, drawY, 1.5, 0, Math.PI * 2); ctx.fill();
  } else {
    ctx.beginPath(); ctx.arc(drawX - 2, drawY, 1.5, 0, Math.PI * 2); ctx.fill();
    ctx.beginPath(); ctx.arc(drawX + 2, drawY, 1.5, 0, Math.PI * 2); ctx.fill();
  }

  ctx.globalAlpha = 1.0;
}

export function drawErrorHaltOverlay(ctx, W, H, msg) {
  // Semi-transparent dark overlay
  ctx.fillStyle = 'rgba(0,0,0,0.65)';
  ctx.fillRect(0, 0, W, H);

  // Red banner
  const bannerH = 60;
  const bannerY = H / 2 - bannerH / 2;
  ctx.fillStyle = 'rgba(220,38,38,0.92)';
  ctx.fillRect(0, bannerY, W, bannerH);

  ctx.strokeStyle = '#f87171';
  ctx.lineWidth = 2;
  ctx.strokeRect(0, bannerY, W, bannerH);

  ctx.fillStyle = '#fff';
  ctx.font = 'bold 16px system-ui';
  ctx.textAlign = 'center';
  ctx.fillText('ERROR_HALT', W / 2, bannerY + 22);

  ctx.font = '11px monospace';
  ctx.fillText(msg, W / 2, bannerY + 44);
}

export function drawBelt(ctx, W, H, simMs, params, liveBinCounts, plowStates, animBricks, replayMode) {
  const p = params;
  const pxPerMm = W * (FX.end - FX.chute) / 500;
  const beltPxPerMs = p.belt_target_mm_s * pxPerMm / 1000;
  const x = f => W * f;

  ctx.fillStyle = '#141414';
  ctx.fillRect(0, 0, W, H);

  ctx.fillStyle = '#222';
  ctx.fillRect(x(FX.chute), BT, x(FX.end) - x(FX.chute), BB - BT);

  ctx.strokeStyle = '#555';
  ctx.lineWidth = 1.5;
  ctx.beginPath(); ctx.moveTo(x(FX.chute), BT); ctx.lineTo(x(FX.end), BT); ctx.stroke();
  ctx.beginPath(); ctx.moveTo(x(FX.chute), BB); ctx.lineTo(x(FX.end), BB); ctx.stroke();

  ctx.strokeStyle = '#2a2a2a';
  ctx.lineWidth = 1;
  const ridgeSpacing = 20;
  const ridgeOffset = (simMs * beltPxPerMs) % ridgeSpacing;
  for (let rx = x(FX.chute) + ridgeOffset - ridgeSpacing; rx < x(FX.end); rx += ridgeSpacing) {
    if (rx < x(FX.chute)) continue;
    ctx.beginPath(); ctx.moveTo(rx, BT + 2); ctx.lineTo(rx, BB - 2); ctx.stroke();
  }

  ctx.strokeStyle = '#666';
  ctx.lineWidth = 2;
  const taperY1 = BT - 18, taperY2 = BT;
  ctx.beginPath();
  ctx.moveTo(x(FX.taperStart), taperY1);
  ctx.lineTo(x(FX.taperEnd), taperY2);
  ctx.stroke();
  ctx.beginPath();
  ctx.moveTo(x(FX.taperStart), BB + 18);
  ctx.lineTo(x(FX.taperEnd), BB);
  ctx.stroke();

  drawLabel(ctx, x(FX.taperStart) + (x(FX.taperEnd) - x(FX.taperStart)) / 2, BT - 24, 'TAPER', 9, '#666');

  ctx.strokeStyle = '#555';
  ctx.lineWidth = 1;
  const chuteW = x(FX.taperStart) - x(FX.chute);
  const chuteH = 60;
  drawHatched(ctx, x(FX.chute), CY - chuteH / 2, chuteW, chuteH, '#333', '#444');
  drawLabel(ctx, x(FX.chute) + chuteW / 2, CY + 28, 'CHUTE', 8, '#888');

  const senseX = x(FX.beam1) - 4;
  const senseW = x(FX.beam2) + 4 - senseX;
  ctx.fillStyle = 'rgba(26,80,120,0.25)';
  ctx.fillRect(senseX, BT, senseW, BB - BT);
  ctx.strokeStyle = '#1a6080';
  ctx.lineWidth = 1;
  ctx.strokeRect(senseX, BT, senseW, BB - BT);

  ctx.strokeStyle = '#ff6666';
  ctx.lineWidth = 1.5;
  ctx.setLineDash([3, 2]);
  ctx.beginPath(); ctx.moveTo(x(FX.beam1), BT - 8); ctx.lineTo(x(FX.beam1), BB + 8); ctx.stroke();
  ctx.setLineDash([]);
  drawLabel(ctx, x(FX.beam1), BT - 14, 'B1', 8, '#ff6666');

  ctx.strokeStyle = '#ff9944';
  ctx.lineWidth = 1.5;
  ctx.setLineDash([3, 2]);
  ctx.beginPath(); ctx.moveTo(x(FX.beam2), BT - 8); ctx.lineTo(x(FX.beam2), BB + 8); ctx.stroke();
  ctx.setLineDash([]);
  drawLabel(ctx, x(FX.beam2), BT - 14, 'B2', 8, '#ff9944');

  const gapPx = x(FX.beam2) - x(FX.beam1);
  ctx.strokeStyle = '#888';
  ctx.lineWidth = 1;
  ctx.beginPath(); ctx.moveTo(x(FX.beam1), BB + 14); ctx.lineTo(x(FX.beam2), BB + 14); ctx.stroke();
  drawLabel(ctx, x(FX.beam1) + gapPx / 2, BB + 22, `${p.beam_gap_mm}mm`, 8, '#888');

  ctx.fillStyle = '#4fc3f7';
  ctx.beginPath(); ctx.arc(x(FX.color), BT - 10, 5, 0, Math.PI * 2); ctx.fill();
  drawLabel(ctx, x(FX.color), BT - 22, 'COLOR', 7, '#4fc3f7');

  drawLabel(ctx, senseX + senseW / 2, CY, 'SIZE + COLOR', 8, '#4fc3f7');

  ctx.strokeStyle = '#444';
  ctx.lineWidth = 1;
  ctx.setLineDash([4, 3]);
  ctx.beginPath(); ctx.moveTo(x(FX.beam2), CY); ctx.lineTo(x(FX.plow1) - 2, CY); ctx.stroke();
  ctx.setLineDash([]);
  drawLabel(ctx, x(FX.beam2) + (x(FX.plow1) - x(FX.beam2)) / 2, CY - 14, `lead: ${p.sol_lead_ms}ms`, 8, '#555');

  const numRuns = p.num_runs || 1;
  drawPlow(ctx, 1, W, H, simMs, liveBinCounts, plowStates, p, numRuns);
  drawPlow(ctx, 2, W, H, simMs, liveBinCounts, plowStates, p, numRuns);
  drawPlow(ctx, 3, W, H, simMs, liveBinCounts, plowStates, p, numRuns);

  drawBinBox(ctx, x(FX.end) + 4, CY - 18, 50, 36, '2x3 BLUE', 3, liveBinCounts[3], numRuns);

  for (const b of animBricks) {
    drawAnimBrick(ctx, b, simMs, beltPxPerMs, W, H, p);
  }

  // REPLAY MODE label
  if (replayMode) {
    ctx.fillStyle = 'rgba(96,165,250,0.18)';
    ctx.fillRect(W - 100, 6, 94, 18);
    ctx.fillStyle = '#60a5fa';
    ctx.font = 'bold 10px system-ui';
    ctx.textAlign = 'center';
    ctx.fillText('REPLAY MODE', W - 53, 19);
  }
}
