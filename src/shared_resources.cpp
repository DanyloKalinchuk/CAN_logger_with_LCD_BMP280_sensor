#include "shared_resources.hpp"

SharedResources* SharedResources::instance_ = nullptr;

SharedResources::SharedResources() :
	logger(LOGS_OUT_FILE), can_ctrl(&logger), lcd(LCD_PINS), sensor() {}

SharedResources* SharedResources::get_instance(){
	if (instance_ == nullptr){
		instance_ = new SharedResources;
	}

	return instance_;
}

