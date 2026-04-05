# roller_coupons.scad Part Card

## Role

- Fit test parts used before printing the final timing pulley, drive roller, and idler.
- Reduces failed full size prints by selecting fit values from real hardware tests.

## Mates With

- NEMA17 motor shaft for timing pulley fit coupon
- Supported conveyor shaft for drive roller fit coupon
- MR115 bearings for bearing coupon
- Belt strip for crown coupon tracking checks

## Required Hardware For Testing

- Real motor shaft from selected conveyor motor
- Real supported conveyor shaft from the timing stage
- Real MR115 bearings from current lot
- Belt strip sample from build stock

## Coupon Set

- Bearing fit ladder: 10.95, 11.10, 11.25
- Shaft-fit ladder pairs: 5.10 or 4.40, 5.20 or 4.50, 5.25 or 4.55
- Crown ladder: 0.25, 0.50, 0.75 sagitta

## Basic Use Steps

1. Print all coupon STL files with same settings as final rollers.
2. Test bearing insertion force on each bearing coupon hole.
3. Test shaft fit and anti wobble behavior on each shaft-fit option.
4. Test belt tracking trend with each crown option.
5. Record selected values in notes.
6. Update `src/rollers_params.scad` and regenerate final STLs.

## Pass or Fail Checks

Pass:
- Selected bearing fit seats by hand with controlled force
- Selected shaft-fit coupon slides on shaft and clamps without wobble
- Selected crown keeps belt centered in sustained run

Fail:
- Bearing fit requires unsafe press force
- Shaft-fit coupon either jams or slips under light torque
- Belt drift persists across crown options

## Locked Truth Dependency

- Coupon interpretation must preserve channel and belt strip constraints used by the real roller models.
- Coupon results should be copied into the timing stage and roller model notes, not guessed from visual fit alone.

## Double Check When Hardware Arrives

- Record measured fit outcome for each coupon value.
- Select the smallest easy fit for production roller values.
- Update `src/rollers_params.scad` and rerun `scripts/build_rollers.sh`.
