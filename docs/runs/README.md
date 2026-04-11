# Run Logs

Store CSV logs from calibration and reliability runs in this directory.

Naming recommendation:
- `run_YYYYMMDD_HHMM_session_label.csv`

Required fields for the states sorting workflow:
- `run_id,brick,ts_feed_ms,ts_size_sense_ms,ts_color_sense_ms,ts_route_command_ms,ts_route_ready_ms,ts_handoff_ms,size,color,target_bin,actual_bin,servo_angle,conveyor_speed_mm_s,samples,error_code,notes`

Use one row per brick and keep the timestamp chain complete enough to replay one-at-a-time conveyor feed, sensing, routing, and handoff.
