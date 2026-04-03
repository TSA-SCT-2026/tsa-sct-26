# roller_coupons.scad Notes

Intent:
- Provide fast fit coupons before full part print to reduce reprint risk.
- Support one pass tuning for bearing seats, D bores, and crown choices.

Coupon set:
- Bearing fit ladder: 10.95, 11.10, 11.25
- D bore ladder pairs: 6.10 or 5.60, 6.20 or 5.70, 6.25 or 5.75
- Crown ladder: 0.25, 0.50, 0.75 sagitta

Locked truth dependency:
- Coupon interpretation must preserve channel and belt strip constraints used by the real roller models.

Double check when hardware arrives:
- Record measured fit outcome for each coupon value.
- Select the smallest easy fit for production roller values.
- Update `rollers_params.scad` and rerun `build_rollers.sh`.
