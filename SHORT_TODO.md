# Short TODO

Updated April 11, 2026 after the states simplification pivot.

Current priority: execute the simplified states CAD plan: manual one-at-a-time feed, conveyor sensing station, MG995/MG996-class servo rotary chute selector, and four labeled bins.

## Next Tasks

1. CAD weekend setup
- Download or import a proven NEMA17 mini conveyor assembly if possible
- Import the heavy servo reference from `docs/datasheet/motion/heavy_servo/`
- Keep the 610mm x 610mm boundary visible in Fusion
- Use wood or 3D printed frame construction, not 2020 extrusion

2. Build the custom states geometry
- Model servo mount, chute body, pivot boss or horn adapter, and bin guides
- Model color sensor shroud
- Model the two break-beam pairs in the sensing shroud
- Model four labeled bins under the chute arc

3. Validate the risky physical assumptions
- Test a short chute section with real bricks at 30, 35, 40, and 45 degrees
- Verify conveyor-to-chute handoff with real bricks before large prints
- Verify servo position overlap with each bin guide in CAD
- Confirm the full assembly stays under 610mm x 610mm

4. Prepare scoring evidence
- Capture CAD screenshots for the inventor's log
- Record break-beam size sensing rationale and test evidence
- Calibrate color sensing only with the shroud installed
- Log full 24-brick runs to CSV in `docs/runs/`
