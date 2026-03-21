# Color Sensor Calibration Procedure

The color sensor must be calibrated with the shroud installed and real bricks.
Open-air calibration is meaningless. Do this once, log all data, lock the thresholds.

---

## When to calibrate

- Before first competition run
- Any time the shroud is removed or repositioned
- Any time the color thresholds are suspected to be wrong
- After any mechanical change to the chute near the sensor window

---

## What you need

- System fully assembled: chute, shroud, sensor wired and responding to I2C
- All 4 brick types: at least 5 of each (2x2 red, 2x2 blue, 2x3 red, 2x3 blue)
- Laptop with serial monitor (or pio device monitor)
- Spreadsheet or notebook to log the data

---

## Procedure

### Step 1: Verify shroud seal

Inspect the sensor shroud. There should be no ambient light leaking into the window
when the system is in a normal room. Block the window with your hand and confirm the
sensor reads near-zero (C channel < 50). If not, find the light leak and seal it.

### Step 2: Start calibration mode

Connect the system to USB. In the serial monitor, send:

```
cal start
```

The system enters calibration mode. The escapement stops. The display shows CAL MODE.

### Step 3: Collect readings for each brick type

For each brick type (do all four):

1. Manually place one brick at the cam chord position in the chute (bottom brick).
2. In serial monitor, send: `cal read`
3. The system takes 20 readings and logs them to serial in CSV format:
   ```
   R,G,B,C,r_ratio
   ```
   where `r_ratio = R / (R + G + B)`
4. Copy all 20 lines to your spreadsheet.
5. Label the rows with the brick type.
6. Repeat steps 1-5 for each of the remaining 3 brick types.

### Step 4: Compute thresholds

For each brick type, compute mean and standard deviation of the `r_ratio` column.

| Type | Mean r_ratio | Std dev |
|------|-------------|---------|
| 2x2 red | ? | ? |
| 2x3 red | ? | ? |
| 2x2 blue | ? | ? |
| 2x3 blue | ? | ? |

Expected: red bricks have r_ratio around 0.50-0.65. Blue bricks around 0.20-0.35.
There should be a clear gap between the red cluster and the blue cluster.

Threshold formula:
```
COLOR_RED_THRESHOLD = midpoint of gap between red_mean and blue_mean
```

Example: red mean = 0.58, blue mean = 0.26, midpoint = 0.42.
Set `COLOR_RED_THRESHOLD = 0.42f`.

### Step 5: Check for overlap

If any blue brick's r_ratio is above the threshold, or any red brick's is below:
- Check the shroud seal (most common cause)
- Check that the LED is on (TCS34725 LED pin must be pulled high)
- Try a different brick (worn bricks may have faded color)
- Re-run with 30 readings instead of 20

Do not proceed if distributions overlap. Fix the hardware first.

### Step 6: Update firmware and commit

Edit `firmware/src/config.h`:
```c
#define COLOR_RED_THRESHOLD  0.42f  // update to your computed value
```

Commit with:
```
calibrate: color thresholds YYYYMMDD
```

Log the full calibration data to `docs/runs/cal_YYYYMMDD.csv`.

### Step 7: Verify with test bricks

Exit calibration mode: `cal stop`

Run 10 bricks of each type through the full system. All 40 must classify correctly.
If any fail: return to step 3, collect more readings, tighten the threshold.

---

## Notes

- Size sensing needs no calibration. It is a single digital read (beam blocked or not).
  The only check is physical: verify a 2x3 brick blocks the beam and a 2x2 does not.
- If a brick is damaged or painted, its color may not match. Use competition-spec bricks only.
- Ambient light changes between calibration and competition can shift readings slightly.
  Calibrate in similar lighting conditions, or ensure the shroud fully blocks ambient light.
