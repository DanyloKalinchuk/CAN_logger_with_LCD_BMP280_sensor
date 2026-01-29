#ifndef BMP_HPP
#define BMP_HPP

extern "C"{
#include "bmp_c/bmp.h"
}

#include <iostream>
#include <utility>
#include <mutex>

class Bmp{
	struct calibration_data calib_dt;
	struct bmp_sensdt temp_press;

	std::mutex read_mtx;

	protected:
	void update_data();

	public:
	Bmp();
	~Bmp();

	double read_temp();
	double read_press();
	std::pair<double, double> read_temp_press();
	std::pair<int, int> read_raw();
};

#endif

