# CAD

CAD for the simplified states sorter.

Active production architecture:
- Manual one-at-a-time feed
- NEMA17 conveyor
- Wood-frame-mounted sensing station near the conveyor start
- MG995/MG996-class servo rotary chute selector
- Four labeled bins
- Wood frame with printed brackets where useful

The previous chamber, release-gate, and NEMA11 selector CAD work is archived for later nationals work. Use it only as fallback reference.

## Directory Structure

```text
cad/
  DIMENSIONS.md    source-backed dimensions and active variable register
  MECHANICAL.md    mechanism rationale and build status
  printing/        local print exports organized by active, deferred, staging, and archive use
  frame/           conveyor and frame prior work or future active parts
  bins/            future bin models
  sensing/         future size and color sensor brackets
  selector/        future servo chute selector models
```

Existing directories may still contain prior work from the archived design until they are pruned or replaced. Do not treat old chamber or release files as active without checking `docs/ARCHITECTURE.md`.

## Current CAD Phase

CAD is the critical path again. The developer owns the full downstream-to-upstream stack:

1. Bins and funnel catch geometry.
2. Servo chute and mount geometry.
3. Conveyor height, exit handoff, and feet.
4. Permanent woodworking.

## CAD Priorities

1. Build a forgiving bin and funnel receiver.
2. Design the servo rotary chute so the exit lands inside those funnels with margin.
3. Derive conveyor height and exit handoff from the chute entry.
4. Check the full system inside the 610mm x 610mm boundary.
5. Reserve labels, display, start control, and cable routing keepouts.

## First Print Order

Print only parts that answer a physical question:

1. One bin or funnel catch coupon.
2. Short UHMW-lined chute coupon.
3. Servo pocket or horn adapter coupon if fit is uncertain.
4. Exit handoff coupon after chute entry is known.
5. Sensing shroud or shroud coupon after conveyor path is stable.

Avoid large frame prints until handoff, chute, and catch geometry are proven.

## Conventions

- Export STL files alongside source files.
- Use descriptive versioned names.
- Record critical dimension deviations in docs only when source-backed.
- Do not freeze geometry from CAD alone.
- Validate with real bricks before large print commitments.

## Tolerance Notes

FDM variance is real, but do not use generic tolerance numbers as a substitute for physical checks.

- Undersized chutes cause jams.
- Oversized chutes allow yaw and poor bin alignment.
- Sensor mount tolerance must preserve the color shroud and the selected size sensor geometry.
