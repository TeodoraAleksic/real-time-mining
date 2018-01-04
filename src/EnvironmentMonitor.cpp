#include "stdafx.h"
#include "EnvironmentMonitor.h"
#include "Sensor.h"


EnvironmentMonitor::EnvironmentMonitor(): 
	Sleepable(150), sensors(3), values(3, 0.0), errors(3, false), alarms(3, false), sensorMutexes(3)
{
	for (int i = 0; i < 3; ++i)
		if (i == SensorID::AIR_FLOW)
			thresholds.push_back(20.0);
		else
			thresholds.push_back(80.0);
}


EnvironmentMonitor::~EnvironmentMonitor()
{
	stop();
}

void EnvironmentMonitor::run()
{
	for (int i = 0; i < 3; ++i)
		sensors[i].start();

	while (running)
	{
		computeNextTime();
		std::this_thread::sleep_until(sleepUntil);

		for (int i = 0; i < 3; ++i) // Iterates over sensors
		{
			double data;
			int status;

			// Reads sensor data and status
			sensors[i].readSensor(data, status);

			if (status == Sensor::SensorStatus::NONE)
				continue; // Skips sensors if no data has been read

			std::lock_guard<std::mutex> guard(sensorMutexes[i]);

			if (status == Sensor::SensorStatus::OK)
			{
				// Resets error and alarm if no error occurred
				errors[i] = false; 
				alarms[i] = false;
			}
			else 
			{
				if (errors[i]) // Double error
				{
					alarms[i] = true;

					if (i == SensorID::CH4)
					{
						// TODO CH4 interrupt
					}

					continue; // Skips comparison against threshold
				}
				else // Single error
				{
					errors[i] = true;
				}
			}

			values[i] = data;

			if ((i == SensorID::AIR_FLOW && data < thresholds[i]) || (i != SensorID::AIR_FLOW && data > thresholds[i]))
			{
				alarms[i] = true;

				if (i == SensorID::CH4)
				{
					// TODO CH4 interrupt
				}
			}
		}
	}
}

void EnvironmentMonitor::setSleepInterval(SensorID sensorID, int sleepInterval)
{
	sensors[sensorID].setSleepInterval(sleepInterval);
}

void EnvironmentMonitor::setThreshold(SensorID sensorID, double threshold)
{
	std::lock_guard<std::mutex> guard(sensorMutexes[sensorID]);
	thresholds[sensorID] = threshold;
}

void EnvironmentMonitor::getSensorData(SensorID sensorID, double& value, bool& error, bool& alarm)
{
	std::lock_guard<std::mutex> guard(sensorMutexes[sensorID]);
	value = values[sensorID];
	error = errors[sensorID];
	alarm = alarms[sensorID];
}
