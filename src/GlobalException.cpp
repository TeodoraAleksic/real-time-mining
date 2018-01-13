#include "GlobalException.h"


GlobalException& GlobalException::getInstance()
{
	static GlobalException instance;
	return instance;
}


bool GlobalException::getGlobalException()
{
	std::lock_guard<std::mutex> guard(mutex);
	return globalException;
}


void GlobalException::setGlobalException()
{
	std::lock_guard<std::mutex> guard(mutex);
	globalException = true;
}

