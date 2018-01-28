#ifndef _GLOABALEXCEPTION_H_
#define _GLOABALEXCEPTION_H_

#include <mutex>

class GlobalEvent
{
private:

	bool globalException;
	std::mutex exceptionMutex;

	std::mutex waterMutex;
	std::condition_variable waterCond;

	GlobalEvent() {}
	GlobalEvent(const GlobalEvent &) = delete;
	void operator=(const GlobalEvent &) = delete;

public:

	static GlobalEvent& getInstance();

	bool getGlobalException();
	void setGlobalException();

	void waitWaterLevel();
	void signalWaterLevel();

};

#endif
