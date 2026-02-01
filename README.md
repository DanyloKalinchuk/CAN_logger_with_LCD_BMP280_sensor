# CAN_logger_with_LCD_BMP280_sensor  

## Libraries Used  
1. libi2c
2. gpiod
3. SocketCAN

## Hardware Used  
1. Raspberry Pi 4
2. MCP2515 CAN module (Wired though the spi0 interface. Pins: 8, 9, 10, 11. The module requires 5V supply)
3. BMP280 Temperature/Pressure sensor (Wired though the i2c1 interface. Pins: 2, 3. The module requires 3.3V supply)
4. 16x2 LCD with hd44780 controller (Can be wired to custom set of GPIOs. In the code pins 17, 18, 22. 23, 24, 27 are used The LCD requires 5V supply)

## Compilation Commands
To compilate application use ***make***  
To delete object files after compilation use ***make clean***  

## Overview  
This is a CAN logger that can send and receive messages through CAN bus. Operations made through CAN bus can be optionally logged out into a separate text file.  
Tools that were written for the project:

1. **CAN and Logger**  
   Those are the classes that implement the basic functionality of the project.
3. **BMP280 driver**  
   It is used to read data from bmp280 sensor via I2C bus.
4. **LCD driver**  
   This driver provides methods for basic operations on LCD display in 4 bit mode with no flag check. For more specific purpose of displaying sensor data, the bmp_lcd children class was written.
5. **Timer**  
   The timer class is a universal tool for implementing multithread scheduled routine. It operates on external pointer to routine handling functions to require no dependencies with other specialized tools.  

Those tools are designed to be independent from each others, thus they can be used in other applications.
