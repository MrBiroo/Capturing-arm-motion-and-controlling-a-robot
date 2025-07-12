# 🎮 Robotic Arm Motion Capture Control (7-DOF)

A 7-degree-of-freedom robotic arm and 3-DOF robotic neck controlled using real-time human motion capture. Built as part of my university graduation project in Mechatronics Engineering.


---

## 🧠 Overview

This project captures human arm motion using IMU sensors and maps it to a robotic arm in real time. It combines embedded systems, motion data filtering, mechanical design, and multi-axis motor control.

It was developed and implemented in Syria as part of my graduation project at the University of Kalamoon.

---

## ⚙️ Features

- 🔁 Real-time motion capture using IMU sensors (BNO055 & MPU-6500)
- 🎮 Wireless communication between sensor suit and control system
- 🔧 Robotic arm with 7 DOF and neck with 3 DOF
- 📊 Live motion visualization in Blender
- 🧭 Sensor fusion and filtering (kalman, complementary)
- ⚡ Mixed actuator control: servo motors, BLDC motors (FOC), and potentiometers

---

## 🔩 Hardware

| Component        | Purpose                          |
|------------------|----------------------------------|
| ESP32            | Main wireless microcontroller    |
| Arduino Nano     | Motor control bridge             |
| BNO055 IMU       | Orientation tracking (motion suit) |
| MPU-6500         | Raw IMU data (lower-cost option) |
| A2212 BLDC Motor | Arm joints (with encoders)       |
| Servo motors     | Head/neck articulation           |
| Magnetic encoders| Feedback for precise control     |
| Potentiometers   | Backup analog joint feedback     |

---

## 🛠️ Software & Tools

- Arduino IDE (C/C++)
- MATLAB (data logging & filtering)
- Blender (robot arm visualization)
- SolidWorks (mechanical design)
- Git (version control)

---

## 📐 System Architecture

```plaintext
  [Human Arm]
      ↓  (IMUs)
  [ESP32 Suit Node]
      ↓  (I2C / Serial / Wireless)
  [ESP32 Bridge Node] ——→ [Arduino Nano]
                             ↓
                [BLDC Motors + Servos]
                             ↓
                     [Robotic Arm + Neck]

