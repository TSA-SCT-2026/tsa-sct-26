// tabs.js

'use strict';

export function switchTab(tab) {
  const ids = ['timeline', 'bricks', 'warnings'];
  document.querySelectorAll('.tab').forEach((el, idx) => {
    el.classList.toggle('active', ids[idx] === tab);
  });
  document.querySelectorAll('.tab-panel').forEach((el) => el.classList.remove('active'));
  const panel = document.getElementById(`panel-${tab}`);
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
  if (!brickLog || brickLog.length === 0) {
    body.innerHTML = '<tr><td colspan="11" style="color:var(--text3);text-align:center;padding:12px">No data yet.</td></tr>';
    return;
  }

  body.innerHTML = brickLog.map((b) => {
    const ok = b.correct !== false;
    const rowStyle = ok ? '' : ' style="background:rgba(248,113,113,0.08)"';
    const tClass = b.thermalState === 'DANGER' ? 'var(--red)' : b.thermalState === 'WARNING' ? 'var(--yellow)' : 'var(--green)';
    const result = ok ? '<span style="color:var(--green)">OK</span>' : '<span style="color:var(--red)">FAIL</span>';
    return `<tr${rowStyle}>
      <td>${b.num}</td>
      <td>${b.run}</td>
      <td>${String(b.type).replace('_', ' ').toUpperCase()}</td>
      <td>${b.sizeResult}</td>
      <td>${b.colorResult}</td>
      <td>${b.samples}</td>
      <td>${b.targetBin}</td>
      <td>${b.actualBin}</td>
      <td>${b.indexMs}</td>
      <td><span style="color:${tClass}">${b.thermalState}</span></td>
      <td>${result}</td>
    </tr>`;
  }).join('');
}

export function updateWarningsUI(warnings) {
  const container = document.getElementById('warnings-content');
  if (!container) return;
  container.innerHTML = warnings.map((w) => `<div class="warn-row ${w.level}">${w.msg}</div>`).join('');
}
