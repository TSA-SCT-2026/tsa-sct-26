// tabs.js - tab switching, timeline log, per-brick table

'use strict';

export function switchTab(tab) {
  document.querySelectorAll('.tab').forEach((el, i) => {
    const ids = ['timeline', 'bricks', 'warnings'];
    el.classList.toggle('active', ids[i] === tab);
  });
  document.querySelectorAll('.tab-panel').forEach(el => el.classList.remove('active'));
  const panel = document.getElementById('panel-' + tab);
  if (panel) panel.classList.add('active');
}

export function addEvent(t, type, detail) {
  const log = document.getElementById('timeline-log');
  if (!log) return;
  const row = document.createElement('div');
  row.className = 'event-row';
  row.innerHTML = `<span class="ev-t">${t}</span><span class="ev-type">${type}</span><span class="ev-detail">${detail}</span>`;
  log.appendChild(row);
  if (log.scrollTop + log.clientHeight >= log.scrollHeight - 40) {
    log.scrollTop = log.scrollHeight;
  }
}

export function populateBrickTable(brickLog) {
  const body = document.getElementById('brick-log-body');
  if (!body) return;
  const thermalColors = {NORMAL:'', WARNING:'y', DANGER:'r'};
  body.innerHTML = brickLog.map(b => {
    const tc = thermalColors[b.thermalState] || '';
    const typeLabel = b.type.replace('_', ' ').toUpperCase();
    const resultStr = b.correct === false
      ? `<span style="color:var(--red)">${b.errorCause || 'error'}</span>`
      : `<span style="color:var(--green)">OK</span>`;
    return `<tr${b.correct === false ? ' style="background:rgba(248,113,113,0.07)"' : ''}>
      <td>${b.num}</td>
      <td>${typeLabel}</td>
      <td>${b.sizeResult}</td>
      <td>${b.colorResult}</td>
      <td><span style="color:${b.samples < 4 ? 'var(--red)' : b.samples < 8 ? 'var(--yellow)' : 'var(--green)'}">${b.samples}</span></td>
      <td>${b.plow === 0 ? 'none' : b.plow}</td>
      <td>${b.binIdx + 1}</td>
      <td>${b.transitMs}</td>
      <td><span style="color:${tc === 'r' ? 'var(--red)' : tc === 'y' ? 'var(--yellow)' : 'var(--green)'}">${b.thermalState}</span></td>
      <td>${resultStr}</td>
    </tr>`;
  }).join('');
}

export function updateWarningsUI(warnings) {
  const container = document.getElementById('warnings-content');
  if (!container) return;
  container.innerHTML = warnings.map(w =>
    `<div class="warn-row ${w.level}">${w.msg}</div>`
  ).join('');
}
