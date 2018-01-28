#include "GlobalEvent.h"


GlobalEvent& GlobalEvent::getInstance()
{
	static GlobalEvent instance;
	return instance;
}


bool GlobalEvent::getGlobalException()
{
	std::lock_guard<std::mutex> guard(exceptionMutex);
	return globalException;
}


void GlobalEvent::setGlobalException()
{
	std::lock_guard<std::mutex> guard(exceptionMutex);
	globalException = true;
}


void GlobalEvent::waitWaterLevel()
{
	std::unique_lock<std::mutex> guard(waterMutex);
	waterCond.wait(guard);
}


void GlobalEvent::signalWaterLevel()
{
	waterCond.notify_one();
}
