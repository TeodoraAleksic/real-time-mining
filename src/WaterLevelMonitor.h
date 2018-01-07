#ifndef _WATERLEVELMONITOR_H_
#define _WATERLEVELMONITOR_H_

#include "Runnable.h"
#include "Sleepable.h"

#include <mutex>

class WaterTank;
class PumpControl;

class WaterLevelMonitor: public Runnable, public Sleepable
{
private:

	WaterTank& waterTank;
	PumpControl& pumpControl;

	double highLevelThreshold;
	double lowLevelThreshold;

	bool highLevelAlarm;
	bool lowLevelAlarm;

	std::mutex monitorMutex;

	void run();

public:

	WaterLevelMonitor(WaterTank& waterTank_, PumpControl& pumpControl_);
	~WaterLevelMonitor();

	void getAlarms(bool& highLevelAlarm_, bool& lowLevelAlarm_);

	void setHighLevelThreshold(double threshold);
	void setLowLevelThreshold(double threshold);

};

#endif
