// motor_mount_bracket.scad
// Flat bracket for NEMA17 mount with slotted frame attachment holes.
//
// Orientation: bracket is a flat plate, print with Z = thickness direction.
// Motor face is the -Z face (bottom when printing) so pilot pocket is on the bed side.
// All holes print vertically with no support needed.
//
// Slot axis: Y (elongated in Y).
// Belt tension direction: slide bracket in +Y to increase center distance.
// Frame bolts at fixed Y positions pass through the Y-oriented slots.
// Tighten frame bolts to lock bracket in place after tensioning.
//
// Hardware needed:
//   4x M3 screws into motor face (NEMA17 M3 tapped holes)
//   4x M3 bolts + nuts for frame attachment through the slots

include <rollers_params.scad>

module motor_mount_bracket(params = default_params()) {
  validate(params);

  w  = pget(params, "provisional_bracket_width_mm");
  h  = pget(params, "provisional_bracket_height_mm");
  t  = pget(params, "provisional_bracket_thickness_mm");

  pilot_d   = pget(params, "provisional_nema17_pilot_clear_d");
  hole_sp   = pget(params, "provisional_nema17_hole_spacing_mm");
  m3_d      = pget(params, "provisional_motor_m3_clear_d");

  slot_w    = pget(params, "provisional_frame_slot_w");
  slot_l    = pget(params, "provisional_frame_slot_l");
  slot_cx   = pget(params, "provisional_frame_slot_cx");
  slot_cy   = pget(params, "provisional_frame_slot_cy");

  slot_half = (slot_l - slot_w) / 2;

  difference() {
    // Main plate, centered at origin in XY
    translate([-w/2, -h/2, 0])
      cube([w, h, t]);

    // Pilot boss and shaft clearance at center (through)
    translate([0, 0, -0.1])
      cylinder(h = t + 0.2, d = pilot_d, $fn = 64);

    // NEMA17 M3 motor mounting holes
    for (xi = [-1, 1])
      for (yi = [-1, 1])
        translate([xi * hole_sp/2, yi * hole_sp/2, -0.1])
          cylinder(h = t + 0.2, d = m3_d, $fn = 32);

    // Frame attachment slots, Y-oriented, one per corner quadrant
    for (xi = [-1, 1])
      for (yi = [-1, 1])
        translate([xi * slot_cx, yi * slot_cy, -0.1])
          hull() {
            translate([0, -slot_half, 0])
              cylinder(h = t + 0.2, d = slot_w, $fn = 32);
            translate([0,  slot_half, 0])
              cylinder(h = t + 0.2, d = slot_w, $fn = 32);
          }
  }
}

build_target = is_undef(build_target) ? "model" : build_target;
params = is_undef(params) ? default_params() : params;

if (build_target == "model") {
  motor_mount_bracket(params);
} else if (build_target == "validate") {
  validate(params);
  cube([1, 1, 1]);
} else {
  motor_mount_bracket(params);
}
