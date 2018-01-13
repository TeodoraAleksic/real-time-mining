#ifndef _CUSTOMSINK_H_
#define _CUSTOMSINK_H_

#include <mutex>
#include <string>
#include <vector>

#include "spdlog\spdlog.h"

class CustomSink
{
private:

	class CustomLogSink: public spdlog::sinks::sink
	{
	private:

		std::mutex sinkMutex;

		std::vector<std::string> messages;

		void log(const spdlog::details::log_msg& msg) override;
		void flush() {}

	public:

		void getMessages(std::vector<std::string>& messages_);

	};

	std::shared_ptr<CustomLogSink> customSink;

	CustomSink(): customSink(std::make_shared<CustomLogSink>()) {}
	CustomSink(const CustomSink&) = delete;
	void operator=(const CustomSink&) = delete;

public:

	static CustomSink& getInstance();
	
	spdlog::sink_ptr getSinkPtr();

	void getMessages(std::vector<std::string>& messages_);

};

#endif
