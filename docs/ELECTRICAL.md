# Electrical Design

## Power architecture

Single 3S LiPo 11.1V 1300mAh XT60. Three rails:

```
[3S LiPo 11.1V]
  |
  +-- Buck converter 1 (LM2596) -> 6V rail
  |     Belt motor (JGB37-520 via L298N)
  |     Solenoids x3 (via IRLZ44N MOSFETs)
  |
  +-- Buck converter 2 (LM2596) -> 5V rail
  |     ESP32 DevKit (Vin pin)
  |     TCS34725 color sensor (Vdd)
  |     Display (Vcc)
  |     IR break-beam emitters (via 100 ohm resistors)
  |
  +-- TMC2209 stepper driver (VM pin, direct 11.1V)
        NEMA 11 stepper motor
```

LM2596 handles up to 40V input. 3S LiPo is within spec. Do not use 2S (7.4V) - the
design requires 6V for solenoids and the motor, which 2S cannot provide with headroom.

**Power budget:**

| Load | Rail | Peak | Average |
|------|------|------|---------|
| Belt motor | 6V | 1.2A | 0.5A |
| Solenoids (1 firing at a time) | 6V | 1A | 0.05A |
| NEMA 11 stepper | 11.1V | 0.4A | 0.3A |
| ESP32 | 5V | 0.24A | 0.15A |
| Sensors + display | 5V | 0.1A | 0.05A |

LiPo 3S 1300mAh = 14.4Wh. Runtime at ~7.6W average = 1.9 hours. Sufficient.

---

## Protection components (non-negotiable)

**Flyback diodes on every solenoid:** 1N4007 or SR5150, across solenoid terminals,
cathode toward + rail. Without these, de-energizing spikes reach 50-100V and destroy
the MOSFET or reset the ESP32.

**Bulk capacitor on 6V rail:** 100uF electrolytic, as close to the solenoid drivers as
possible. Absorbs current spikes when solenoids fire.

**Bulk capacitor on TMC2209 VM input:** 100uF electrolytic. Protects driver from
back-EMF on power-on. TMC2209 datasheet requires this.

Do not generate or follow any wiring instructions that omit these three items.

---

## Solenoid driver circuit (per solenoid, build x3)

```
ESP32 GPIO --> [1k resistor] --> IRLZ44N Gate
                                      |
                                   Drain --[Solenoid-] --[1N4007 cathode]
                                      |                          |
                                   Source -- GND         [Solenoid+] -- 6V
                                                                  |
                                                          [1N4007 anode]
```

IRLZ44N is a logic-level N-channel MOSFET (Vgs threshold 1-2V, fully on at 3.3V gate).
Build all three on perfboard. Label each channel. Common 6V bus with the 100uF bulk cap.

No PWM hold needed. Direct push, spring return is fast. Firmware fires 40ms on, then off.

---

## GPIO pin assignments

From firmware/src/config.h (authoritative). If config.h and this doc conflict, config.h wins.

```c
// Stepper (escapement) via TMC2209
PIN_STEP       = GPIO 25    // step pulses
PIN_DIR        = GPIO 26    // direction
PIN_ENABLE     = GPIO 27    // active low
PIN_TMC_UART   = GPIO 23    // UART config (1k series resistor)

// Belt motor via L298N
PIN_MOTOR_ENA  = GPIO 14    // PWM speed control
PIN_MOTOR_IN1  = GPIO 12    // direction (boot-safe: does not affect flash at HIGH)
PIN_MOTOR_IN2  = GPIO 13    // direction

// Solenoids via IRLZ44N MOSFETs
PIN_SOL1       = GPIO 32    // pusher 1 (2x2 red -> bin 1, at 75mm)
PIN_SOL2       = GPIO 33    // pusher 2 (2x2 blue -> bin 2, at 150mm)
PIN_SOL3       = GPIO 15    // pusher 3 (2x3 blue -> bin 3, at 225mm)

// Sensing (in chute, at escapement level)
PIN_SIZE_BEAM  = GPIO 34    // single beam in chute: LOW=blocked=2x3, HIGH=clear=2x2
PIN_CHUTE_EXIT = GPIO 35    // chute exit confirmation beam

// Bin confirmation beams
PIN_BIN1_BEAM  = GPIO 16
PIN_BIN2_BEAM  = GPIO 17
PIN_BIN3_BEAM  = GPIO 5
PIN_BIN4_BEAM  = GPIO 18

// I2C bus (TCS34725 at 0x29, display at I2C or SPI)
PIN_SDA        = GPIO 21
PIN_SCL        = GPIO 22

// Operator interface
PIN_START_BTN  = GPIO 19    // momentary button, external pull-up, active low
PIN_BUZZER     = GPIO 2     // piezo buzzer
```

**GPIO errata notes:**
- GPIO 34, 35: input-only, NO internal pull-ups. Must have 10k external pull-ups to 3.3V.
- GPIO 36, 39: NOT USED. These have phantom interrupt issues (270pF internal capacitance
  causes crosstalk with GPIO 37/38 when ADC1 is active). ESP32 ECO errata 3.11.
- GPIO 12: affects flash voltage at boot if pulled HIGH. The L298N IN1 connection is safe
  because the motor does not run until firmware configures the pin after boot.
- GPIO 0: has internal pull-up, boot button on most devkits. Safe for polled use only
  (Hall sensor, polled after boot completes).

---

## I2C configuration

I2C clock must be explicitly set to 400kHz before any TCS34725 communication.
The default 100kHz is insufficient for the polling rate. Set in initialization:

```c
Wire.begin(PIN_SDA, PIN_SCL);
Wire.setClock(400000);
```

---

## TMC2209 wiring

| TMC2209 Pin | Connection |
|-------------|------------|
| VM | 11.1V (LiPo direct, through main power switch) |
| GND | Common ground |
| VCC_IO | 3.3V (from ESP32) |
| STEP | GPIO 25 |
| DIR | GPIO 26 |
| EN | GPIO 27 (active low) |
| PDN/UART | GPIO 23 via 1k resistor (single-wire UART) |
| OA1, OA2 | NEMA 11 coil A |
| OB1, OB2 | NEMA 11 coil B |

Firmware configures TMC2209 via UART at startup: run current 400mA, StealthChop on,
StallGuard enabled (SGTHRS tuned during testing), 8x microstepping (1600 steps/rev).

---

## Wiring reference table

| From | To | Notes |
|------|----|-------|
| LiPo + | Main power switch | XT60, red wire |
| Power switch | Buck 1 in+, Buck 2 in+, TMC2209 VM | Star distribution |
| LiPo - | Buck 1 in-, Buck 2 in-, TMC2209 GND | Common ground bus |
| Buck 1 out (6V) | L298N +Vs, MOSFET 6V bus | Belt motor + solenoid power |
| Buck 2 out (5V) | ESP32 Vin, sensor Vcc bus | Logic + sensor power |
| ESP32 3.3V | TMC2209 VCC_IO, 10k pull-up bus for beams 34/35 | 3.3V reference |
| ESP32 GPIO 25 | TMC2209 STEP | Stepper step |
| ESP32 GPIO 26 | TMC2209 DIR | Stepper direction |
| ESP32 GPIO 27 | TMC2209 EN | Stepper enable |
| ESP32 GPIO 23 | TMC2209 PDN_UART via 1k | UART config |
| ESP32 GPIO 14 | L298N ENA | Belt motor PWM |
| ESP32 GPIO 12 | L298N IN1 | Belt direction |
| ESP32 GPIO 13 | L298N IN2 | Belt direction |
| ESP32 GPIO 32 | MOSFET gate 1 via 1k | Solenoid 1 |
| ESP32 GPIO 33 | MOSFET gate 2 via 1k | Solenoid 2 |
| ESP32 GPIO 15 | MOSFET gate 3 via 1k | Solenoid 3 |
| ESP32 GPIO 21 | TCS34725 SDA, display SDA | I2C data |
| ESP32 GPIO 22 | TCS34725 SCL, display SCL | I2C clock |
| ESP32 GPIO 34 | Size beam receiver + 10k to 3.3V | External pull-up required |
| ESP32 GPIO 35 | Chute exit beam receiver + 10k to 3.3V | External pull-up required |
| ESP32 GPIO 16 | Bin 1 beam receiver | Internal pull-up enabled in firmware |
| ESP32 GPIO 17 | Bin 2 beam receiver | Internal pull-up enabled in firmware |
| ESP32 GPIO 5 | Bin 3 beam receiver | Internal pull-up enabled in firmware |
| ESP32 GPIO 18 | Bin 4 beam receiver | Internal pull-up enabled in firmware |
| ESP32 GPIO 19 | Start button + external pull-up | Active low |
| ESP32 GPIO 2 | Piezo buzzer | -- |
