# Calibration

Run calibration with the real conveyor, shroud, chute, bins, and LiPo power installed.

## Servo Angles

1. Flash the firmware.
2. Use the servo tuning command to move the chute one bin at a time.
3. Record the centered angle for each bin in firmware config.
4. Drop a 2x3 brick through each position and confirm it lands cleanly.

## Break-Beam Size

1. Feed several 2x2 bricks and several 2x3 bricks at the same conveyor speed.
2. Capture CSV serial output.
3. Compare measured blocked duration and computed length.
4. Set the size threshold midway between the stable 2x2 and 2x3 clusters.

## Color Sensor

1. Keep the shroud fully installed.
2. Run samples for red bricks and blue bricks on the moving belt.
3. Use belt-subtracted readings from the log.
4. Set the color threshold midway between the stable red and blue clusters.

## Acceptance Check

Run all 24 bricks once in the documented feed orientation. The accepted result is every brick landing in its matching labeled bin.
