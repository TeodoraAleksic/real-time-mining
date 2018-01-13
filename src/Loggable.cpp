#include "Loggable.h"

#include "CustomSink.h"


Loggable::Loggable(std::string name)
{
	std::vector<spdlog::sink_ptr> sinks;

	static spdlog::sink_ptr fileSink = Loggable::getFileSink();
	static spdlog::sink_ptr consoleSink = std::make_shared<spdlog::sinks::stdout_sink_mt>();

	if (fileSink) sinks.push_back(fileSink);
	else sinks.push_back(consoleSink);

	sinks.push_back(CustomSink::getInstance().getSinkPtr());

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

