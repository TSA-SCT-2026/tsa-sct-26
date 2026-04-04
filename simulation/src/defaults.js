// defaults.js
// Keep this file aligned with firmware/src/config.h defaults.

'use strict';

export const BIN_LABELS = ['2x2 RED', '2x2 BLUE', '2x3 BLUE', '2x3 RED'];
export const BIN_DIRECTIONS = ['NW (315)', 'NE (45)', 'SE (135)', 'SW (225)'];
export const CATEGORY_TO_BIN = {
  '2x2_red': 0,
  '2x2_blue': 1,
  '2x3_blue': 2,
  '2x3_red': 3,
};
export const CATEGORY_COLOR = {
  '2x2_red': '#E24B4A',
  '2x2_blue': '#185FA5',
  '2x3_blue': '#185FA5',
  '2x3_red': '#E24B4A',
};

export const DEFAULTS = {
  // Conveyor and chamber timing
  belt_target_mm_s: 100,      // CONVEYOR_TARGET_SPEED_MM_S
  transport_distance_mm: 120, // docs timing budget basis
  entry_to_stop_ms: 10,
  feed_timeout_ms: 5000,
  approach_timeout_ms: 500,
  settle_ms: 50,

  // Sensing
  size_read_ms: 5,
  color_sample_count: 8,
  color_min_samples: 4,
  color_integration_ms: 24,
  color_timeout_ms: 500,

  // Chute selector stepper
  stepper_steps_per_rev: 1600,
  stepper_run_sps: 400,
  stepper_start_sps: 100,
  stepper_accel_sps: 50,
  stepper_decel_steps: 50,
  rehome_period_bricks: 8,

  // Release and confirmation
  solenoid_on_ms: 80,
  fall_settle_ms: 400,
  platform_return_ms: 300,
  bin_confirm_latency_ms: 50,
  bin_confirm_timeout_ms: 1000,
  token_restore_ms: 20,

  // Thermal
  thermal_heat_per_sol: 0.15,
  thermal_heat_per_step: 0.05,
  thermal_cooling_pct: 50,
  thermal_warn_level: 0.60,
  thermal_danger_level: 0.85,

  // Brick geometry and counts
  brick_2x2_mm: 15.8,
  brick_2x3_mm: 23.7,
  total_bricks: 24,
  counts: {
    '2x2_red': 6,
    '2x2_blue': 6,
    '2x3_blue': 8,
    '2x3_red': 4,
  },

  // Disc absolute positions in steps [bin1, bin2, bin3, bin4]
  bin_steps: [1400, 200, 600, 1000],
  home_bin_index: 3,

  // Multi-run and sequencing
  num_runs: 1,
  sequence: 'interleaved', // interleaved | worst_case | default_first | random | worst_case_accuracy
  interrun_gap_ms: 5000,
};

export const SECTIONS = [
  {
    id: 'timing',
    label: 'Timing',
    controls: [
      {
        id: 'belt_target_mm_s',
        label: 'Belt speed',
        unit: 'mm/s',
        min: 60,
        max: 260,
        step: 10,
        desc: 'Conveyor transport speed from feed chute to chamber.',
      },
      {
        id: 'transport_distance_mm',
        label: 'Feed distance',
        unit: 'mm',
        min: 80,
        max: 180,
        step: 5,
        desc: 'Distance from feed to chamber stop wall.',
      },
      {
        id: 'entry_to_stop_ms',
        label: 'Entry to stop',
        unit: 'ms',
        min: 5,
        max: 40,
        step: 1,
        desc: 'Delay between entry beam trigger and seated confirmation.',
      },
      {
        id: 'settle_ms',
        label: 'Settle time',
        unit: 'ms',
        min: 20,
        max: 120,
        step: 5,
        desc: 'Static settle before size and color sensing.',
      },
      {
        id: 'token_restore_ms',
        label: 'Token restore',
        unit: 'ms',
        min: 5,
        max: 60,
        step: 5,
        desc: 'Reset gap before next feed cycle starts.',
      },
    ],
  },
  {
    id: 'sensing',
    label: 'Sensing',
    controls: [
      {
        id: 'size_read_ms',
        label: 'Size read time',
        unit: 'ms',
        min: 1,
        max: 20,
        step: 1,
        desc: 'Dual beam evaluation time in static chamber.',
      },
      {
        id: 'color_sample_count',
        label: 'Color samples',
        unit: 'samples',
        min: 2,
        max: 16,
        step: 1,
        desc: 'Number of color-module samples to average.',
      },
      {
        id: 'color_integration_ms',
        label: 'Integration per sample',
        unit: 'ms',
        min: 2,
        max: 40,
        step: 1,
        desc: 'Per-sample integration period.',
      },
      {
        id: 'color_timeout_ms',
        label: 'Color timeout',
        unit: 'ms',
        min: 120,
        max: 1000,
        step: 20,
        desc: 'Maximum sensing window before sensor fault.',
      },
      {
        id: 'color_min_samples',
        label: 'Minimum valid samples',
        unit: 'samples',
        min: 2,
        max: 8,
        step: 1,
        desc: 'Below this threshold the run halts with SENSOR_FAULT.',
      },
    ],
  },
  {
    id: 'stepper',
    label: 'Chute Selector',
    controls: [
      {
        id: 'stepper_run_sps',
        label: 'Run speed',
        unit: 'steps/s',
        min: 200,
        max: 2200,
        step: 50,
        desc: 'Disc indexing speed at nominal thermal state.',
      },
      {
        id: 'stepper_start_sps',
        label: 'Start speed',
        unit: 'steps/s',
        min: 50,
        max: 500,
        step: 10,
        desc: 'Ramp entry and exit speed.',
      },
      {
        id: 'stepper_steps_per_rev',
        label: 'Steps per rev',
        unit: 'steps',
        min: 1600,
        max: 1600,
        step: 1,
        locked: true,
        desc: 'Fixed by firmware microstepping config.',
      },
      {
        id: 'stepper_decel_steps',
        label: 'Decel steps',
        unit: 'steps',
        min: 10,
        max: 200,
        step: 5,
        desc: 'Ramp distance used for accel and decel estimation.',
      },
      {
        id: 'rehome_period_bricks',
        label: 'Rehome period',
        unit: 'bricks',
        min: 2,
        max: 24,
        step: 1,
        desc: 'Periodic home verification cadence.',
      },
    ],
  },
  {
    id: 'release',
    label: 'Trapdoor Release',
    controls: [
      {
        id: 'solenoid_on_ms',
        label: 'Solenoid pulse',
        unit: 'ms',
        min: 30,
        max: 160,
        step: 5,
        desc: '0530-series solenoid pulse width for lever actuation.',
      },
      {
        id: 'fall_settle_ms',
        label: 'Fall and settle',
        unit: 'ms',
        min: 120,
        max: 800,
        step: 20,
        desc: 'Total release window including platform return.',
      },
      {
        id: 'platform_return_ms',
        label: 'Platform return target',
        unit: 'ms',
        min: 100,
        max: 500,
        step: 10,
        desc: 'Expected platform relatch timing.',
      },
      {
        id: 'bin_confirm_latency_ms',
        label: 'Bin confirm latency',
        unit: 'ms',
        min: 20,
        max: 200,
        step: 5,
        desc: 'Expected beam confirm delay after drop completes.',
      },
      {
        id: 'bin_confirm_timeout_ms',
        label: 'Bin confirm timeout',
        unit: 'ms',
        min: 200,
        max: 1500,
        step: 20,
        desc: 'Maximum wait before MISS_BIN halt.',
      },
    ],
  },
  {
    id: 'thermal',
    label: 'Thermal',
    controls: [
      {
        id: 'thermal_heat_per_sol',
        label: 'Heat per release',
        unit: '',
        min: 0.01,
        max: 0.40,
        step: 0.01,
        dec: 2,
        desc: 'Heat increment applied when the release solenoid fires.',
      },
      {
        id: 'thermal_heat_per_step',
        label: 'Heat per index unit',
        unit: '',
        min: 0.01,
        max: 0.20,
        step: 0.01,
        dec: 2,
        desc: 'Heat increment scaled by stepper move distance.',
      },
      {
        id: 'thermal_cooling_pct',
        label: 'Cooling rate',
        unit: '%',
        min: 0,
        max: 100,
        step: 1,
        desc: '0 means no cooling, 50 means half per second.',
      },
      {
        id: 'thermal_warn_level',
        label: 'Warning threshold',
        unit: '',
        min: 0.30,
        max: 0.90,
        step: 0.05,
        dec: 2,
        desc: 'Crossing this threshold slows indexing.',
      },
      {
        id: 'thermal_danger_level',
        label: 'Danger threshold',
        unit: '',
        min: 0.45,
        max: 1.00,
        step: 0.05,
        dec: 2,
        desc: 'Crossing this threshold applies stronger thermal throttling.',
      },
    ],
  },
  {
    id: 'run',
    label: 'Run Settings',
    controls: [],
  },
];
