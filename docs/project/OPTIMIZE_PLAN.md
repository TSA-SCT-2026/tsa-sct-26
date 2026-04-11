# States Optimization Plan

Do not optimize speed before the sorter is accurate.

## Order Of Work

1. Sort correctly with manual one-at-a-time feed.
2. Make conveyor-to-chute handoff reliable.
3. Calibrate the selected size sensor.
4. Calibrate color with the shroud installed.
5. Tune servo angles for four bins.
6. Run 10 complete 24-brick sets.
7. Only then increase conveyor speed or reduce pauses.

## Metrics

Track:
- Correct bin count per 24-brick run
- Size classification errors
- Color classification errors
- Chute miss or stick events
- Servo target angle and settle time
- Conveyor speed
- Total run time

## Do Not Optimize Yet

Do not spend time on:
- automated hopper feed
- archived release-gate timing
- NEMA11 selector motion profiles
- chamber pitch
- compressed queue timing

These belong to the archived nationals candidate, not the states sprint.
