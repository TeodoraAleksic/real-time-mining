#ifndef _GLOABALEXCEPTION_H_
#define _GLOABALEXCEPTION_H_

#include <mutex>

class GlobalException
{
private:

	bool globalException;

	std::mutex mutex;

	GlobalException() {}
	GlobalException(const GlobalException &) = delete;
	void operator=(const GlobalException &) = delete;

public:

	static GlobalException& getInstance();

	bool getGlobalException();
	void setGlobalException();

};

#endif
