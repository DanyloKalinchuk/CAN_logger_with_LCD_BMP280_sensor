#include "bmp.hpp"

void Bmp::update_data(){
	std::lock_guard<std::mutex> lock(this->read_mtx);
	int res = bmp_read_data(&this->temp_press, &this->calib_dt);
	if (res){
		throw std::runtime_error("Failed to read sensor data");
	}
}

Bmp::Bmp(){
	int res = bmp_init(&this->calib_dt);
	if (res){
		throw std::runtime_error("Failed to initialize BMP sensor");
	}
}

Bmp::~Bmp(){
	bmp_exit();
}

double Bmp::read_temp(){
	this->update_data();
	return (double)this->temp_press.temperature / 100;
}

double Bmp::read_press(){
	this->update_data();
	return (double)this->temp_press.pressure / 256;
}

std::pair<double, double> Bmp::read_temp_press(){
	this->update_data();
	std::pair<float, float> res = {
		(double)this->temp_press.temperature / 100,
		(double)this->temp_press.pressure / 256
	};

	return res;
}

std::pair<int, int> Bmp::read_raw(){
	this->update_data();
	return {this->temp_press.temperature, this->temp_press.pressure};
}

