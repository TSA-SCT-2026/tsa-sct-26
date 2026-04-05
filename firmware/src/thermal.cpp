#include "thermal.h"
#include <math.h>

ThermalModel gThermal;

void ThermalModel::onSolenoidFire(uint8_t actuator) {
    if (actuator < 1 || actuator > 3) return;
    _solHeat[actuator - 1] = min(1.0f, _solHeat[actuator - 1] + THERMAL_HEAT_PER_SOL);
}

void ThermalModel::onSelectorMove() {
    _selectorHeat = min(1.0f, _selectorHeat + THERMAL_HEAT_PER_STEP);
}

void ThermalModel::update() {
    uint32_t now = millis();
    if (_lastUpdateMs == 0) {
        _lastUpdateMs = now;
        return;
    }

    float dt = (float)(now - _lastUpdateMs) / 1000.0f;
    _lastUpdateMs = now;
    float factor = expf(-THERMAL_DECAY_RATE * dt);

    for (int i = 0; i < 3; i++) _solHeat[i] *= factor;
    _selectorHeat *= factor;
}

ThermalState ThermalModel::state() const {
    float m = maxHeat();
    if (m >= THERMAL_DANGER_LEVEL) return ThermalState::DANGER;
    if (m >= THERMAL_WARN_LEVEL) return ThermalState::WARNING;
    return ThermalState::NORMAL;
}

float ThermalModel::solenoidHeat(uint8_t actuator) const {
    if (actuator < 1 || actuator > 3) return 0.0f;
    return _solHeat[actuator - 1];
}

float ThermalModel::stepperHeat() const {
    return _selectorHeat;
}

float ThermalModel::maxHeat() const {
    float m = _selectorHeat;
    for (int i = 0; i < 3; i++) m = max(m, _solHeat[i]);
    return m;
}

uint16_t ThermalModel::adjustedSelectorRpm() const {
    return (uint16_t)((uint32_t)adjustedSelectorSps() * 60u / SELECTOR_STEPS_PER_REV);
}

uint16_t ThermalModel::adjustedSelectorSps() const {
    switch (state()) {
        case ThermalState::WARNING: return SELECTOR_SPS_WARNING;
        case ThermalState::DANGER:  return SELECTOR_SPS_DANGER;
        default:                    return SELECTOR_SPS_NORMAL;
    }
}

const char* ThermalModel::stateName() const {
    switch (state()) {
        case ThermalState::WARNING: return "WARNING";
        case ThermalState::DANGER:  return "DANGER";
        default:                    return "NORMAL";
    }
}
