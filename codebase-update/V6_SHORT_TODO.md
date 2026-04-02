# Short TODO V6

Updated March 23 2026. Architecture locked: gravity trapdoor with class 3 lever.

---

## Immediately: run MIGRATION_V6.md via Claude Code

Archive V1 -> install V6 docs -> update firmware stubs.

---

## Tonight: order full BOM

See docs/BOM.md. No cuts needed.

Key items to verify before checkout:
- Solenoid: 6V (not 12V or 24V)
- LiPo: 3S 11.1V. Two units. NOT 2S.
- Neoprene: 3/4" (19mm). NOT 1". Search TORRAMI.
- Tension springs: light assortment (0.3-0.4mm wire, 4-6mm OD). Get range of rates.
- Shaft hub: 5mm aluminum, set-screw.
- Micro-switches: Omron D2F equiv, pin actuator, 10 units.
- UHMW tape for platform surface.
- M3 nylon locknuts (for lever pivot) in the screw assortment.

---

## This week: firmware

Priority order:
1. config.h: new GPIO assignments, V6 constants, remove V1 pusher constants.
2. state_machine: V6 states.
3. actuators: remove pushers, add chute selector stepper, rename PIN_RELEASE.
4. sensors: rename senseBrickInChute -> senseBrickInChamber.
5. events, test_harness, display.
6. pio run: zero errors.

---

## This week: CAD priority 1 (gate for everything)

Print and test the lever + platform mechanism BEFORE any other CAD.

Parts to print (all small, fast):
1. Platform (22mm x 20mm x 3mm, hinge ears, far edge tab with 30-deg chamfer lead-in)
2. Hinge bracket (mounts to chamber entry wall, receives 3mm hinge rod)
3. Lever arm (30mm, 4mm x 4mm, fulcrum hole at 0mm, 30-deg chamfer tip at 30mm end)
4. Lever pivot bracket (mounts to chamber side wall, M3 through-hole at lever fulcrum)
5. Solenoid mounting bracket (positions solenoid with plunger at 8mm from lever fulcrum)

Test sequence before printing anything else:
1. Assemble platform on hinge rod. Verify smooth pivot.
2. Insert lever arm on M3 pivot with nylon locknut. Verify smooth rotation.
3. Verify lever tip tucks under platform tab cleanly when lever is in rest position.
4. Attach solenoid. Energize: verify lever tip sweeps fully clear of tab.
5. De-energize: verify lever return spring pulls tip back under tab via chamfer.
6. Attach platform return spring. Verify platform returns to level within 200ms.
7. Place real LEGO brick on platform. Energize solenoid. Verify brick drops cleanly.
8. 50 consecutive cycles. No failures.

If chamfer re-engagement fails: adjust chamfer angle (start 30 deg, try 20 and 40).
Budget 3 lever arm reprints for chamfer tuning. These are 5-minute prints.

Gate: all 8 steps pass before printing chamber, disc, or belt channel.

---

## Open questions

OQ-01: RESOLVED. Mechanism: class 3 lever sweeping outward, gravity drop.
OQ-02: RESOLVED. Springs: one platform return spring, one lever return spring.
OQ-03: RESOLVED. Nothing new needed on BOM. Lever arm is printed PLA.
OQ-04: RESOLVED. Shaft hub: aluminum, set-screw, mandatory.

OQ-05: Disc funnel surface. Test plain PLA first. Add PTFE tape if brick catches on edge.
OQ-06: Belt transport length. 100-120mm for phase 1. Shorten for speed in phase 2.
OQ-07: Lever chamfer angle. Start at 30 degrees. Tune empirically. Not knowable in advance.
