# Architecture

The sorter uses a simple feed, conveyor sensing, and a servo rotary chute selector. The design favors reliable classification and routing over extra feed mechanisms.

## Final Layout

```text
feed guide
  -> conveyor
  -> sensing station
  -> belt exit
  -> servo rotary chute selector
  -> four labeled bins
```

## Subsystems

- Feed: one brick at a time, studs up, long side along travel.
- Conveyor: NEMA17-driven belt on a wood frame.
- Size sensing: two IR break-beam pairs in the sensing shroud.
- Color sensing: TCS3200/GY-31 module looking down inside the shroud.
- Routing: MG995/MG996-class servo rotates a chute toward one of four bins.
- Power: 3S LiPo input, fuse, buck converter, common ground.
- Operator UX: start button, display states, feed cue, bin labels.

## Operator Flow

1. Put one brick on the feed end of the conveyor.
2. Start the sorter.
3. The brick passes the sensing station.
4. Firmware locks size and color classification.
5. The chute rotates to the matching bin.
6. The conveyor carries the brick into the chute.
7. The brick lands in the labeled bin.

Display states are `READY`, `SORTING`, `SORT COMPLETE`, and `ERROR`.

## Bin Map

- Bin 1: `2x2 RED`
- Bin 2: `2x2 BLUE`
- Bin 3: `2x3 BLUE`
- Bin 4: `2x3 RED`

## Design Notes

- The shroud is required for color calibration.
- The chute positions are calibration values in firmware.
- The machine footprint is constrained to 610mm x 610mm.
- Clean labels and cable routing are part of the finished build.
