#include "classifier.h"

BrickSize classifySize(uint32_t gap_us) {
    // gap_us == 0 means SIZE_TIMEOUT fired: beam 2 never broke -> brick is 2x2
    if (gap_us == 0)                    return BrickSize::SIZE_2x2;
    if (gap_us < SIZE_THRESHOLD_US)     return BrickSize::SIZE_2x3;
    // Gap exists but above threshold: shouldn't happen with 150ms timeout, treat as 2x2
    return BrickSize::SIZE_2x2;
}

float colorRatio(uint16_t r, uint16_t g, uint16_t b) {
    uint32_t total = (uint32_t)r + g + b;
    if (total == 0) return -1.0f;
    return (float)r / (float)total;
}

bool isValidColorSample(uint16_t r, uint16_t g, uint16_t b, uint16_t c) {
    return ((uint32_t)r + g + b + c) >= COLOR_BLACK_FLOOR;
}

BrickColor classifyColor(uint32_t sum_r, uint32_t sum_g, uint32_t sum_b, uint16_t sample_count) {
    if (sample_count == 0) return BrickColor::UNKNOWN;
    float ratio = (float)sum_r / (float)(sum_r + sum_g + sum_b);
    return (ratio >= COLOR_RED_RATIO) ? BrickColor::RED : BrickColor::BLUE;
}

BrickType classifyBrick(BrickSize size, BrickColor color) {
    if (size == BrickSize::UNKNOWN || color == BrickColor::UNKNOWN) return BrickType::UNKNOWN;
    if (size == BrickSize::SIZE_2x2 && color == BrickColor::BLUE) return BrickType::TYPE_2x2_BLUE;
    if (size == BrickSize::SIZE_2x2 && color == BrickColor::RED)  return BrickType::TYPE_2x2_RED;
    if (size == BrickSize::SIZE_2x3 && color == BrickColor::RED)  return BrickType::TYPE_2x3_RED;
    if (size == BrickSize::SIZE_2x3 && color == BrickColor::BLUE) return BrickType::TYPE_2x3_BLUE;
    return BrickType::UNKNOWN;
}

const char* sizeName(BrickSize s) {
    switch (s) {
        case BrickSize::SIZE_2x2: return "2x2";
        case BrickSize::SIZE_2x3: return "2x3";
        default:                  return "UNKNOWN";
    }
}

const char* colorName(BrickColor c) {
    switch (c) {
        case BrickColor::RED:  return "RED";
        case BrickColor::BLUE: return "BLUE";
        default:               return "UNKNOWN";
    }
}

const char* typeName(BrickType t) {
    switch (t) {
        case BrickType::TYPE_2x2_BLUE: return "2x2_BLUE";
        case BrickType::TYPE_2x2_RED:  return "2x2_RED";
        case BrickType::TYPE_2x3_RED:  return "2x3_RED";
        case BrickType::TYPE_2x3_BLUE: return "2x3_BLUE";
        default:                       return "UNKNOWN";
    }
}
