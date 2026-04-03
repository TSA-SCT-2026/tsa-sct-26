$fn = 128;

// Canonical parameters for roller prototype models.
// LOCKED_TRUTH values derive from LEGO geometry and widthwise transport rule.
// PROVISIONAL_ESTIMATE values depend on purchased hardware not yet measured.

function kv(key, value) = [key, value];

function default_params() = [
  kv("truth_locked_brick_width", 15.8),
  kv("truth_locked_brick_2x2_length", 15.8),
  kv("truth_locked_brick_2x3_length", 23.7),
  kv("truth_locked_brick_height", 11.4),
  kv("truth_locked_orientation_widthwise", 1),
  kv("truth_locked_channel_width", 20.0),
  kv("truth_locked_belt_strip_target", 19.0),

  kv("provisional_drive_roller_od", 25.0),
  kv("provisional_idler_roller_od", 25.0),
  kv("provisional_contact_face_width", 20.0),
  kv("provisional_flange_width", 2.0),
  kv("provisional_flange_od", 27.0),

  kv("provisional_d_bore_round", 6.20),
  kv("provisional_d_bore_to_flat", 5.70),
  kv("provisional_split_clamp_slot", 1.8),
  kv("provisional_clamp_screw_clear_d", 3.2),
  kv("provisional_clamp_insert_od", 4.6),
  kv("provisional_clamp_insert_depth", 5.5),
  kv("provisional_drive_hub_od", 14.0),
  kv("provisional_drive_hub_width", 10.0),

  kv("provisional_crown_sagitta", 0.50),
  kv("provisional_bearing_od", 11.0),
  kv("provisional_bearing_width", 4.0),
  kv("provisional_bearing_pocket_d", 11.15),
  kv("provisional_bearing_pocket_depth", 4.10),
  kv("provisional_bearing_lead_chamfer", 0.50),
  kv("provisional_axle_clear_d", 5.20),

  kv("provisional_magnet_d", 3.20),
  kv("provisional_magnet_depth", 2.20),
  kv("provisional_magnet_radial_offset", 9.60),

  kv("provisional_coupon_block_height", 8.0),
  kv("provisional_coupon_spacing", 4.0)
];

function pget(params, key, fallback = undef, i = 0) =
  i >= len(params)
    ? fallback
    : (params[i][0] == key ? params[i][1] : pget(params, key, fallback, i + 1));

function crown_edge_diameter(center_od, crown_sagitta) = center_od - (2 * crown_sagitta);

function crown_radius(face_width, crown_sagitta) =
  ((face_width * face_width) / (8 * crown_sagitta)) + (crown_sagitta / 2);

module validate(params = default_params()) {
  assert(pget(params, "truth_locked_orientation_widthwise") == 1,
    "LOCKED_TRUTH failed: bricks must run widthwise across belt");
  assert(pget(params, "truth_locked_brick_width") == 15.8,
    "LOCKED_TRUTH failed: brick width changed");
  assert(pget(params, "truth_locked_channel_width") == 20.0,
    "LOCKED_TRUTH failed: channel width changed");
  assert(pget(params, "truth_locked_belt_strip_target") == 19.0,
    "LOCKED_TRUTH failed: belt strip target changed");

  assert(pget(params, "provisional_contact_face_width") <= pget(params, "truth_locked_channel_width"),
    "Contact face can not exceed channel width");
  assert(pget(params, "provisional_d_bore_to_flat") < pget(params, "provisional_d_bore_round"),
    "D bore flat dimension must be less than round diameter");
  assert(pget(params, "provisional_bearing_pocket_d") >= pget(params, "provisional_bearing_od"),
    "Bearing pocket must be at least nominal bearing OD");
  assert(pget(params, "provisional_flange_width") > 0,
    "Flange width must be positive");

  if (pget(params, "provisional_d_bore_round") == 6.20)
    echo("PROVISIONAL_ESTIMATE: D bore round diameter set to 6.20mm");
  if (pget(params, "provisional_bearing_pocket_d") == 11.15)
    echo("PROVISIONAL_ESTIMATE: MR115 pocket diameter set to 11.15mm");
  if (pget(params, "provisional_crown_sagitta") == 0.50)
    echo("PROVISIONAL_ESTIMATE: crown sagitta set to 0.50mm");
}

module dimension_report(params = default_params()) {
  face_w = pget(params, "provisional_contact_face_width");
  crown_h = pget(params, "provisional_crown_sagitta");
  crown_r = crown_radius(face_w, crown_h);

  echo("ROLLER_DIMENSION_REPORT_START");
  echo(str("LOCKED_TRUTH brick_width=", pget(params, "truth_locked_brick_width")));
  echo(str("LOCKED_TRUTH channel_width=", pget(params, "truth_locked_channel_width")));
  echo(str("LOCKED_TRUTH belt_strip_target=", pget(params, "truth_locked_belt_strip_target")));
  echo(str("PROVISIONAL_ESTIMATE drive_roller_od=", pget(params, "provisional_drive_roller_od")));
  echo(str("PROVISIONAL_ESTIMATE idler_roller_od=", pget(params, "provisional_idler_roller_od")));
  echo(str("PROVISIONAL_ESTIMATE contact_face_width=", face_w));
  echo(str("PROVISIONAL_ESTIMATE flange_width=", pget(params, "provisional_flange_width")));
  echo(str("PROVISIONAL_ESTIMATE flange_od=", pget(params, "provisional_flange_od")));
  echo(str("PROVISIONAL_ESTIMATE d_bore_round=", pget(params, "provisional_d_bore_round")));
  echo(str("PROVISIONAL_ESTIMATE d_bore_to_flat=", pget(params, "provisional_d_bore_to_flat")));
  echo(str("PROVISIONAL_ESTIMATE bearing_pocket_d=", pget(params, "provisional_bearing_pocket_d")));
  echo(str("PROVISIONAL_ESTIMATE crown_sagitta=", crown_h));
  echo(str("PROVISIONAL_ESTIMATE crown_radius=", crown_r));
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
