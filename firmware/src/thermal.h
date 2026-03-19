#pragma once
#include <Arduino.h>
#include "config.h"

enum class ThermalState { NORMAL, WARNING, DANGER };

// Software thermal model. No temperature sensor needed.
// Heat accumulates on each actuator fire and decays exponentially over time.
// Protects solenoids and stepper during back-to-back demo runs.
class ThermalModel {
public:
    // Call on every solenoid fire. plow is 1-3.
    void onSolenoidFire(uint8_t plow);

    // Call on every stepper release.
    void onStepperRelease();

    // Call every loop iteration to apply time-based decay.
    void update();

    // Current thermal state across all actuators.
    ThermalState state() const;

    // Heat level for each actuator (0.0 = cool, 1.0 = fully saturated).
    float solenoidHeat(uint8_t plow) const;  // plow 1-3
    float stepperHeat() const;
    float maxHeat() const;

    // Stepper RPM adjusted down for thermal state.
    uint16_t adjustedStepperRpm() const;

    const char* stateName() const;

private:
    float    _solHeat[3]  = {0.0f, 0.0f, 0.0f};
    float    _stepHeat    = 0.0f;
    uint32_t _lastUpdateMs = 0;

    void applyDecay(float& heat, float dt_seconds);
};

extern ThermalModel gThermal;
