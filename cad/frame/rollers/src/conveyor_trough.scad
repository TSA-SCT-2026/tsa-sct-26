include <rollers_params.scad>

function motor_slot_total_length(params = default_params()) =
  pget(params, "provisional_motor_m3_clear_d") + pget(params, "provisional_motor_slot_travel_mm");

function trough_outer_width(params = default_params()) =
  pget(params, "truth_locked_channel_width") + (2 * pget(params, "provisional_channel_wall_thickness_mm"));

function trough_half_outer_width(params = default_params()) = trough_outer_width(params) / 2;

function drive_roller_total_width(params = default_params()) =
  pget(params, "provisional_drive_roller_face_width") +
  (2 * pget(params, "provisional_drive_roller_flange_width"));

function idler_roller_total_width(params = default_params()) =
  pget(params, "provisional_contact_face_width") +
  (2 * pget(params, "provisional_flange_width"));

function roller_clear_span_required(params = default_params()) =
  max(drive_roller_total_width(params), idler_roller_total_width(params));

function roller_relief_depth(params = default_params()) =
  max(0, (roller_clear_span_required(params) - pget(params, "truth_locked_channel_width")) / 2);

function roller_relief_length(params = default_params()) =
  (max(pget(params, "provisional_drive_roller_od"), pget(params, "provisional_idler_roller_od")) / 2) +
  (pget(params, "provisional_channel_wall_thickness_mm") / 2);

function drive_bearing_cheek_thickness(params = default_params()) =
  pget(params, "provisional_bearing_pocket_depth");

function drive_axis_z(params = default_params()) =
  trough_foot_height(params) +
  pget(params, "provisional_trough_shelf_thickness_mm") +
  (pget(params, "provisional_drive_roller_od") / 2) -
  pget(params, "provisional_belt_strip_thickness_mm");

function trough_foot_height(params = default_params()) =
  max(
    pget(params, "provisional_channel_wall_thickness_mm"),
    ceil(
      (pget(params, "provisional_nema17_hole_spacing_mm") / 2) +
      (motor_slot_total_length(params) / 2) +
      pget(params, "provisional_channel_wall_thickness_mm") -
      pget(params, "provisional_trough_shelf_thickness_mm") -
      (pget(params, "provisional_drive_roller_od") / 2) +
      pget(params, "provisional_belt_strip_thickness_mm")
    )
  );

function motor_plate_half_y(params = default_params()) =
  (pget(params, "provisional_nema17_hole_spacing_mm") / 2) +
  (motor_slot_total_length(params) / 2) +
  pget(params, "provisional_channel_wall_thickness_mm");

function motor_axis_x(params = default_params()) =
  trough_half_outer_width(params) +
  (pget(params, "provisional_nema17_hole_spacing_mm") / 2) +
  (2 * pget(params, "provisional_channel_wall_thickness_mm"));

function motor_plate_top_z(params = default_params()) =
  drive_axis_z(params) +
  (pget(params, "provisional_nema17_hole_spacing_mm") / 2) +
  (motor_slot_total_length(params) / 2) +
  pget(params, "provisional_channel_wall_thickness_mm");

function belt_window_width(params = default_params()) =
  pget(params, "provisional_timing_belt_width_mm") +
  (2 * pget(params, "provisional_channel_wall_thickness_mm"));

function belt_window_z0(params = default_params()) =
  drive_axis_z(params) -
  (pget(params, "provisional_timing_belt_width_mm") / 2) -
  pget(params, "provisional_channel_wall_thickness_mm");

function roller_relief_z0(params = default_params()) =
  max(0, drive_axis_z(params) - (pget(params, "provisional_flange_od") / 2));

module x_axis_slot(depth, clear_d, total_l) {
  slot_half = (total_l - clear_d) / 2;

  hull() {
    translate([0, -slot_half, 0])
      rotate([0, 90, 0])
        cylinder(h = depth, d = clear_d, $fn = 32);
    translate([0, slot_half, 0])
      rotate([0, 90, 0])
        cylinder(h = depth, d = clear_d, $fn = 32);
  }
}

module outward_bearing_pocket_left(depth, pocket_d, lead_chamfer) {
  chamfer_h = min(lead_chamfer, depth / 2);
  straight_h = depth - chamfer_h;

  rotate([0, 90, 0]) {
    if (chamfer_h > 0)
      cylinder(h = chamfer_h, d1 = pocket_d + (2 * lead_chamfer), d2 = pocket_d, $fn = 64);
    if (straight_h > 0)
      translate([0, 0, chamfer_h])
        cylinder(h = straight_h, d = pocket_d, $fn = 64);
  }
}

module outward_bearing_pocket_right(depth, pocket_d, lead_chamfer) {
  chamfer_h = min(lead_chamfer, depth / 2);
  straight_h = depth - chamfer_h;

  rotate([0, -90, 0]) {
    if (chamfer_h > 0)
      cylinder(h = chamfer_h, d1 = pocket_d + (2 * lead_chamfer), d2 = pocket_d, $fn = 64);
    if (straight_h > 0)
      translate([0, 0, chamfer_h])
        cylinder(h = straight_h, d = pocket_d, $fn = 64);
  }
}

module conveyor_trough(params = default_params()) {
  validate(params);

  channel_w = pget(params, "truth_locked_channel_width");
  wall_t = pget(params, "provisional_channel_wall_thickness_mm");
  trough_l = pget(params, "provisional_trough_length_mm");
  wall_h = pget(params, "provisional_trough_wall_height_mm");
  shelf_t = pget(params, "provisional_trough_shelf_thickness_mm");
  flange_w = pget(params, "provisional_trough_flange_width_mm");
  mount_hole_d = pget(params, "provisional_trough_mount_hole_d_mm");
  mount_hole_inset = pget(params, "provisional_trough_mount_hole_inset_mm");
  motor_offset = pget(params, "provisional_motor_offset_mm");

  drive_shaft_d = pget(params, "provisional_drive_roller_shaft_round");
  idler_axle_d = pget(params, "provisional_axle_clear_d");
  pocket_d = pget(params, "provisional_bearing_pocket_d");
  pocket_depth = pget(params, "provisional_bearing_pocket_depth");
  pocket_chamfer = pget(params, "provisional_bearing_lead_chamfer");

  pilot_d = pget(params, "provisional_nema17_pilot_clear_d");
  hole_spacing = pget(params, "provisional_nema17_hole_spacing_mm");
  motor_slot_d = pget(params, "provisional_motor_m3_clear_d");
  motor_slot_l = motor_slot_total_length(params);

  half_channel = channel_w / 2;
  half_outer = trough_half_outer_width(params);
  foot_h = trough_foot_height(params);
  drive_cheek_t = drive_bearing_cheek_thickness(params);
  drive_support_t = wall_t + drive_cheek_t;
  relief_depth = roller_relief_depth(params);
  relief_len = roller_relief_length(params);
  relief_z0 = roller_relief_z0(params);
  belt_window_w = belt_window_width(params);
  belt_window_z_start = belt_window_z0(params);
  drive_cheek_span = pocket_d + (2 * wall_t);
  shelf_z0 = foot_h;
  shelf_z1 = foot_h + shelf_t;
  wall_z1 = shelf_z1 + wall_h;
  axis_z = drive_axis_z(params);

  left_inner_x = -half_channel;
  left_outer_x = -half_outer;
  right_inner_x = half_channel;
  right_outer_x = half_outer;

  motor_center_y = -motor_offset;
  motor_half_y = motor_plate_half_y(params);
  motor_axis_x_pos = motor_axis_x(params);
  motor_plate_x0 = motor_axis_x_pos - (wall_t / 2);
  motor_plate_x1 = motor_plate_x0 + wall_t;
  motor_plate_z1 = motor_plate_top_z(params);
  bridge_z1 = axis_z + wall_t;
  overlap = 0.02;
  belt_window_x0 = motor_plate_x0 - belt_window_w;

  assert(relief_depth <= wall_t,
    "Roller flange relief exceeds available wall thickness");
  assert(belt_window_w >= pget(params, "provisional_timing_belt_width_mm"),
    "Belt window must be at least as wide as the timing belt");
  assert(belt_window_x0 > right_outer_x,
    "Belt window must leave material between the wall and motor plate");
  assert(belt_window_z_start < bridge_z1,
    "Belt window must open through the bridge");

  difference() {
    union() {
      // Left side wall.
      translate([left_outer_x - overlap, 0, 0])
        cube([wall_t + (2 * overlap), trough_l, wall_z1]);

      // Right side wall.
      translate([right_inner_x - overlap, 0, 0])
        cube([wall_t + (2 * overlap), trough_l, wall_z1]);

      // Local drive-end bearing cheeks reinforce the MR85 pockets without changing
      // the documented 3mm channel-wall thickness through the transport path.
      translate([left_outer_x - drive_cheek_t, 0, axis_z - (drive_cheek_span / 2)])
        cube([drive_cheek_t + overlap, drive_cheek_span, drive_cheek_span]);

      translate([right_outer_x - overlap, 0, axis_z - (drive_cheek_span / 2)])
        cube([drive_cheek_t + overlap, drive_cheek_span, drive_cheek_span]);

      // Bottom mounting flanges.
      translate([left_outer_x - flange_w, 0, 0])
        cube([flange_w + overlap, trough_l, foot_h]);

      translate([right_outer_x - overlap, 0, 0])
        cube([flange_w + overlap, trough_l, foot_h]);

      // Flat belt bed shelf between the two side plates.
      translate([left_inner_x - overlap, 0, shelf_z0])
        cube([channel_w + (2 * overlap), trough_l, shelf_t]);

      // Upstream extension of the motor-side wall.
      translate([right_inner_x - overlap, motor_center_y - motor_half_y, 0])
        cube([wall_t + (2 * overlap), motor_offset + motor_half_y + overlap, wall_z1]);

      // Simple mount leg from the wall to the outboard motor face.
      translate([right_outer_x - overlap, motor_center_y - motor_half_y, 0])
        cube([motor_plate_x1 - right_outer_x + overlap, motor_offset + motor_half_y + overlap, bridge_z1]);

      // Outboard NEMA17 motor face plate.
      translate([motor_plate_x0, motor_center_y - motor_half_y, 0])
        cube([wall_t, 2 * motor_half_y, motor_plate_z1]);
    }

    // Drive-end supported shaft bores and MR85 pockets.
    translate([left_outer_x - drive_cheek_t - 0.1, 0, axis_z])
      rotate([0, 90, 0])
        cylinder(h = drive_support_t + 0.2, d = drive_shaft_d, $fn = 64);

    translate([right_inner_x - 0.1, 0, axis_z])
      rotate([0, 90, 0])
        cylinder(h = drive_support_t + 0.2, d = drive_shaft_d, $fn = 64);

    translate([left_outer_x - drive_cheek_t - 0.01, 0, axis_z])
      outward_bearing_pocket_left(pocket_depth + 0.01, pocket_d, pocket_chamfer);

    translate([right_outer_x + drive_cheek_t + 0.01, 0, axis_z])
      outward_bearing_pocket_right(pocket_depth + 0.01, pocket_d, pocket_chamfer);

    // Idler end keeps the existing roller architecture: M5 axle clearance only.
    for (x_side = [left_outer_x, right_outer_x]) {
      translate([x_side - 0.1, trough_l, axis_z])
        rotate([0, 90, 0])
          cylinder(h = wall_t + 0.2, d = idler_axle_d, $fn = 64);
    }

    // Bottom mounting holes for MDF base attachment.
    for (x_hole = [left_outer_x - (flange_w / 2), right_outer_x + (flange_w / 2)])
      for (y_hole = [mount_hole_inset, trough_l - mount_hole_inset])
        translate([x_hole, y_hole, -0.1])
          cylinder(h = wall_z1 + 0.2, d = mount_hole_d, $fn = 48);

    // Timing belt lane through the bridge to the outboard motor plate.
    translate([belt_window_x0, motor_center_y - motor_half_y - 0.1, belt_window_z_start])
      cube([belt_window_w, motor_offset + motor_half_y + 0.2, bridge_z1 - belt_window_z_start + 0.2]);

    // Roller flange reliefs: the rollers stay unchanged, so the trough opens locally
    // at both ends to clear the 29mm flange envelope from a 25mm channel.
    if (relief_depth > 0) {
      for (y_start = [0, trough_l - relief_len]) {
        translate([left_inner_x - relief_depth, y_start, relief_z0])
          cube([relief_depth, relief_len, wall_z1 - relief_z0 + 0.1]);

        translate([right_inner_x, y_start, relief_z0])
          cube([relief_depth, relief_len, wall_z1 - relief_z0 + 0.1]);
      }
    }

    // NEMA17 pilot clearance.
    translate([motor_plate_x0 - 0.1, motor_center_y, axis_z])
      rotate([0, 90, 0])
        cylinder(h = wall_t + 0.2, d = pilot_d, $fn = 64);

    // NEMA17 mounting slots. Slot axis follows the belt axis.
    for (y_sign = [-1, 1])
      for (z_sign = [-1, 1])
        translate([motor_plate_x0 - 0.1, motor_center_y + (y_sign * hole_spacing / 2), axis_z + (z_sign * hole_spacing / 2)])
          x_axis_slot(wall_t + 0.2, motor_slot_d, motor_slot_l);
  }
}

module trough_end_cap_coupon(params = default_params()) {
  coupon_trim_y = pget(params, "provisional_trough_mount_hole_inset_mm") + 12.0;
  motor_half_y = motor_plate_half_y(params);
  motor_offset = pget(params, "provisional_motor_offset_mm");
  half_outer = trough_half_outer_width(params);
  flange_w = pget(params, "provisional_trough_flange_width_mm");
  motor_axis_x_pos = motor_axis_x(params);
  wall_t = pget(params, "provisional_channel_wall_thickness_mm");

  intersection() {
    conveyor_trough(params);
    translate([
      -half_outer - flange_w - 1,
      -motor_offset - motor_half_y - 1,
      -1
    ])
      cube([
        motor_axis_x_pos + wall_t + half_outer + flange_w + 2,
        motor_offset + motor_half_y + coupon_trim_y + 2,
        motor_plate_top_z(params) + 2
      ]);
  }
}

build_target = is_undef(build_target) ? "model" : build_target;
params = is_undef(params) ? default_params() : params;

if (build_target == "model") {
  conveyor_trough(params);
} else if (build_target == "coupon") {
  trough_end_cap_coupon(params);
} else if (build_target == "validate") {
  validate(params);
  echo(str("PROVISIONAL_ESTIMATE trough_foot_height_mm=", trough_foot_height(params)));
  echo(str("PROVISIONAL_ESTIMATE drive_axis_z_mm=", drive_axis_z(params)));
  echo(str("PROVISIONAL_ESTIMATE roller_relief_depth_mm=", roller_relief_depth(params)));
  echo(str("PROVISIONAL_ESTIMATE drive_bearing_cheek_thickness_mm=", drive_bearing_cheek_thickness(params)));
  cube([1, 1, 1]);
} else if (build_target == "report") {
  dimension_report(params);
  echo(str("PROVISIONAL_ESTIMATE trough_foot_height_mm=", trough_foot_height(params)));
  echo(str("PROVISIONAL_ESTIMATE drive_axis_z_mm=", drive_axis_z(params)));
  echo(str("PROVISIONAL_ESTIMATE roller_relief_depth_mm=", roller_relief_depth(params)));
  echo(str("PROVISIONAL_ESTIMATE drive_bearing_cheek_thickness_mm=", drive_bearing_cheek_thickness(params)));
  cube([1, 1, 1]);
} else {
  conveyor_trough(params);
}
