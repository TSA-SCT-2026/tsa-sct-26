include <rollers_params.scad>

module crowned_band(face_w, center_od, crown_sagitta, slices = 72) {
  edge_d = crown_edge_diameter(center_od, crown_sagitta);

  for (i = [0 : slices - 1]) {
    z0 = (i * face_w) / slices;
    z1 = ((i + 1) * face_w) / slices;

    t0 = ((z0 / face_w) * 2) - 1;
    t1 = ((z1 / face_w) * 2) - 1;

    d0 = edge_d + (2 * crown_sagitta * (1 - (t0 * t0)));
    d1 = edge_d + (2 * crown_sagitta * (1 - (t1 * t1)));

    hull() {
      translate([0, 0, z0])
        cylinder(h = 0.01, d = d0);
      translate([0, 0, z1])
        cylinder(h = 0.01, d = d1);
    }
  }
}

module bearing_pocket_from_end(depth, pocket_d, lead_chamfer) {
  chamfer_h = min(lead_chamfer, depth / 2);
  straight_h = depth - chamfer_h;

  if (chamfer_h > 0)
    cylinder(h = chamfer_h, d1 = pocket_d + (2 * lead_chamfer), d2 = pocket_d);

  if (straight_h > 0)
    translate([0, 0, chamfer_h])
      cylinder(h = straight_h, d = pocket_d);
}

module idler_roller(params = default_params()) {
  validate(params);

  roller_od = pget(params, "provisional_idler_roller_od");
  face_w = pget(params, "provisional_contact_face_width");
  flange_w = pget(params, "provisional_flange_width");
  flange_od = pget(params, "provisional_flange_od");
  crown_sagitta = pget(params, "provisional_crown_sagitta");

  pocket_d = pget(params, "provisional_bearing_pocket_d");
  pocket_depth = pget(params, "provisional_bearing_pocket_depth");
  pocket_chamfer = pget(params, "provisional_bearing_lead_chamfer");
  axle_d = pget(params, "provisional_axle_clear_d");

  magnet_d = pget(params, "provisional_magnet_d");
  magnet_depth = pget(params, "provisional_magnet_depth");
  magnet_r = pget(params, "provisional_magnet_radial_offset");

  total_w = face_w + (2 * flange_w);

  difference() {
    union() {
      cylinder(h = flange_w, d = flange_od);
      translate([0, 0, flange_w])
        crowned_band(face_w, roller_od, crown_sagitta);
      translate([0, 0, flange_w + face_w])
        cylinder(h = flange_w, d = flange_od);
    }

    translate([0, 0, -0.01])
      bearing_pocket_from_end(pocket_depth + 0.01, pocket_d, pocket_chamfer);

    translate([0, 0, total_w - pocket_depth])
      mirror([0, 0, 1])
        bearing_pocket_from_end(pocket_depth + 0.01, pocket_d, pocket_chamfer);

    translate([0, 0, -0.25])
      cylinder(h = total_w + 0.5, d = axle_d);

    for (a = [0, 180]) {
      rotate([0, 0, a])
        translate([magnet_r, 0, total_w / 2])
          rotate([90, 0, 0])
            cylinder(h = magnet_depth + 0.4, d = magnet_d, center = true);
    }
  }
}

build_target = is_undef(build_target) ? "model" : build_target;
params = is_undef(params) ? default_params() : params;

if (build_target == "model") {
  idler_roller(params);
} else if (build_target == "validate") {
  validate(params);
  dimension_report(params);
  cube([1, 1, 1]);
} else if (build_target == "report") {
  dimension_report(params);
  cube([1, 1, 1]);
} else {
  idler_roller(params);
}
