#include "thermal.h"
#include <math.h>

ThermalModel gThermal;

void ThermalModel::onServoMove() {
    _servoHeat = min(1.0f, _servoHeat + THERMAL_HEAT_PER_SERVO_MOVE);
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

    _servoHeat *= factor;
}

ThermalState ThermalModel::state() const {
    float m = maxHeat();
    if (m >= THERMAL_DANGER_LEVEL) return ThermalState::DANGER;
    if (m >= THERMAL_WARN_LEVEL) return ThermalState::WARNING;
    return ThermalState::NORMAL;
}

float ThermalModel::servoHeat() const {
    return _servoHeat;
}

float ThermalModel::maxHeat() const {
    return _servoHeat;
}

const char* ThermalModel::stateName() const {
    switch (state()) {
        case ThermalState::WARNING: return "WARNING";
        case ThermalState::DANGER:  return "DANGER";
        default:                    return "NORMAL";
    }
}
