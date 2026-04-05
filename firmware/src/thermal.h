#pragma once
#include <Arduino.h>
#include "config.h"

enum class ThermalState { NORMAL, WARNING, DANGER };

class ThermalModel {
public:
    void onSolenoidFire(uint8_t actuator);
    void onSelectorMove();
    void update();

    ThermalState state() const;

    float solenoidHeat(uint8_t actuator) const;
    float stepperHeat() const;
    float maxHeat() const;

    uint16_t adjustedSelectorRpm() const;
    uint16_t adjustedSelectorSps() const;

    const char* stateName() const;

private:
    float _solHeat[3] = {0.0f, 0.0f, 0.0f};
    float _selectorHeat = 0.0f;
    uint32_t _lastUpdateMs = 0;
};

extern ThermalModel gThermal;
