#pragma once

#include <Arduino.h>
#include <Preferences.h>
#include "config.h"

namespace tuning_profile {

static constexpr uint32_t PROFILE_VERSION = 1;
static constexpr const char* PREFS_NAMESPACE = "sct26_tuning";

struct Profile {
    uint32_t version = PROFILE_VERSION;
    float colorThreshold = COLOR_RED_THRESHOLD;
    float sizeThresholdMm = SIZE_TWO_BY_THREE_MIN_LENGTH_MM;
    float beamSpacingMm = SIZE_BEAM_CENTER_SPACING_MM;
    uint32_t beltSampleCount = 0;
    float beltRedHz = 0.0f;
    float beltBlueHz = 0.0f;
    float beltClearHz = 0.0f;
};

inline Profile defaultProfile() {
    return Profile{};
}

inline void saveProfile(const Profile& profile) {
    Preferences prefs;
    prefs.begin(PREFS_NAMESPACE, false);
    prefs.putUInt("version", profile.version);
    prefs.putFloat("color_th", profile.colorThreshold);
    prefs.putFloat("size_mm", profile.sizeThresholdMm);
    prefs.putFloat("spacing", profile.beamSpacingMm);
    prefs.putUInt("belt_cnt", profile.beltSampleCount);
    prefs.putFloat("belt_red", profile.beltRedHz);
    prefs.putFloat("belt_blue", profile.beltBlueHz);
    prefs.putFloat("belt_clear", profile.beltClearHz);
    prefs.end();
}

inline bool loadProfile(Profile& profile) {
    Preferences prefs;
    prefs.begin(PREFS_NAMESPACE, true);
    uint32_t version = prefs.getUInt("version", 0);
    if (version != PROFILE_VERSION) {
        prefs.end();
        return false;
    }

    profile.version = version;
    profile.colorThreshold = prefs.getFloat("color_th", COLOR_RED_THRESHOLD);
    profile.sizeThresholdMm = prefs.getFloat("size_mm", SIZE_TWO_BY_THREE_MIN_LENGTH_MM);
    profile.beamSpacingMm = prefs.getFloat("spacing", SIZE_BEAM_CENTER_SPACING_MM);
    profile.beltSampleCount = prefs.getUInt("belt_cnt", 0);
    profile.beltRedHz = prefs.getFloat("belt_red", 0.0f);
    profile.beltBlueHz = prefs.getFloat("belt_blue", 0.0f);
    profile.beltClearHz = prefs.getFloat("belt_clear", 0.0f);
    prefs.end();
    return true;
}

inline void clearProfile() {
    Preferences prefs;
    prefs.begin(PREFS_NAMESPACE, false);
    prefs.clear();
    prefs.end();
}

}  // namespace tuning_profile
