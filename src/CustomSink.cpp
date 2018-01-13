#include "CustomSink.h"


void CustomSink::CustomLogSink::log(const spdlog::details::log_msg& msg)
{
	std::lock_guard<std::mutex> guard(sinkMutex);
	messages.push_back(msg.formatted.str());
}


void CustomSink::CustomLogSink::getMessages(std::vector<std::string>& messages_)
{
	std::lock_guard<std::mutex> guard(sinkMutex);
	messages_ = messages;
	messages.clear();
}


CustomSink& CustomSink::getInstance()
{
	static CustomSink instance;
	return instance;
}


spdlog::sink_ptr CustomSink::getSinkPtr()
{
	return customSink;
}


void CustomSink::getMessages(std::vector<std::string>& messages_)
{
	customSink->getMessages(messages_);
}
