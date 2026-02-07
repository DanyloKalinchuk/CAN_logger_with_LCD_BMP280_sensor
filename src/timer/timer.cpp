#include "timer.hpp"

void Timer::handle_function(){
	while (this->running.load()){
		std::this_thread::sleep_for(std::chrono::seconds(this->delay_sec));
		this->function();
	}
}

Timer::Timer(unsigned int delay_sec, void (*function)()){
	this->function = function;
	this->delay_sec = delay_sec;

	this->running.store(true);
	this->timer_thread = std::thread(&Timer::handle_function, this);
}

Timer::~Timer(){
	this->running.store(false);

	if (this->timer_thread.joinable()){
		this->timer_thread.join();
	}
}


