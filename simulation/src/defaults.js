// defaults.js - DEFAULTS and SECTIONS definitions
// This is the only file that syncs with firmware/src/config.h
// When you change a value in config.h, update the matching entry here.

'use strict';

export const DEFAULTS = {
  // Escapement
  stepper_sps:          800,
  stepper_steps_per_rev: 200,

  // Belt
  belt_target_mm_s:     200,
  motor_noload_rpm:     600,
  motor_load_factor:    0.65,

  // Sensing
  beam_gap_mm:          19,
  size_timeout_ms:      150,
  color_integration_ms: 2,

  // Solenoid
  sol_full_ms:          20,
  sol_hold_pct:         40,
  sol_deenergize_ms:    280,
  sol_lead_ms:          150,

  // Thermal
  thermal_heat_per_sol:    0.15,
  thermal_heat_per_step:   0.05,
  thermal_cooling_pct:     50,
  thermal_warn_level:      0.60,
  thermal_danger_level:    0.85,

  // Geometry (from MECHANICAL.md / EMBEDDED.md)
  brick_2x2_mm:         15.8,
  brick_2x3_mm:         23.7,
  brick_spacing_mm:     28,
  total_bricks:         24,

  // Bin distances from sensing zone (estimates - update from CAD)
  bin_distances_mm:     [250, 280, 310, 360],

  // Brick counts by type (bin order: 2x2_blue, 2x2_red, 2x3_red, 2x3_blue)
  counts: { '2x2_blue': 6, '2x2_red': 6, '2x3_red': 4, '2x3_blue': 8 },

  // Plow assignment: 0 = default path (no fire)
  plow_map: { '2x2_blue': 1, '2x2_red': 2, '2x3_red': 3, '2x3_blue': 0 },

  // Run settings
  num_runs:   1,
  sequence:   'interleaved',  // 'interleaved' | 'worst_case' | 'default_first' | 'random'
  interrun_gap_ms: 5000,
};

export const SECTIONS = [
  {
    id: 'escapement', label: 'Escapement',
    controls: [
      { id:'stepper_sps', label:'Release speed', unit:'sps', min:200, max:2000, step:50,
        desc:'How fast the stepper spins to release one brick. Higher = more bricks/sec but fewer color samples.' },
      { id:'stepper_steps_per_rev', label:'Steps per revolution', unit:'steps', min:200, max:200, step:1, locked:true,
        desc:'Full-step NEMA 11 (1.8 deg/step). Fixed - shown for reference only.' },
    ]
  },
  {
    id: 'belt', label: 'Belt',
    controls: [
      { id:'belt_target_mm_s', label:'Belt speed', unit:'mm/s', min:50, max:400, step:10,
        desc:'How fast bricks travel from sensor to bins. Must stay ahead of the escapement rate.' },
      { id:'motor_load_factor', label:'Motor load factor', unit:'%', min:40, max:95, step:1,
        desc:'How much the motor slows under real load vs no-load spec. 65% is a realistic starting estimate.',
        scale:100 },
      { id:'motor_noload_rpm', label:'Motor no-load RPM', unit:'RPM', min:100, max:1000, step:50,
        desc:'JGB37-520 at 6V is rated 600 RPM no-load. Used to compute belt speed at load.' },
    ]
  },
  {
    id: 'sensing', label: 'Sensing Zone',
    controls: [
      { id:'beam_gap_mm', label:'Beam spacing', unit:'mm', min:10, max:30, step:1,
        desc:'Physical distance between the two size detection beams. Fixed in CAD.' },
      { id:'size_timeout_ms', label:'Size timeout', unit:'ms', min:50, max:300, step:5,
        desc:'If beam 2 does not break within this window the brick is classified 2x2. Must be above 95ms at 200mm/s.' },
      { id:'color_integration_ms', label:'Color integration time', unit:'ms', min:2, max:10, step:1,
        desc:'How long the color sensor takes per reading. TCS34725 minimum is ~2.4ms. Longer = fewer total samples.' },
    ]
  },
  {
    id: 'solenoid', label: 'Solenoid Timing',
    controls: [
      { id:'sol_full_ms', label:'Full power duration', unit:'ms', min:5, max:50, step:5,
        desc:'How long the solenoid gets full voltage to reach full extension. Too short = incomplete stroke.' },
      { id:'sol_hold_pct', label:'Hold duty cycle', unit:'%', min:20, max:100, step:5,
        desc:'PWM level to hold plow in position after full extension. 40% holds against spring, reduces heat.' },
      { id:'sol_deenergize_ms', label:'De-energize time', unit:'ms', min:100, max:500, step:10,
        desc:'Total time solenoid stays on before turning off. Must be long enough for brick to fully clear plow.' },
      { id:'sol_lead_ms', label:'Sensor-to-plow travel', unit:'ms', min:50, max:300, step:10,
        desc:'How long it takes a brick to travel from sensing zone to plow 1. Solenoid fires immediately on classification.' },
    ]
  },
  {
    id: 'thermal', label: 'Thermal Model',
    controls: [
      { id:'thermal_heat_per_sol', label:'Heat per solenoid fire', unit:'', min:0.01, max:0.5, step:0.01, dec:2,
        desc:'How much heat accumulates each time a plow fires. Tune after measuring real solenoid temperature.' },
      { id:'thermal_heat_per_step', label:'Heat per stepper release', unit:'', min:0.01, max:0.2, step:0.01, dec:2,
        desc:'Heat added each time the escapement releases a brick.' },
      { id:'thermal_cooling_pct', label:'Cooling rate', unit:'%', min:0, max:100, step:1, dec:0,
        desc:'How fast heat dissipates when idle. 0% = no cooling (heat stays). 100% = instant. 50% = heat halves every second.' },
      { id:'thermal_warn_level', label:'Warning threshold', unit:'', min:0.3, max:0.9, step:0.05, dec:2,
        desc:'Heat level at which escapement slows to WARNING rate (~2.7 bricks/sec).' },
      { id:'thermal_danger_level', label:'Danger threshold', unit:'', min:0.5, max:1.0, step:0.05, dec:2,
        desc:'Heat level at which escapement slows to DANGER rate (~1.3 bricks/sec).' },
    ]
  },
  {
    id: 'run', label: 'Run Settings',
    controls: [] // special: handled as selects/inputs
  }
];
