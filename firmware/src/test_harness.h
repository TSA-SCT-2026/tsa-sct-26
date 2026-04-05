#pragma once
#include <Arduino.h>

// Serial harness for exercising the event contract.
//
// Commands:
//   sim start
//   sim entry
//   sim seated
//   sim sensing <cat>
//   sim bin <1-4>
//   sim platform
//   sim reset
//
//   test release
//   test home
//   test index <1-4>
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
