#pragma once
#include <Arduino.h>
#include "config.h"

enum class ThermalState { NORMAL, WARNING, DANGER };

class ThermalModel {
public:
    void onServoMove();
    void update();

    ThermalState state() const;

    float servoHeat() const;
    float maxHeat() const;

    const char* stateName() const;

private:
    float _servoHeat = 0.0f;
    uint32_t _lastUpdateMs = 0;
};

extern ThermalModel gThermal;
