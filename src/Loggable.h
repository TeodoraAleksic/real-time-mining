#ifndef _LOGGABLE_H_
#define _LOGGABLE_H_

#include <string>

#include "spdlog\spdlog.h"

class Loggable
{
protected:

	std::shared_ptr<spdlog::logger> logger;

public:

	Loggable(std::string name, std::shared_ptr<spdlog::sinks::simple_file_sink_mt> sink);
	Loggable(std::shared_ptr<spdlog::logger> logger_);
	~Loggable();

};

#endif
