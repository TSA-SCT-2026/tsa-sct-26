#pragma once
#include <Arduino.h>

// Serial test harness. Reads commands from USB serial and injects events
// into gEventQueue exactly as real hardware ISRs would.
//
// Commands:
//
//   sim start                    inject START_BUTTON
//   sim beam1                    inject BEAM1_BREAK (brick entered sensing zone)
//   sim beam2 <gap_us>           inject BEAM2_BREAK with given gap (2x3 = ~95000)
//   sim timeout                  inject SIZE_TIMEOUT (2x2 brick)
//   sim color <r> <g> <b> <c>   inject one COLOR_SAMPLE reading
//   sim colordone                inject COLOR_DONE (end of dwell window)
//   sim bin <n>                  inject BIN{n}_CONFIRM (n = 1-4)
//   sim jam                      inject CONFIRM_TIMEOUT (no bin arrived)
//   sim reset                    inject RESET (clears ERROR_HALT)
//
//   test classify <r> <g> <b> <gap_us>   classify a brick from raw inputs, print result
//   test route <size> <color>             show plow and bin for a brick type
//   test thermal <n>                      fire all solenoids n times, show heat evolution
//   test fullrun                          simulate a complete 24-brick run
//
//   log human                    switch to human-readable output (default)
//   log csv                      switch to CSV output (paste into spreadsheet)
//
//   status                       print current state, brick count, bin counts, thermal
//   help                         print this command list

namespace testHarness {
    void begin();
    void poll();  // call every loop iteration
}
