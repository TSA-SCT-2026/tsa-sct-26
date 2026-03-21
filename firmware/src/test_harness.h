#pragma once
#include <Arduino.h>

// Serial test harness. Reads commands from USB serial and injects events
// into gEventQueue exactly as real hardware ISRs would.
//
// Commands:
//
//   sim start                          inject START_BUTTON
//   sim sensing <cat>                  inject SENSING_DONE
//                                        cat: 2x2_RED | 2x2_BLUE | 2x3_RED | 2x3_BLUE
//   sim chute_exit                     inject CHUTE_EXIT (brick entered belt)
//   sim pusher <n>                     inject PUSHER_FIRED (n = 1-3)
//   sim bin <n>                        inject BIN{n}_CONFIRM (n = 1-4)
//   sim jam                            inject CONFIRM_TIMEOUT (no bin arrived)
//   sim reset                          inject RESET (clears ERROR_HALT)
//
//   test thermal [n]                   fire all solenoids n times, show heat evolution
//   test fullrun                       simulate a complete 24-brick run
//
//   log human                          switch to human-readable output (default)
//   log csv                            switch to CSV output (paste into spreadsheet)
//
//   status                             print current state, brick count, bin counts, thermal
//   help                               print this command list

namespace testHarness {
    void begin();
    void poll();  // call every loop iteration
}
