# CAD Plan

Updated April 20, 2026 after the CAD ownership pivot.

CAD is the critical path again. The developer is taking over the full downstream-to-upstream mechanical chain:

1. Bins and funnel catch geometry.
2. Servo rotary chute selector.
3. Conveyor height, exit handoff, and feet.
4. Permanent woodworking.

The goal is not perfect CAD. The goal is to create printable physical truth fast enough to leave time for troubleshooting before May 2, 2026.

## CAD Truth Policy

Tracked docs should not store exact draft CAD geometry unless the number is source-backed.

Safe sources:
- Competition footprint.
- BOM rows or exact purchased listings.
- Datasheets for standard or exact-model parts.
- Real hardware measurements.
- Finalized CAD exports after physical fit confirmation.

Unsafe sources:
- Draft Fusion coordinates.
- Old session logs.
- Failed shroud or guide rail instructions.
- Convenience dimensions from a previous assembly layout.
- Any value that changed during real fitting but was not remeasured and re-justified.

Temporary working dimensions may live in `SESSION_CONTEXT.log` while sprinting. Do not promote them into tracked docs until they are validated.

## Dependency Order

### 1. Receiving Bins

Design the bins first because they define the target the chute must hit.

Requirements:
- Four separate labeled categories.
- Wide funnel mouths or catch zones.
- Enough capacity for the expected brick counts.
- Easy removal without disturbing the chute.
- Footprint checked against the 610mm x 610mm limit.

### 2. Servo Rotary Chute

Design the chute from the bin catch zones back toward the conveyor.

Requirements:
- Short and stiff arm.
- UHMW-lined sliding surface.
- Servo mount tied to a stiff frame member.
- Chute exit lands inside each funnel with margin.
- Servo target angles calibrated from physical tests.

### 3. Conveyor Handoff

Design the conveyor exit after the chute entry is known.

Requirements:
- Conveyor height derived from chute entry.
- Exit handoff tested with real bricks.
- Sensing station remains near the feed end.
- Feet and drill patterns locked only after the handoff stack is plausible.

### 4. Wood Frame

Permanent woodworking comes after the mechanical stack is known.

Requirements:
- Stiff enough for servo and sensor alignment.
- Accessible bins.
- Clean wiring path.
- Operator-facing labels and controls.

## First Validation Prints

Print only what retires a specific risk.

Priority:
1. One bin or funnel catch coupon.
2. Short UHMW-lined chute segment.
3. Servo pocket or horn adapter coupon if fit is uncertain.
4. Conveyor-to-chute handoff coupon.
5. Sensing shroud or shroud coupon after conveyor path is stable.

Do not wait for a perfect full assembly before printing the first risk coupon.

## Sprint Bias

- Prefer adjustable physical interfaces over exact theoretical placement.
- Prefer wide funnels over precise servo aiming.
- Prefer short stiff chute geometry over elegant long geometry.
- Prefer fast coupons over large prints.
- Prefer real-brick tests over CAD confidence.
