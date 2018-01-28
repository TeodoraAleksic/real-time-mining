#ifndef _WATERLEVELMONITOR_H_
#define _WATERLEVELMONITOR_H_

#include "Loggable.h"
#include "Runnable.h"
#include "Sleepable.h"

#include <mutex>

class PumpControl;
class WaterTank;

class WaterLevelMonitor: public Loggable, public Runnable
{
private:

	PumpControl& pumpControl;
	WaterTank& waterTank;

	std::mutex monitorMutex;

	void run();

public:

	WaterLevelMonitor(PumpControl& pumpControl_, WaterTank& waterTank_);
	~WaterLevelMonitor();

	void stop();

};

#endif
