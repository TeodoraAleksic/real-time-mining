#ifndef _LOGGABLE_H_
#define _LOGGABLE_H_

#include <string>
#include <vector>

#include "spdlog\spdlog.h"

class Loggable
{
private:

	std::string getLogDir();
	std::string getTimeStamp();
	spdlog::sink_ptr getFileSink();

protected:

	std::shared_ptr<spdlog::logger> logger;

public:

	Loggable(std::string name);
	Loggable(std::shared_ptr<spdlog::logger> logger_);
	~Loggable();

};

#endif
