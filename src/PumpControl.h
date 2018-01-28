#ifndef _PUMPCONTROL_H_
#define _PUMPCONTROL_H_

#include "Loggable.h"
#include "Runnable.h"
#include "Sleepable.h"

#include <mutex>

class EnvironmentMonitor;
class WaterLevelMonitor;
class WaterTank;

class PumpControl: public Loggable, public Sleepable, public Runnable
{
private:

	EnvironmentMonitor& envMonitor;
	WaterTank& waterTank;

	bool turnedOn;

	const int maxErrors;

	bool pumpAlarm;
	bool ch4Alarm;

	std::mutex pumpMutex;

	void run();

public:

	PumpControl(EnvironmentMonitor& envMonitor_, WaterTank& waterTank_);
	~PumpControl();

	bool getPumpAlarm();

	bool isTurnedOn();

	void turnOn();
	void turnOff();

	friend class WaterLevelMonitor;

};

#endif
