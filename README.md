# CAN_logger_with_LCD_BMP280_sensor

## Libraries used
1. libi2c
2. gpiod
3. SocketCAN

## Hardware used
1. Raspberry Pi 4
2. MCP2515 CAN module (wired through the spi0 interface — pins 8, 9, 10, 11). The module requires a 5V supply.
3. BMP280 temperature/pressure sensor (wired through the i2c1 interface — pins 2, 3). The module requires a 3.3V supply.
4. 16×2 LCD with HD44780 controller (can be wired to a custom set of GPIOs). In the code, pins 17, 18, 22, 23, 24, and 27 are used. The LCD requires a 5V supply.

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
