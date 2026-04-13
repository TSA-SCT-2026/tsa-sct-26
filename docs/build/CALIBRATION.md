# Calibration Procedures

Calibration is valid only with final mechanical geometry installed, including the color shroud, conveyor handoff, servo chute, bins, and the chosen size sensor.

## Preconditions

- LiPo power, charged in normal operating range
- Conveyor mounted and tracking correctly
- Servo chute mounted on the final pivot or horn adapter
- Four bins and bin guides installed
- Color shroud installed and light leaks closed
- Display, start control, feed cue, and bin labels installed

## 1. Conveyor And Handoff Calibration

Goal: confirm the brick travels through sensing and enters the chute repeatably.

Procedure:
1. Run the conveyor at low speed.
2. Feed one brick at a time in the marked orientation.
3. Record whether the brick stays centered enough for sensing.
4. Record whether the brick enters the chute cleanly.
5. Increase speed only after repeatable handoff is proven.

Acceptance:
- No missed chute entries in 25 consecutive handoff trials
- No bracket or shroud catches
- Conveyor does not walk or stall during trials

## 2. Servo Chute Position Calibration

Goal: map four category outputs to repeatable servo angles.

Procedure:
1. Command the servo to the first target angle.
2. Drop or convey 10 bricks through the chute to the matching bin.
3. Adjust angle until the chute exit is centered on the bin guide.
4. Repeat for all four categories.
5. Record final angle values.

Acceptance:
- Each bin receives 10 of 10 forced-route trials
- Chute clears frame and wiring at each angle
- Servo does not hit a hard stop
- Final angle table is recorded

Calibration evidence template:

| Category | Servo angle | Trials | Correct bin count | Notes | Operator | Date |
|----------|-------------|--------|-------------------|-------|----------|------|
| | | | | | | |

## 3. Size Sensor Calibration

Status: size sensor family is decided as two-pair break-beam timing in the sensing shroud.

Procedure:
1. Mount both break-beam pairs in the sensing shroud.
2. Feed repeated 2x2 and 2x3 bricks at the chosen conveyor speed.
3. Record blocked-duration timing windows from pair A and pair B.
4. Set the simplest threshold that separates 2x2 from 2x3 with margin.
5. Re-test after any bracket or conveyor speed change.

Acceptance:
- 2x2 and 2x3 readings stay separated across repeated trials
- No rule depends on perfect hand placement
- Threshold choice is recorded with raw evidence

Calibration evidence template:

| Trial set | Sensor family | Brick | Raw value or timing | Size result | Operator | Date |
|-----------|---------------|-------|---------------------|-------------|----------|------|
| | | | | | | |

## 4. Color Threshold Calibration

Procedure:
1. Install the color shroud.
2. Feed repeated red and blue bricks through the final sensing station.
3. Collect repeated sample windows.
4. Compute the color feature used by firmware.
5. Set threshold with safety margin between clusters.

Acceptance:
- Red and blue clusters are separated with visible margin
- Classification stability holds across repeated trials
- Shroud remains installed for all recorded calibration data

Calibration evidence template:

| Trial set | Shroud installed | Red feature | Blue feature | Separation | Threshold chosen | Operator | Date |
|-----------|------------------|-------------|--------------|------------|------------------|----------|------|
| | | | | | | | |

## 5. Full-System Verification

Run at least 10 full 24-brick runs and log CSV data for notebook use.

Acceptance:
- Correct final bin counts
- No unrecovered ERROR state
- Logged failures are investigated and re-tested
- Run summary reports correctness truthfully, not just throughput
