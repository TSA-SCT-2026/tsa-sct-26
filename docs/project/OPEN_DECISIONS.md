# Open Decisions

Use this file for unresolved design choices that should not be treated as active architecture yet.

Status rule:
- `OPEN`: still undecided
- `DECIDED`: resolved and ready to move into the main architecture docs
- `DROPPED`: no longer worth pursuing

## Active open items

| Topic | Status | Current options | Current guidance |
|------|--------|-----------------|------------------|
| Timing-stage tooth count after baseline runtime | OPEN | `20T/20T` baseline first, `16T/20T` later only if real data justifies more timing-stage work | Start runtime testing from `20T/20T`. Do not freeze a ratio change into core architecture before logs show that conveyor refill still limits cycle time after release and selector optimization |
| Final conveyor target speed | OPEN | Keep baseline speed, raise speed within `20T/20T`, or try a later ratio change if runtime data still points at the conveyor | Derive this from measured `reset_confirm` and selector/chute-clear data. Do not optimize from bin-arrival time alone |
| Selector overlap gate | OPEN | Allow selector move after chute clear, require full bin confirm, or use another safety truth | Prefer the earliest physical truth that still prevents selector-path interference. Keep this open until drop-path data exists |

## Frozen recently

| Topic | Status | Decision |
|------|--------|----------|
| Start gate | DECIDED | Active subsystem. Side-sweep servo gate above the transition, not in the one-brick throat |
| Release actuator family | DECIDED | `0530`-class solenoid driving a retracting support |
| Conveyor strip width | DECIDED | Freeze to `25mm` active strip width. Treat `15mm` stock as spare only unless testing proves a real need to reopen it |
| Low-friction liner material | DECIDED | Freeze to adhesive-backed UHMW tape. PTFE thread-seal tape is not a production wear surface |
| Conveyor belt bed | DECIDED | Freeze to an integrated printed flat support path. Do not add a separate metal bed in the first CAD pass |
| Selector home truth | DECIDED | Required selector home micro-switch |
| Chamber seat truth | DECIDED | Required stop-wall micro-switch for chamber seat truth |
| Release reset switch | DECIDED | No dedicated release-reset switch in the initial build, but reserve mounting provision and use physical support-return truth before production freeze |
