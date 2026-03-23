# 📡 Arduino Radar System

## 📖 Overview
This project implements a **radar-like system** using an Arduino-based setup. It scans the surrounding environment over a 360° angle and visually represents detected obstacles using LED rings.

The system uses ultrasonic sensors mounted on a servo motor to measure distances and display real-time feedback through colored LEDs.

<p float="left">
<img src="Images/1.png" width="350"/>
  <img src="Images/Schematic.jpeg" width="350"/>
</p>

## 🎯 Features
- 180° environment scanning
- Dual ultrasonic sensors for left/right detection
- Real-time obstacle detection
- Visual radar sweep effect using LEDs
- Distance-based detection indication

## 🧰 Hardware Components
- Arduino UNO
- SG90 Servo Motor
- 2 × HC-SR04 Ultrasonic Sensors
- WS2812B LED Rings:
  - 16 LEDs ring
  - 8 LEDs ring

## ⚙️ System Description
The system consists of a rotating platform where:
- The **servo motor** sweeps from 0° to 180°
- Two ultrasonic sensors measure distances (left & right)
- LED rings display:
  - 🟢 Green → radar sweep
  - 🔴 Red → detected obstacle

## 🧠 How It Works

### 1. Environment Scanning
- The servo continuously rotates between 0° and 180°
- Sensors measure distances at each angle

### 2. Radar Sweep Display
- Current angle is mapped to LED position
- LEDs light up in green
- Previous LEDs gradually fade

### 3. Obstacle Detection
- If distance < threshold:
  - LED turns red
- Indicates object presence at a specific angle
