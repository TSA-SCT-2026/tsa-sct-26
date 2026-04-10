// canvas.js
// Lightweight V6 visualization: chamber, release mechanism, selector, bins.

'use strict';

import { BIN_DIRECTIONS, BIN_LABELS, CATEGORY_COLOR } from './defaults.js';

export const BRICK_COLOR = { ...CATEGORY_COLOR };

const DEG = Math.PI / 180;

function drawText(ctx, txt, x, y, color = '#9ca3af', font = '11px system-ui', align = 'center') {
  ctx.fillStyle = color;
  ctx.font = font;
  ctx.textAlign = align;
  ctx.fillText(txt, x, y);
}

function chamberRect(W, H) {
  const cx = W * 0.26;
  const cy = H * 0.54;
  return { x: cx - 64, y: cy - 40, w: 128, h: 80 };
}

function drawChamber(ctx, W, H, scene) {
  const c = chamberRect(W, H);
  ctx.strokeStyle = '#6b7280';
  ctx.lineWidth = 2;
  ctx.strokeRect(c.x, c.y, c.w, c.h);

  const tofX = c.x + c.w;
  const tof1y = c.y + 20;
  const tof2y = c.y + c.h - 20;
  ctx.setLineDash([3, 2]);
  ctx.strokeStyle = '#ef4444';
  ctx.beginPath();
  ctx.moveTo(tofX, tof1y);
  ctx.lineTo(c.x + 14, tof1y);
  ctx.stroke();
  ctx.strokeStyle = '#fb923c';
  ctx.beginPath();
  ctx.moveTo(tofX, tof2y);
  ctx.lineTo(c.x + 14, tof2y);
  ctx.stroke();
  ctx.setLineDash([]);

  ctx.fillStyle = '#ef4444';
  ctx.fillRect(tofX - 4, tof1y - 6, 8, 12);
  ctx.fillStyle = '#fb923c';
  ctx.fillRect(tofX - 4, tof2y - 6, 8, 12);
  drawText(ctx, 'T1', tofX + 12, tof1y + 3, '#f87171', '10px monospace', 'left');
  drawText(ctx, 'T2', tofX + 12, tof2y + 3, '#fb923c', '10px monospace', 'left');

  const colorX = c.x + c.w + 18;
  const colorY = c.y + c.h * 0.55;
  ctx.fillStyle = '#60a5fa';
  ctx.fillRect(colorX - 8, colorY - 8, 16, 16);
  drawText(ctx, 'COLOR', colorX, colorY - 14, '#60a5fa', '9px monospace');

  // Release support
  const hingeX = c.x + 24;
  const platW = 86;
  const platY = c.y + c.h - 18;
  const tilt = scene.platformAngle || 0;
  ctx.save();
  ctx.translate(hingeX, platY);
  ctx.rotate(-tilt * DEG);
  ctx.fillStyle = '#9ca3af';
  ctx.fillRect(0, -4, platW, 8);
  ctx.fillStyle = '#4b5563';
  ctx.fillRect(platW - 6, -6, 6, 12);
  ctx.restore();

  ctx.fillStyle = '#4b5563';
  ctx.beginPath();
  ctx.arc(hingeX, platY, 4, 0, Math.PI * 2);
  ctx.fill();

  // Lever
  const leverBaseX = c.x + c.w + 4;
  const leverBaseY = platY;
  ctx.save();
  ctx.translate(leverBaseX, leverBaseY);
  ctx.rotate((scene.leverAngle || 0) * DEG);
  ctx.strokeStyle = '#94a3b8';
  ctx.lineWidth = 4;
  ctx.beginPath();
  ctx.moveTo(0, 0);
  ctx.lineTo(34, 0);
  ctx.stroke();
  ctx.restore();
  ctx.fillStyle = '#64748b';
  ctx.beginPath();
  ctx.arc(leverBaseX, leverBaseY, 3, 0, Math.PI * 2);
  ctx.fill();

  drawText(ctx, 'CHAMBER', c.x + c.w / 2, c.y - 24, '#9ca3af', '10px monospace');
}

function drawDiscAndBins(ctx, W, H, scene, binCounts, numRuns, expectedCounts) {
  const discCx = W * 0.54;
  const discCy = H * 0.58;
  const discR = 56;
  const rot = (scene.discAngleDeg || 225) * DEG;

  ctx.save();
  ctx.translate(discCx, discCy);
  ctx.rotate(rot);
  ctx.fillStyle = '#1f2937';
  ctx.beginPath();
  ctx.arc(0, 0, discR, 0, Math.PI * 2);
  ctx.fill();
  ctx.strokeStyle = '#475569';
  ctx.lineWidth = 2;
  ctx.stroke();

  for (let i = 0; i < 4; i++) {
    const a = (i * 90 + 45) * DEG;
    const sx = Math.cos(a) * 28;
    const sy = Math.sin(a) * 28;
    ctx.fillStyle = '#334155';
    ctx.fillRect(sx - 10, sy - 6, 20, 12);
  }
  ctx.restore();

  drawText(ctx, `Selector ${Math.round(scene.discAngleDeg || 225)}deg`, discCx, discCy + discR + 16, '#94a3b8', '10px monospace');

  for (let i = 0; i < 4; i++) {
    const deg = [315, 45, 135, 225][i];
    const ax = discCx + Math.cos(deg * DEG) * 118;
    const ay = discCy + Math.sin(deg * DEG) * 86;
    const bw = 88;
    const bh = 42;
    const cnt = binCounts[i] || 0;
    const exp = (expectedCounts[i] || 0) * (numRuns || 1);
    const good = cnt === exp && !scene.running;
    ctx.fillStyle = good ? 'rgba(34,197,94,0.08)' : 'rgba(30,41,59,0.75)';
    ctx.strokeStyle = good ? '#22c55e' : '#475569';
    ctx.lineWidth = 1;
    ctx.fillRect(ax - bw / 2, ay - bh / 2, bw, bh);
    ctx.strokeRect(ax - bw / 2, ay - bh / 2, bw, bh);
    drawText(ctx, BIN_LABELS[i], ax, ay - 8, '#cbd5e1', '9px monospace');
    drawText(ctx, `${cnt} / ${exp}`, ax, ay + 10, '#f8fafc', 'bold 11px monospace');
    drawText(ctx, BIN_DIRECTIONS[i], ax, ay + 22, '#64748b', '9px monospace');
  }
}

function drawBelts(ctx, W, H) {
  const y = H * 0.54;
  ctx.strokeStyle = '#374151';
  ctx.lineWidth = 14;
  ctx.beginPath();
  ctx.moveTo(W * 0.05, y);
  ctx.lineTo(W * 0.24, y);
  ctx.stroke();
}

function drawAnimationBrick(ctx, b, scene) {
  if (!b || b.done) return;
  const color = BRICK_COLOR[b.type] || '#94a3b8';
  const w = b.sizeResult === '2x3' ? 20 : 14;
  const h = 10;
  ctx.fillStyle = color;
  ctx.fillRect(b.x - w / 2, b.y - h / 2, w, h);
  if (scene.highlightBrick === b.id) {
    ctx.strokeStyle = '#f8fafc';
    ctx.lineWidth = 1;
    ctx.strokeRect(b.x - w / 2 - 1, b.y - h / 2 - 1, w + 2, h + 2);
  }
}

export function drawErrorHaltOverlay(ctx, W, H, msg) {
  ctx.fillStyle = 'rgba(0,0,0,0.65)';
  ctx.fillRect(0, 0, W, H);
  ctx.fillStyle = 'rgba(220,38,38,0.92)';
  ctx.fillRect(0, H / 2 - 32, W, 64);
  drawText(ctx, 'ERROR_HALT', W / 2, H / 2 - 6, '#fff', 'bold 16px system-ui');
  drawText(ctx, msg, W / 2, H / 2 + 16, '#fff', '11px monospace');
}

export function drawBelt(ctx, W, H, simMs, params, liveBinCounts, sceneState, animBricks, replayMode) {
  ctx.fillStyle = '#111827';
  ctx.fillRect(0, 0, W, H);
  drawBelts(ctx, W, H);
  drawChamber(ctx, W, H, sceneState || {});
  drawDiscAndBins(
    ctx,
    W,
    H,
    sceneState || {},
    liveBinCounts || [0, 0, 0, 0],
    params.num_runs || 1,
    [params.counts['2x2_red'], params.counts['2x2_blue'], params.counts['2x3_blue'], params.counts['2x3_red']],
  );

  for (const b of animBricks || []) drawAnimationBrick(ctx, b, sceneState || {});

  if (replayMode) {
    ctx.fillStyle = 'rgba(96,165,250,0.15)';
    ctx.fillRect(W - 106, 8, 98, 20);
    drawText(ctx, 'REPLAY MODE', W - 57, 22, '#60a5fa', 'bold 10px system-ui');
  }
}
