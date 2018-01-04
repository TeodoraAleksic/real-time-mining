#ifndef _SENSOR_H
#define _SENSOR_H

#include "Runnable.h"
#include "Sleepable.h"

class Sensor : public Runnable, public Sleepable
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

	enum SensorStatus { NONE, OK, ERROR };

	Sensor();
	~Sensor();

	void readSensor(double& data_, int& status_);

};

#endif
