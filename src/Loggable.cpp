#include "Loggable.h"


Loggable::Loggable(std::string name, std::shared_ptr<spdlog::sinks::simple_file_sink_mt> sink):
	logger(std::make_shared<spdlog::logger>(name, sink))
{
}


Loggable::Loggable(std::shared_ptr<spdlog::logger> logger_): logger(logger_)
{
}


Loggable::~Loggable()
{
}
