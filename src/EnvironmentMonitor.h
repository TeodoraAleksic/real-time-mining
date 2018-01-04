#ifndef _ENVIRONMENTMONITOR_H
#define _ENVIRONMENTMONITOR_H

#include <vector>

using namespace std;

#include "Runnable.h"
#include "Sleepable.h"

class Sensor;

class EnvironmentMonitor : public Runnable, public Sleepable
{
private:

	std::vector<Sensor> sensors;

	std::vector<double> thresholds;
	std::vector<double> values;
	std::vector<bool> errors;
	std::vector<bool> alarms;

	std::vector<std::mutex> sensorMutexes;

	void run();

public:

	enum SensorID { CH4, CO, AIR_FLOW };

	EnvironmentMonitor();
	~EnvironmentMonitor();

	void setSleepInterval(SensorID sensorID, int sleepInterval);

	void setThreshold(SensorID sensorID, double threshold);

	void getSensorData(SensorID sensorID, double& value, bool& error, bool& alarm);

	using Sleepable::setSleepInterval;

};

#endif
