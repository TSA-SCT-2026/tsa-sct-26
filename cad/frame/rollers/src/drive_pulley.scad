include <rollers_params.scad>

module drive_pulley(params = default_params()) {
  validate(params);

  roller_od = pget(params, "provisional_drive_roller_od");
  face_w = pget(params, "provisional_contact_face_width");
  flange_w = pget(params, "provisional_flange_width");
  flange_od = pget(params, "provisional_flange_od");

  hub_od = pget(params, "provisional_drive_hub_od");
  hub_w = pget(params, "provisional_drive_hub_width");

  d_bore_round = pget(params, "provisional_d_bore_round");
  d_bore_to_flat = pget(params, "provisional_d_bore_to_flat");
  clamp_slot_w = pget(params, "provisional_split_clamp_slot");
  clamp_screw_axis_x = pget(params, "provisional_clamp_screw_axis_x");
  clamp_screw_d = pget(params, "provisional_clamp_screw_clear_d");
  clamp_screw_head_d = pget(params, "provisional_clamp_screw_head_d");
  clamp_screw_head_depth = pget(params, "provisional_clamp_screw_head_depth");
  clamp_nut_flat = pget(params, "provisional_clamp_nut_flat");
  clamp_nut_thickness = pget(params, "provisional_clamp_nut_thickness");

  total_w = face_w + (2 * flange_w);
  hub_start = (total_w - hub_w) / 2;

  difference() {
    union() {
      cylinder(h = flange_w, d = flange_od);
      translate([0, 0, flange_w])
        cylinder(h = face_w, d = roller_od);
      translate([0, 0, flange_w + face_w])
        cylinder(h = flange_w, d = flange_od);

      translate([0, 0, hub_start])
        cylinder(h = hub_w, d = hub_od);
    }

    translate([0, 0, -0.25])
      d_bore_cutout(total_w + 0.5, d_bore_round, d_bore_to_flat);

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
  drive_pulley(params);
} else if (build_target == "validate") {
  validate(params);
  dimension_report(params);
  cube([1, 1, 1]);
} else if (build_target == "report") {
  dimension_report(params);
  cube([1, 1, 1]);
} else {
  drive_pulley(params);
}
