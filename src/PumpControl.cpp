#include "PumpControl.h"
#include "EnvironmentMonitor.h"
#include "WaterLevelMonitor.h"
#include "WaterTank.h"


PumpControl::PumpControl(EnvironmentMonitor& envMonitor_, WaterTank& waterTank_): 
	Sleepable(150), envMonitor(envMonitor_), waterTank(waterTank_), turnedOn(false), errors(0), maxErrors(6), pumpAlarm(false), ch4Alarm(false)
{
}


PumpControl::~PumpControl()
{
}


void PumpControl::run()
{
	double previousLevel = waterTank.getWaterLevel();

	while (running)
	{
		computeNextTime();
		std::this_thread::sleep_until(sleepUntil);

		double value;
		bool error;
		bool alarm;

		// Gets CH4 sensor data
		envMonitor.getSensorData(EnvironmentMonitor::SensorID::CH4, value, error, alarm);

		std::lock_guard<std::mutex> guard(pumpMutex);

		if (alarm)
		{
			ch4Alarm = true;

			if (turnedOn)
			{
				// Turns pump off if CH4 alarm is on
				turnedOn = false;
				waterTank.increase();
				continue;
			}
		}
		else ch4Alarm = false;

		double currentLevel = waterTank.getWaterLevel();

		if ((turnedOn && previousLevel < currentLevel) || (!turnedOn && previousLevel > currentLevel))
		{
			errors += 1;

			if (errors >= maxErrors)
			{
				pumpAlarm = true;

				if (turnedOn)
					waterTank.increase();
				else
					waterTank.decrease();
			}
		}
		else errors = 0;

		previousLevel = currentLevel;
	}
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
	if (!ch4Alarm)
	{
		turnedOn = true;
		waterTank.decrease();
	}
}


void PumpControl::turnOff()
{
	std::lock_guard<std::mutex> guard(pumpMutex);
	turnedOn = false;
	waterTank.increase();
}
