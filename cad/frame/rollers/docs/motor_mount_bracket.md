# Motor Mount Bracket

## Purpose

Mounts the NEMA17 to the frame with 8mm of belt tension travel in the Y direction.

## Geometry

| Feature | Value |
|---------|-------|
| Overall size | 70mm x 70mm x 4mm |
| Motor pilot clearance | 22.4mm diameter (through) |
| Motor M3 holes | 4x, 3.4mm dia, on 31mm square pattern |
| Frame slots | 4x, 3.5mm wide x 12mm long, Y-oriented |
| Frame slot centers | 28mm from bracket center in X, 28mm in Y |
| Tension travel | ~8.5mm (slot length minus slot width) |
| Print orientation | Flat, Z = thickness |

## Slot axis and tension direction

Slots are elongated in Y. The bracket slides in Y relative to the frame to adjust belt tension.

Move the bracket in +Y (away from the driven shaft) to increase center distance and tension the belt.

Frame bolts (M3) pass through the frame wall, through the bracket slots, and are secured with nuts on the other side. Loosen to slide, tighten to lock.

## Motor attachment

Motor face screws (M3) thread directly into the NEMA17 face from the bracket back. The 22.4mm center hole clears the motor pilot boss (22mm OD) and any motor shaft hardware on the far side of the bracket.

Motor pulley (20T GT2, purchased) mounts on the motor shaft on the frame side of the bracket (not between motor and bracket).

## Print and assembly notes

- Print flat with motor-face side down on the bed for best pilot bore surface.
- All holes are vertical, no support required.
- M3 nut traps are not present in this bracket; use standard M3 nuts against the frame face.
- Hub wall material between motor holes and frame slots is approximately 12mm, sufficient for M3 clamping loads.

## Nominal placement (Fusion)

Motor shaft center is 80mm from the driven shaft center in the belt direction (Y axis in conveyor layout). Place this bracket so the bracket center aligns with the motor shaft center at that 80mm offset.

Driven shaft and motor shaft must be parallel (both along X axis) and coplanar in Z unless the frame intentionally places the motor lower for packaging reasons.
