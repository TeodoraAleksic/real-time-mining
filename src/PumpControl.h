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

	int errors;
	const int maxErrors;

	bool pumpAlarm;
	bool ch4Alarm;

	std::mutex pumpMutex;

	void run();

	void highLevelInterrupt();
	void lowLevelInterrupt();

public:

	PumpControl(std::shared_ptr<spdlog::sinks::simple_file_sink_mt> sink, EnvironmentMonitor& envMonitor_, WaterTank& waterTank_);
	~PumpControl();

	bool getPumpAlarm();

	bool isTurnedOn();

	void turnOn();
	void turnOff();

	friend class WaterLevelMonitor;

};

#endif
