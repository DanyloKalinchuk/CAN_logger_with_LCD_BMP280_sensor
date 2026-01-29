#include "bmp_lcd.hpp"

BMP_LCD::BMP_LCD(unsigned int rs, unsigned int e, unsigned int d4,
			unsigned int d5, unsigned int d6, unsigned int d7)
			: LCD_4BIT(rs, e, d4, d5, d6, d7)
{
	this->write_str("Temp: ");
	this->set_cursor(1, 0);
	this->write_str("Press: ");
}

void BMP_LCD::write_temp_press(double temp, double press){
	std::lock_guard<std::mutex> lock(this->update_mtx);

	this->set_cursor(0, 6);
	this->write_str(std::to_string(temp));

	this->set_cursor(1, 7);
	this->write_str(std::to_string(press));
}

