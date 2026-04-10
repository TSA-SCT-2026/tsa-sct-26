# Rollers Assembly Guide

This guide covers the integrated conveyor trough timing stage in `cad/frame/rollers`.

Use it when you are assembling:
- The one-piece trough body
- The supported drive roller on the drive shaft
- The crowned idler on the fixed M5 axle
- The purchased GT2 pulleys and timing belt

Read this with:
- `cad/frame/rollers/README.md`
- `cad/frame/rollers/docs/`
- `cad/DIMENSIONS.md`

## What each STL does

`conveyor_trough_v1.stl`
- This is the integrated timing-stage body.
- It combines the belt bed, both side plates, drive-end MR85 support pockets, idler-end M5 axle holes, bottom mounting flanges, and the outboard NEMA17 motor face.
- Print it upright with the belt-bed face down.

`trough_end_cap_coupon_v1.stl`
- This is the drive-end validation print.
- It checks drive-side MR85 pocket circularity, the supported-shaft bore location, the NEMA17 motor-face geometry, the bridge belt window, the tension-slot print quality, and one bottom M4 mounting hole before you commit to the full trough.

`motor_pulley_proto_v1.stl`
- This is a layout reference for the timing pulley position.
- The active production pulley is the purchased 20T GT2 pulley, not this print.
- Do not print this unless you need a quick envelope check.

`drive_roller_proto_v1.stl`
- This is the smooth roller on the supported conveyor shaft.
- It contacts the neoprene conveyor belt.
- It has a D-bore that mates with the D-flat on the shaft. Torque is transferred by the flat, not by a clamp.
- Axial positioning is set by shaft collars placed on each side of the roller.
- It has side flanges to help keep the belt centered.

`idler_roller_proto_v1.stl`
- This is the free-spinning roller on the other end.
- It has a slight crown in the middle.
- The crown helps the belt self-center.
- Its MR85 bearings stay inside the roller body. The trough only provides M5 axle clearance holes at this end.

`coupon_bearing_fit_v1.stl`
- This is a test block for MR85 fit.
- You press a real bearing into each test hole.
- You pick the hole size that gives secure fit without damage.

`coupon_shaft_fit_v1.stl`
- This is a test block for shaft fit.
- Each bore version has a different shaft-fit size.
- You pick the bore that slides on by hand and does not wobble.

`coupon_crown_v1.stl`
- This is a test strip for crown height.
- It lets you compare belt tracking behavior before printing full rollers.

## Hardware needed for the timing stage

- One integrated trough print
- Two MR85ZZ bearings for the drive-end trough pockets
- One supported 5mm drive shaft
- One smooth drive roller on the supported shaft
- Two shaft collars for drive-roller axial retention, one on each side
- One crowned idler roller with two internal MR85ZZ bearings
- One M5 axle bolt or shaft for the idler
- Two M5 washers for the idler axle hardware, one per outer wall face
- One purchased 20T GT2 pulley for the motor shaft
- One purchased 20T GT2 pulley for the supported drive shaft
- One toothed timing belt with the chosen tooth count ratio
- Four M4 base fasteners for the trough flanges
- Four M3 motor screws for the NEMA17 face

## Print setup for first assembly pass

Use your current baseline, then tune from coupons:
- Layer height: 0.2mm
- Material: PLA
- Walls: at least 4
- Infill:
- Trough: 30 to 40 percent is a reasonable first pass
- Drive roller: 100 percent
- Idler: 80 to 100 percent
- Orientation:
- Print the trough upright with the belt-bed face down on the print bed
- Print the trough coupon in the same orientation as the full trough
- Print rollers standing on axis if bore and bearing circularity are priority
- Print fit coupons flat
- Print-risk note:
- If your printer bed is close to 220mm in Y, run a short motor-end adhesion test first or add brim margin before committing to the full trough. The one-piece trough is long and asymmetric.

## Step by step assembly

### Step 1: Validate the drive-end coupon before anything else

1. Print `trough_end_cap_coupon_v1.stl`.
2. Press an MR85ZZ into the coupon pocket and confirm a square seat.
3. Pass the real supported shaft through the coupon bore and check free rotation with no visible skew.
4. Hold the NEMA17 against the coupon motor face and confirm pilot fit plus slot reach across the full 8mm tension travel.
5. Verify the bridge belt window is open at the pulley plane. A 6mm strip should pass through the opening without rubbing.
6. Check the M4 mounting hole against the real base fastener.

Pass target:
- Bearing seats square without cracking the print.
- Supported shaft passes through cleanly.
- Motor face sits flush with all four screws starting cleanly in the slot range.
- Belt window is visibly open through the bridge and clears a 6mm strip.
- The coupon lies flat with no rocking.

Do not print the full trough until this coupon passes.

### Step 2: Run the roller fit coupons

1. Print `coupon_bearing_fit_v1.stl`, `coupon_shaft_fit_v1.stl`, and `coupon_crown_v1.stl`.
2. Test the bearing coupon with real MR85 bearings.
3. Test the shaft-fit coupon with the real shafts that will carry the timing pulley and drive roller.
4. Test the crown coupon with your belt strip.
5. Record which option gives the best fit and tracking.

### Step 3: Print the final timing-stage parts

1. If needed, update `src/rollers_params.scad`.
2. Run `./scripts/build_rollers.sh`.
3. If your printer has limited bed margin, confirm the full trough footprint and adhesion plan before starting the print.
4. Print `conveyor_trough_v1.stl`, `drive_roller_proto_v1.stl`, and `idler_roller_proto_v1.stl`.

### Step 4: Build the drive end

1. Press one MR85ZZ into each drive-end side-plate pocket in the trough.
2. Mark the D-flat orientation on the visible shaft end before insertion.
3. Slide the supported 5mm shaft through the first bearing.
4. Slide the drive roller onto the shaft. Align the D-bore to the marked D-flat before pushing through.
5. Continue the shaft through the second bearing.
6. Center the roller on the channel.
7. Place one shaft collar on each side of the roller and tighten them so the roller cannot drift axially.
8. Keep each collar flush with or up to 1mm clear of the adjacent bearing inner-race line. Do not preload the bearings axially through the collars.

Pass target:
- Bearings sit square and stay retained.
- Shaft turns freely with no visible bind.
- Roller spins true.
- No axial movement appears when the belt is tensioned by hand.

### Step 5: Build the idler end

1. Press both MR85 bearings into the idler roller body.
2. Confirm both bearings seat square and flush to the pocket shoulders.
3. Pass the M5 axle through one trough side plate, the idler roller, and the opposite side plate.
4. Place one M5 washer under the bolt head or nut at each outer wall face.
5. Tighten the axle hardware only enough to keep the roller secure while preserving free spin.

Pass target:
- Free spin is smooth.
- No scraping noise.
- No visible wobble.

### Step 6: Mount the motor and pulleys

Use the purchased 20T GT2 pulleys on both shafts. The printed motor pulley is only a layout reference.

1. Bolt the NEMA17 to the trough motor face using the integrated Y-axis slots.
2. Start with the motor centered in the slot travel.
3. Slide the purchased GT2 pulley onto the motor shaft.
4. Slide the matching pulley onto the supported drive shaft on the motor side of the trough.
5. Align both pulleys to the same belt plane before final tightening.
6. Snug the drive-pulley set screw before the timing belt goes on so the hub remains movable but controlled.
7. Snug the motor-pulley set screw.

Pass target:
- Motor face sits flush on the printed mount.
- Both pulleys spin true.
- Neither pulley slips under light hand-applied torque.

### Step 7: Install belt and align

1. Install the timing belt between the motor pulley and the drive pulley.
2. Shift the motor in the slot direction to set belt tension.
3. Verify pulley alignment with the timing belt installed. If the drive-pulley set screw needs correction, remove the timing belt before final torque.
4. Final-torque both pulley set screws only after the alignment check passes.
5. Wrap the neoprene transport belt around the drive roller and idler, then close the scarf splice in place. Do not close the splice on the bench first.
6. Run the motor at low speed first.
7. Watch belt position for 30 to 60 seconds.
8. Adjust alignment or tension if the transport belt walks to one side.

Pass target:
- Timing belt stays in plane.
- Neoprene belt stays centered during the run.
- Belt does not rub the trough walls or roller flanges continuously.
- Tension travel stays inside the documented 8mm range.

## Validation checks before system integration

Mechanical checks:
- Drive-end MR85 bearings remain seated after repeated start-stop cycles.
- Drive-roller shaft collars remain tight and the roller shows no axial shift.
- Idler bearings remain seated.
- The idler axle does not ovalize the printed holes.
- The motor plate does not show visible belt-cycle flex during low-speed runs.
- No abnormal wear dust appears near the roller flanges or wall tops.

Tracking checks:
- The neoprene belt remains centered across low and moderate speed.
- Crown behavior is stable with the expected belt tension.
- Center distance and ratio stay inside the model envelope.

Electrical checks:
- No contact occurs between moving belts and sensor bodies or wiring.

## Common beginner issues and fixes

Drive-end bearing too tight:
- Use the coupon-selected larger pocket value.
- Clean the chamfer lightly if the first layer ridge is the only interference.

Drive-end bearing too loose:
- Use the coupon-selected smaller pocket value.
- If urgent, use retaining compound as a temporary backup.

Shaft bore too tight:
- Use the coupon-selected larger shaft-fit size.
- Clean first-layer artifacts.

Shaft bore too loose:
- Use the coupon-selected smaller shaft-fit size.

Belt walks to one side:
- Check roller parallel alignment.
- Check crown orientation and magnitude.
- Reduce over-tension.

Motor pulley chatters under load:
- Check belt alignment and center distance.
- Reduce over-tension.
- Confirm the pulley ratio matches the timing-stage model.

Tracking looks unstable near the motor:
- Check for visible flex at the printed motor plate during low-speed running.
- If the plate is moving, reduce belt tension first before redesigning anything else.

## Document what you choose

After the first good assembly:
1. Record chosen bore and pocket values in `src/rollers_params.scad`.
2. Save notes about fit outcomes in your run logs.
3. Re-export STL files from the locked parameters.

This gives repeatable prints for future replacements and competition spares.
