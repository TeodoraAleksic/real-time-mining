#include "Sensor.h"

#include <stdlib.h>


Sensor::Sensor(): Sleepable(50), control(0.0), data(0.0), status(SensorStatus::NONE)
{
}


Sensor::~Sensor()
{
}


void Sensor::run()
{
	while (running)
	{
		readEnv();
		convertSignal();
	}
}


void Sensor::readEnv()
{
	if (control == 0) {
		srand(std::hash<std::thread::id>{}(std::this_thread::get_id()));
		int number = rand();

		// Generates control between the max and min of the sin function
		if (number % 3 == 0)
			// Generates control in range [0, 45]
			control = (double)(0 + (number % (45 - 0 + 1)));
		else if (number % 3 == 1)
			// Generates control in range [135, 225]
			control = (double)(135 + (number % (225 - 135 + 1)));
		else 
			// Generates control in range [315, 360]
			control = (double)(315 + (number % (360 - 315 + 1)));
	}

	control += 0.5;
}


void Sensor::convertSignal()
{
	computeNextTime();
	std::this_thread::sleep_until(sleepUntil);

	std::lock_guard<std::mutex> guard(sensorMutex);

	// A periodic function that produces values from 0 to 100
	data = (sin(control * M_PI / 180.0) + 1) * 100 / 2;

	bool error = (int)std::ceil(data) % 3 == 0;

	if (error)
		status = SensorStatus::ERROR;
	else
		status = SensorStatus::OK;
}


void Sensor::readSensor(double& data_, int& status_)
{
	std::lock_guard<std::mutex> guard(sensorMutex);
	data_ = data;
	status_ = status;
}
