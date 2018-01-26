#include "Sensor.h"

#include <stdlib.h>
#include <iostream>


Sensor::Sensor(std::string name):
	Loggable(name), Sleepable(50), control(0.0), data(0.0), status(SensorStatus::NONE), previousError(false)
{
}


Sensor::Sensor(const Sensor& sensor):
	Loggable(sensor.logger), Sleepable(50), control(0.0), data(0.0), status(SensorStatus::NONE)
{
}


Sensor::~Sensor()
{
}


void Sensor::run()
{
	logger->info("Starting");

	initControl();

	while (running)
	{
		readEnv();
		convertSignal();
	}

	logger->info("Stopping");
}


void Sensor::initControl()
{
	srand((int)std::hash<std::thread::id>{}(std::this_thread::get_id()));
	int number = rand();

	// Generates control between the max and min of the sin function
	if (number % 3 == 0)
		// Generates control in range [0, 35]
		control = (double)(0 + (number % (35 - 0 + 1)));
	else if (number % 3 == 1)
		// Generates control in range [135, 215]
		control = (double)(135 + (number % (215 - 135 + 1)));
	else
		// Generates control in range [315, 350]
		control = (double)(315 + (number % (350 - 315 + 1)));
}


void Sensor::readEnv()
{
	std::unique_lock<std::mutex> guard(sensorMutex);
	sensorCond.wait_until(guard, std::chrono::steady_clock::now() + std::chrono::milliseconds(150));

	status = SensorStatus::NONE;

	control += 0.5;
}


void Sensor::convertSignal()
{
	computeNextTime();
	std::this_thread::sleep_until(sleepUntil);

	std::lock_guard<std::mutex> guard(sensorMutex);

	// A periodic function that produces values from 0 to 100
	data = (sin(control * M_PI / 180.0) + 1) * 100 / 2;

	// Generates error
	bool error = (int)std::ceil(data) % 7 == 0;

	if (error)
	{
		auto currentError = std::chrono::steady_clock::now();
		auto diff = std::chrono::duration_cast<chrono::milliseconds>(currentError - lastDoubleError);

		if (!previousError) // Single error
		{
			status = SensorStatus::ERR;
			previousError = true;
		}
		else if (diff > std::chrono::milliseconds(5000)) // Allow double error if last double error was more than 5 seconds ago
		{
			lastDoubleError = currentError;
			status = SensorStatus::ERR;
		}
		else // Ignore double error
		{
			status = SensorStatus::OK;
			previousError = false;
		}
	}
	else 
	{
		status = SensorStatus::OK;
		previousError = false;
	}
}


void Sensor::readSensor(double& data_, int& status_)
{
	std::lock_guard<std::mutex> guard(sensorMutex);
	data_ = data;
	status_ = status;
}


void Sensor::signal()
{
	sensorCond.notify_one();
}
