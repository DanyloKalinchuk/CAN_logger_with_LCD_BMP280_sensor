#ifndef SHARED_RESOURCES
#define SHARED_RESOURCES

#include "can/can.hpp"
#include "logger/logger.hpp"
#include "bmp/bmp.hpp"
#include "lcd/bmp_lcd.hpp"

//Configurations
#define LCD_PINS 17, /*rs*/ \
	27, /*e*/  \
	22, /*d4*/ \
	18, /*d5*/ \
	23, /*d6*/ \
	24  /*d7*/ \

#define LOGS_OUT_FILE ("logs_output.txt")

class SharedResources{
	static SharedResources* instance_;

	protected:
	SharedResources();

	public:
	Logger logger;
	Can can_ctrl;
	BMP_LCD lcd;
	Bmp sensor;

	static SharedResources* get_instance();
};

#endif

