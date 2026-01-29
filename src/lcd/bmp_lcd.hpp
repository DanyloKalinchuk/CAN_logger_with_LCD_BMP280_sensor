#ifndef BMP_LCD_HPP
#define BMP_LCD_HPP

#include "4_bit/hd44780_4bit.hpp"
#include <mutex>

class BMP_LCD : public LCD_4BIT{
	std::mutex update_mtx;

	public:
	BMP_LCD(unsigned int rs, unsigned int e, unsigned int d4,
			unsigned int d5, unsigned int d6, unsigned int d7);

	void write_temp_press(double temp, double press);
};

#endif

