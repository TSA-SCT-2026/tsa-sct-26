# LEGO Brick Sorter

An ESP32-controlled conveyor that classifies red and blue 2x2 and 2x3 LEGO
bricks, then positions a servo-driven chute over one of four bins. The project
was built for the 2026 TSA System Control Technology event.

This repository is the compact technical record: buildable firmware, an
operator-assisted calibration workflow, a reconstruction guide, a normalized
bill of materials, and a conveyor CAD export.

> **Media placeholder:** add a final-system hero image here after the team
> selects an approved photo.

> **Demo placeholder:** add the best uninterrupted sorting-run video here. Do
> not publish an accuracy or throughput number until it is reproduced from the
> recorded run or a new evidence-producing test.

## System overview

```text
single-brick feed
  -> NEMA17 / GT2 conveyor
  -> two break-beams + shrouded TCS3200 sensor
  -> size and color classification
  -> timed MG995/MG996 chute position
  -> one of four labeled bins
```

The firmware measures each beam's blocked duration and the transit time
between beams. From those timings it estimates belt speed and brick length.
The color sensor samples red, blue, and clear channels; when a belt baseline is
available, it subtracts that baseline before computing `red / (red + blue)`.
The current controller handles one brick at a time so the chute is committed
before the brick reaches the belt exit.

| Bin | Classification | Target count in the event set |
| --- | --- | ---: |
| 1 | 2x2 red | 6 |
| 2 | 2x3 red | 4 |
| 3 | 2x3 blue | 8 |
| 4 | 2x2 blue | 6 |

The counts above describe the configured 24-brick test set. They are not a
claim that a physical acceptance run has passed.

## Calibration

Calibration is **operator-assisted**, not automatic threshold fitting. The
firmware can capture a moving-belt baseline, display the active settings,
accept manually selected color and size thresholds, and persist the resulting
profile in ESP32 NVS. Selecting thresholds still requires reviewing real runs
from the assembled machine.

See [BUILD.md](BUILD.md#calibration) for the procedure and commands.

## Evidence and verification

| Check | Current evidence |
| --- | --- |
| Main ESP32 firmware | PlatformIO build passes |
| Servo-tuning utility | PlatformIO `servo_tuning` build passes |
| Physical 24-brick result | TODO: add an uninterrupted run and its serial log |
| Accuracy and throughput | TODO: calculate only from a documented physical run |
| Wiring diagram | TODO: add `hardware/wiring.png` after validating it against the machine |
| CAD provenance | TODO: confirm authorship, finality, and redistribution approval for the conveyor STEP export |

No browser simulator is included in this portfolio tree. It was useful during
development, but the physical firmware and hardware evidence are the relevant
public artifacts.

## Code tour

- [Sensor sampling and classification](firmware/tsa_sorter/sensors.cpp)
- [Sequential control state machine](firmware/tsa_sorter/state_machine.cpp)
- [Conveyor and chute actuation](firmware/tsa_sorter/actuators.cpp)
- [Serial calibration and test harness](firmware/tsa_sorter/test_harness.cpp)

## Build and repository map

1. Follow [BUILD.md](BUILD.md) for mechanical assembly, wiring, flashing, and
   calibration.
2. Read [ARCHITECTURE.md](ARCHITECTURE.md) for the data and control flow.
3. Review [hardware/BOM.csv](hardware/BOM.csv) for the active-system parts.
4. Build the firmware with the commands in [firmware/README.md](firmware/README.md).

The conveyor export is at
[`cad/exports/conveyor-assembly.step`](cad/exports/conveyor-assembly.step).
Editable source CAD and final-system renders are not included yet.

## Team and authorship

This was a team TSA project, and the Git history contains multiple
contributors. The repository does not establish a reliable division of the
mechanical, electrical, firmware, assembly, calibration, and testing work.

- TODO: confirm each contributor's public name and role wording.
- TODO: confirm who authored the conveyor CAD export before attributing it.
- TODO: confirm that the current license is appropriate for all team-authored
  code and any future CAD or media.

Until those items are resolved, do not infer contribution percentages or sole
ownership from commit counts.

## Limitations

- The checked-in thresholds and servo angles are calibration starting points;
  they must be validated on the assembled sorter.
- The display-facing functions currently write status through the logger; a
  physical display integration is not implemented in this firmware.
- Timed confirmation records the commanded bin after the handoff window. There
  is no independent per-bin sensor confirming where a brick landed.
- Hardware upload, power integrity, sensor performance, routing accuracy, and
  throughput require the physical machine and are not covered by compile
  checks.

## License

The existing [LICENSE](LICENSE) is preserved. Team approval and asset-specific
rights still need confirmation before expanding the public release.
