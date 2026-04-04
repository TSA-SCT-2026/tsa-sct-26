# Rollers Assembly Guide

This guide is for beginners and focuses on the parts in `cad/frame/rollers`.

Use this when you are assembling:
- Drive pulley on the motor shaft
- Idler roller on the axle
- Belt tracking test setup

Read this with:
- `cad/frame/rollers/README.md`
- `cad/DIMENSIONS.md`

## What each STL does

`drive_pulley_proto_v1.stl`
- This is the powered roller.
- It mounts to the motor shaft.
- It has a D bore and a split clamp so it can grip the shaft.
- It has side flanges to help keep the belt centered.

`idler_roller_proto_v1.stl`
- This is the free spinning roller on the other end.
- It has a slight crown in the middle.
- The crown helps the belt self center.
- It has bearing pockets for MR115 bearings.
- It has magnet pockets for Hall speed sensing.

`coupon_bearing_fit_v1.stl`
- This is a test block for bearing fit.
- You press a real bearing into each test hole.
- You pick the hole size that gives secure fit without damage.

`coupon_d_bore_fit_v1.stl`
- This is a test block for motor shaft fit.
- Each bore version has a different D shape size.
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
2. A screw crosses the gap.
3. Tightening the screw pulls both sides together.
4. The bore grips the D shaft.

## Extra hardware needed for split clamp

Yes, it needs extra hardware.

Minimum hardware:
- One M3 clamp screw
- One M3 nut or one M3 heat set insert

Recommended:
- M3 heat set insert in the modeled insert pocket
- M3 screw length chosen so full threads engage but tip does not hit the motor body

## Hardware needed for idler assembly

- Two MR115ZZ flanged bearings
- One M5 axle bolt or shaft
- Washer and nut set for your frame style
- Two 3mm magnets if Hall sensing is used

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
3. Test D bore coupon with real motor shaft.
4. Test crown coupon with your belt strip.
5. Record which option gives best fit and tracking.

Do not skip this step. It is the fastest way to avoid failed full size prints.

### Step 2: Print final pulley and idler

1. If needed, update `rollers_params.scad`.
2. Run `./build_rollers.sh`.
3. Print `drive_pulley_proto_v1.stl` and `idler_roller_proto_v1.stl`.

### Step 3: Build idler roller

1. Press first MR115 bearing into one side pocket.
2. Press second MR115 bearing into the opposite side pocket.
3. Confirm both bearings seat square and flush to pocket shoulders.
4. Add magnets to pockets if using Hall sensing.
5. Place idler on M5 axle and check free spin.

Pass target:
- Free spin is smooth.
- No scraping noise.
- No visible wobble.

### Step 4: Build drive pulley

1. Slide pulley onto motor D shaft.
2. Align pulley so belt will run centered in channel.
3. Install clamp hardware.
4. Tighten gradually while checking rotation.
5. Verify pulley does not slip by holding belt and applying light motor torque.

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
- Hall sensor reads clean pulses if magnets are installed.
- No contact between moving belt and sensor body.

## Common beginner issues and fixes

Bore too tight:
- Use coupon selected larger D bore.
- Clean first layer artifacts.

Bore too loose:
- Use coupon selected smaller D bore.
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
- Verify insert install did not over expand local plastic.

## Document what you choose

After first good assembly:
1. Record chosen bore and pocket values in `rollers_params.scad`.
2. Save notes about fit outcomes in your run logs.
3. Re export STL files from locked parameters.

This gives repeatable prints for future replacements and competition spares.
