include <rollers_params.scad>

// Drive roller for the supported conveyor shaft.
// D-bore grips the shaft flat for torque. Shaft collars on each side handle axial retention.
// No split clamp. No screw hardware.

module drive_roller(params = default_params()) {
  validate(params);

  roller_od = pget(params, "provisional_drive_roller_od");
  face_w = pget(params, "provisional_drive_roller_face_width");
  flange_w = pget(params, "provisional_drive_roller_flange_width");
  flange_od = pget(params, "provisional_drive_roller_flange_od");

  d_bore_round = pget(params, "provisional_drive_roller_shaft_round");
  d_bore_to_flat = pget(params, "provisional_drive_roller_shaft_to_flat");

  total_w = face_w + (2 * flange_w);

  difference() {
    union() {
      cylinder(h = flange_w, d = flange_od);
      translate([0, 0, flange_w])
        cylinder(h = face_w, d = roller_od);
      translate([0, 0, flange_w + face_w])
        cylinder(h = flange_w, d = flange_od);
    }

    translate([0, 0, -0.25])
      d_bore_cutout(total_w + 0.5, d_bore_round, d_bore_to_flat);
  }
}

build_target = is_undef(build_target) ? "model" : build_target;
params = is_undef(params) ? default_params() : params;

if (build_target == "model") {
  drive_roller(params);
} else if (build_target == "validate") {
  validate(params);
  dimension_report(params);
  cube([1, 1, 1]);
} else if (build_target == "report") {
  dimension_report(params);
  cube([1, 1, 1]);
} else {
  drive_roller(params);
}
