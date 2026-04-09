include <rollers_params.scad>

// Motor pulley layout reference for the NEMA17 timing stage.
// Production path uses purchased 20T GT2 pulleys. This model is a fit aid and
// layout envelope only. It has no tooth profile and does not replace the purchased part.

module motor_pulley(params = default_params()) {
  validate(params);

  teeth = pget(params, "provisional_motor_pulley_teeth");
  pitch_mm = pget(params, "provisional_timing_belt_pitch_mm");
  tooth_depth_mm = pget(params, "provisional_timing_tooth_depth_mm");
  bore_round = pget(params, "provisional_motor_pulley_bore_round");
  bore_to_flat = pget(params, "provisional_motor_pulley_bore_to_flat");
  belt_width = pget(params, "provisional_timing_belt_width_mm");

  pitch_d = pulley_pitch_diameter(teeth, pitch_mm);
  tooth_od = pitch_d + (2 * tooth_depth_mm);

  total_w = belt_width + 3.0;

  difference() {
    union() {
      cylinder(h = 1.5, d = tooth_od + 1.0);
      translate([0, 0, 1.5])
        cylinder(h = belt_width, d = tooth_od);
      translate([0, 0, 1.5 + belt_width])
        cylinder(h = 1.5, d = tooth_od + 1.0);
    }

    translate([0, 0, -0.25])
      d_bore_cutout(total_w + 0.5, bore_round, bore_to_flat);
  }
}

build_target = is_undef(build_target) ? "model" : build_target;
params = is_undef(params) ? default_params() : params;

if (build_target == "model") {
  motor_pulley(params);
} else if (build_target == "validate") {
  validate(params);
  dimension_report(params);
  cube([1, 1, 1]);
} else if (build_target == "report") {
  dimension_report(params);
  cube([1, 1, 1]);
} else {
  motor_pulley(params);
}
