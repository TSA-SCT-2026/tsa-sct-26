# roller_coupons.scad Part Card

## Role

- Fit test parts used before printing final pulley and idler.
- Reduces failed full size prints by selecting fit values from real hardware tests.

## Mates With

- Motor D shaft for D bore coupon
- MR115 bearings for bearing coupon
- Belt strip for crown coupon tracking checks

## Required Hardware For Testing

- Real motor shaft from selected conveyor motor
- Real MR115 bearings from current lot
- Belt strip sample from build stock

## Coupon Set

- Bearing fit ladder: 10.95, 11.10, 11.25
- D bore ladder pairs: 6.10 or 5.60, 6.20 or 5.70, 6.25 or 5.75
- Crown ladder: 0.25, 0.50, 0.75 sagitta

## Basic Use Steps

1. Print all coupon STL files with same settings as final rollers.
2. Test bearing insertion force on each bearing coupon hole.
3. Test shaft fit and anti wobble behavior on each D bore option.
4. Test belt tracking trend with each crown option.
5. Record selected values in notes.
6. Update `rollers_params.scad` and regenerate final STLs.

## Pass or Fail Checks

Pass:
- Selected bearing fit seats by hand with controlled force
- Selected D bore slides on shaft and clamps without wobble
- Selected crown keeps belt centered in sustained run

Fail:
- Bearing fit requires unsafe press force
- D bore either jams or slips under light torque
- Belt drift persists across crown options

## Locked Truth Dependency

- Coupon interpretation must preserve channel and belt strip constraints used by the real roller models.

## Double Check When Hardware Arrives

- Record measured fit outcome for each coupon value.
- Select the smallest easy fit for production roller values.
- Update `rollers_params.scad` and rerun `build_rollers.sh`.
