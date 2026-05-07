#include "sensors.h"
#include "config.h"
#include "events.h"
#include "logger.h"
#include "tuning_profile.h"

#include <math.h>
#include <driver/gpio.h>

namespace sensors {

namespace {

enum FilterMode : uint8_t { FILTER_RED, FILTER_BLUE, FILTER_CLEAR };

struct ColorReading {
    float rawRedHz   = 0.0f;
    float rawBlueHz  = 0.0f;
    float rawClearHz = 0.0f;
    float redHz      = 0.0f;
    float blueHz     = 0.0f;
    float clearHz    = 0.0f;
    float redRatio   = 0.0f;
    bool ok          = false;
    bool beltRejected = false;
    bool bestEffort  = false;
};

struct ColorAccumulator {
    uint16_t totalSamples          = 0;
    uint16_t keptSamples           = 0;
    uint16_t beltRejectedSamples   = 0;
    float sumRawRedHz              = 0.0f;
    float sumRawBlueHz             = 0.0f;
    float sumRawClearHz            = 0.0f;
    float sumRedNetHz              = 0.0f;
    float sumBlueNetHz             = 0.0f;
    float sumClearNetHz            = 0.0f;
    float bestRejectedScore        = -1.0f;
    ColorReading bestRejected;
};

struct BeltBaseline {
    uint32_t count  = 0;
    float sumRedHz  = 0.0f;
    float sumBlueHz = 0.0f;
    float sumClearHz = 0.0f;
};

static constexpr uint32_t COLOR_TIMEOUT_US  = 25000;
static constexpr uint8_t  COLOR_PASS_CYCLES = 4;
static constexpr uint32_t SENSE_PASS_TIMEOUT_MS = 3500;
static constexpr float    PASS_MIN_BLOCKED_MS = 5.0f;
static constexpr uint32_t COLOR_READ_BUDGET_US = 3000;

static float gActiveColorThreshold   = COLOR_RED_THRESHOLD;
static float gActiveSizeThresholdMm  = SIZE_TWO_BY_THREE_MIN_LENGTH_MM;
static float gBeamSpacingMm          = SIZE_BEAM_CENTER_SPACING_MM;
static BeltBaseline gBeltBaseline;

static volatile uint32_t gBeamAEnterUs = 0;
static volatile uint32_t gBeamALastExitUs = 0;
static volatile uint32_t gBeamBLastEnterUs = 0;
static volatile uint32_t gBeamBLastExitUs = 0;
static volatile uint8_t gBeamAEdgeCount = 0;
static volatile uint8_t gBeamBEdgeCount = 0;
static volatile bool gBeamABlocked = false;
static volatile bool gBeamBBlocked = false;
static bool gPassArmed    = false;
static bool gLastBeamA    = false;
static bool gLastBeamB    = false;
static float gLastSpeedMms = CONVEYOR_TARGET_SPEED_MM_S;

static bool gHasSimulatedSense = false;
static SenseResult gSimulatedSense;

static float avgBeltRedHz()   { return gBeltBaseline.count == 0 ? 0.0f : gBeltBaseline.sumRedHz   / (float)gBeltBaseline.count; }
static float avgBeltBlueHz()  { return gBeltBaseline.count == 0 ? 0.0f : gBeltBaseline.sumBlueHz  / (float)gBeltBaseline.count; }
static float avgBeltClearHz() { return gBeltBaseline.count == 0 ? 0.0f : gBeltBaseline.sumClearHz / (float)gBeltBaseline.count; }

void IRAM_ATTR onBeamAChange() {
    uint32_t nowUs = micros();
    bool blocked = gpio_get_level((gpio_num_t)PIN_SIZE_SENSOR_A) == 0;
    gBeamAEdgeCount++;
    if (blocked && !gBeamABlocked) {
        gBeamAEnterUs = nowUs;
    } else if (!blocked) {
        gBeamALastExitUs = nowUs;
    }
    gBeamABlocked = blocked;
}

void IRAM_ATTR onBeamBChange() {
    uint32_t nowUs = micros();
    bool blocked = gpio_get_level((gpio_num_t)PIN_SIZE_SENSOR_B) == 0;
    gBeamBEdgeCount++;
    if (blocked && !gBeamBBlocked) {
        gBeamBLastEnterUs = nowUs;
    } else if (!blocked) {
        gBeamBLastExitUs = nowUs;
    }
    gBeamBBlocked = blocked;
}

static void setColorFilter(FilterMode mode) {
    switch (mode) {
        case FILTER_RED:
            digitalWrite(PIN_COLOR_S2, LOW);
            digitalWrite(PIN_COLOR_S3, LOW);
            break;
        case FILTER_BLUE:
            digitalWrite(PIN_COLOR_S2, LOW);
            digitalWrite(PIN_COLOR_S3, HIGH);
            break;
        case FILTER_CLEAR:
            digitalWrite(PIN_COLOR_S2, HIGH);
            digitalWrite(PIN_COLOR_S3, LOW);
            break;
    }
    delay(3);
}

static float readFilterHz(FilterMode mode, uint8_t cycles) {
    setColorFilter(mode);
    uint64_t totalPeriodUs = 0;
    uint8_t goodCycles = 0;
    for (uint8_t i = 0; i < cycles; i++) {
        uint32_t lowUs  = pulseIn(PIN_COLOR_OUT, LOW,  COLOR_READ_BUDGET_US);
        uint32_t highUs = pulseIn(PIN_COLOR_OUT, HIGH, COLOR_READ_BUDGET_US);
        if (lowUs == 0 || highUs == 0) continue;
        totalPeriodUs += lowUs + highUs;
        goodCycles++;
    }
    if (goodCycles == 0 || totalPeriodUs == 0) return 0.0f;
    return 1000000.0f / ((float)totalPeriodUs / (float)goodCycles);
}

static ColorReading sampleColorRaw(uint8_t cycles) {
    ColorReading r;
    r.rawRedHz   = readFilterHz(FILTER_RED,   cycles);
    r.rawBlueHz  = readFilterHz(FILTER_BLUE,  cycles);
    r.rawClearHz = readFilterHz(FILTER_CLEAR, cycles);
    r.redHz   = r.rawRedHz;
    r.blueHz  = r.rawBlueHz;
    r.clearHz = r.rawClearHz;
    if (r.rawRedHz <= 0.0f || r.rawBlueHz <= 0.0f || r.rawClearHz <= 0.0f) return r;
    r.ok = true;
    return r;
}

static ColorReading processColorReading(const ColorReading& raw) {
    if (!raw.ok) return raw;
    ColorReading p = raw;
    float redNet   = raw.redHz;
    float blueNet  = raw.blueHz;
    float clearNet = raw.clearHz;
    if (gBeltBaseline.count > 0) {
        redNet   = fmaxf(0.0f, raw.redHz   - avgBeltRedHz());
        blueNet  = fmaxf(0.0f, raw.blueHz  - avgBeltBlueHz());
        clearNet = fmaxf(0.0f, raw.clearHz - avgBeltClearHz());
    }
    p.redHz   = redNet;
    p.blueHz  = blueNet;
    p.clearHz = clearNet;
    float colorSum = redNet + blueNet;
    if (colorSum < COLOR_BELT_MIN_NET_SUM_HZ || clearNet < COLOR_BELT_MIN_NET_CLEAR_HZ) {
        p.ok = false;
        p.beltRejected = gBeltBaseline.count > 0;
        p.redRatio = 0.0f;
        return p;
    }
    p.redRatio = redNet / colorSum;
    p.ok = true;
    p.beltRejected = false;
    return p;
}

static void accumulateColorSample(ColorAccumulator& accum,
                                  const ColorReading& raw,
                                  const ColorReading& processed) {
    accum.totalSamples++;
    accum.sumRawRedHz   += raw.rawRedHz;
    accum.sumRawBlueHz  += raw.rawBlueHz;
    accum.sumRawClearHz += raw.rawClearHz;
    if (!processed.ok) {
        accum.beltRejectedSamples++;
        float score = processed.redHz + processed.blueHz + processed.clearHz;
        if (score > accum.bestRejectedScore) {
            accum.bestRejectedScore = score;
            accum.bestRejected = processed;
            accum.bestRejected.rawRedHz   = raw.rawRedHz;
            accum.bestRejected.rawBlueHz  = raw.rawBlueHz;
            accum.bestRejected.rawClearHz = raw.rawClearHz;
        }
        return;
    }
    accum.keptSamples++;
    accum.sumRedNetHz   += processed.redHz;
    accum.sumBlueNetHz  += processed.blueHz;
    accum.sumClearNetHz += processed.clearHz;
}

static ColorReading finalizePassColor(const ColorAccumulator& accum) {
    ColorReading result;
    if (accum.keptSamples < COLOR_PASS_MIN_SAMPLES) {
        if (accum.bestRejectedScore <= 0.0f) return result;
        result = accum.bestRejected;
        float colorSum = result.redHz + result.blueHz;
        if (colorSum <= 0.0f) return result;
        result.redRatio = result.redHz / colorSum;
        result.ok = false;
        result.beltRejected = true;
        result.bestEffort = true;
        return result;
    }
    float n = (float)accum.totalSamples;
    float kept = (float)accum.keptSamples;
    result.rawRedHz   = accum.sumRawRedHz   / n;
    result.rawBlueHz  = accum.sumRawBlueHz  / n;
    result.rawClearHz = accum.sumRawClearHz / n;
    result.redHz   = accum.sumRedNetHz   / kept;
    result.blueHz  = accum.sumBlueNetHz  / kept;
    result.clearHz = accum.sumClearNetHz / kept;
    float colorSum = result.redHz + result.blueHz;
    if (colorSum <= 0.0f) return result;
    result.redRatio = result.redHz / colorSum;
    result.ok = true;
    return result;
}

}  // namespace

void begin() {
    // Keep internal pull-ups enabled as a firmware safety net.
    // External 10k pull-ups are still the primary hardware path.
    pinMode(PIN_SIZE_SENSOR_A, INPUT_PULLUP);
    pinMode(PIN_SIZE_SENSOR_B, INPUT_PULLUP);
    pinMode(PIN_COLOR_OUT, INPUT);
    pinMode(PIN_COLOR_S2, OUTPUT);
    pinMode(PIN_COLOR_S3, OUTPUT);
    // No physical start button: rocker switch supplies power, firmware auto-starts.

    tuning_profile::Profile profile;
    if (tuning_profile::loadProfile(profile)) {
        gActiveColorThreshold  = profile.colorThreshold;
        gActiveSizeThresholdMm = profile.sizeThresholdMm;
        gBeamSpacingMm         = profile.beamSpacingMm;
        if (profile.beltSampleCount > 0) {
            gBeltBaseline.count    = profile.beltSampleCount;
            gBeltBaseline.sumRedHz   = profile.beltRedHz   * (float)profile.beltSampleCount;
            gBeltBaseline.sumBlueHz  = profile.beltBlueHz  * (float)profile.beltSampleCount;
            gBeltBaseline.sumClearHz = profile.beltClearHz * (float)profile.beltSampleCount;
        }
        gLogger.info("sensors: loaded tuning profile from NVS");
    } else {
        gLogger.info("sensors: using config defaults");
    }

    gPassArmed    = false;
    gLastBeamA    = (digitalRead(PIN_SIZE_SENSOR_A) == LOW);
    gLastBeamB    = (digitalRead(PIN_SIZE_SENSOR_B) == LOW);
    gBeamABlocked = gLastBeamA;
    gBeamBBlocked = gLastBeamB;
    gPassArmed    = !gLastBeamA;

    attachInterrupt(digitalPinToInterrupt(PIN_SIZE_SENSOR_A), onBeamAChange, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_SIZE_SENSOR_B), onBeamBChange, CHANGE);
}

void pollInputs() {
    bool beamABlocked = (digitalRead(PIN_SIZE_SENSOR_A) == LOW);
    bool beamBBlocked = (digitalRead(PIN_SIZE_SENSOR_B) == LOW);

    // Beam A is the normal entry trigger. Beam B can start a fallback pass
    // if the brick misses beam A.
    if (!beamABlocked) {
        gPassArmed = true;
    }

    if (beamABlocked && !gLastBeamA && gPassArmed) {
        uint32_t nowUs = micros();
        // If ISR timing missed this entry edge, avoid reusing a stale timestamp
        // from a previous pass. Keep ISR timing when it is fresh.
        if (gBeamAEnterUs == 0 || (uint32_t)(nowUs - gBeamAEnterUs) > 300000UL) {
            gBeamAEnterUs = nowUs;
        }
        // ISR already set gBeamAEnterUs at the true interrupt time.
        // Do not overwrite with micros() here - that later timestamp would
        // make gBeamBLastEnterUs (also ISR-set) appear to precede aEnterUs,
        // breaking the early-pass detection in senseBrickAtStation.
        gPassArmed = false;
        pushEvent(EventType::SIZE_ENTRY_DETECTED);
    }

    if (beamBBlocked && !gLastBeamB && gPassArmed) {
        uint32_t nowUs = micros();
        if (gBeamBLastEnterUs == 0 || (uint32_t)(nowUs - gBeamBLastEnterUs) > 300000UL) {
            gBeamBLastEnterUs = nowUs;
        }
        gPassArmed = false;
        pushEvent(EventType::SIZE_ENTRY_DETECTED);
    }

    gLastBeamA = beamABlocked;
    gLastBeamB = beamBBlocked;
}

SenseResult senseBrickAtStation() {
    if (gHasSimulatedSense) {
        gHasSimulatedSense = false;
        return gSimulatedSense;
    }

    SenseResult result;

    uint32_t aEnterUs = gBeamAEnterUs;
    uint32_t aExitUs  = 0;
    uint32_t bEnterUs = 0;
    uint32_t bExitUs  = 0;
    uint8_t  aStartEdges = gBeamAEdgeCount;
    uint8_t  bStartEdges = gBeamBEdgeCount;
    bool aExitSeen  = false;
    bool bEnterSeen = false;
    bool bExitSeen  = false;
    bool beamAWasBlocked = gBeamABlocked;
    bool beamBWasBlocked = gBeamBBlocked;

    ColorAccumulator colorAccum;
    uint32_t lastColorSampleUs = micros();
    uint32_t passStartMs       = millis();

    if (aEnterUs == 0 && beamAWasBlocked) {
        aEnterUs = lastColorSampleUs;
    }
    // Catch beam A that already exited before this function started.
    if (!aExitSeen && aEnterUs > 0 && gBeamALastExitUs >= aEnterUs) {
        aExitSeen = true;
        aExitUs   = gBeamALastExitUs;
    }
    if (beamBWasBlocked) {
        bEnterSeen = true;
        bEnterUs = (gBeamBLastEnterUs >= aEnterUs && gBeamBLastEnterUs > 0) ? gBeamBLastEnterUs : lastColorSampleUs;
    }
    // Catch beam B that already passed before this function started.
    // This works because both gBeamBLastEnterUs and gBeamAEnterUs are now
    // set by ISRs at interrupt time, so the timestamps are directly comparable.
    if (!bEnterSeen && aEnterUs > 0 && gBeamBLastEnterUs >= aEnterUs) {
        bEnterSeen = true;
        bEnterUs = gBeamBLastEnterUs;
        if (gBeamBLastExitUs >= bEnterUs) {
            bExitSeen = true;
            bExitUs = gBeamBLastExitUs;
        }
    }

    while (!bExitSeen) {
        if (millis() - passStartMs > SENSE_PASS_TIMEOUT_MS) {
            char tbuf[96];
            snprintf(tbuf, sizeof(tbuf),
                     "sense: timeout aExit=%s bEnter=%s aUs=%lu aExUs=%lu bEnUs=%lu",
                     aExitSeen ? "y" : "n", bEnterSeen ? "y" : "n",
                     (unsigned long)aEnterUs,
                     (unsigned long)gBeamALastExitUs,
                     (unsigned long)gBeamBLastEnterUs);
            gLogger.info(tbuf);
            break;
        }

        uint32_t nowUs = micros();

        if ((uint32_t)(nowUs - lastColorSampleUs) >= COLOR_PASS_SAMPLE_INTERVAL_US) {
            lastColorSampleUs = nowUs;
            ColorReading raw       = sampleColorRaw(COLOR_PASS_CYCLES);
            ColorReading processed = processColorReading(raw);
            accumulateColorSample(colorAccum, raw, processed);
        }

        if (!aExitSeen && !gBeamABlocked && gBeamAEdgeCount != aStartEdges && gBeamALastExitUs >= aEnterUs) {
            aExitSeen = true;
            aExitUs   = gBeamALastExitUs;
        }

        if (!bEnterSeen && gBeamBEdgeCount != bStartEdges && gBeamBLastEnterUs >= aEnterUs) {
            bEnterSeen = true;
            bEnterUs   = gBeamBLastEnterUs;
        }

        if (bEnterSeen && !bExitSeen && !gBeamBBlocked && gBeamBLastExitUs >= bEnterUs) {
            bExitSeen = true;
            bExitUs   = gBeamBLastExitUs;
        }

        delay(1);
    }

    // Last-chance aExit capture: handles the case where bExitSeen was set in the
    // pre-loop early-pass block (loop never ran) but aExit happened after aEnterUs.
    if (!aExitSeen && aEnterUs > 0 && gBeamALastExitUs >= aEnterUs) {
        aExitSeen = true;
        aExitUs   = gBeamALastExitUs;
    }

    // Finalize color
    ColorReading finalColor = finalizePassColor(colorAccum);
    result.redRatio    = finalColor.redRatio;
    result.rawRedHz    = finalColor.rawRedHz;
    result.rawBlueHz   = finalColor.rawBlueHz;
    result.rawClearHz  = finalColor.rawClearHz;
    result.redNetHz    = finalColor.redHz;
    result.blueNetHz   = finalColor.blueHz;
    result.sampleCount = colorAccum.keptSamples;

    // Compute size metrics
    float blockedAMs = (aExitSeen  && aEnterUs > 0) ? (float)(aExitUs  - aEnterUs) / 1000.0f : 0.0f;
    float blockedBMs = (bExitSeen  && bEnterUs > 0) ? (float)(bExitUs  - bEnterUs) / 1000.0f : 0.0f;
    bool aOk = blockedAMs >= PASS_MIN_BLOCKED_MS;
    bool bOk = blockedBMs >= PASS_MIN_BLOCKED_MS;

    bool haveBlockedSample = aOk || bOk;
    bool haveBeamBTiming = bEnterSeen && bExitSeen;
    result.sizeValid = false;

    if (haveBlockedSample) {
        float beamLeadDeltaS  = (bEnterSeen && aEnterUs > 0) ? (float)(bEnterUs - aEnterUs) / 1000000.0f : 0.0f;
        float beamTrailDeltaS = (aExitSeen  && bExitSeen)    ? (float)(bExitUs  - aExitUs)  / 1000000.0f : 0.0f;

        float vLead = 0.0f, vTrail = 0.0f, vAvg = 0.0f;
        if (beamLeadDeltaS > 0.0f && beamTrailDeltaS > 0.0f) {
            vLead  = gBeamSpacingMm / beamLeadDeltaS;
            vTrail = gBeamSpacingMm / beamTrailDeltaS;
            vAvg   = (vLead + vTrail) * 0.5f;
        } else if (beamLeadDeltaS > 0.0f) {
            vLead = gBeamSpacingMm / beamLeadDeltaS;
            vAvg  = vLead;
        }

        result.beltSpeedLeadMms  = vLead;
        result.beltSpeedTrailMms = vTrail;
        result.beltSpeedAvgMms   = vAvg;
        if (vAvg > 0.0f) gLastSpeedMms = vAvg;

        float lengthSpeedMms = vAvg > 0.0f ? vAvg : CONVEYOR_SENSING_FALLBACK_SPEED_MM_S;
        float lenA = lengthSpeedMms * (blockedAMs / 1000.0f);
        float lenB = lengthSpeedMms * (blockedBMs / 1000.0f);
        result.measuredLengthAmm = lenA;
        result.measuredLengthBmm = lenB;

        float chosenLen = 0.0f;
        if (aOk && bOk) {
            float lenAvg    = (lenA + lenB) * 0.5f;
            float speedSkew = vAvg > 0.0f ? fabsf(vLead - vTrail) / vAvg * 100.0f : 0.0f;
            float lenSkew   = lenAvg > 0.0f ? fabsf(lenA - lenB) / lenAvg * 100.0f : 0.0f;
            result.speedSkewPct = speedSkew;
            result.lengthSkewPct = lenSkew;
            uint8_t aExtraEdges = (uint8_t)(gBeamAEdgeCount - aStartEdges);
            uint8_t bExtraEdges = (uint8_t)(gBeamBEdgeCount - bStartEdges);
            if (aExtraEdges <= 1 && bExtraEdges <= 2 && speedSkew <= 40.0f && lenSkew <= 40.0f) {
                chosenLen = lenAvg;
            } else if (aExtraEdges < bExtraEdges) {
                chosenLen = lenA;
            } else if (bExtraEdges < aExtraEdges) {
                chosenLen = lenB;
            } else if (lenSkew > 40.0f || speedSkew > 40.0f) {
                chosenLen = fmaxf(lenA, lenB);
            } else {
                chosenLen = lenAvg;
            }
        } else if (aOk) {
            chosenLen = lenA;
        } else {
            chosenLen = lenB;
        }

        result.measuredLengthAvgMm = chosenLen;
        result.isTwoByThree = chosenLen >= gActiveSizeThresholdMm;
        // States scoring priority is accuracy. If beam B timing is missing,
        // keep classification uncertain instead of trusting one-beam fallback.
        result.sizeValid = haveBeamBTiming;
    }

    // Classify
    bool colorUsable = finalColor.ok || finalColor.bestEffort;
    bool isRed       = colorUsable ? (finalColor.redRatio >= gActiveColorThreshold) : false;

    if (result.sizeValid && colorUsable) {
        if (result.isTwoByThree) {
            result.category = isRed ? BrickCategory::CAT_2x3_RED : BrickCategory::CAT_2x3_BLUE;
        } else {
            result.category = isRed ? BrickCategory::CAT_2x2_RED : BrickCategory::CAT_2x2_BLUE;
        }
    } else {
        result.category = BrickCategory::UNCERTAIN;
    }

    char buf[128];
    snprintf(buf, sizeof(buf),
             "sense: cat=%s len=%.1fmm red=%.4f samp=%u/%u bTiming=%s",
             categoryName(result.category),
             result.measuredLengthAvgMm,
             result.redRatio,
             colorAccum.keptSamples,
             colorAccum.totalSamples,
             haveBeamBTiming ? "y" : "n");
    gLogger.info(buf);

    return result;
}

float activeColorThreshold()   { return gActiveColorThreshold; }
float activeSizeThresholdMm()  { return gActiveSizeThresholdMm; }

void setSimulatedSenseResult(const SenseResult& result) {
    gSimulatedSense    = result;
    gHasSimulatedSense = true;
}

void clearSimulatedSenseResult() {
    gHasSimulatedSense = false;
}

void resetPassCapture() {
    noInterrupts();
    gBeamAEnterUs = 0;
    gBeamALastExitUs = 0;
    gBeamBLastEnterUs = 0;
    gBeamBLastExitUs = 0;
    gBeamAEdgeCount = 0;
    gBeamBEdgeCount = 0;
    interrupts();

    gLastBeamA = (digitalRead(PIN_SIZE_SENSOR_A) == LOW);
    gLastBeamB = (digitalRead(PIN_SIZE_SENSOR_B) == LOW);
    gBeamABlocked = gLastBeamA;
    gBeamBBlocked = gLastBeamB;
    gPassArmed = !gLastBeamA && !gLastBeamB;
}

void attachBinBeams() {
    // Bin exit detection is not installed.
}

float beltSpeedMms() {
    return gLastSpeedMms;
}

float activeBeamSpacingMm() {
    return gBeamSpacingMm;
}

const char* categoryName(BrickCategory cat) {
    switch (cat) {
        case BrickCategory::CAT_2x2_RED:  return "2x2_RED";
        case BrickCategory::CAT_2x2_BLUE: return "2x2_BLUE";
        case BrickCategory::CAT_2x3_BLUE: return "2x3_BLUE";
        case BrickCategory::CAT_2x3_RED:  return "2x3_RED";
        default:                          return "UNCERTAIN";
    }
}

void captureBeltBaseline(uint8_t samples) {
    gBeltBaseline = BeltBaseline{};
    char buf[64];
    snprintf(buf, sizeof(buf), "cal: capturing %u belt baseline samples", samples);
    gLogger.info(buf);
    for (uint8_t i = 0; i < samples; i++) {
        ColorReading r = sampleColorRaw(COLOR_PASS_CYCLES);
        if (r.rawRedHz <= 0.0f || r.rawBlueHz <= 0.0f || r.rawClearHz <= 0.0f) {
            gLogger.info("cal: belt sample invalid, skipping");
            continue;
        }
        gBeltBaseline.count++;
        gBeltBaseline.sumRedHz   += r.rawRedHz;
        gBeltBaseline.sumBlueHz  += r.rawBlueHz;
        gBeltBaseline.sumClearHz += r.rawClearHz;
        delay(30);
    }
    snprintf(buf, sizeof(buf), "cal: belt baseline set n=%u red=%.1f blue=%.1f clear=%.1f",
             gBeltBaseline.count, avgBeltRedHz(), avgBeltBlueHz(), avgBeltClearHz());
    gLogger.info(buf);
}

void setColorThreshold(float t) {
    gActiveColorThreshold = t;
    char buf[48];
    snprintf(buf, sizeof(buf), "cal: color threshold -> %.4f", t);
    gLogger.info(buf);
}

void setSizeThreshold(float mm) {
    gActiveSizeThresholdMm = mm;
    char buf[48];
    snprintf(buf, sizeof(buf), "cal: size threshold -> %.2f mm", mm);
    gLogger.info(buf);
}

bool saveCalibration() {
    tuning_profile::Profile p;
    p.colorThreshold  = gActiveColorThreshold;
    p.sizeThresholdMm = gActiveSizeThresholdMm;
    p.beamSpacingMm   = gBeamSpacingMm;
    if (gBeltBaseline.count > 0) {
        p.beltSampleCount = gBeltBaseline.count;
        p.beltRedHz   = avgBeltRedHz();
        p.beltBlueHz  = avgBeltBlueHz();
        p.beltClearHz = avgBeltClearHz();
    }
    tuning_profile::saveProfile(p);
    gLogger.info("cal: saved to NVS");
    return true;
}

void showCalibration() {
    char buf[96];
    snprintf(buf, sizeof(buf), "cal: color=%.4f size=%.2fmm spacing=%.2fmm",
             gActiveColorThreshold, gActiveSizeThresholdMm, gBeamSpacingMm);
    gLogger.info(buf);
    if (gBeltBaseline.count > 0) {
        snprintf(buf, sizeof(buf), "cal: belt n=%u red=%.1f blue=%.1f clear=%.1f",
                 gBeltBaseline.count, avgBeltRedHz(), avgBeltBlueHz(), avgBeltClearHz());
    } else {
        snprintf(buf, sizeof(buf), "cal: belt baseline not set");
    }
    gLogger.info(buf);
}

}  // namespace sensors
