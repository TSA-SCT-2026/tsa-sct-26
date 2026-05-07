#pragma once
#include <Arduino.h>

// Serial harness for exercising the event contract.
//
// Commands:
//   sim start
//   sim brick
//   sim sensing <cat>
//   sim bin <1-4>
//   sim reset
//
//   test home
//   test route <1-4>
//   test fullrun
//   test thermal [n]
//
//   log csv
//   log human
//   status
//   help

namespace testHarness {
    void begin();
    void poll();
}
