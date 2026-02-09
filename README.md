# CAN logger with LCD and BMP280 sensor

## Libraries used
1. libi2c
2. gpiod
3. SocketCAN

## Hardware used
1. Raspberry Pi 4
2. MCP2515 CAN module (wired through the spi0 interface — pins 8, 9, 10, 11). The module requires a 5V supply.
3. BMP280 temperature/pressure sensor (wired through the i2c1 interface — pins 2, 3). The module requires a 3.3V supply.
4. 16×2 LCD with HD44780 controller (can be wired to a custom set of GPIOs). In the code, pins 17, 18, 22, 23, 24, and 27 are used. The LCD requires a 5V supply.

## Installing of Libraries and Configuration 

### libi2c  
1. Use `sudo apt install libi2c-dev libi2c-tools` to install the library.  
3. To enable I2C interface on your Raspberry Pi you can either use `sudo raspi-config` and enable I2C in the *Interface Options*, or
   open config directly with `sudo nano /boot/firmware/config.txt` and uncomment lines connected with I2C.
4. Reboot the system by entering `sudo reboot`.

### gpiod
1. Enter `sudo apt install -y libgpiod2 libgpiod-dev gpiod`.

### SocketCAN
1. To install enter `sudo apt install can-utils`.
2. Open config.txt by entering `sudo nano /boot/firmware/config.txt`  
   and add line `dtoverlay=mcp2515-can0,oscillator=8000000,interrupt=25`.  
   You also need to enable SPI interface either by uncommenting SPI line in the config.txt or in raspi-config menu as it was done for I2C.
4. To turn on CAN port enter `sudo ip link set can0 type can bitrate 500000 loopback on`  
   and `sudo ip link set can0 up`

### LCD Pins and Logger Output
- To change pins that will be used for LCD change values of the `LCD_PINS` macro in `src/shared_resources.hpp`.  
- To change the path of the *Logger* output change the value of the `LOGS_OUT_FILE` macro in `src/shared_resources.hpp`.

## Compilation commands  
To compile the application, use `make`.  
To remove object files after compilation, use `make clean`.

## Overview
This is a CAN logger that can send and receive messages over the CAN bus. CAN bus operations can optionally be logged to a separate text file.

The project includes the following components:

1. **CAN and Logger**  
   These classes implement the basic CAN and logging functionality of the project.

2. **BMP280 driver**  
   Used to read data from the BMP280 sensor via the I2C bus.

3. **LCD driver**  
   Provides methods for basic operations on the LCD display in 4‑bit mode without flag checking. For displaying sensor data, a `bmp_lcd` child class is included.

4. **Timer**  
   A general-purpose timer class for implementing multithreaded scheduled routines. It operates using an external pointer to routine-handling functions, so it has no dependencies on other specialized components.

These components are designed to be independent of each other and can be reused in other applications.
