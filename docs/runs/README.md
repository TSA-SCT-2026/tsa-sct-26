# Run Logs

Store CSV logs from calibration and reliability runs in this directory.

Naming recommendation:
- `run_YYYYMMDD_HHMM_session_label.csv`

Required fields for sorting and replay workflow:
- `run_id,brick,state,ts_state_ms,ts_entry_ms,ts_seated_ms,ts_sensed_ms,ts_selector_ready_ms,ts_release_ms,ts_bin_confirm_ms,ts_reset_confirm_ms,ts_chamber_clear_ms,ts_pitch_advance_ms,category,size,color,target_bin,actual_bin,samples,selector_index_steps,selector_index_ms,release_actuator_on_ms,thermal_state,error_code`

Use one row per brick and keep the timestamp chain complete enough to replay the queue-fed cadence and reset sequence.
