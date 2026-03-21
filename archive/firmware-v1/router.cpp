#include "router.h"

// Indexed by BrickType enum: {2x2_BLUE=0, 2x2_RED=1, 2x3_RED=2, 2x3_BLUE=3, UNKNOWN=4}
static const uint8_t PLOW_TABLE[5] = {1, 2, 3, 0, 0};
static const uint8_t BIN_TABLE[5]  = {1, 2, 3, 4, 0};

uint8_t routePlow(BrickType type) {
    return PLOW_TABLE[(uint8_t)type];
}

uint8_t targetBin(BrickType type) {
    return BIN_TABLE[(uint8_t)type];
}
