#include "can.hpp"

Can::Can(){
	this->sockfd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
	struct sockaddr_can addr;
	struct ifreq ifr;


	strcpy(ifr.ifr_name, "can0");
	ioctl(this->sockfd, SIOCGIFINDEX, &ifr);

	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	if (bind(this->sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0){
		throw std::runtime_error("Failed to bind can socket");
	}
}

Can::Can(Logger* logger){
	this->sockfd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
	struct sockaddr_can addr;
	struct ifreq ifr;


	strcpy(ifr.ifr_name, "can0");
	ioctl(this->sockfd, SIOCGIFINDEX, &ifr);

	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	if (bind(this->sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0){
		throw std::runtime_error("Failed to bind can socket");
	}

	this->logger = logger;
	this->logs_on = true;
}

std::string Can::log_message(bool recieved, struct can_frame *frame){
	std::string message = "";

	if (recieved){
		message += "RECIEVED";
	}else{
		message += "SENDED";
	}

	std::stringstream hex_num;
	std::string hex_str;

	hex_num << std::hex << std::setfill('0') << std::setw(2)
		<< (frame->can_id & (SENDER_MASK));
	hex_str = hex_num.str();
	hex_num.str(std::string());

	message += " | Sender: " + hex_str;

	hex_num << std::hex << std::setfill('0') << std::setw(2)
		<< (frame->can_id & (SOURCE_MASK));
	hex_str = hex_num.str();
	hex_num.str(std::string());

	message += " | Source: " + hex_str;
	message += " | Data:";

	for (int i = 0; i < 8; i++){
		hex_num << std::hex << std::setfill('0') << std::setw(2)
			<< static_cast<unsigned int>(frame->data[i]);
		hex_str = hex_num.str();
		hex_num.str(std::string());

		message += " " + hex_str;
	}

	message += "\n";

	return message;
}

void Can::send(unsigned int id, unsigned short len, std::uint64_t data){
	struct can_frame frame;
	frame.can_id = id;
	frame.len = len;
	for (int i = 0; i < len; i++){
		frame.data[i] = (data >> (i * 8)) & 0xFFUL;
	}

	std::lock_guard<std::mutex> lock(this->send_mtx);
	int nbytes = ::write(this->sockfd, &frame, sizeof(struct can_frame));

	if (nbytes < 0){
		throw std::runtime_error("Failed to send can frame");
	}

	if (this->logs_on){
		this->logger->log_out(log_message(false, &frame));
	}
}

can::frame_data Can::read(){
	struct can_frame frame;

	std::lock_guard<std::mutex> lock(this->read_mtx);
	int nbytes = ::read(this->sockfd, &frame, sizeof(struct can_frame));
	if (nbytes < 0){
		throw std::runtime_error("Failed to read a frame");
	}

	struct can::frame_data fr_data;
	fr_data.id = frame.can_id;
	fr_data.len = frame.len;
	for (int i = 0; i < frame.len; i++){
		fr_data.data |= ((std::uint64_t)frame.data[i] << (i * 8));
	}

	if (this->logs_on){
		this->logger->log_out(log_message(true, &frame));
	}

	return fr_data;
}

can::frame_data Can::read(unsigned int mask){
	struct can_frame frame;

	std::lock_guard<std::mutex> lock(this->read_mtx);
	int nbytes = ::read(this->sockfd, &frame, sizeof(struct can_frame));
	if (nbytes < 0){
		throw std::runtime_error("Failed to read a frame");
	}

	struct can::frame_data fr_data;

	if ((frame.can_id & mask) == mask){
		fr_data.id = frame.can_id;
		fr_data.len = frame.len;
		for (int i = 0; i < frame.len; i++){
			fr_data.data |= ((std::int64_t)frame.data[i] << (i * 8));
		}

		if (this->logs_on){
			this->logger->log_out(log_message(true, &frame));
		}

	}else{
		fr_data.id = INVALID_MASK;
		if (this->logs_on){
			this->logger->log_out("Recieved frame doesn't satisfy the provided mask filter");
		}
	}

	return fr_data;
}


