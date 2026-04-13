# CAD

CAD for the simplified states sorter.

Active production architecture:
- Manual one-at-a-time feed
- NEMA17 conveyor
- Conveyor-mounted sensing station
- MG995/MG996-class servo rotary chute selector
- Four labeled bins
- Wood frame with printed brackets where useful

The previous chamber, release-gate, and NEMA11 selector CAD work is archived for later nationals work. Use it only as fallback reference.

## Directory Structure

```text
cad/
  DIMENSIONS.md    critical geometry source of truth
  MECHANICAL.md    mechanism rationale and build order
  frame/           conveyor and frame prior work or future active parts
  bins/            future bin models
  sensing/         future size and color sensor brackets
  selector/        future servo chute selector models
```

Existing directories may still contain prior work from the archived design until they are pruned or replaced. Do not treat old chamber or release files as active without checking `docs/ARCHITECTURE.md`.

## CAD Priority

1. Top-level assembly with 610mm x 610mm boundary
2. Tutorial-style NEMA17 conveyor with modified measurements
3. Wood frame layout with printed brackets where useful
4. Servo mount and rotary chute body
5. Four bin positions and bin entry guides
6. Sensing shroud with TCS3200 color mount and two break-beam pairs
7. Break-beam wire routing and clearance
8. Labels, display, start control, and cable routing keepouts

## First Print Order

1. Short chute angle coupon
- Test 30, 35, 40, and 45 degrees with real bricks
- Gate: bricks slide without sticking

2. Servo mount and chute pivot coupon
- Gate: servo horn alignment, low slop, no wire interference

3. Conveyor-to-chute handoff prototype
- Gate: brick exits belt and enters chute repeatably

4. Color shroud fit part
- Gate: brick clearance and no obvious light leaks

5. Bin guide and one bin
- Gate: chute exit overlaps guide at the target servo angle

6. Remaining frame and bin parts
- Gate: footprint, bin removal, and label visibility verified

## Conventions

- Export STL files alongside source files
- Use descriptive versioned names
- Record critical dimension deviations in docs
- Do not freeze geometry from CAD alone
- Validate with real bricks before large print commitments

## Tolerance Notes

FDM variance is usually about 0.2mm to 0.5mm. Brick-facing paths, sensor brackets, and servo pivots need explicit clearance.

- Undersized chutes cause jams
- Oversized chutes allow yaw and poor bin alignment
- Sensor mount tolerance must preserve the color shroud and the selected size sensor geometry
