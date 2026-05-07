# TSA LEGO Brick Sorter

Open-source reference for a LEGO brick sorter built for TSA System Control Technology. The machine sorts 24 bricks by size and color into four labeled bins using a conveyor, a shrouded sensing station, and a servo rotary chute selector.

## What It Sorts

- 6 red 2x2 bricks
- 6 blue 2x2 bricks
- 4 red 2x3 bricks
- 8 blue 2x3 bricks

Bricks are fed studs up with the long side along the conveyor travel direction.

## System

- ESP32 controller
- NEMA17 conveyor drive
- Two IR break-beam pairs for size sensing
- TCS3200/GY-31 color sensor in a light-blocking shroud
- MG995/MG996-class servo rotary chute selector
- Four labeled bins: `2x2 RED`, `2x2 BLUE`, `2x3 BLUE`, `2x3 RED`
- Wood frame with printed brackets
- 3S LiPo power with fused input and 5V buck converter

## Quickstart

1. Build the frame, conveyor, shroud, chute, and bins from [Assembly](docs/build/ASSEMBLY.md).
2. Wire the controller from [Electrical](wiring/ELECTRICAL.md).
3. Flash the firmware from [Firmware](firmware/README.md).
4. Calibrate sensing and servo angles from [Calibration](docs/build/CALIBRATION.md).
5. Place one brick on the conveyor, start the run, and verify the target bin.

## Repository Map

- [Architecture](docs/ARCHITECTURE.md): final system layout and operator flow
- [Build docs](docs/build/ASSEMBLY.md): compact assembly sequence
- [BOM](docs/project/BOM.md): current parts summary
- [CAD](cad/README.md): CAD file placement
- [Datasheets](docs/datasheet/README.md): component matrix
- [Simulation](simulation/README.md): browser model
