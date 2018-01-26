#include "EnvironmentMonitor.h"
#include "Sensor.h"

#include "fmt\ostream.h"


EnvironmentMonitor::EnvironmentMonitor():
	Loggable("env_monitor"), Sleepable(150), values(3, 0.0), alarms(3, false), sensorMutexes(3)
{
	for (int i = 0; i < 3; ++i)
		switch (i)
		{
		case SensorID::CH4:
			sensors.emplace_back("ch4_sensor");
			thresholds.push_back(80.0);
			break;
		case SensorID::CO:
			sensors.emplace_back("co_sensor");
			thresholds.push_back(80.0);
			break;
		case SensorID::AIR_FLOW:
			sensors.emplace_back("air_flow_sensor");
			thresholds.push_back(20.0);
			break;
		default:
			break;
		}
}


EnvironmentMonitor::~EnvironmentMonitor()
{
	stop();
}

void EnvironmentMonitor::run()
{
	logger->info("Starting");

	for (int i = 0; i < 3; ++i)
		sensors[i].start();

	std::vector<bool> nones{ false, false, false};
	std::vector<bool> errors{ false, false, false };
	std::vector<bool> invalids{ false, false, false };

	while (running)
	{
		for (int i = 0; i < 3; ++i)
			sensors[i].signal();

		computeNextTime();

		// Checks if thread missed deadline
		if (std::chrono::steady_clock::now() > sleepUntil)
		{
			logger->error("Missed deadline");

			// Sets all sensor alarms if monitor missed deadline
			for (int i = 0; i < 3; ++i)
				alarms[i] = true;
		}

		std::this_thread::sleep_until(sleepUntil);

		for (int i = 0; i < 3; ++i) // Iterates over sensors
		{
			double data;
			int status;

			// Reads sensor data and status
			sensors[i].readSensor(data, status);

			std::lock_guard<std::mutex> guard(sensorMutexes[i]);

			// Checks if sensor read data
			if (status == Sensor::SensorStatus::NONE)
			{
				// Turns on alarm if sensor missed deadline
				logger->info(fmt::format("Sensor {} missed deadline", sensorIDToStr(i)));
				alarms[i] = true;
				nones[i] = true;
				continue;
			}
			else if (nones[i]) 
			{
				// Waits for next data if sensor delivered old data
				alarms[i] = false;
				nones[i] = false;
				continue;
			}

			// Checks status register
			if (status == Sensor::SensorStatus::OK)
			{
				// Resets error and alarm if no error occurred
				alarms[i] = false;
				errors[i] = false; 
			}
			else if (errors[i]) // Double error
			{
				logger->error(fmt::format("Sensor {} double error", sensorIDToStr(i)));
				alarms[i] = true;
				continue; // Skips comparison against threshold
			}
			else // Single error
			{
				errors[i] = true;
			}

			values[i] = data;

			// Checks sensor value against threshold
			if ((i == SensorID::AIR_FLOW && data < thresholds[i]) || (i != SensorID::AIR_FLOW && data > thresholds[i]))
			{
				// Logs the invalid value at first detection
				if (!invalids[i])
					if (i == SensorID::AIR_FLOW) logger->warn(fmt::format("Sensor {} value bellow threshold", sensorIDToStr(i)));
					else logger->warn(fmt::format("Sensor {} value above threshold", sensorIDToStr(i)));

				alarms[i] = true;
				invalids[i] = true;
			}
			else
			{
				// Logs the valid value at first detection
				if (invalids[i])
					if (i == SensorID::AIR_FLOW) logger->info(fmt::format("Sensor {} value above threshold", sensorIDToStr(i)));
					else logger->info(fmt::format("Sensor {} value bellow threshold", sensorIDToStr(i)));

				alarms[i] = false;
				invalids[i] = false;
			}
		}
	}

	for (int i = 0; i < 3; ++i)
		sensors[i].stop();

	logger->info("Stopping");
}


std::string EnvironmentMonitor::sensorIDToStr(int sensorID)
{
	switch (sensorID)
	{
	case SensorID::CH4:
		return "CH4";
	case SensorID::CO:
		return "CO";
	case SensorID::AIR_FLOW:
		return "AIR_FLOW";
	default:
		return "";
	}
}


void EnvironmentMonitor::setThreshold(SensorID sensorID, double threshold)
{
	std::lock_guard<std::mutex> guard(sensorMutexes[sensorID]);

	if (threshold > 0 && threshold < 100)
		thresholds[sensorID] = threshold;
	else
		logger->error("Invalid threshold value {}", threshold);
}


void EnvironmentMonitor::getSensorData(SensorID sensorID, double& value, bool& alarm)
{
	std::lock_guard<std::mutex> guard(sensorMutexes[sensorID]);
	value = values[sensorID];
	alarm = alarms[sensorID];
}
