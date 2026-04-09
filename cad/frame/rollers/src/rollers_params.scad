$fn = 128;

// Canonical parameters for the conveyor timing stage and roller models.
// LOCKED_TRUTH values derive from LEGO geometry and long-side-across transport rule.
// PROVISIONAL_ESTIMATE values depend on purchased hardware not yet measured.

function kv(key, value) = [key, value];

function default_params() = [
  kv("truth_locked_brick_width", 15.8),
  kv("truth_locked_brick_2x2_length", 15.8),
  kv("truth_locked_brick_2x3_length", 23.7),
  kv("truth_locked_brick_height", 11.4),
  kv("truth_locked_orientation_long_side_across", 1),
  kv("truth_locked_channel_width", 25.0),
  kv("provisional_drive_roller_od", 25.0),
  kv("provisional_drive_roller_face_width", 25.0),
  kv("provisional_drive_roller_flange_width", 2.0),
  kv("provisional_drive_roller_flange_od", 27.0),
  kv("provisional_belt_strip_selected_width", 25.0),
  kv("provisional_drive_roller_shaft_round", 5.20),
  kv("provisional_drive_roller_shaft_to_flat", 4.50),
  kv("provisional_drive_roller_split_slot", 1.8),
  kv("provisional_drive_roller_clamp_axis_x", 6.0),
  kv("provisional_drive_roller_clamp_clear_d", 3.2),
  kv("provisional_drive_roller_clamp_head_d", 5.8),
  kv("provisional_drive_roller_clamp_head_depth", 3.0),
  kv("provisional_drive_roller_clamp_nut_flat", 5.7),
  kv("provisional_drive_roller_clamp_nut_thickness", 2.6),
  kv("provisional_drive_roller_hub_od", 20.0),
  kv("provisional_drive_roller_hub_width", 10.0),

  kv("provisional_motor_pulley_teeth", 20),
  kv("provisional_drive_pulley_teeth", 20),
  kv("provisional_timing_belt_pitch_mm", 2.0),
  kv("provisional_timing_belt_width_mm", 6.0),
  kv("provisional_timing_center_distance_mm", 80.0),
  kv("provisional_tension_adjust_travel_mm", 8.0),
  kv("provisional_timing_tooth_depth_mm", 1.20),
  kv("provisional_motor_pulley_bore_round", 5.20),
  kv("provisional_motor_pulley_bore_to_flat", 4.50),
  kv("provisional_motor_pulley_hub_od", 20.0),
  kv("provisional_motor_pulley_hub_width", 10.0),

  kv("provisional_idler_roller_od", 25.0),
  kv("provisional_contact_face_width", 25.0),
  kv("provisional_flange_width", 2.0),
  kv("provisional_flange_od", 27.0),
  kv("provisional_crown_sagitta", 0.50),
  kv("provisional_bearing_od", 8.0),
  kv("provisional_bearing_width", 2.5),
  kv("provisional_bearing_pocket_d", 8.15),
  kv("provisional_bearing_pocket_depth", 2.60),
  kv("provisional_bearing_lead_chamfer", 0.35),
  kv("provisional_axle_clear_d", 5.20),
  kv("provisional_drive_shaft_bearing_span_mm", 38.0),

  kv("provisional_magnet_d", 3.20),
  kv("provisional_magnet_depth", 2.20),
  kv("provisional_magnet_radial_offset", 9.60),

  kv("provisional_coupon_block_height", 8.0),
  kv("provisional_coupon_spacing", 4.0),

  // Motor mount bracket
  kv("provisional_bracket_width_mm", 70.0),
  kv("provisional_bracket_height_mm", 70.0),
  kv("provisional_bracket_thickness_mm", 4.0),
  kv("provisional_nema17_pilot_clear_d", 22.4),
  kv("provisional_nema17_hole_spacing_mm", 31.0),
  kv("provisional_motor_m3_clear_d", 3.4),
  kv("provisional_frame_slot_w", 3.5),
  kv("provisional_frame_slot_l", 12.0),
  kv("provisional_frame_slot_cx", 28.0),
  kv("provisional_frame_slot_cy", 28.0),

  // Bearing block
  kv("provisional_bearing_block_x_mm", 14.0),
  kv("provisional_bearing_block_y_mm", 16.0),
  kv("provisional_bearing_block_z_mm", 22.0),
  kv("provisional_bearing_block_m3_z_offset", 7.0)
];

function pget(params, key, fallback = undef, i = 0) =
  i >= len(params)
    ? fallback
    : (params[i][0] == key ? params[i][1] : pget(params, key, fallback, i + 1));

function crown_edge_diameter(center_od, crown_sagitta) = center_od - (2 * crown_sagitta);

function crown_radius(face_width, crown_sagitta) =
  ((face_width * face_width) / (8 * crown_sagitta)) + (crown_sagitta / 2);

function pulley_pitch_diameter(teeth, pitch_mm) = (teeth * pitch_mm) / PI;
function pulley_outer_diameter(teeth, pitch_mm, tooth_depth_mm) =
  pulley_pitch_diameter(teeth, pitch_mm) + (2 * tooth_depth_mm);
function timing_ratio(driver_teeth, driven_teeth) = driven_teeth / driver_teeth;

module validate(params = default_params()) {
  assert(pget(params, "truth_locked_orientation_long_side_across") == 1,
    "LOCKED_TRUTH failed: bricks must run long-side-across the belt");
  assert(pget(params, "truth_locked_brick_width") == 15.8,
    "LOCKED_TRUTH failed: brick width changed");
  assert(pget(params, "truth_locked_channel_width") == 25.0,
    "LOCKED_TRUTH failed: channel width changed");

  assert(pget(params, "provisional_contact_face_width") <= pget(params, "truth_locked_channel_width"),
    "Contact face can not exceed channel width");
  assert(pget(params, "provisional_drive_roller_face_width") <= pget(params, "truth_locked_channel_width"),
    "Drive roller face can not exceed channel width");
  assert(pget(params, "provisional_belt_strip_selected_width") > 0,
    "Selected belt strip width must be positive");
  assert(pget(params, "provisional_belt_strip_selected_width") <= pget(params, "truth_locked_channel_width"),
    "Selected belt strip width can not exceed channel width");
  assert(pget(params, "provisional_drive_roller_shaft_to_flat") < pget(params, "provisional_drive_roller_shaft_round"),
    "D bore flat dimension must be less than round diameter");
  assert(pget(params, "provisional_drive_roller_clamp_axis_x") > (pget(params, "provisional_drive_roller_shaft_round") / 2),
    "Clamp screw axis must stay outside the shaft bore");
  assert(pget(params, "provisional_drive_roller_clamp_axis_x") < ((pget(params, "provisional_drive_roller_hub_od") / 2) - (pget(params, "provisional_drive_roller_clamp_clear_d") / 2)),
    "Clamp screw axis must stay inside the hub wall");
  assert((pget(params, "provisional_drive_roller_hub_od") / 2) - pget(params, "provisional_drive_roller_clamp_axis_x") - (pget(params, "provisional_drive_roller_clamp_clear_d") / 2) >= 1.5,
    "Hub outer wall beside clamp screw is too thin");
  assert(pget(params, "provisional_drive_roller_clamp_axis_x") - (pget(params, "provisional_drive_roller_clamp_clear_d") / 2) - (pget(params, "provisional_drive_roller_shaft_round") / 2) >= 1.5,
    "Hub inner wall between shaft bore and clamp screw is too thin");
  assert(pget(params, "provisional_bearing_pocket_d") >= pget(params, "provisional_bearing_od"),
    "Bearing pocket must be at least nominal bearing OD");
  assert(pget(params, "provisional_flange_width") > 0,
    "Flange width must be positive");
  assert(pget(params, "provisional_motor_pulley_teeth") > 0,
    "Motor pulley tooth count must be positive");
  assert(pget(params, "provisional_drive_pulley_teeth") > 0,
    "Drive pulley tooth count must be positive");
  assert(pget(params, "provisional_timing_belt_pitch_mm") > 0,
    "Timing belt pitch must be positive");
  assert(pget(params, "provisional_timing_belt_width_mm") > 0,
    "Timing belt width must be positive");
  assert(pget(params, "provisional_timing_center_distance_mm") > 0,
    "Timing center distance must be positive");
  assert(pget(params, "provisional_tension_adjust_travel_mm") > 0,
    "Tension travel must be positive");
  assert(pget(params, "provisional_timing_center_distance_mm") >
      ((pulley_pitch_diameter(pget(params, "provisional_motor_pulley_teeth"), pget(params, "provisional_timing_belt_pitch_mm")) +
        pulley_pitch_diameter(pget(params, "provisional_drive_pulley_teeth"), pget(params, "provisional_timing_belt_pitch_mm"))) / 2),
    "Timing center distance must exceed combined pitch radii");

  if (pget(params, "provisional_drive_roller_shaft_round") == 5.20)
    echo("PROVISIONAL_ESTIMATE: shaft clamp bore round diameter set to 5.20mm");
  if (pget(params, "provisional_bearing_pocket_d") == 8.15)
    echo("PROVISIONAL_ESTIMATE: MR85 pocket diameter set to 8.15mm");
  if (pget(params, "provisional_crown_sagitta") == 0.50)
    echo("PROVISIONAL_ESTIMATE: crown sagitta set to 0.50mm");
}

module dimension_report(params = default_params()) {
  face_w = pget(params, "provisional_contact_face_width");
  crown_h = pget(params, "provisional_crown_sagitta");
  crown_r = crown_radius(face_w, crown_h);
  motor_teeth = pget(params, "provisional_motor_pulley_teeth");
  drive_teeth = pget(params, "provisional_drive_pulley_teeth");
  pitch_mm = pget(params, "provisional_timing_belt_pitch_mm");
  motor_pitch_d = pulley_pitch_diameter(motor_teeth, pitch_mm);
  drive_pitch_d = pulley_pitch_diameter(drive_teeth, pitch_mm);

  echo("ROLLER_DIMENSION_REPORT_START");
  echo(str("LOCKED_TRUTH brick_width=", pget(params, "truth_locked_brick_width")));
  echo(str("LOCKED_TRUTH channel_width=", pget(params, "truth_locked_channel_width")));
  echo(str("PROVISIONAL_ESTIMATE belt_strip_selected_width=", pget(params, "provisional_belt_strip_selected_width")));
  echo(str("PROVISIONAL_ESTIMATE drive_roller_od=", pget(params, "provisional_drive_roller_od")));
  echo(str("PROVISIONAL_ESTIMATE idler_roller_od=", pget(params, "provisional_idler_roller_od")));
  echo(str("PROVISIONAL_ESTIMATE contact_face_width=", face_w));
  echo(str("PROVISIONAL_ESTIMATE drive_roller_face_width=", pget(params, "provisional_drive_roller_face_width")));
  echo(str("PROVISIONAL_ESTIMATE flange_width=", pget(params, "provisional_flange_width")));
  echo(str("PROVISIONAL_ESTIMATE flange_od=", pget(params, "provisional_flange_od")));
  echo(str("PROVISIONAL_ESTIMATE d_bore_round=", pget(params, "provisional_drive_roller_shaft_round")));
  echo(str("PROVISIONAL_ESTIMATE d_bore_to_flat=", pget(params, "provisional_drive_roller_shaft_to_flat")));
  echo(str("PROVISIONAL_ESTIMATE clamp_screw_axis_x=", pget(params, "provisional_drive_roller_clamp_axis_x")));
  echo(str("PROVISIONAL_ESTIMATE bearing_pocket_d=", pget(params, "provisional_bearing_pocket_d")));
  echo(str("PROVISIONAL_ESTIMATE crown_sagitta=", crown_h));
  echo(str("PROVISIONAL_ESTIMATE crown_radius=", crown_r));
  echo(str("PROVISIONAL_ESTIMATE motor_pulley_teeth=", motor_teeth));
  echo(str("PROVISIONAL_ESTIMATE drive_pulley_teeth=", drive_teeth));
  echo(str("PROVISIONAL_ESTIMATE timing_pitch_mm=", pitch_mm));
  echo(str("PROVISIONAL_ESTIMATE motor_pitch_diameter=", motor_pitch_d));
  echo(str("PROVISIONAL_ESTIMATE drive_pitch_diameter=", drive_pitch_d));
  echo(str("PROVISIONAL_ESTIMATE timing_ratio=", timing_ratio(motor_teeth, drive_teeth)));
  echo(str("PROVISIONAL_ESTIMATE center_distance_mm=", pget(params, "provisional_timing_center_distance_mm")));
  echo(str("PROVISIONAL_ESTIMATE tension_adjust_travel_mm=", pget(params, "provisional_tension_adjust_travel_mm")));
  echo("ROLLER_DIMENSION_REPORT_END");
}

module d_bore_cutout(depth, round_d, to_flat) {
  flat_x = (round_d / 2) - to_flat;
  difference() {
    cylinder(h = depth, d = round_d);
    translate([flat_x - 20, -20, -0.5])
      cube([20, 40, depth + 1]);
  }
}
