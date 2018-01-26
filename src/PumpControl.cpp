#include "PumpControl.h"

#include "GlobalException.h"
#include "EnvironmentMonitor.h"
#include "WaterLevelMonitor.h"
#include "WaterTank.h"

#include "fmt\ostream.h"


PumpControl::PumpControl(EnvironmentMonitor& envMonitor_, WaterTank& waterTank_):
	Loggable("pump_controll"), Sleepable(150), envMonitor(envMonitor_), waterTank(waterTank_), 
	turnedOn(false), maxErrors(6), pumpAlarm(false), ch4Alarm(false)
{
}


PumpControl::~PumpControl()
{
}


void PumpControl::run()
{
	logger->info("Starting");

	bool deadlineMissed = false;
	int errors = 0;
	double previousLevel = waterTank.getWaterLevel();

	while (running)
	{
		computeNextTime();

		// Checks if thread missed deadline
		if (std::chrono::steady_clock::now() > sleepUntil)
		{
			logger->error("Missed deadline");

			if (deadlineMissed)
			{
				// Sets global exception if thread missed deadline twice
				logger->error("Missed deadline twice");
				GlobalException::getInstance().setGlobalException();
			}
			
			deadlineMissed = true;
			pumpAlarm = true;
		}
		else
		{
			deadlineMissed = false;
			pumpAlarm = false;
		}

		std::this_thread::sleep_until(sleepUntil);

		double value;
		bool alarm;

		// Gets CH4 sensor data
		envMonitor.getSensorData(EnvironmentMonitor::SensorID::CH4, value, alarm);

		std::lock_guard<std::mutex> guard(pumpMutex);

		if (alarm) // Checks is the CH4 alarm is on
		{
			ch4Alarm = true;

			if (turnedOn)
			{
				// Turns pump off if CH4 alarm is on
				logger->info("Turning pump off");
				turnedOn = false;
				waterTank.increase();
			}

			continue;
		}
		else ch4Alarm = false;

		double currentLevel = waterTank.getWaterLevel();

		// Checks if the water level is incresing/decreasing
		if ((turnedOn && previousLevel < currentLevel) || (!turnedOn && previousLevel > currentLevel))
		{
			logger->warn(fmt::format("Pump error {}/{}", errors, maxErrors));
			errors += 1;

			// Turns the alarm on if the maximum number of pump errors has been reached
			if (errors >= maxErrors)
			{
				logger->error("Exceeded max pump errors");

				errors = 0;
				pumpAlarm = true;

				// Repeats the original pump action
				if (turnedOn)
					waterTank.increase();
				else
					waterTank.decrease();
			}
		}
		else
		{
			errors = 0;
			pumpAlarm = false;
		}

		previousLevel = currentLevel;
	}

	logger->info("Stopping");
}


bool PumpControl::getPumpAlarm()
{
	std::lock_guard<std::mutex> guard(pumpMutex);
	return pumpAlarm;
}


bool PumpControl::isTurnedOn()
{
	std::lock_guard<std::mutex> guard(pumpMutex);
	return turnedOn;
}


void PumpControl::highLevelInterrupt()
{
	turnOn();
}


void PumpControl::lowLevelInterrupt()
{
	turnOff();
}


void PumpControl::turnOn()
{
	std::lock_guard<std::mutex> guard(pumpMutex);

	if (!ch4Alarm && !turnedOn)
	{
		logger->info("Turning pump on");
		turnedOn = true;
		waterTank.decrease();
	}
}


void PumpControl::turnOff()
{
	std::lock_guard<std::mutex> guard(pumpMutex);

	if (turnedOn)
	{
		logger->info("Turning pump off");
		turnedOn = false;
		waterTank.increase();
	}
}
