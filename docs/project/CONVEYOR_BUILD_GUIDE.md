# Conveyor Build Guide

This guide explains the active conveyor build path without preserving stale draft CAD coordinates.

## Purpose

The states conveyor needs to carry one manually placed LEGO brick through the sensing station and into the servo rotary chute selector. It should be simple, repairable, inspectable, and reliable before it is fast.

The tutorial-style conveyor remains the active mechanical reference. Treat the tutorial as build logic, not a dimension source.

## Current Dependency

The conveyor is downstream of the bin and chute geometry.

Order:
1. Design receiving bins and funnel catch zones.
2. Design the servo chute to hit those zones.
3. Set conveyor height and exit handoff from the chute entry.
4. Lock conveyor feet and woodworking.

## Source Rules

Use exact numbers only when they come from:
- Competition footprint.
- BOM row or exact purchased listing.
- Datasheet-backed standard part.
- Real measurement.
- Finalized CAD export after physical fit confirmation.

Do not use:
- Old session logs.
- Old Fusion coordinates.
- Failed shroud or guide rail instructions.
- Convenience dimensions copied from a previous layout.

## Active Conveyor Concept

The conveyor uses:
- NEMA17 motor.
- 5mm shaft and matching bearing path from the active hardware plan.
- Neoprene belt stock from the purchased material.
- Wood side or support pieces where they are faster and more repairable than printed structure.
- Printed brackets, bearing holders, guides, or coupons where they reduce risk.
- Serviceable motor and pulley access.

The conveyor support height is not locked. It depends on the final chute entry, chute exit, bin catch height, and footprint layout.

## Build Sequence

1. Dry-fit the belt, rollers, shafts, and side plates.
2. Confirm the belt tracks without rubbing.
3. Fit the wood support under the top belt run from the real assembly.
4. Keep the motor and pulley path accessible.
5. Add the sensing station only after brick clearance and belt tracking are believable.
6. Add exit handoff geometry after the bin and chute layout exists.
7. Lock feet, drill templates, and wood holes only after the handoff stack is known.

## Sensing Station Constraints

The sensing station sits near the feed end and includes:
- Two break-beam pairs through the shroud side walls.
- TCS3200/GY-31 color sensor in the shroud roof.
- Installed shroud calibration only.
- Removable mounting so fit can be corrected.

Derive shroud width, height, sensor pocket, and rail geometry from:
- real brick envelope,
- real belt path,
- real sensor modules,
- print tolerance,
- and physical clearance checks.

Do not reuse old shroud dimensions as final. The previous shroud and guide rail instructions failed physical fitting and should not drive the next version.

## Guide Rails

Guide rails are allowed, but they must be stable and physically mounted. Do not design floating rails.

Rail intent:
- Help the evaluator place one brick consistently.
- Center the brick enough for the sensing station.
- Avoid pinching or catching studs.
- Stay adjustable until tested with real bricks.

The rail gap should be derived from the brick width plus yaw tolerance and confirmed on the real conveyor.

## Exit Handoff

The exit handoff is derived from the chute entry.

Use:
- final belt surface,
- chute entry height,
- chute entry angle or funnel,
- brick momentum,
- and real-brick tests.

Use a small test piece first. Do not lock permanent wood or printed feet from old lip coordinates.

## Motor And Feet

The motor board, conveyor feet, and drill templates depend on final support height.

Do not print or drill fixed-height parts until:
- receiving bin geometry exists,
- chute entry height is known,
- belt exit and chute entry are checked together,
- and the full assembly remains inside 610mm x 610mm.

Slots and shims are acceptable for first fit. Final locked geometry should come after real alignment.

## Validation Checklist

- Belt runs smoothly without walking into side parts.
- Wood support does not rub the return path.
- Motor and pulleys can be inspected and adjusted.
- Brick travels through the sensing station without catching.
- Color shroud blocks ambient light in the installed position.
- Break-beam timing separates 2x2 from 2x3 at the selected belt speed.
- Handoff sends real bricks into the chute repeatably.
- Full system footprint remains inside 610mm x 610mm.
