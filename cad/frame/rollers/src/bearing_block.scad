// bearing_block.scad
// Single-bearing support block for the 5mm driven shaft.
// Two blocks are needed per shaft, spaced ~38-50mm apart (set from frame layout).
// Each block holds one MR85ZZ bearing (5mm ID, 8mm OD, 2.5mm wide).
//
// Coordinate system (assembled):
//   X = shaft axis direction
//   Y = lateral (left-right, perpendicular to shaft)
//   Z = vertical (up)
//   Shaft center is at the block center (origin when centered).
//
// Bearing pocket opens at the +X face (outer shaft end).
// M3 mounting holes run through the Y faces for side-wall frame mounting.
// Two M3 bolts pass through the block from the outer frame wall,
// captured by M3 nuts on the inner face.
//
// Print orientation: shaft axis vertical (X axis = print Z) gives the
// best circularity for the bearing pocket. Alternatively print flat and
// verify pocket fit with coupon before committing.
//
// Hardware needed per block:
//   1x MR85ZZ bearing
//   2x M3 bolt + nut

include <rollers_params.scad>

module bearing_block(params = default_params()) {
  validate(params);

  bx = pget(params, "provisional_bearing_block_x_mm");   // shaft axis depth
  by = pget(params, "provisional_bearing_block_y_mm");   // lateral width
  bz = pget(params, "provisional_bearing_block_z_mm");   // vertical height

  pocket_d       = pget(params, "provisional_bearing_pocket_d");
  pocket_depth   = pget(params, "provisional_bearing_pocket_depth");
  pocket_chamfer = pget(params, "provisional_bearing_lead_chamfer");
  shaft_d        = pget(params, "provisional_axle_clear_d");
  m3_z_offset    = pget(params, "provisional_bearing_block_m3_z_offset");
  m3_d           = 3.4;
  m3_nut_flat    = 5.5;
  m3_nut_depth   = 2.6;

  straight_depth = pocket_depth - pocket_chamfer;

  // Block centered at origin in all axes
  difference() {
    translate([-bx/2, -by/2, -bz/2])
      cube([bx, by, bz]);

    // Shaft bore through full X depth
    rotate([0, 90, 0])
      translate([0, 0, -bx/2 - 0.1])
        cylinder(h = bx + 0.2, d = shaft_d, $fn = 64);

    // Bearing pocket at +X face: lead chamfer then straight bore
    // Chamfer: widens toward the +X face
    translate([bx/2, 0, 0])
      rotate([0, 90, 0])
        cylinder(h = pocket_chamfer + 0.1,
                 d1 = pocket_d + 2*pocket_chamfer,
                 d2 = pocket_d,
                 $fn = 64);
    // Straight pocket (inward from chamfer end)
    translate([bx/2 - pocket_depth, 0, 0])
      rotate([0, 90, 0])
        cylinder(h = straight_depth + 0.1, d = pocket_d, $fn = 64);

    // M3 mounting holes through Y faces (for side-wall mounting)
    for (zs = [-1, 1])
      translate([0, -by/2 - 0.1, zs * m3_z_offset])
        rotate([-90, 0, 0])
          cylinder(h = by + 0.2, d = m3_d, $fn = 32);

    // M3 nut trap at -Y face (inner frame wall side, accessible for nut insertion)
    for (zs = [-1, 1])
      translate([0, -by/2, zs * m3_z_offset])
        rotate([-90, 0, 0])
          cylinder(h = m3_nut_depth + 0.1, d = m3_nut_flat / cos(30), $fn = 6);
  }
}

build_target = is_undef(build_target) ? "model" : build_target;
params = is_undef(params) ? default_params() : params;

if (build_target == "model") {
  bearing_block(params);
} else if (build_target == "validate") {
  validate(params);
  cube([1, 1, 1]);
} else {
  bearing_block(params);
}
