# PiCAN

Welcome to the PiCAN project! This repository showcases the capabilities of the RP2040 microchip in combination with CAN bus technology.

## Overview

PiCAN demonstrates how to use the Raspberry Pi Pico with CAN bus communication to transfer data between devices. The project is divided into two main branches:
- `main`: A simple CAN bus message transfer example.
- `add-joystick-servo-controls`: An advanced example where a joystick controls a servo via CAN bus.

## Branches

### [Main Branch](https://github.com/moeux/PiCAN)

The `main` branch contains a straightforward example of CAN bus message transfer. In this example:
1. One Raspberry Pi Pico receives input through the Serial Monitor.
2. This input is then transmitted over the CAN bus.
3. Another Raspberry Pi Pico receives the CAN bus message and outputs it through its Serial Monitor.

![can-msg-transfer](https://github.com/user-attachments/assets/f758def9-36b1-4b01-b0f3-8c0d13aad8c2)

### [Add Joystick Servo Controls Branch](https://github.com/moeux/PiCAN/tree/add-joystick-servo-controls)

The `add-joystick-servo-controls` branch extends the functionality by allowing a servo to be controlled with a joystick. The setup is as follows:
1. A Raspberry Pi Pico with a joystick attached sends the joystick values over the CAN bus.
2. Another Raspberry Pi Pico with a servo attached receives the joystick values.
3. The receiving Pico sets the angle of the servo based on the joystick input.

![joystick-servo-control](https://github.com/user-attachments/assets/e4045800-73e8-4a96-861c-4c57c120c4da)

## Hardware
- [Raspberry Pi Pico](https://www.raspberrypi.com/products/raspberry-pi-pico/)
- [CAN Bus Transceiver Module](https://www.az-delivery.de/products/can-bus-module)
- [KY-023 Joystick Module](https://www.az-delivery.de/products/joystick-modul)
- [SG90 Micro Servo Motor](https://www.az-delivery.de/products/az-delivery-micro-servo-sg90)
- Wiring materials and a breadboard

## Libraries
Special thanks to the authors of these libraries.
- [can2040](https://github.com/KevinOConnor/can2040) (Software CAN bus implementation)
- [Pico-Servo](https://github.com/Thomas-Kleist/Pico-Servo) (Pico C/C++ Servo Library)

## Getting Started

### Installation

1. **Clone the repository:**

    ```bash
    git clone https://github.com/moeux/PiCAN.git
    cd PiCAN
    ```

2. **Switch to the desired branch:**

    For the basic CAN bus message transfer:
    ```bash
    git checkout main
    ```

    For the joystick to servo control example:
    ```bash
    git checkout add-joystick-servo-controls
    ```

3. **Upload the code to your Raspberry Pi Pico boards:**

    Use your preferred method to upload the code to the Raspberry Pi Pico boards. <br>
    I recommend flashing the code onto the Raspberry Pi Pico using the [Raspberry Pi Pico VSCode Extension](https://marketplace.visualstudio.com/items?itemName=raspberry-pi.raspberry-pi-pico). <br>
    For further setup instructions refer to the [Getting Started Guide](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf) by Raspberry Pi.

Enjoy exploring the possibilities of RP2040 and CAN bus technology with PiCAN!
