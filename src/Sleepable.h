#ifndef _SLEEPABLE_H
#define _SLEEPABLE_H

#include <chrono>
#include <mutex>

using namespace std;

class Sleepable
{
protected:

	bool initialized;

	std::chrono::system_clock::time_point sleepUntil;
	int sleepInterval;

	void computeNextTime();

public:

	Sleepable(int sleepInterval_);
	~Sleepable();

};

#endif
