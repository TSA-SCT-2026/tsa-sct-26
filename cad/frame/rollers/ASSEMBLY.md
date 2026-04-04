# Rollers Assembly Guide

This guide is for beginners and focuses on the parts in `cad/frame/rollers`.

Use this when you are assembling:
- Drive pulley on the motor shaft
- Idler roller on the axle
- Belt tracking test setup

Read this with:
- `cad/frame/rollers/README.md`
- `cad/frame/rollers/docs/`
- `cad/DIMENSIONS.md`

## What each STL does

`drive_pulley_proto_v1.stl`
- This is the powered roller.
- It mounts to the motor shaft.
- It has a shaft-fit bore and a split clamp so it can grip the shaft.
- It has side flanges to help keep the belt centered.
- The clamp uses one recessed M3 screw head pocket and one recessed M3 hex nut trap.

`idler_roller_proto_v1.stl`
- This is the free spinning roller on the other end.
- It has a slight crown in the middle.
- The crown helps the belt self center.
- It has bearing pockets for MR115 bearings.
- It has optional magnet pockets for diagnostic Hall sensing.

`coupon_bearing_fit_v1.stl`
- This is a test block for bearing fit.
- You press a real bearing into each test hole.
- You pick the hole size that gives secure fit without damage.

`coupon_d_bore_fit_v1.stl`
- This is a test block for motor shaft fit.
- Each bore version has a different shaft-fit size.
- You pick the bore that slides on by hand and does not wobble.

`coupon_crown_v1.stl`
- This is a test strip for crown height.
- It lets you compare belt tracking behavior before printing full rollers.

## What the split clamp is

The split clamp is a slot in the pulley hub that lets the hub squeeze closed.

Why it exists:
- Printed plastic bores can vary by printer and material.
- The clamp gives controlled grip on the shaft.
- It is more reliable than friction only fit.

How it works:
1. The pulley hub has a split gap.
2. A screw crosses the gap beside the shaft bore, not through the shaft center.
3. Tightening the screw pulls both sides together.
4. The bore grips the motor shaft.

Beginner picture in words:
- Imagine the shaft hole in the middle.
- Now imagine a thin slit from that hole out to one side of the hub.
- The screw is next to that hole, crossing the slit.
- Tightening the screw pinches the slit closed a small amount.
- That small motion is what grips the shaft.

## Extra hardware needed for split clamp

Yes, it needs extra hardware.

Minimum hardware:
- One M3 socket head clamp screw
- One M3 hex nut

Recommended:
- M3 screw length chosen so full threads engage the nut without excess overhang
- Test fit hardware in the printed recesses before final assembly

## Hardware needed for idler assembly

- Two MR115ZZ flanged bearings
- One M5 axle bolt or shaft
- Washer and nut set for your frame style
- Two 3mm magnets only if you choose to keep diagnostic Hall sensing

## Print setup for first assembly pass

Use your current baseline, then tune from coupons:
- Layer height: 0.2mm
- Material: PLA
- Walls: at least 4
- Infill:
- Drive pulley: 100 percent
- Idler: 80 to 100 percent
- Orientation:
- Print rollers standing on axis if bore and bearing circularity are priority
- Print coupons flat

## Step by step assembly

### Step 1: Run coupon fits first

1. Print all coupon STL files.
2. Test bearing coupon with real MR115 bearings.
3. Test the shaft-fit coupon with the real motor shaft.
4. Test crown coupon with your belt strip.
5. Record which option gives best fit and tracking.

Do not skip this step. It is the fastest way to avoid failed full size prints.

### Step 2: Print final pulley and idler

1. If needed, update `src/rollers_params.scad`.
2. Run `./scripts/build_rollers.sh`.
3. Print `drive_pulley_proto_v1.stl` and `idler_roller_proto_v1.stl`.

### Step 3: Build idler roller

1. Press first MR115 bearing into one side pocket.
2. Press second MR115 bearing into the opposite side pocket.
3. Confirm both bearings seat square and flush to pocket shoulders.
4. Add magnets to pockets only if using diagnostic Hall sensing.
5. Place idler on M5 axle and check free spin.

Pass target:
- Free spin is smooth.
- No scraping noise.
- No visible wobble.

### Step 4: Build drive pulley

1. Slide pulley onto the motor shaft.
2. Align pulley so belt will run centered in channel.
3. Press an M3 hex nut into the recessed nut trap.
4. Insert the M3 screw from the recessed head side.
5. Tighten gradually while checking rotation.
6. Verify pulley does not slip by holding belt and applying light motor torque.

Pass target:
- Pulley spins true.
- No hub crack at split line.
- No shaft slip under expected belt load.

### Step 5: Install belt and align

1. Mount both rollers in frame.
2. Install belt strip and set tension.
3. Run motor at low speed first.
4. Watch belt position for 30 to 60 seconds.
5. Adjust alignment or tension if belt walks to one side.

Pass target:
- Belt stays centered during run.
- Belt does not rub walls or flanges continuously.

## Validation checks before system integration

Mechanical checks:
- Pulley clamp remains tight after repeated start stop cycles.
- Idler bearings remain seated.
- No abnormal wear dust near flanges.

Tracking checks:
- Belt remains centered across low and moderate speed.
- Crown behavior is stable with expected belt tension.

Electrical checks:
- Hall sensor reads clean pulses if optional diagnostic magnets are installed.
- No contact between moving belt and sensor body.

## Common beginner issues and fixes

Bore too tight:
- Use the coupon-selected larger shaft-fit size.
- Clean first layer artifacts.

Bore too loose:
- Use the coupon-selected smaller shaft-fit size.
- Increase clamp grip, or increase hub wall thickness in params.

Bearing too tight:
- Use coupon selected larger bearing pocket.
- Add light chamfer cleanup only if needed.

Bearing too loose:
- Use coupon selected smaller pocket.
- If urgent, use retaining compound as temporary measure.

Belt walks to one side:
- Check roller parallel alignment.
- Check crown orientation and magnitude.
- Reduce over tension.

Hub crack near split:
- Increase hub outer diameter or wall thickness.
- Use lower clamp torque.
- Verify the nut trap is not forcing the hub open before tightening.

I do not understand why we use a split clamp:
- It is okay to ask that. The reason is repeatability, not complexity for its own sake.
- A plain printed shaft bore can work for a test print, but if it comes out a little loose you have to reprint.
- A split clamp gives one extra adjustment after printing.
- For a competition machine, that adjustability is useful.
- If you want the simplest possible prototype, you can print the pulley and test the shaft fit first without clamp hardware. For the competition version, keep the split clamp.

## Document what you choose

After first good assembly:
1. Record chosen bore and pocket values in `src/rollers_params.scad`.
2. Save notes about fit outcomes in your run logs.
3. Re export STL files from locked parameters.

This gives repeatable prints for future replacements and competition spares.
