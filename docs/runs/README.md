# Run Logs

Store CSV logs from calibration and reliability runs in this directory.

Naming recommendation:
- `run_YYYYMMDD_HHMM_session_label.csv`

Required fields for sorting and replay workflow:
- `run_id,brick,state,ts_state_ms,category,size,color,target_bin,actual_bin,samples,index_steps,index_ms,solenoid_on_ms,disc_steps,thermal_state,error_code`
