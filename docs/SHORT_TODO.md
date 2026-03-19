# Short TODO

Immediate and near-term action items. Keep this list short and current.

---

## Urgent (overdue)

- [ ] Order AliExpress parts (BOM.md): 10-15 day lead time, every day of delay matters
- [ ] Order Adafruit parts (BOM.md): ships in 2-5 days
- [ ] Pick and order display (Pimoroni 1.3in ST7789 recommended)

---

## This week (before March 21)

- [ ] Draw top-down footprint layout: verify 610x610mm constraint before any frame CAD commits
- [x] Firmware skeleton: state machine, classifier, router, thermal model, test harness, logging suite
- [ ] Start chute transition piece CAD: print as soon as CAD is done, iterate until reliable

## Firmware - next steps (no hardware needed)

- [ ] Verify firmware compiles clean: `pio run` from firmware/
- [ ] Run `test fullrun` over serial, confirm all 24 bricks route correctly and run summary matches expected bin counts
- [ ] Implement display module (ST7789 SPI driver, state screens, brick animation)

---

## Open decisions

- [ ] Confirm AC outlet availability at judging table: contact event organizers or find a past competitor. Determines whether to use bench supply or LiPo at competition. See ISSUES.md for full context.

- [ ] Confirm H206 optocoupler is in the AliExpress order (belt speed encoder, already in BOM)
- [ ] Find stepper cam reference models if useful (grabcad, printables): geometry is simple enough to draw from scratch but a reference doesn't hurt

---

## Waiting on (unblocked once parts arrive ~April 1)

- Validate actual belt speed at target PWM
- Validate color sensor readings under shroud LED
- Confirm break-beam size detection geometry at actual speed
- Select final spring weight from assortment (test against actual solenoid)
