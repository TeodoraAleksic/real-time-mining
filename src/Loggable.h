#ifndef _LOGGABLE_H_
#define _LOGGABLE_H_

#include <mutex>
#include <string>
#include <vector>

#include "spdlog\spdlog.h"

class CustomSink: public spdlog::sinks::base_sink<std::mutex>
{
private:

	std::mutex customMutex;

	std::vector<std::string> messages;

protected:

	void _sink_it(const spdlog::details::log_msg& msg) override
	{
		messages.push_back(msg.formatted.str());
	}

	void _flush() override
	{
	}

public:

	void getMessages(std::vector<std::string>& messages_);

};

class Loggable
{
private:

	static spdlog::sink_ptr fileSink;
	static spdlog::sink_ptr consoleSink;
	static spdlog::sink_ptr customSink;

	static std::string getLogDir();
	static std::string getTimeStamp();
	static spdlog::sink_ptr getFileSink();

protected:

	std::shared_ptr<spdlog::logger> logger;

public:

	Loggable(std::string name);
	Loggable(std::shared_ptr<spdlog::logger> logger_);
	~Loggable();

};

#endif
