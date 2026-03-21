# Short TODO

Updated March 21 2026. Design switch complete. Parts not yet ordered.

---

## Tonight: order parts

See docs/BOM.md. Key things to verify before checkout:
- Solenoid: 6V variant (not 12V or 24V)
- JGB37: 600 RPM at 6V
- LiPo: 3S 11.1V 1300mAh XT60 (NOT 2S - design now uses 3S)
- GT2 belt: 640mm closed loop, 20mm width (shorter than before)
- No lever arm needed - direct solenoid push only

---

## This week: firmware (unblocked)

Fix firmware compilation after design switch. Does not require hardware.
Priority order: actuators.cpp, events.cpp, test_harness.cpp, then logger, main.
Delete classifier.cpp/h and router.cpp/h (replaced by state_machine routing table).
Goal: pio run compiles clean with zero errors.

---

## This week: docs restructure (parallel)

Rewrite ARCHITECTURE.md for new design. Create DIMENSIONS.md, CALIBRATION.md, TEST_PROTOCOL.md.
Update MECHANICAL, ELECTRICAL, EMBEDDED, BOM for direct pusher and chute sensing.

---

## This week: simulator update (parallel, lower priority)

Update simulator defaults for escapement cycle (280ms), new pusher positions.
Replace belt-mounted color dwell model with stationary chute sensing model.

---

## Unresolved from Opus gaps doc (before April 1)

- Cam profile exact geometry: chord depth, lobe radius, transition angle, shaft bore
  -> Needs dimensioned drawing or parametric OpenSCAD script
- Chute-to-belt ramp geometry: transition piece dimensions
  -> Needs actual mm dimensions before CAD starts
- JGB37 shaft-to-pulley coupling: 6mm D-shaft, need 6mm bore GT2 pulley or adapter
  -> Already ordering 6mm bore pulleys - confirm in cart
- Wire routing plan: how to make 30+ wires disconnectable for transport
  -> Decide: JST connectors per subsystem, or header pins on perfboard
- Notebook physical format decision: composition book + printed inserts (recommended)

---

## Waiting on parts (~April 1)

Everything physical waits on parts. Firmware and docs are unblocked.
