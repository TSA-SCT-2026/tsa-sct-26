# Electrical Design Architecture

## Power architecture

Two independent power rails from a single 2S LiPo (7.4V, 2000mAh minimum), each stepped down to 5V via a dedicated buck converter.

```
Rail 1: solenoids only (high current, noisy switching)
Rail 2: stepper motor, belt motor, microcontroller, sensors, display (clean logic rail)
Common ground between rails -- only the ground is shared
```

Separating rails keeps solenoid switching noise off the logic rail and stepper driver switching noise off the solenoid rail. Both are real failure modes on a shared rail at this current draw.

### Power budget

```
3 plow solenoids (peak simultaneous): 3 * 500mA = 1500mA
Stepper motor:                        ~400mA
Belt motor:                           ~400mA
Microcontroller:                      ~250mA
Sensors and display:                  ~150mA
Total worst case:                     ~2700mA
Realistic operating peak:             ~1500mA
```

A USB power bank will not work. Most cut out at 1A or less under pulse load. Use LiPo.

Bench supply is acceptable for early development but must not be used for any calibration run that will be logged as performance data, and must not be used for the final demo. Test on LiPo before April 20.

## Component list and electrical specs

| Part | Purpose | Voltage | Peak current |
|------|---------|---------|-------------|
| JF-0530B solenoid (x3) | plow actuation | 5V | ~500mA each |
| NEMA 11 stepper motor | escapement | 6-8V (motor rail) | ~400mA |
| A4988 stepper driver | stepper control | motor rail + 3.3V logic | -- |
| TT gearmotor (x2, one spare) | belt drive | 5V | ~400mA |
| L298N motor driver | belt motor control | 5V | -- |
| LM2596 buck converter (x2) | 7.4V -> 5V | -- | 3A each rated |
| ESP32 DevKit | microcontroller | 3.3V (via onboard reg) | ~250mA |
| Adafruit IR break-beam pairs (x6 used) | size and bin sensing | 3.3-5V | low |
| TCS34725 color sensor | color classification | 3.3V | low |
| ST7789 color TFT display | live status display | 3.3V | ~80mA |
| H206 slot optocoupler | belt speed encoder | 3.3-5V | low |
| 2S LiPo 7.4V 2000mAh | main power source | 7.4V nominal | 2A continuous |

## Critical protection components

These are not optional. Skipping any of them risks destroying hardware.

### Capacitor on stepper driver motor power input

A 100uF electrolytic capacitor must be placed physically adjacent to the motor power input on the A4988 driver module. This is the single most common way the A4988 is destroyed -- back-EMF from the motor travels back up the power line on power connect. Without this capacitor, the chip dies. Place it right next to the chip, not at the power connector.

Before applying power to the stepper circuit for the first time: verify this capacitor is present and correctly oriented (negative leg to ground) with a teammate watching. Say it out loud.

### Flyback diodes on all solenoids

Every solenoid must have a flyback diode across its terminals. When the solenoid de-energizes, the collapsing magnetic field generates a large voltage spike. Without a flyback diode, this spike travels back into the microcontroller and will cause brownouts or permanent damage. 1N4007 diodes are sufficient.

Orientation: cathode (banded end) toward the positive supply rail, anode toward the solenoid terminal. A reversed diode is a short circuit.

Verify all three diodes with a multimeter before first solenoid energization.

### Pullup resistors on I2C lines

If the color sensor drops reads during high-speed polling, add 4.7k resistors from SDA and SCL to 3.3V. This is a diagnostic step, not mandatory upfront, but have the resistors available.

## Stepper driver wiring

```
A4988 VMOT + GND  -> motor rail (6-8V from LM2596)
A4988 VDD + GND   -> microcontroller 3.3V, shared ground
A4988 STEP        -> microcontroller GPIO
A4988 DIR         -> microcontroller GPIO (set once at startup)
A4988 SLEEP       -> pull HIGH (enables the driver)
A4988 RESET       -> tie to SLEEP pin
A4988 MS1/MS2/MS3 -> all LOW (full step mode, maximum torque)
A4988 coil pairs  -> NEMA 11 stepper coil wires (check datasheet for coil grouping)
```

VMOT should be 6-8V for the NEMA 11. Do not run from 5V -- insufficient torque at speed. Use the dedicated motor rail from Rail 2.

Between releases, reduce hold current via the driver's reference voltage trim. Sufficient torque to hold against queue pressure, much lower heat. This is done once during setup, not dynamically during normal operation.

## Belt motor wiring

Belt motor connects through the L298N module. The L298N enable pins accept PWM for speed control -- this is how the PI controller adjusts belt speed. No separate speed controller module is needed.

L298N PWM frequency: keep between 1kHz and 10kHz. Below 1kHz the motor audibly ticks. Above 25kHz the L298N's internal circuitry generates excessive heat and switching losses become significant. 5kHz is a reliable starting point.

Motor direction should be wired to run belt surface in the correct direction. Verify this before installing sensors or plows by running a brick manually.

## Bulk capacitor on logic rail

Add a 1000uF electrolytic capacitor between 5V and GND on the logic rail, physically close to the microcontroller. When solenoids fire on Rail 1, even with rail isolation, transients can couple through the common ground. The bulk cap buffers the logic supply during these events and prevents brief undervoltage that could cause a reset.

This is separate from the 100uF cap on the A4988 motor power input -- that one is for the stepper driver, this one is for the microcontroller supply. Both are needed.

## Solenoid control

Each solenoid connects to a microcontroller GPIO through a driver transistor or the L298N. Flyback diode across each solenoid (see above).

Solenoid timing pattern per brick:
- Full power for 15-20ms (full extension)
- Drop to ~40% duty cycle PWM for the remaining hold time (maintains position, roughly half the heat)
- De-energize once the brick has cleared the plow

Brick clears the plow roughly 280ms after the trigger (calculated from brick length, plow footprint, and belt speed). This is the de-energize point. It can be implemented as a fixed timer.

Add a small aluminum heatsink (20x20mm) to each solenoid body using thermal adhesive tape. This is cheap insurance against heat during extended calibration sessions, which will run more solenoid cycles than the competition demo.

## Sensor wiring

### IR break-beams

Each break-beam pair has an emitter and a receiver. The receiver output is a digital signal -- high when beam is intact, low when broken. Wire receivers to microcontroller GPIO pins configured as inputs. The emitter connects to 5V (check Adafruit datasheet for exact supply).

Two pairs for size detection in the sensing zone. One pair per bin entrance, four pairs total for confirmation. Six pairs used, two spare.

### Color sensor

Connects to the microcontroller over I2C. Set the I2C bus clock to 400kHz explicitly in firmware. The default 100kHz is insufficient for the rapid polling rate needed during brick dwell time.

### Display

Connects to the microcontroller over SPI. Runs at 40MHz SPI clock -- verify this is within the display panel's spec (ST7789 supports it). Power at 3.3V.

### Belt speed encoder (if using slotted disk option)

A photointerrupter (H206 or equivalent slot optocoupler) mounts next to the belt pulley. Its output is a digital pulse train -- one pulse per slot as the disk rotates. Wire the output to a GPIO interrupt pin on the microcontroller. Use the ESP32's hardware pulse counter peripheral for counting rather than software interrupts -- it handles high pulse rates without CPU overhead.

The disk can be printed directly into the belt pulley face (add 20 evenly spaced slots around the perimeter). No separate part needed.

## Wiring checklist before first power-on

- [ ] 100uF capacitor adjacent to A4988 motor power input, correctly oriented
- [ ] 1000uF capacitor on logic rail 5V supply, close to microcontroller
- [ ] All three solenoid flyback diodes installed, orientation verified with multimeter
- [ ] Rail 1 and Rail 2 separated, common ground only
- [ ] Both LM2596 outputs measured at 5V before connecting anything
- [ ] Stepper SLEEP pulled HIGH
- [ ] RESET tied to SLEEP
- [ ] Microcontroller powered from clean rail, not motor rail
- [ ] All I2C devices on same bus with shared SDA, SCL, and ground
- [ ] Display SPI connections correct (MOSI, SCLK, CS, DC, RST)
- [ ] Photointerrupter output connected to GPIO interrupt pin (if using slotted disk encoder)

## Common failure modes

| Failure | Cause | Prevention |
|---------|-------|-----------|
| A4988 destroyed on power-on | Missing or misplaced 100uF cap | Verify cap before power |
| Microcontroller brownout | Solenoid back-EMF, no flyback diode | Install all flyback diodes |
| Microcontroller brownout | Bench supply current limiting | Test on LiPo |
| Color sensor dropped reads | I2C too slow or noise | Set 400kHz, add pullups if needed |
| Solenoid overheating | Full power hold, no PWM | Implement PWM hold in firmware |
| Belt slipping | Loose belt from print tolerance | Use tensioner slot, re-tension |
| Stepper skipping steps | Insufficient VMOT voltage | Use dedicated motor rail at 6-8V |
