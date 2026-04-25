# Printing

Local print exports for the states build.

This directory is organized for one question first: what should be printed now versus later. The `.3mf`, `.f3d`, and related CAD exports here stay gitignored until the geometry is stable enough to publish.

Read [SPRINT.log](/Users/joeroche/Developer/tsa-sct-26/SPRINT.log) for the live weekend queue. Use this README for stable folder rules so the directory stays understandable after the sprint changes.

## Folder Layout

```text
cad/printing/
  active/          current exports that are valid for immediate or near-term printing
  deferred/        valid exports that should wait for a measurement or geometry lock
  staging/         USB and slicer batching workspace
  archive/         superseded exports kept only for rollback or reference
```

### `active/`

Use this for parts that are part of the current states architecture and are either already proven or expected to be printed in the current sprint.

Current buckets:
- `active/conveyor/`: rollers, bearing holders, spacers, side rails, motor board, and drill or alignment jigs
- `active/sensing/`: sensing shroud and related sensing-station print exports
- `active/selector/`: active servo rotary chute selector exports
- `active/electronics/`: current electronics mounting exports that are still part of the build path

### `deferred/`

Use this for parts that are still part of the active architecture but should not be printed yet because a dependent measurement is not locked.

Typical examples:
- feet that depend on final conveyor height
- bins that depend on tested chute sweep and catch geometry
- electronics or selector mounting variants waiting on packaging confirmation

Current buckets:
- `deferred/bins/`
- `deferred/conveyor/`
- `deferred/electronics/`
- `deferred/selector/`

### `staging/`

Temporary print-prep workspace. This is for copies prepared for USB transfer, slicer import, or plate batching. Do not treat `staging/` as source-of-truth design history.

Current buckets:
- `staging/usb_input/`: single-part exports copied over for handoff to Cura or external media
- `staging/plates/`: per-plate batches such as `plate1_spacers_jig`

### `archive/`

Superseded exports only. If a part is no longer part of the active states build, or if a newer version clearly replaces it, move it here instead of leaving it mixed with current parts.

Archive rules:
- Keep older variants only when they help rollback or explain a design pivot.
- Do not leave active printing candidates in `archive/`.
- If a file is archived because the architecture changed, prefer documenting that decision in the tracked CAD docs instead of only encoding it in filenames.

## Current Sprint Intent

As of April 25, 2026:

Print now or soon:
- conveyor hardware parts and spacers needed for the weekend conveyor build
- `outside_motor_board_v7`
- `sensing_shroud`
- drill or alignment jig exports
- current electronics board and foot exports for print queue prep

Defer:
- `conveyor_foot` until final dry-fit height is known
- bin parts until chute sweep and catch geometry are verified
- servo feet until chute alignment is known

Already printed or validated parts can stay under `active/` if they are still the current design.

## Naming Rules

- Keep filenames descriptive and versioned when there is a real revision, such as `outside_motor_board_v7.3mf`.
- Avoid spaces in new folder names.
- Name jigs by function, not by vague alignment language.
- If a split export is temporary, keep that fact obvious in the name or park it under `staging/`.

## What To Commit

Commit the structure and documentation only:
- `README.md`
- directory placeholders such as `.gitkeep`

Do not commit:
- `.3mf`, `.stl`, `.f3d`, and other unstable print exports during active iteration
- slicer staging copies that only exist to fill a USB or printer queue
