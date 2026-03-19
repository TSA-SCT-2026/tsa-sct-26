#include "thermal.h"
#include <math.h>

ThermalModel gThermal;

void ThermalModel::onSolenoidFire(uint8_t plow) {
    if (plow < 1 || plow > 3) return;
    _solHeat[plow - 1] = min(1.0f, _solHeat[plow - 1] + THERMAL_HEAT_PER_SOL);
}

void ThermalModel::onStepperRelease() {
    _stepHeat = min(1.0f, _stepHeat + THERMAL_HEAT_PER_STEP);
}

void ThermalModel::update() {
    uint32_t now = millis();
    if (_lastUpdateMs == 0) {
        _lastUpdateMs = now;
        return;
    }
    float dt = (float)(now - _lastUpdateMs) / 1000.0f;
    _lastUpdateMs = now;

    // Exponential decay: heat *= e^(-DECAY_RATE * dt)
    float factor = expf(-THERMAL_DECAY_RATE * dt);
    for (int i = 0; i < 3; i++) _solHeat[i] *= factor;
    _stepHeat *= factor;
}

ThermalState ThermalModel::state() const {
    float m = maxHeat();
    if (m >= THERMAL_DANGER_LEVEL) return ThermalState::DANGER;
    if (m >= THERMAL_WARN_LEVEL)   return ThermalState::WARNING;
    return ThermalState::NORMAL;
}

float ThermalModel::solenoidHeat(uint8_t plow) const {
    if (plow < 1 || plow > 3) return 0.0f;
    return _solHeat[plow - 1];
}

float ThermalModel::stepperHeat() const {
    return _stepHeat;
}

float ThermalModel::maxHeat() const {
    float m = _stepHeat;
    for (int i = 0; i < 3; i++) m = max(m, _solHeat[i]);
    return m;
}

uint16_t ThermalModel::adjustedStepperRpm() const {
    switch (state()) {
        case ThermalState::WARNING: return STEPPER_RPM_WARNING;
        case ThermalState::DANGER:  return STEPPER_RPM_DANGER;
        default:                    return STEPPER_RPM_NORMAL;
    }
}

const char* ThermalModel::stateName() const {
    switch (state()) {
        case ThermalState::WARNING: return "WARNING";
        case ThermalState::DANGER:  return "DANGER";
        default:                    return "NORMAL";
    }
}
