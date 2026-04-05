# Short TODO

Updated April 5, 2026 after architecture reset.
Current priority: align the repo to the queue-fed chamber sorter that we are actually building.

## Immediate priorities

1. Sync architecture truth
- Remove old direct-drive conveyor assumptions
- Remove old full-transport-per-brick timing assumptions
- Make chamber pitch, reset confirmation, and weighted selector travel the active model

2. Lock conveyor production path
- Build the conveyor around off-axis NEMA17 drive, toothed timing belt, supported roller shaft, and smooth drive roller
- Freeze packaging interfaces early enough that chamber and frame work do not drift

3. Repair firmware control contract
- Align events, state machine, actuators, logger, harness, and docs to the same physical-truth model
- Treat compile consistency as a gate before any optimization pass

4. Protect the selector decision
- Keep the 4-index selector chute active for now
- Add the selector evidence gate, routing study, and bench-test expectations before final production freeze

5. Raise documentation quality
- Add local instruction files where missing
- Expand notebook alternatives, calibration, testing, and run-log expectations
- Keep operator-facing physical UX explicit in every subsystem
