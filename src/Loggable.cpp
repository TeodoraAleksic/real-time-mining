#include "Loggable.h"


void CustomSink::getMessages(std::vector<std::string>& messages_)
{
	std::lock_guard<std::mutex> guard(customMutex);
	messages_ = messages;
	messages.clear();
}


spdlog::sink_ptr Loggable::fileSink = Loggable::getFileSink();
spdlog::sink_ptr Loggable::consoleSink = std::make_shared<spdlog::sinks::stdout_sink_mt>();
spdlog::sink_ptr Loggable::customSink = std::make_shared<CustomSink>();


Loggable::Loggable(std::string name)
{
	std::vector<spdlog::sink_ptr> sinks;

	if (fileSink) sinks.push_back(fileSink);
	else sinks.push_back(consoleSink);

	// fileSink ? sinks.push_back(fileSink) : sinks.push_back(consoleSink);
	sinks.push_back(customSink);

	logger = std::make_shared<spdlog::logger>(name, begin(sinks), end(sinks));
}


Loggable::Loggable(std::shared_ptr<spdlog::logger> logger_): logger(logger_)
{
}


Loggable::~Loggable()
{
}


std::string Loggable::getLogDir()
{
	std::string path = "logs/";

	DWORD fileAttr = GetFileAttributesA(path.c_str());

	// Checks if directory exists
	if (!(fileAttr != INVALID_FILE_ATTRIBUTES && (fileAttr & FILE_ATTRIBUTE_DIRECTORY)))
		// Tries to creates directory
		if (!CreateDirectoryA(path.c_str(), nullptr))
			path = "";

	return path;
}


std::string Loggable::getTimeStamp()
{
	time_t rawtime;
	struct tm* timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "%d-%m-%y-%H-%M-%S", timeinfo);

	return std::string(buffer);
}


spdlog::sink_ptr Loggable::getFileSink()
{
	try
	{
		// Tries to get a file logger sink
		return std::make_shared<spdlog::sinks::simple_file_sink_mt>(fmt::format("{}rtm-{}.log", getLogDir(), getTimeStamp()));
	}
	catch (...)
	{
		return nullptr;
	}
}

