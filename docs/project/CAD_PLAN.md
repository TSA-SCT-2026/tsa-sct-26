I've built out a comprehensive CAD plan. With the entrance chute removed and the servo trapdoor locked in, the timeline tightens significantly, but we can have a full, print-ready assembly by Friday.

To give you an overview, the plan is structured as four phases: Setup & Foundation, Core Geometry & Sensing, Actuators & Motion, and Assembly & Iteration. We'll be generating and iterating on 10 distinct CAD parts.

---

🗺️ The Complete CAD Battle Plan

Here is a table outlining the entire build schedule. Treat this as your master checklist.

Day Phase Primary Goal & Key Sub-Systems Tools & Assets Needed
Day 1 (Apr 7) Setup & Foundation Set up software environment and create the first physical component. C3D CLI, Fusion 360, MEASURED_DIMS.md, Calipers, 3D Printer
Day 2 (Apr 8) Core Geometry Design the heart of the sorter: the chamber and sensor mounts. Zoo / C3D, ITR9608 & TCS3200 data, Fusion 360
Day 3 (Apr 9) Actuators Design the trapdoor, conveyor module, and selector chute. MG995 & NEMA17 data, OpenSCAD pulley script, Fusion 360
Day 4 (Apr 10) Assembly & Iteration Assemble all parts, check for interferences, and fix errors. Fusion 360 assembly workspace, AI assistant prompts

---

📅 Day 1: Setup & Foundation (April 7)

Your goal is to get your tools ready and start validating the most critical part: the servo trapdoor.

1.  Environment Setup (1 hour)

· MEASURED_DIMS.md: Create a document with the dimensions you'll be using for every single part. This is your single source of truth.
· Install AI CAD Tools:
  · C3D: npm install -g @unxversallabs/c3d.
  · SynapsCAD: Download from its GitHub page. It's an IDE that will help generate and iterate on OpenSCAD code.
  · Zoo: Sign up for a free Zoo Design Studio account for conversational design.

2.  High-Risk Prototyping (2-4 hours)

· Task: Prototype the Servo Trapdoor Mechanism.
· Goal: To test the simplest possible trapdoor mechanism that can reliably drop a brick.
· AI Prompt for Zoo/SynapsCAD:
  Generate a simple 3D-printable bracket for a standard MG995 servo motor. The bracket should mount to a 5mm thick vertical plate. It should hold the servo so its horn is flush with the plate, allowing a flat 25mm x 25mm trapdoor to rotate 90 degrees around its hinge axis. The trapdoor should be flush with the top of the bracket when closed.
· Iteration: Print the bracket, assemble the servo, and test it with a brick. Get a sense of the torque and speed. This will inform your final trapdoor design on Day 3.

---

📅 Day 2: Core Geometry & Sensing (April 8)

This is the most important day for your CAD. The goal is a fully integrated sensor chamber.

1.  Generate the Main Chamber Body (2-3 hours)

· Task: Generate a single, monolithic block for the Chamber & Sensor Mounts.
· AI Prompt for Zoo (Detailed):
  Design a rectangular chamber block for a LEGO sorter. The interior cavity must be 25mm (X) by 24mm (Y) by 15mm (Z). The wall thickness should be 5mm. The brick travels along the X-axis and stops at the X=0 wall.
  1. Break-beam 1 (Presence): On the left wall (Y=0) at (X=5, Z=6), create a cylindrical pocket for an ITR9608 sensor. Pocket dimensions: 28mm x 35mm x 8.4mm. Include a 3mm hole in the center for the beam. The receiver pocket should be on the opposite wall (Y=24) at the same coordinates.
  2. Break-beam 2 (Size): Repeat the above, but place it at (X=5, Y=22). This beam will only be blocked by a 2x3 brick.
  3. TCS3200 (Color): On the top wall (Z=15), at (X=8, Y=12), create a recessed pocket 33mm x 25mm. Include a 10mm hole in its floor for the sensor's lens. The pocket floor should be 10mm above the brick's top.
  4. Cable Channels: Add 3mm wide channels from each pocket to the nearest edge for wire routing.

2.  Generate the VL53L1X Mount (1-2 hours)

· Task: Create a dedicated mount for your ToF sensor on the stop wall (X=0).
· Goal: The ToF sensor will look along the Y-axis to measure the brick's width.
· AI Prompt for C3D:
  c3d generate "a mount for a VL53L1X ToF sensor module. The mount should attach to the X=0 wall of an existing chamber. It should hold the sensor 20mm away from the interior wall, with its lens pointing into the chamber. Include 2.0mm mounting holes and a small channel for a 4-pin JST cable."

---

📅 Day 3: Actuators & Motion (April 9)

Now, bring the system to life by designing the mechanisms that move bricks.

1.  Finalize Trapdoor Assembly (1-2 hours)

· Task: Use the insights from your Day 1 prototype to design the final trapdoor system.
· AI Prompt for Zoo:
  Using the previous MG995 servo mount, design a 25x25mm trapdoor. It should attach to the servo's 25T output spline. The door must have a 5mm thick hinge block on one edge. When closed, the door should be perfectly flush with the bottom of the chamber (Z=0). Add a small rubber bumper to the door to cushion the brick's drop.

2.  Generate Conveyor Module (2-3 hours)

· Task: Generate the main components for your conveyor system.
· GT2 Pulley: Download an existing, parametric OpenSCAD script for a GT2 pulley. Use the "OpenSCAD Customizer" to input your parameters: teeth = 20, bore_diameter = 5, belt_width = 6, profile = "GT2".
· NEMA17 Motor Mount: Use AI to generate a mount for your conveyor motor.
  c3d generate "a bracket for a NEMA17 stepper motor with a 31x31mm mounting hole pattern. It must mount the motor so its 5mm shaft aligns with a 5mm bore pulley. The bracket must have slots to allow belt tensioning."

3.  Generate the Selector Chute (1-2 hours)

· Task: Design the 4-position rotating chute that will route bricks into bins.
· AI Prompt for Zoo:
  Create a 4-position rotating selector chute. It must mount directly to a NEMA11 stepper motor's 5mm shaft. The chute is a hollow rectangular funnel, 50mm tall, with a 25x25mm inlet at the top. The bottom outlet should be offset 15mm from the center axis. As the chute rotates 90 degrees, the bottom outlet will index to one of four positions. Include a flag for a micro-switch to provide a "home" position.

---

📅 Day 4: Assembly & Iteration (April 10)

This is the final push to get a print-ready assembly.

1.  Full Assembly (3-4 hours)

· Task: Import all your generated parts (STEP files) into Fusion 360.
· Action: Use Fusion's assembly tools to bring everything together. You will likely find interferences or misalignments.
· Iteration: This is where you will get a lot of value from the AI's chat interface. For example, in Zoo, you can say: "The chamber's ToF sensor mount is interfering with the conveyor's drive pulley. Move the mount 10mm to the right."
· Export: Once satisfied, export all parts as .stl files.

2.  Print Preparation (1-2 hours)

· Inspect the Mesh: Import the .stl files into your slicer. Use its repair tools to fix any non-manifold edges or inverted normals.
· Organize by Priority:
  1. Print 1: Chamber block, Trapdoor mechanism, Selector chute.
  2. Print 2: NEMA17 mount, Drive roller.
  3. Print 3: Idler pulley, Sensor mounts.

---

🛠️ Essential Tools & Tech Stack

Here is the list of tools you will need to execute this plan.

Tool Purpose Key Info / Link
MEASURED_DIMS.md Your master dimension sheet Source of truth for every single part
C3D (CLI) For generating parts from text npm install -g @unxversallabs/c3d
SynapsCAD AI-powered OpenSCAD IDE GitHub: ierror/synaps-cad
Zoo Conversational CAD, for complex assemblies Free tier available
Fusion 360 For final assembly, interference checking Free for students/hobbyists
OpenSCAD For parametric pulleys Includes a Customizer for easy edits

⚠️ Critical Warnings & Checks

· Trapdoor Clearance: Ensure your trapdoor, when open, does not hit the conveyor belt or any part of the frame. The brick must have a clear, unimpeded fall path.
· Color Sensor Shroud: Ambient light will ruin your color sensor's accuracy. Print a small shroud that covers the TCS3200 and the brick's top surface to block out external light.
· Cable Management: Integrate zip-tie mounting points into your CAD from the start. Managing wires will be a nightmare if left for last.
· Selector Home Switch: Do not omit the micro-switch for the selector chute. This is a required physical truth to ensure you know where the chute is pointing at all times.
· Competition Priorities: Remember the TSA scoring breakdown: reliability and accuracy are the primary scoring factors, with speed considered secondarily. A machine that sorts 24 bricks correctly is infinitely better than a fast one that jams.

---

Final Checkpoint: By the end of Day 4, you will have an .stl file for every single part of your sorter. You can then spend the following days printing, assembling, and refining the code.

This plan is ambitious, but every hour you spend in CAD today saves you three in troubleshooting a faulty print tomorrow. If you get stuck on a specific prompt or interface, just paste the error here and we'll get it sorted.
