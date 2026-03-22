# Short TODO

Updated March 21 2026. Design switch complete. Parts not yet ordered.

---

## Tonight: order parts

See docs/BOM.md. Key things to verify before checkout:
- Solenoid: 6V variant (not 12V or 24V)
- JGB37: 600 RPM at 6V
- LiPo: 3S 11.1V 1300mAh XT60 (NOT 2S)
- Neoprene rubber strip 1/8" x 3/4" x 10ft (Amazon, search TORRAMI neoprene strip -- 3/4" = 19mm, NOT 1")
- A3144 Hall sensor 3-pack (Amazon)
- 3mm neodymium disc magnets 10-pack (Amazon)
- No GT2 belt of any kind needed for primary design
- No GT2 pulleys needed - rollers are smooth cylinders
- JST-XH connector kit: 2-pin and 4-pin, for wire bundle connectors
- No lever arm needed - direct solenoid push only

---

## This week: firmware (DONE)

Firmware compiles clean. Deleted classifier.h/cpp and router.h/cpp.
Aligned actuators, events, logger, test_harness to V3 design:
BrickCategory replaces old size/color/type enums, new event types throughout.
Also fixed CONFIRM timeout hole: timeout now checked during TRANSIT state,
preventing a machine hang if a bin beam never fires.

---

## This week: docs restructure (parallel)

Rewrite ARCHITECTURE.md for new design. Create DIMENSIONS.md, CALIBRATION.md, TEST_PROTOCOL.md.
Update MECHANICAL, ELECTRICAL, EMBEDDED, BOM for direct pusher and chute sensing.

---

## This week: simulator update (parallel, lower priority)

Simulator is outdated - it predates the belt redesign and the switch from solenoid plow
to direct pusher. Migrate to reflect: escapement cycle (~280ms), new pusher positions
(75/150/225mm), chute-stationary sensing model, flat neoprene belt with Hall PID.

---

## Resolved design gaps (before April 1)

All previously open gaps are now closed:

- Cam profile exact geometry: fully specified in docs/DIMENSIONS.md
- Chute-to-belt ramp: 10mm long, 2mm drop, 11.3 degrees, integrated into chute base piece
- Motor shaft coupling: 6mm D-bore GT2 pulley on motor shaft, drive roller beside it on same shaft
- Wire routing: JST-XH connectors in 3 bundles per subsystem, see docs/ELECTRICAL.md
- Notebook format: composition book with handwritten entries + printed inserts for equations/data

---

## Waiting on parts (~April 1)

Everything physical waits on parts. Firmware and docs are unblocked.
