# Glossary

Short definitions for the main project terms. If a term in the architecture is unclear, start here.

## System Flow

- `manual one-at-a-time feed`: the active states feed mode where the operator places one brick on the conveyor at a time
- `feed orientation cue`: the physical label or guide that tells the operator how to place each brick
- `sensing station`: the sensor shroud area mounted to the wood conveyor bed or rigid frame near the conveyor start
- `classification lock`: the moment firmware has decided size, color, and target bin for the current brick
- `handoff`: the transition from belt exit into the servo chute
- `ready state`: the state where the system can accept the next brick

## Core Mechanisms

- `conveyor`: the belt system that moves a single brick through sensing and into the chute
- `servo rotary chute selector`: the MG995/MG996-class servo-driven chute that points at one of four bins
- `chute position`: one of the four calibrated servo angles for a target bin
- `bin guide`: a fixed guide at a bin entry that catches the chute output
- `color shroud`: the light-blocking structure around the color sensor
- `size sensor`: the two break-beam pairs used to separate 2x2 from 2x3 by blocked-duration timing

## Conveyor Terms

- `belt exit`: the downstream end of the conveyor where the brick leaves the belt
- `belt tracking`: how consistently the belt stays centered on the rollers
- `handoff gap`: the physical gap between the conveyor exit and chute entry
- `usable belt length`: the straight conveyor distance available for feed, sensing, and handoff

## Control Terms

- `route command`: firmware command that sends the servo to the target chute angle
- `route ready`: confirmation or timed settle point after the servo reaches the target angle
- `ERROR`: visible state when the machine cannot safely continue without operator help
- `SORT COMPLETE`: visible state after all 24 bricks are processed

## Evidence And Validation

- `notebook evidence`: data, decision matrices, test logs, and calibration records that support the engineering notebook
- `chute angle test`: real-brick test comparing chute slide behavior at candidate angles
- `full-run log`: CSV record of a complete 24-brick sorting run

## Archived Terms

These terms belong to the previous architecture unless the user explicitly reopens nationals work:
- `compressed queue`
- `start gate`
- `isolation chamber`
- `release gate`
- `retracting support`
- `chamber pitch`
- `NEMA11 selector`
- `dual-ToF chamber sizing`
