#pragma once
#include <Arduino.h>
#include "config.h"

// Pure functions. No hardware access. Fully testable without hardware.

enum class BrickSize : uint8_t {
    SIZE_2x2 = 0,
    SIZE_2x3 = 1,
    UNKNOWN  = 2,
};

enum class BrickColor : uint8_t {
    BLUE    = 0,
    RED     = 1,
    UNKNOWN = 2,
};

// Ordered to match routing table index: {2x2_BLUE=0, 2x2_RED=1, 2x3_RED=2, 2x3_BLUE=3}
enum class BrickType : uint8_t {
    TYPE_2x2_BLUE = 0,  // plow 1
    TYPE_2x2_RED  = 1,  // plow 2
    TYPE_2x3_RED  = 2,  // plow 3
    TYPE_2x3_BLUE = 3,  // default path, no solenoid fires
    UNKNOWN       = 4,
};

// Classify size from beam-to-beam gap. gap_us==0 means SIZE_TIMEOUT fired (2x2).
BrickSize classifySize(uint32_t gap_us);

// Returns R/(R+G+B) ratio for a single sample. Returns -1.0 if total is zero.
float colorRatio(uint16_t r, uint16_t g, uint16_t b);

// Returns true if sample is valid (above the black belt floor threshold).
bool isValidColorSample(uint16_t r, uint16_t g, uint16_t b, uint16_t c);

// Classify color from the accumulated sum across all valid samples.
// Returns UNKNOWN if sample_count is zero.
BrickColor classifyColor(uint32_t sum_r, uint32_t sum_g, uint32_t sum_b, uint16_t sample_count);

// Combine size and color into final brick type.
BrickType classifyBrick(BrickSize size, BrickColor color);

// Human-readable names for logging.
const char* sizeName(BrickSize s);
const char* colorName(BrickColor c);
const char* typeName(BrickType t);
