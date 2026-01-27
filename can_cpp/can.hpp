#ifndef CAN_HPP
#define CAN_HPP

extern "C"{
#include <string.h>
#include <net/if.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <unistd.h>
}

#include <memory>
#include <iostream>
#include <sstream>
#include "logger.hpp"

#define INVALID_MASK 0xFFFFUL
#define SENDER_MASK 0xFF00UL
#define SOURCE_MASK 0xFFUL

namespace can{
	struct frame_data{
		unsigned int id;
		unsigned long data;
		unsigned short len;
	};

};

class Can{
	int sockfd;
	std::unique_ptr<Logger> logger;
	bool logs_on = false;

	protected:
	std::string log_message(bool recieved, struct can_frame *frame);

	public:
	Can();
	Can(Logger* logger);
	~Can() = default;

	void send(unsigned int id, unsigned short len, unsigned long data);
	can::frame_data read();
	can::frame_data read(unsigned int mask);
};

#endif

