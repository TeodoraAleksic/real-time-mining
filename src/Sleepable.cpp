#include "Sleepable.h"


Sleepable::Sleepable(int sleepInterval_): sleepInterval(sleepInterval_), initialized(false)
{
}


Sleepable::~Sleepable()
{
}


void Sleepable::setSleepInterval(int sleepInterval_)
{
	std::lock_guard<std::mutex> guard(sleepMutex);
	sleepInterval = sleepInterval_;
}


void Sleepable::computeNextTime()
{
	if (!initialized)
		sleepUntil = std::chrono::system_clock::now();

	std::lock_guard<std::mutex> guard(sleepMutex);
	sleepUntil += std::chrono::milliseconds(sleepInterval);
}
