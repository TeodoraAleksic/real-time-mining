#ifndef _ENVIRONMENTMONITOR_H
#define _ENVIRONMENTMONITOR_H

#include <string>
#include <vector>

#include "Loggable.h"
#include "Runnable.h"
#include "Sleepable.h"

class Sensor;

class EnvironmentMonitor : public Loggable, public Sleepable, public Runnable
{
public:

	enum SensorID { CH4, CO, AIR_FLOW };

private:

	std::vector<Sensor> sensors;

	std::vector<double> thresholds;
	std::vector<double> values;
	std::vector<bool> errors;
	std::vector<bool> alarms;

	std::vector<std::mutex> sensorMutexes;

	void run();

	std::string sensorIDToStr(int sensorID);

public:

	EnvironmentMonitor(std::shared_ptr<spdlog::sinks::simple_file_sink_mt> sink);
	~EnvironmentMonitor();

	void setSleepInterval(SensorID sensorID, int sleepInterval);

	void setThreshold(SensorID sensorID, double threshold);

	void getSensorData(SensorID sensorID, double& value, bool& error, bool& alarm);

	using Sleepable::setSleepInterval;

};

#endif
