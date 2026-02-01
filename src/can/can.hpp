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

#include <cstdint>
#include <memory>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <mutex>
#include "../logger/logger.hpp"

#define INVALID_MASK 0xFFUL
#define SENDER_MASK 0xF0UL
#define SOURCE_MASK 0x0FUL

namespace can{
	struct frame_data{
		unsigned int id;
		std::uint64_t  data = 0;
		unsigned short len;
	};

};

class Can{
	int sockfd;
	Logger* logger;
	bool logs_on = false;

	std::mutex read_mtx, send_mtx;

	protected:
	std::string log_message(bool recieved, struct can_frame *frame);

	public:
	Can();
	Can(Logger* logger);
	~Can() = default;

	void send(unsigned int id, unsigned short len, std::uint64_t data);
	can::frame_data read();
	can::frame_data read(unsigned int mask);
};

#endif


