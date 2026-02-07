#include "hd44780_4bit.hpp"

void LCD_4BIT::clean_up(){
	gpiod_line_request_release(this->request);
	gpiod_chip_close(this->chip);
}

void LCD_4BIT::set_rs_value(unsigned int value){
	if (gpiod_line_request_set_value(this->request, this->rs, gpiod_val(value)) < 0){
		this->clean_up();
		throw std::runtime_error("Failed to set RS value");
	}
}

void LCD_4BIT::set_line_value(unsigned int line, unsigned int value){
	if (gpiod_line_request_set_value(this->request, this->lines[line], gpiod_val(value)) < 0){
		this->clean_up();
		throw std::runtime_error("Failed to set value of d" + (line + 4));
	}
}

void LCD_4BIT::enable_pulse(){
	delay_us(1);

	if (gpiod_line_request_set_value(this->request, this->e, gpiod_val(1)) < 0){
		this->clean_up();
		throw std::runtime_error("Failed to set E value");
	}

	delay_us(2);

	if (gpiod_line_request_set_value(this->request, this->e, gpiod_val(0)) < 0){
		this->clean_up();
		throw std::runtime_error("Failed to set E value");
	}

	delay_us(2);
}

void LCD_4BIT::write_nibble(unsigned int val){
	for (int i = 0; i < 4; i++){
		unsigned int line_val = ((val >> i) & 1UL);
		this->set_line_value(i, line_val);
	}
	this->enable_pulse();
}

void LCD_4BIT::delay_ms(unsigned int ms){
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void LCD_4BIT::delay_us(unsigned int us){
	std::this_thread::sleep_for(std::chrono::microseconds(us));
}

/*
Writes Data/Command to LCD as a byte
is_str defines byte as Data if TRUE and Command if FALSE
*/
void LCD_4BIT::write_byte(unsigned int val, bool is_str){
	this->set_rs_value(is_str);

	write_nibble(val >> 4);
	write_nibble(val & 0xFUL);
	delay_us(160);
}

/*
Variable(LCD Pin) | Description
rs(R/S)           | Instruction/Register Select
e(E)              | Enable. Starts data read/write
dX(DX)            | Data/Command bits
*/
LCD_4BIT::LCD_4BIT(unsigned int rs, unsigned int e,  unsigned int d4,
			unsigned int d5, unsigned int d6, unsigned int d7){

	this->rs = rs;
	this->e = e;
	this->lines[0] = d4;
	this->lines[1] = d5;
	this->lines[2] = d6;
	this->lines[3] = d7;

	this->chip = gpiod_chip_open("/dev/gpiochip0");
	if (!this->chip){
		throw std::runtime_error("Failed to open the chip");
	}

	struct gpiod_line_settings *settings = gpiod_line_settings_new();
	if (!settings){
		gpiod_chip_close(this->chip);
		throw std::runtime_error("Failed to creat line_settings");
	}

	gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);
	gpiod_line_settings_set_output_value(settings, GPIOD_LINE_VALUE_INACTIVE);

	struct gpiod_line_config *config = gpiod_line_config_new();
	if (!config){
		gpiod_line_settings_free(settings);
		gpiod_chip_close(this->chip);
		throw std::runtime_error("Failed to create line_config");
	}

	unsigned int offsets[] = {rs, e, d4, d5, d6, d7};
	if (gpiod_line_config_add_line_settings(config, offsets, 6, settings) < 0){
		gpiod_line_config_free(config);
		gpiod_line_settings_free(settings);
		gpiod_chip_close(this->chip);
		throw std::runtime_error("Failed to add line settings");
	}

	struct gpiod_request_config *request_conf = gpiod_request_config_new();
	if (!request_conf){
		gpiod_line_config_free(config);
		gpiod_line_settings_free(settings);
		gpiod_chip_close(this->chip);
		throw std::runtime_error("Failed to create request_config");
	}

	gpiod_request_config_set_consumer(request_conf, "hd44780_lcd");

	this->request = gpiod_chip_request_lines(this->chip, request_conf, config);
	gpiod_request_config_free(request_conf);
	gpiod_line_config_free(config);
	gpiod_line_settings_free(settings);
	if (!this->request){
		gpiod_chip_close(this->chip);
		throw std::runtime_error("Failed to request the lines");
	}


	delay_ms(15);

	this->set_rs_value(0);

	this->write_nibble(0x03UL);
	delay_ms(5);

	this->write_nibble(0x03UL);
	delay_us(160);

	this->write_nibble(0x03UL);
	delay_us(160);

	this->write_nibble(0x02UL);
	delay_us(160);

	this->write_byte(0x28UL, 0);
	delay_us(160);

	this->write_byte(0x10UL, 0);

	this->clear();

	this->write_byte(0x06UL, 0);
	delay_us(160);

	this->write_byte(0x0DUL, 0);
	delay_us(160);
}

LCD_4BIT::~LCD_4BIT(){
	this->clean_up();
}

void LCD_4BIT::write_str(std::string str){
	for (char ch : str){
		this->write_byte(ch, 1);
	}
}


void LCD_4BIT::clear(){
	this->write_byte(0x01UL, 0);
	delay_ms(2);
}

void LCD_4BIT::turn_on(){
	this->write_byte(0x0EUL, 0);
	delay_ms(2);
}

void LCD_4BIT::turn_off(){
	this->write_byte(0x08UL, 0);
	delay_ms(2);
}

void LCD_4BIT::set_cursor(unsigned short line, unsigned short position){
	if (line > 1 && position > 15){
		throw std::runtime_error("Position out of borders");
	}

	this->write_byte((0x80UL | ((line * 0x40UL) | position)), 0);
}


