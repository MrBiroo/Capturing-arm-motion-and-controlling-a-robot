# Capturing-Arm-Motion

Project Capturing-Arm-Motion brings together hardware design, embedded firmware, visualization, and analysis tools to capture human arm motion and drive a robotic arm model in real-time. The repository is organized to separate CAD and mechanical design, embedded firmware for IMU-based capture, electronics schematics and protocol documentation, visualization and control scripts (Blender, MATLAB), kinematics utilities, and data parsing utilities.

## Project overview

Capturing-Arm-Motion is a modular multi-disciplinary project to:
- Capture arm motion using distributed IMUs and an ESP32-based module.
- Transmit packetized orientation/acceleration data over UDP to a host.
- Parse incoming IMU packets and map them to a virtual armature in Blender for visualization and recording.
- Provide MATLAB Simscape entry points for physics-based simulation and stress analysis.
- Maintain hardware design artifacts for eventual physical build (CAD, gearbox, finger mechanisms, wrist differential).

This repository holds prototypes, placeholders, and reference implementations to bootstrap development and collaboration.

## Features

- Motion capture
  - IMU sampling on ESP32
  - Lightweight packet protocol carried over UDP
- Visualization
  - Real-time Blender armature mapping and playback
- Robotic arm
  - Kinematics utilities for forward kinematics
  - MATLAB Simscape entry script for simulation and analysis
- Project artifacts
  - CAD and mechanical design placeholders
  - Schematics and communication protocol specification directory

## Repository structure

Top-level:
- hardware/ — mechanical and CAD assets
  - CAD/
  - Gearbox/
  - Finger_Mechanisms/
  - Wrist_Differential/
- electronics/ — firmware and electronics
  - ESP32_Firmware/
    - esp32_firmware.ino — example firmware (placeholder)
  - Schematics/
  - Communication_Protocol/
- software/ — visualization, parsing, kinematics, and sim scripts
  - blender/
    - real_time_control.py — Blender UDP listener and mapping (placeholder)
  - matlab/
    - main_simscape.m — MATLAB entry script (placeholder)
  - kinematics/
    - forward_kinematics.m — function header for FK
  - data_parsing/
    - parser.py — packet decoding utilities (placeholder)
- docs/
  - Stress_Analysis/ — future stress analysis documents
- README.md — this file
- init_structure.sh — script to recreate this structure locally

Each folder contains a `.keep` file to ensure Git tracks empty directories in initial commits.

## How to run components (quick start)

Important: The files in this repository are initial placeholders. Use them as a starting point for development.

ESP32 firmware (esp32_firmware.ino)
- Open `electronics/ESP32_Firmware/esp32_firmware.ino` in the Arduino IDE or PlatformIO.
- Install board support for an ESP32 (e.g., Espressif ESP32 in the Board Manager).
- Configure WiFi SSID, password, target host IP and target UDP port in the sketch.
- Compile and flash to your ESP32.
- The placeholder sketch demonstrates reading IMUs, packing data, and sending via UDP. Replace IMU code with your chosen sensor library (e.g., Adafruit LSM6DS/MPU9250/ICM20948).

Blender script (software/blender/real_time_control.py)
- This script is intended to run inside Blender's embedded Python interpreter (bpy).
- Open Blender, switch to the "Scripting" workspace, and open the script.
- The script contains a UDP listener scaffold; adapt packet parsing and bone naming to your armature.
- To run headless (for recording), run Blender with: blender --background --python software/blender/real_time_control.py

MATLAB / Simscape (software/matlab/main_simscape.m)
- Open MATLAB and set the path to the repository root.
- Open `software/matlab/main_simscape.m`.
- The placeholder script outlines starting the Simscape model and interfacing data.
- Customize the script to load your Simscape model file (.slx) and tune simulation parameters.

Data parsing and kinematics
- `software/data_parsing/parser.py` contains packet decoding helpers — integrate with your network listener or Blender script.
- `software/kinematics/forward_kinematics.m` is a header for a forward kinematics function. Implement your robot's kinematic chain and test with unit cases.

## Future improvements

- Implement robust packet framing with CRC and sequence numbers.
- Secure communication (authenticated/encrypted) or TLS tunneling for remote streaming.
- Add calibration routines for IMUs (orientation drift compensation and sensor fusion).
- Implement inverse kinematics and an actuator control stack to drive a physical robotic arm.
- Add CI checks, unit tests for parser/kinematics, and simulation-based validation.
- Populate hardware CAD, BOMs, and detailed electronics schematics.

## Authors and affiliation

- Project: Capturing-Arm-Motion
- Contributors: Initial scaffolding by the Capturing-Arm-Motion team
- University / Lab: [Your University / Lab Name]
- For inquiries or contributions, open an issue or submit a pull request once the repository is hosted on GitHub.

License
- Add an appropriate license file when ready (e.g., MIT, Apache-2.0). This repository currently contains placeholders and is intended for educational and collaborative development.
