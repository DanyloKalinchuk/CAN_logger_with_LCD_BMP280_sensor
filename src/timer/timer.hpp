#ifndef TIMER_HPP
#define TIMER_HPP

#include <thread>
#include <chrono>

class Timer{
	unsigned int delay_sec;
	void (*function)();
	std::thread timer_thread;
	bool running = false;

	void handle_function();

	public:
	Timer(unsigned int delay_sec, void (*function)());
	~Timer();
};

#endif

