#pragma once
#include "classifier.h"

// Pure routing logic. No hardware access.

// Returns which plow to fire (1-3), or 0 for the default path (no solenoid).
// 2x3 blue -> 0 (most common brick, default path minimizes solenoid duty cycle)
// 2x2 blue -> 1
// 2x2 red  -> 2
// 2x3 red  -> 3
uint8_t routePlow(BrickType type);

// Returns which bin (1-4) a brick type should land in.
uint8_t targetBin(BrickType type);
