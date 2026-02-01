#include "can/can.hpp"
#include "logger/logger.hpp"
#include "bmp/bmp.hpp"
#include "lcd/bmp_lcd.hpp"
#include "timer/timer.hpp"

#define BMP_SRC (0x0AUL)
#define PI_SNDR (0xA0UL)
#define STOP_ID (0xEEUL)

void screen_update();
void can_sensor();

Logger logger = Logger("CanLogs.txt");
Can can_ctrl = Can(&logger);
Bmp sensor;
BMP_LCD lcd = BMP_LCD(17, 27, 22, 18, 23, 24);

int main(){
	Timer screen_timer = Timer(1, screen_update);
	Timer can_timer = Timer(5, can_sensor);

	while(1){
		can::frame_data frame_dt = can_ctrl.read();

		if ((frame_dt.id & 0xFUL) == BMP_SRC){

			double temp = ((frame_dt.data >> 32) & 0xFFFFFFFFUL);
			temp = temp / 100;

			double press = frame_dt.data & 0xFFFFFFFFUL;
			press = press / 256;

			std::cout << "Temp: " << temp <<
				"; Press: " << press << "\n";
		}else if (frame_dt.id == STOP_ID){
			break;
		}
	}

	return 0;
}

void screen_update(){
	std::pair<double, double> res = sensor.read_temp_press();
	int temp = res.first * 100;
	res.first = temp / 100;

	temp = res.second * 100;
	res.second = temp / 100;

	lcd.write_temp_press(res.first, res.second);
}

void can_sensor(){
	std::pair<int, int> res = sensor.read_raw();
	std::uint64_t data = ((std::uint64_t)res.first << 32) | res.second;

	can_ctrl.send((PI_SNDR | BMP_SRC), 8, data);
}


