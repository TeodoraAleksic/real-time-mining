#ifndef _PUMPCONTROL_H_
#define _PUMPCONTROL_H_

#include "Runnable.h"
#include "Sleepable.h"

#include <mutex>

class EnvironmentMonitor;
class WaterLevelMonitor;
class WaterTank;

class PumpControl: public Runnable, public Sleepable
{
private:

	EnvironmentMonitor& envMonitor;
	WaterTank& waterTank;

	bool turnedOn;

	int errors;
	const int maxErrors;

	bool pumpAlarm;
	bool ch4Alarm;

	std::mutex pumpMutex;

	void run();

	void highLevelInterrupt();
	void lowLevelInterrupt();

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
