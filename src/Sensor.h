#ifndef _SENSOR_H
#define _SENSOR_H

#include <math.h>
#define _USE_MATH_DEFINES

#include <mutex>

#include "Loggable.h"
#include "Runnable.h"
#include "Sleepable.h"

class Sensor : public Loggable, public Sleepable, public Runnable
{
private:

	double control;
	double data;
	int status;

	std::mutex sensorMutex;

	void run();

	void readEnv();
	void convertSignal();

public:

	enum SensorStatus { NONE, OK, ERR };

	Sensor(std::string logName);
	Sensor(const Sensor& sensor);
	~Sensor();

	void readSensor(double& data_, int& status_);

};

#endif
