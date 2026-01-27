#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <memory>

class Logger{
	std::fstream file;

	public:
	Logger(std::string file_path);
	~Logger();

	void log_out(std::string message);
};

#endif

