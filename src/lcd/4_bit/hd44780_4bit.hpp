#ifndef HD44780_4BIT_HPP
#define HD44780_4BIT_HPP

extern "C"{
#include <gpiod.h>
}

#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#define gpiod_val(val) ((val) == 0 ? GPIOD_LINE_VALUE_INACTIVE : GPIOD_LINE_VALUE_ACTIVE)

class LCD_4BIT{
	struct gpiod_chip *chip;
	struct gpiod_line_request *request;
	unsigned int lines[4];
	unsigned int rs;
	unsigned int e;

	void clean_up();
	void set_rs_value(unsigned int value);
	void set_line_value(unsigned int line, unsigned int value);
	void enable_pulse();
	void write_nibble(unsigned int val);

	protected:
	void delay_ms(unsigned int ms);
	void delay_us(unsigned int us);
	void write_byte(unsigned int val, bool is_str);

	public:
	LCD_4BIT(unsigned int rs, unsigned int e, unsigned int d4,
			unsigned int d5, unsigned int d6, unsigned int d7);

	~LCD_4BIT();
	void write_str(std::string str);

	void clear();
	void turn_on();
	void turn_off();
	void set_cursor(unsigned short line, unsigned short position);
};

#endif

