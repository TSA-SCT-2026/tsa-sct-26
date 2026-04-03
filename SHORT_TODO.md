# Short TODO V6

Updated March 23 2026. Architecture locked: gravity trapdoor with class 3 lever.

---


## CAD

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

