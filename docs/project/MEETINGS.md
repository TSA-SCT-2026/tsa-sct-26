# Meeting Notes

## Meeting 1: 3/20/26 (1 hour)

- Talked about how to plan the upcoming season.
- Discussed a design idea of using a level 3 lever on the conveyor for size and color sorting.
- Talked about parts to order.
- Decided to finalize the final design next Tuesday.

## Meeting 2: 3/24/26 (1 hr 20 mins)

- Finalized design.
- Started the notebook:
  - Problem statement and design brief
  - Research
  - Talk about each design individually (pros and cons, etc.)
  - Decision matrix with backup designs
  - Parts we bought and why
  - 2D and 3D CAD drawings and models
  - Any prework we did to fluff the notebook
- CAD:
  - 2D drawing
  - Individual 3D parts and joining them in assemblies
- Simulate with JS website:
  - Test different dimensions
  - How gravity and other forces will impact simulation
  - How positioning and mechanics can best optimize the assembly for performance (cannot focus on speed, etc.)
  - Heat management with stepper motor and solenoid
  - Equations on how things move and the physics forces involved
- Extra notes:
  - PTF tape to reduce friction on the conveyor
  - Create datasets of different aspects of the conveyor and make graphs, tables, and equations to show performance. Use that metric to improve design and regenerate statistics to show improved performance objectively.
  - Use Matlab alternatives such as SageMath, GNU Octave, Python, and Excel for modeling (whatever is efficient).
- Team process:
  - Ground design choices in measured parts, drawings, and test data
  - Keep decision notes short enough that another builder can follow them later
  - Separate ideas from proof so the notebook stays honest
  - Use calculations and mock data only when they are clearly labeled as estimates

## Meeting 3: 4/3/26 (35 minutes)

- Verify that all parts have been ordered.
- Finalize CAD plan:
  - Divided work for belt system, entry chute, diverter chute, frame, and miscellaneous parts
  - Set CAD prototyping due date to 4/10/26. Should be able to assemble the entire project with a master assembly by that due date.
  - Discussed that the hardest CAD design will be the entry chute and planned accordingly.
  - Discussed that early CAD designs are prototypes. Must wait for parts to arrive before exact measurements can be taken.
- Create rough team roles based on skillset.

## Status update: 4/5/26

- AliExpress orders have arrived and are available for measurement and fit checks.
- Amazon orders have not arrived yet. Keep Amazon-dependent dimensions and validation blocked until delivery.
- CAD updates should now use measured AliExpress hardware where available and keep other dimensions provisional until Amazon parts are in hand.

## Status update: 4/11/26

- States build simplified to manual one-at-a-time conveyor feed, sensing station, and MG995/MG996-class servo rotary chute selector.
- Previous chamber and release-gate architecture moved to archive context for later nationals work.
- Size sensor is now frozen as two-pair break-beam timing in the sensing shroud, with real-brick timing evidence still required before final shroud printing.
- Frame material is wood or 3D printed structure from available stock, not 2020 extrusion.
- BOM purchase truth still traces to `docs/project/BOM.xlsx`; do not convert recommendations into ordered status without spreadsheet rows.

## Meeting 4: 4/12/26 (40 minutes)

- Locked CAD and build schedule:
  - Full Fusion 360 assembly due Friday 4/17/26 so physical build can start over the weekend.
  - One member owns servo chute selector CAD, one member owns conveyor and frame CAD. Chute member assists frame member when done.
  - Agreed to accept rough initial geometry and build early. Physical assembly experience will inform CAD faster than trying to resolve everything in software first.
- Design constraints:
  - Brick inertia at belt exit requires a curved exit lip, 5 to 6mm above belt surface, to redirect horizontal momentum into the chute entry. First validation print candidate.
  - Sensing shroud needs a printed screw-in mount. Exact position resolved during conveyor and frame CAD.
- Locked frame material as wood. Aluminum extrusion adds complexity with no performance advantage. Time saved goes to notebook, test data, and tuning.
- Consulting an experienced engineering teacher this week for CAD positioning and assembly validation on the conveyor and pulley system.
