# Capturing Arm Motion – Motion Capture & 7-DOF Humanoid Robotic Arm

## Overview
Capturing Arm Motion is a mechatronics graduation project integrating a real-time motion-capture system with a 7-DOF humanoid robotic arm. The project captures human arm motion via wearable sensors and IMUs, transmits pose data to a controller, visualizes the motion in Blender, and commands a custom-designed humanoid arm with a modular gearbox and differential wrist.

## Key Features
- Real-time motion capture using ESP32-based sensor nodes and a lightweight communication protocol.
- Live visualization and control integration with Blender for simulation and operator feedback.
- 7-DOF humanoid robotic arm hardware design with modular gearbox, finger mechanisms, and a differential wrist.
- MATLAB Simscape models for system-level simulation and validation.
- Data parsing tools for translating motion capture streams into kinematic commands.

## Repository Structure
- hardware/
  - CAD/ — 3D CAD files and assemblies (placeholders).
  - Gearbox/ — Gearbox design files and BOM (placeholders).
  - Finger_Mechanisms/ — Finger and gripper subassemblies (placeholders).
  - Wrist_Differential/ — Differential wrist mechanism files (placeholders).
- electronics/
  - ESP32_Firmware/ — Firmware for motion-capture sensor nodes and communication (placeholder + sample).
  - Schematics/ — Electronics schematics, PCB board files (placeholders).
  - Communication_Protocol/ — Protocol definition, message formats (placeholders).
- software/
  - blender/ — Blender scripts for visualization and real-time control (contains real_time_control.py).
  - matlab/ — MATLAB / Simscape models and entry scripts (contains main_simscape.m).
  - kinematics/ — Kinematics and dynamics functions (contains forward_kinematics.m).
  - data_parsing/ — Parser utilities to convert sensor streams to robot commands (contains parser.py).
- docs/
  - Stress_Analysis/ — Simulation results and FEA reports (placeholders).

Each folder contains a .keep file so Git tracks empty directories.

## How to run the system (high-level)
1. ESP32 firmware
   - Flash the provided ESP32 firmware (located in electronics/ESP32_Firmware/) onto each sensor node using the Arduino IDE or PlatformIO.
   - Configure Wi-Fi / UDP settings in the firmware header to point to the controller PC IP and port.
   - Start the sensor nodes; they will stream motion data using the defined communication protocol.

2. Blender visualization and control
   - Open Blender and install the provided real-time control script (software/blender/real_time_control.py).
   - Run the script to open a UDP/TCP listener that receives motion data and maps it to the arm rig.
   - Use Blender's timeline and visualization tools to verify motion before commanding the real robot.

3. MATLAB Simscape
   - Open MATLAB and run software/matlab/main_simscape.m to initialize the Simscape model.
   - Use the Simscape model for dynamic validation, trajectory testing, and closed-loop controller tuning.

4. Data parsing and kinematics
   - The software/data_parsing/parser.py contains utilities to validate and translate incoming sensor packets to joint-space trajectories.
   - Kinematic math (forward & inverse) is provided in software/kinematics/ to compute joint references for the 7-DOF arm.

## Development & Contribution Workflow
- Branch from main for feature work and open PRs with clear descriptions.
- Follow the coding conventions of each language (Python PEP8, MATLAB function headers, Arduino/ESP32 style).
- Include simulation logs and CAD export PDFs in docs/ for reproducibility.

## Future Improvements
- Add complete hardware CAD exports (STEP/IGES) and annotated BOMs for manufacturing.
- Implement robust inverse kinematics with singularity handling and redundancy resolution for the 7-DOF arm.
- Add encrypted, authenticated communication between sensor nodes and controller.
- Implement embedded safety features and a hardware abstraction layer for multiple end-effectors.
- Continuous integration for firmware build checks and automated Simscape test runs.

## Authors
- Baraa Akbik
- Ahmad Malas

University of Kalamoon – 2025
