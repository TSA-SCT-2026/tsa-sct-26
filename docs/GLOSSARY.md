# Glossary

Short definitions for the main project terms. If a term in the architecture is unclear, start here.

## System flow

- `compressed queue`: all 24 bricks are preloaded and packed closely enough that the next brick is already near the chamber workflow
- `cold start`: the first feed cycle from the initial queued rest state
- `steady state`: the repeating cycle after the queue is already staged against the chamber workflow
- `chamber pitch`: the effective advance distance from one seated brick to the next seated brick in the compressed queue
- `safe restart condition`: the physical truth required before the next brick is allowed to advance

## Core mechanisms

- `isolation chamber`: the one-brick sensing and release area
- `trapdoor platform`: the hinged chamber floor that drops when support is removed
- `class 3 lever`: the lever geometry used to pull support away from the trapdoor platform
- `selector`: the routing mechanism below the trapdoor
- `selector chute`: the active 4-index routing mechanism that sends bricks to one of four bins
- `index position`: one of the four valid routed positions of the selector chute
- `home reference`: the known selector position used to restore position truth and detect drift

## Conveyor terms

- `off-axis conveyor`: a conveyor where the motor does not directly carry the drive roller on its shaft
- `timing-belt stage`: the toothed belt and pulley stage between the motor and supported conveyor shaft
- `supported roller shaft`: the shaft carried by bearings or frame supports that holds the smooth drive roller
- `drive roller`: the powered smooth roller that moves the conveyor belt
- `idler roller`: the non-driven roller that supports tracking and tension

## Control terms

- `static sensing`: size and color sensing while the brick is stopped in the chamber
- `selector ready`: confirmation that the selector reached its target indexed position and release is safe
- `drop window`: the bounded release-and-fall interval before bin confirmation is expected
- `platform level`: the reset truth that restores permission to feed the next brick
- `event-gated control`: control logic that advances only when the required physical event is confirmed

## Evidence and validation

- `selector evidence gate`: the rule that keeps the selector chute active unless measured or modeled results show it cannot support a winning design
- `notebook evidence`: data, decision matrices, test logs, and calibration records that support the engineering notebook
