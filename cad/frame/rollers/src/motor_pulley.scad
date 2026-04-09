include <rollers_params.scad>

module motor_pulley(params = default_params()) {
  validate(params);

  teeth = pget(params, "provisional_motor_pulley_teeth");
  pitch_mm = pget(params, "provisional_timing_belt_pitch_mm");
  tooth_depth_mm = pget(params, "provisional_timing_tooth_depth_mm");

  pitch_d = pulley_pitch_diameter(teeth, pitch_mm);
  tooth_od = pitch_d + (2 * tooth_depth_mm);

  hub_od = pget(params, "provisional_motor_pulley_hub_od");
  hub_w = pget(params, "provisional_motor_pulley_hub_width");
  bore_round = pget(params, "provisional_motor_pulley_bore_round");
  bore_to_flat = pget(params, "provisional_motor_pulley_bore_to_flat");
  clamp_slot_w = pget(params, "provisional_drive_roller_split_slot");
  clamp_screw_axis_x = pget(params, "provisional_drive_roller_clamp_axis_x");
  clamp_screw_d = pget(params, "provisional_drive_roller_clamp_clear_d");
  clamp_screw_head_d = pget(params, "provisional_drive_roller_clamp_head_d");
  clamp_screw_head_depth = pget(params, "provisional_drive_roller_clamp_head_depth");
  clamp_nut_flat = pget(params, "provisional_drive_roller_clamp_nut_flat");
  clamp_nut_thickness = pget(params, "provisional_drive_roller_clamp_nut_thickness");
  belt_width = pget(params, "provisional_timing_belt_width_mm");

  total_w = max(belt_width + 3.0, hub_w);
  hub_start = (total_w - hub_w) / 2;

  difference() {
    union() {
      cylinder(h = 1.5, d = tooth_od + 1.0);
      translate([0, 0, 1.5])
        cylinder(h = belt_width, d = tooth_od);
      translate([0, 0, 1.5 + belt_width])
        cylinder(h = 1.5, d = tooth_od + 1.0);
      translate([-hub_od / 2, -hub_od / 2, hub_start])
        cube([hub_od, hub_od, hub_w]);
    }

    translate([0, 0, -0.25])
      d_bore_cutout(total_w + 0.5, bore_round, bore_to_flat);

    translate([0, -clamp_slot_w / 2, -0.25])
      cube([hub_od, clamp_slot_w, total_w + 0.5]);

    translate([clamp_screw_axis_x, 0, total_w / 2])
      rotate([90, 0, 0])
        cylinder(h = hub_od + 2, d = clamp_screw_d, center = true);

    translate([clamp_screw_axis_x, -(hub_od / 2) + (clamp_screw_head_depth / 2), total_w / 2])
      rotate([90, 0, 0])
        cylinder(h = clamp_screw_head_depth + 0.4, d = clamp_screw_head_d, center = true);

    translate([clamp_screw_axis_x, (hub_od / 2) - (clamp_nut_thickness / 2), total_w / 2])
      rotate([90, 0, 0])
        cylinder(h = clamp_nut_thickness + 0.4, d = clamp_nut_flat / cos(30), center = true, $fn = 6);
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
