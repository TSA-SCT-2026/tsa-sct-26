include <rollers_params.scad>

function list_min_len(a, b) = len(a) < len(b) ? len(a) : len(b);

module bearing_fit_coupon(od_list = [10.95, 11.10, 11.25], params = default_params()) {
  validate(params);

  h = pget(params, "provisional_coupon_block_height");
  spacing = pget(params, "provisional_coupon_spacing");
  wall = 4.0;

  slot_w = 16;
  total_w = len(od_list) * slot_w + (len(od_list) + 1) * spacing;
  total_d = 28;

  difference() {
    cube([total_w, total_d, h]);

    for (i = [0 : len(od_list) - 1]) {
      x = spacing + (slot_w / 2) + i * (slot_w + spacing);
      y = total_d / 2;
      translate([x, y, wall])
        cylinder(h = h - wall + 0.1, d = od_list[i]);
    }
  }
}

module d_bore_coupon(round_list = [5.10, 5.20, 5.25], flat_list = [4.40, 4.50, 4.55], params = default_params()) {
  validate(params);

  h = pget(params, "provisional_coupon_block_height");
  spacing = pget(params, "provisional_coupon_spacing");
  slot_w = 18;
  wall = 4.0;

  n = list_min_len(round_list, flat_list);
  total_w = n * slot_w + (n + 1) * spacing;
  total_d = 32;

  difference() {
    cube([total_w, total_d, h]);

    for (i = [0 : n - 1]) {
      x = spacing + (slot_w / 2) + i * (slot_w + spacing);
      y = total_d / 2;
      translate([x, y, wall])
        d_bore_cutout(h - wall + 0.1, round_list[i], flat_list[i]);
    }
  }
}

module crown_coupon(crown_list = [0.25, 0.50, 0.75], face_width = 20.0, params = default_params()) {
  validate(params);

  center_od = pget(params, "provisional_idler_roller_od");
  spacing = pget(params, "provisional_coupon_spacing");
  sample_w = 10.0;

  for (i = [0 : len(crown_list) - 1]) {
    x = i * (face_width + spacing);
    z_shift = i * 0.001;
    translate([x, 0, z_shift])
      rotate([90, 0, 0])
        linear_extrude(height = sample_w)
          polygon([
            [0, center_od / 2],
            [face_width / 2, (center_od / 2) - crown_list[i]],
            [face_width, center_od / 2],
            [face_width, 0],
            [0, 0]
          ]);
  }
}

build_target = is_undef(build_target) ? "all" : build_target;
params = is_undef(params) ? default_params() : params;

if (build_target == "bearing") {
  bearing_fit_coupon(params = params);
} else if (build_target == "dbore") {
  d_bore_coupon(params = params);
} else if (build_target == "crown") {
  crown_coupon(params = params);
} else if (build_target == "validate") {
  validate(params);
  dimension_report(params);
  cube([1, 1, 1]);
} else {
  union() {
    bearing_fit_coupon(params = params);
    translate([0, 40, 0])
      d_bore_coupon(params = params);
    translate([0, 85, 0])
      crown_coupon(params = params);
  }
}
