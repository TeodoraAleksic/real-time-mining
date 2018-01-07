#include "PumpControl.h"
#include "EnvironmentMonitor.h"
#include "WaterLevelMonitor.h"
#include "WaterTank.h"

#include "fmt\ostream.h"


PumpControl::PumpControl(std::shared_ptr<spdlog::sinks::simple_file_sink_mt> sink, EnvironmentMonitor& envMonitor_, WaterTank& waterTank_):
	Loggable("pump_controll", sink), Sleepable(150), envMonitor(envMonitor_), waterTank(waterTank_), 
	turnedOn(false), errors(0), maxErrors(6), pumpAlarm(false), ch4Alarm(false)
{
}


PumpControl::~PumpControl()
{
}


void PumpControl::run()
{
	logger->info("Starting");

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
				logger->info("Turning pump off");
				turnedOn = false;
				waterTank.increase();
				continue;
			}
		}
		else ch4Alarm = false;

		double currentLevel = waterTank.getWaterLevel();

		if ((turnedOn && previousLevel < currentLevel) || (!turnedOn && previousLevel > currentLevel))
		{
			logger->warn(fmt::format("Pump error {}/{}", errors, maxErrors));
			errors += 1;

			if (errors >= maxErrors)
			{
				if (!pumpAlarm)
				{
					logger->error("Exceeded max errors");

					pumpAlarm = true;

					if (turnedOn)
						waterTank.increase();
					else
						waterTank.decrease();
				}
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
	if (!ch4Alarm)
	{
		logger->info("Turning pump on");
		turnedOn = true;
		waterTank.decrease();
	}
}


void PumpControl::turnOff()
{
	std::lock_guard<std::mutex> guard(pumpMutex);
	logger->info("Turning pump off");
	turnedOn = false;
	waterTank.increase();
}
