#include "Sleepable.h"


Sleepable::Sleepable(int sleepInterval_): sleepInterval(sleepInterval_), initialized(false)
{
}


Sleepable::~Sleepable()
{
}


void Sleepable::computeNextTime()
{
	if (!initialized)
		sleepUntil = std::chrono::steady_clock::now();

	sleepUntil += std::chrono::milliseconds(sleepInterval);
}
