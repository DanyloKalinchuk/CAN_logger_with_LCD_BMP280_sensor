#include "logger.hpp"

Logger::Logger(std::string file_path) :
	file(file_path, std::ios::out)
{
	if (!this->file){
		throw std::runtime_error("Failed to open logging file");
	}
}

Logger::~Logger(){
	this->file.close();
}

void Logger::log_out(std::string message){
	this->file << message;
}


