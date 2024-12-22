# Smart Office IoT Project

## Overview

The **Smart Office** project demonstrates how IoT technology can revolutionize workplaces by enhancing safety, comfort, and efficiency. Using hardware such as Arduino and ESP32 alongside Cisco Packet Tracer for network simulation, we built a connected office environment with six dedicated rooms: Server Room, Conference Room, Reception Area, Inventory Room, Workstation Room, and Manager’s Cabin.

This project collects real-time data from sensors and actuates responses accordingly, showcasing a functional example of a modern IoT-enabled workspace.

---

## Features

### Rooms and Their Functions
1. **Server Room**:
   - **Sensors**: Temperature, Water Detector.
   - **Actuators**: Air Cooler, Piezo Speaker.
   - Automates cooling and alerts staff in case of water leaks.

2. **Conference Room**:
   - **Sensors**: Motion, Sound.
   - **Actuators**: RGB LED, Speaker.
   - Automates device activation and sound alerts to maintain an ideal meeting environment.

3. **Reception Area**:
   - **Sensors**: Metal Detector, Smoke Sensor.
   - **Actuators**: Alarm, Ceiling Sprinkler.
   - Enhances security and fire safety with automated responses.

4. **Inventory Room**:
   - **Sensors**: Humidity, Trip Wire.
   - **Actuators**: LED, Servo.
   - Monitors environmental conditions and secures access.

5. **Workstation Room**:
   - **Sensors**: Humiture, Water Detector.
   - **Actuators**: Heating Element, LCD.
   - Maintains a comfortable working environment with real-time monitoring.

6. **Manager’s Cabin**:
   - **Sensors**: Photo, Flex.
   - **Actuators**: Smart LED, Motor.
   - Adjusts lighting and blinds based on the manager's presence and ambient light levels.

---

## Tools and Technologies

- **Hardware**: Arduino, ESP32.
- **Simulation**: Cisco Packet Tracer.
- **Sensors**: Temperature, Humidity, Water, Smoke, Motion, Sound, etc.
- **Actuators**: LEDs, Motors, Alarms, Sprinklers, Heating Elements, etc.

---

## Project Workflow

1. **Hardware Implementation**:
   - Sensors and actuators are integrated into specific rooms for their respective tasks.
   - Controlled by microcontrollers (Arduino and ESP32).

2. **Simulation**:
   - The network is simulated in Cisco Packet Tracer using a Home Gateway for IoT connectivity.
   - Sensors and actuators communicate through an MCU.

3. **Programming**:
   - Each room's functionality is programmed using Arduino IDE.
   - Scenarios such as fire detection, noise control, and environmental monitoring are automated.

---

## Additional Resources

- [Project Video](https://drive.google.com/file/d/11g0LR0zHHhMdZFEYxGP3xZoaqhelhlE2/view?usp=sharing)

