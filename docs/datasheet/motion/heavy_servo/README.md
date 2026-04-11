# Heavy Servo Reference

Active status: active actuator for the states-build servo rotary chute selector.

## Traceability

| Field | Value |
|-------|-------|
| BOM trace | User-confirmed on hand, not yet traced to a spreadsheet row |
| Received status | Yes |
| Servo class | MG995/MG996/MG996R-class heavy servo |
| States-build use | Rotates the chute toward four labeled bins |
| Trust level | Listing transcription plus local CAD references |
| Next action | Verify exact horn fit, usable throw, and final chute angles with the real servo installed |

## CAD References

This folder intentionally keeps two useful CAD sources:

| Path | Why it matters |
|------|----------------|
| `cad/solidworks/` | Includes horn attachments, screw, and assembly context |
| `cad/step/servo_mg996r.step` | Use this for Fusion layout when the SolidWorks assembly is not directly usable |
| `images/` | Visual references for the assembly and STEP source |

Keep the SolidWorks files unless a replacement STEP assembly with horns is confirmed. The horn and screw context matters for the chute pivot interface.

## Key Specs

| Feature | Value |
|---------|-------|
| Body dimensions | 40mm x 19mm x 43mm |
| Operating voltage | 4.8V to 7.2V |
| Speed at 4.8V, no load | 0.17s per 60 degrees |
| Speed at 6.0V, no load | 0.13s per 60 degrees |
| Stall torque at 4.8V | 13 kg-cm |
| Stall torque at 6.0V | 15 kg-cm |
| Connector wire length | 300mm |
| Weight | 69g listed |

## Integration Notes

- Power the servo from an appropriate external rail, not directly from an ESP32 pin.
- Keep servo wiring clear of the horn, chute sweep, and brick path.
- Final chute angles must be tested with real bricks and the actual bins.
- Do not assume the STEP body alone proves horn geometry. Use the SolidWorks horn files or measure the real horn before freezing the pivot adapter.
