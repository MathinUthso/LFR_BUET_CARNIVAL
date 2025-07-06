# Line Following Robot (LFR) for BUET Carnival

This repository contains the Arduino code for a Line Following Robot (LFR) designed for the BUET Carnival. The robot is built to navigate a track using a sensor array, PID control, and maze-solving capabilities. It includes features like a user interface with an OLED display, adjustable parameters via buttons, and EEPROM storage for persistent settings.

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Directory Structure](#directory-structure)
- [Setup Instructions](#setup-instructions)
- [Usage](#usage)
- [Code Structure](#code-structure)
- [Calibration](#calibration)
- [PID Control](#pid-control)
- [Contributing](#contributing)
- [License](#license)

## Overview
The Line Following Robot (LFR) is designed to follow a line (white or black) on a track, handle intersections, and navigate complex paths using a PID controller. It supports inverse line detection, U-turns, and dead-end handling. The robot features an OLED-based user interface for real-time monitoring and parameter adjustments, with settings stored in EEPROM for persistence.

## Features
- **Line Following**: Follows white or black lines using a 6-sensor array.
- **PID Control**: Implements Proportional-Integral-Derivative (PID) control for smooth line tracking.
- **Inverse Line Detection**: Automatically toggles between white and black line modes.
- **Maze Solving**: Supports left-hand or right-hand rules for navigating T-sections, crosses, and dead-ends.
- **User Interface**: OLED display with a menu system for adjusting parameters like base speed, turn speed, and PID constants.
- **Button Controls**: Three buttons (Increment, Decrement, Select) for menu navigation and parameter tuning.
- **EEPROM Storage**: Saves calibration data and user settings for persistence across power cycles.
- **Calibration**: Automatic sensor calibration for adapting to different track surfaces.
- **Motor Control**: Precise motor control with PWM for speed and direction, including braking and 90-degree turns.

## Hardware Requirements
- **Microcontroller**: Arduino-compatible board (e.g.Arduino nano).
- **Sensors**: 6 analog line sensors (connected to A0–A3, A6–A7).
- **Motors**: 2 DC motors with drivers (e.g.TB6612FNG).
- **Motor Pins**:
  - Right Motor: PWM (Pin 11), Positive (Pin 7), Negative (Pin 8).
  - Left Motor: PWM (Pin 3), Positive (Pin 4), Negative (Pin 5).
- **OLED Display**: SSD1306-based 128x64 0.96 I2C OLED display (address: 0x3C).
- **Buttons**:
  - Increment Button: Pin 10 (with pull-up resistor).
  - Decrement Button: Pin 12 (with pull-up resistor).
  - Select Button: Pin 9 (with pull-up resistor).
- **LED**: Pin 13 for status indication.
- **Power Supply**: Suitable battery or power source for motors and electronics (i.e.11.1V 3S Li-Po).

## Software Requirements
- **Arduino IDE**: For compiling and uploading the code.
- **Libraries**:
  - `Wire.h`: For I2C communication with the OLED display.
  - `EEPROM.h`: For storing and retrieving settings.
  - `SSD1306Ascii.h` and `SSD1306AsciiWire.h`: Lightweight libraries for controlling the SSD1306 OLED display.

Install the SSD1306Ascii library via the Arduino Library Manager or download it from [GitHub](https://github.com/greiman/SSD1306Ascii).

## Directory Structure
```
mathinuthso-lfr_buet_carnival/
├── ADJUSTMENT.ino      # Parameter adjustment menu for speed, delays, etc.
├── DISPLAY.ino         # OLED display functions for text and team name.
├── iMode.ino          # Inverse line detection logic.
├── INPUT.ino          # Button input handling with debounce and long/short press detection.
├── LFR.ino            # Simple PID-based line following logic.
├── MainCode.ino       # Main setup, loop, and initialization code.
├── manual.ino         # Advanced line following with maze-solving capabilities.
├── MENU.ino           # Main menu system for user interaction.
├── motor.ino          # Motor control functions, including braking and 90-degree turns.
├── PID_adjust.ino     # PID parameter adjustment menu.
├── PID_display.ino    # Display functions for PID and line following modes.
├── PID_Follow.ino     # Main PID-based line following with maze-solving logic.
└── SENSOR.ino         # Sensor reading, calibration, and digital/analog display functions.
```

## Setup Instructions
1. **Clone the Repository**:
   ```bash
   git clone https://github.com/mathinuthso/lfr_buet_carnival.git
   ```
2. **Install Libraries**:
   - Install `Wire.h` and `EEPROM.h` (included with Arduino IDE).
   - Install `SSD1306Ascii` via Arduino Library Manager or manually.
3. **Connect Hardware**:
   - Wire the sensors, motors, OLED display, and buttons as per the pin definitions in `MainCode.ino`.
   - Ensure pull-up resistors are used for buttons.
4. **Upload Code**:
   - Open `MainCode.ino` in the Arduino IDE.
   - Select the appropriate board and port.
   - Upload the code to the Arduino.
5. **Calibrate Sensors**:
   - Navigate to the "Calibration" menu (option 5) using the buttons.
   - Follow the on-screen prompts to calibrate sensors on the track surface.

## Usage
1. **Power On**: The robot displays the team name ("Zarek Tia") on startup.
2. **Menu Navigation**:
   - Press the **Select Button** to enter the menu.
   - Use **Increment** and **Decrement Buttons** to navigate menu options.
   - Short press the **Select Button** to choose an option; long press to exit.
3. **Menu Options**:
   - **Counter**: Adjust a counter (0–25) stored in EEPROM.
   - **Adjustment**: Tune parameters like base speed, turn speed, and delays.
   - **Analog Display**: Show raw analog sensor values.
   - **Digital Display**: Show binary sensor states and sum.
   - **Calibration**: Run automatic sensor calibration.
   - **PID with Inverse**: Run PID line following with inverse line support.
   - **Simple PID**: Run basic PID line following.
   - **PID Adjustment**: Tune PID constants (Kp, Kd, Ki) and hand rule.
   - **Manual**: Run advanced line following with maze-solving.
   - **Straight/Backward**: Move the robot straight or backward at base speed.
   - **Turn Left/Right**: Perform a 90-degree turn left or right.
4. **Line Following**:
   - Select "PID with Inverse" or "Manual" to start line following.
   - The robot follows the line, handles intersections, and toggles inverse mode for black/white lines.

## Code Structure
- **MainCode.ino**: Initializes hardware, sets up the OLED, and runs the main loop with menu access.
- **SENSOR.ino**: Handles sensor reading and calibration, converting analog inputs to digital states.
- **PID_Follow.ino**: Implements the main PID-based line following with maze-solving and inverse line detection.
- **manual.ino**: Similar to `PID_Follow.ino` but with slight variations for advanced navigation.
- **LFR.ino**: A simplified PID line following algorithm.
- **motor.ino**: Controls motor speed and direction, including braking and 90-degree turns.
- **ADJUSTMENT.ino**: Manages the adjustment menu for tuning parameters.
- **PID_adjust.ino**: Manages PID constant tuning and hand rule selection.
- **MENU.ino**: Implements the main menu system.
- **DISPLAY.ino**: Handles OLED display output.
- **INPUT.ino**: Processes button inputs with debounce and long/short press detection.
- **iMode.ino**: Manages inverse line detection logic.
- **PID_display.ino**: Displays PID-related information during line following.

## Calibration
- **Automatic Calibration**:
  - Select the "Calibration" menu option.
  - The robot moves forward and backward to sample sensor values.
  - Minimum and maximum sensor values are recorded, and thresholds are calculated as `threshold = (max - min) * 0.5 + min`.
  - Calibration data is stored in EEPROM for persistence.
- **Manual Adjustment**:
  - Use the "Adjustment" menu to fine-tune parameters like base speed, turn speed, and delays.
  - Use the "PID Adjustment" menu to tune Kp, Kd, Ki, and the hand rule (left or right).

## PID Control
- **Parameters**:
  - **Kp**: Proportional gain for error correction.
  - **Kd**: Derivative gain for error rate correction.
  - **Ki**: Integral gain (not used in the current implementation).
  - **Error**: Calculated as `35 - avg`, where `avg` is the weighted average sensor position.
- **Operation**:
  - The PID controller adjusts motor speeds based on the error from the line's center.
  - The robot handles line loss, intersections, and dead-ends using timers and turn logic.
  - Inverse mode toggles the sensor logic for black/white lines.

## Contributing
Contributions are welcome! Please:
1. Fork the repository.
2. Create a new branch (`git checkout -b feature-branch`).
3. Make changes and test thoroughly.
4. Submit a pull request with a clear description of changes.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
