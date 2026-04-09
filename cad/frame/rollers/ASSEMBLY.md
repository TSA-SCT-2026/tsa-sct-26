# Rollers Assembly Guide

This guide is for beginners and focuses on the parts in `cad/frame/rollers`.

Use this when you are assembling:
- Timing pulley on the motor shaft
- Supported drive roller on the conveyor shaft
- Idler roller on the axle
- Belt tracking test setup

Read this with:
- `cad/frame/rollers/README.md`
- `cad/frame/rollers/docs/`
- `cad/DIMENSIONS.md`

## What each STL does

`motor_pulley_proto_v1.stl`
- This is a layout reference for the timing pulley position.
- The active production pulley is the purchased 20T GT2 pulley, not this print.
- Don't print this.

`drive_roller_proto_v1.stl`
- This is the smooth roller on the supported conveyor shaft.
- It contacts the neoprene conveyor belt.
- It has a D-bore that mates with the D-flat on the shaft. Torque is transferred by the flat, not by a clamp.
- Axial positioning is set by shaft collars placed on each side of the roller.
- It has side flanges to help keep the belt centered.

`idler_roller_proto_v1.stl`
- This is the free spinning roller on the other end.
- It has a slight crown in the middle.
- The crown helps the belt self center.
- It has bearing pockets for MR85 bearings.

`coupon_bearing_fit_v1.stl`
- This is a test block for bearing fit.
- You press a real bearing into each test hole.
- You pick the hole size that gives secure fit without damage.

`coupon_shaft_fit_v1.stl`
- This is a test block for shaft fit.
- Each bore version has a different shaft-fit size.
- You pick the bore that slides on by hand and does not wobble.

`coupon_crown_v1.stl`
- This is a test strip for crown height.
- It lets you compare belt tracking behavior before printing full rollers.

## Hardware needed for idler assembly

- Two MR85ZZ bearings
- One M5 axle bolt or shaft
- Washer and nut set for your frame style

## Hardware needed for the timing stage

- One purchased 20T GT2 pulley for the motor shaft (printed envelope is fallback only)
- One purchased 20T GT2 pulley for the supported conveyor shaft
- One supported conveyor shaft with bearings in the frame
- One smooth drive roller on the supported shaft
- Two shaft collars for drive roller axial retention, one on each side
- One toothed timing belt with the chosen tooth count ratio
- One tension adjustment feature with the travel called out in `cad/DIMENSIONS.md`

## Print setup for first assembly pass

Use your current baseline, then tune from coupons:
- Layer height: 0.2mm
- Material: PLA
- Walls: at least 4
- Infill:
- Motor pulley and drive roller: 100 percent
- Idler: 80 to 100 percent
- Orientation:
- Print rollers standing on axis if bore and bearing circularity are priority
- Print coupons flat

## Step by step assembly
 
### Step 1: Run coupon fits first

1. Print all coupon STL files.
2. Test bearing coupon with real MR85 bearings.
3. Test the shaft-fit coupon with the real shafts that will carry the timing pulley and drive roller.
4. Test crown coupon with your belt strip.
5. Record which option gives best fit and tracking.

Do not skip this step. It is the fastest way to avoid failed full size prints.

### Step 2: Print final timing stage parts

1. If needed, update `src/rollers_params.scad`.
2. Run `./scripts/build_rollers.sh`.
3. Print `motor_pulley_proto_v1.stl`, `drive_roller_proto_v1.stl`, and `idler_roller_proto_v1.stl`.

### Step 3: Build drive roller

1. Slide the drive roller onto the supported shaft. The D-bore registers to the D-flat on the shaft.
2. Align the roller centerline to the conveyor channel centerline.
3. Place one shaft collar on each side of the roller. Snug each collar against its roller face.
4. Tighten both collar set screws to the manufacturer torque spec while confirming the roller stays centered.
5. Verify the roller does not shift axially or slip rotationally under expected belt load.

Pass target:
- Roller spins true.
- No axial movement when belt load is applied.
- No rotational slip on the shaft under expected belt load.

### Step 4: Build idler roller

1. Press first MR85 bearing into one side pocket.
2. Press second MR85 bearing into the opposite side pocket.
3. Confirm both bearings seat square and flush to pocket shoulders.
4. Place idler on M5 axle and check free spin.

Pass target:
- Free spin is smooth.
- No scraping noise.
- No visible wobble.

### Step 5: Mount timing pulleys

Use the purchased 20T GT2 pulleys on both shafts. The printed motor_pulley envelope is a layout reference and fallback only.

1. Slide the purchased pulley onto the motor shaft.
2. Align the pulley so the belt will run centered in the timing stage.
3. Tighten the pulley set screw following the manufacturer torque recommendation.
4. Repeat for the purchased pulley on the supported conveyor shaft.
5. Verify neither pulley slips by applying light motor torque.

Pass target:
- Both pulleys spin true.
- No slip on either shaft under expected belt load.

### Step 6: Install belt and align

1. Mount both rollers in frame.
2. Install belt strip and set tension.
3. Run motor at low speed first.
4. Watch belt position for 30 to 60 seconds.
5. Adjust alignment or tension if belt walks to one side.

Pass target:
- Belt stays centered during run.
- Belt does not rub walls or flanges continuously.
- Tensioner travel stays inside the documented range.

## Validation checks before system integration

Mechanical checks:
- Motor pulley set screw remains tight after repeated start stop cycles.
- Drive roller shaft collars remain tight and roller shows no axial shift.
- Idler bearings remain seated.
- No abnormal wear dust near flanges.
- Supported shaft bearings remain aligned.

Tracking checks:
- Belt remains centered across low and moderate speed.
- Crown behavior is stable with expected belt tension.
- Center distance and ratio stay within the model envelope.

Electrical checks:
- No contact between moving belt and sensor body.

## Common beginner issues and fixes

Bore too tight:
- Use the coupon-selected larger shaft-fit size.
- Clean first layer artifacts.

Bore too loose:
- Use the coupon-selected smaller shaft-fit size.

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

Motor pulley chatters under load:
- Check belt alignment and center distance.
- Reduce over tension.
- Confirm the pulley ratio matches the timing-stage model.

## Document what you choose

After first good assembly:
1. Record chosen bore and pocket values in `src/rollers_params.scad`.
2. Save notes about fit outcomes in your run logs.
3. Re export STL files from locked parameters.

This gives repeatable prints for future replacements and competition spares.

---
