#include "EnvironmentMonitor.h"
#include "Sensor.h"

#include "fmt\ostream.h"


EnvironmentMonitor::EnvironmentMonitor(std::shared_ptr<spdlog::sinks::simple_file_sink_mt> sink):
	Loggable("env_monitor", sink), Sleepable(150), values(3, 0.0), errors(3, false), alarms(3, false), sensorMutexes(3)
{
	for (int i = 0; i < 3; ++i)
		switch (i)
		{
		case SensorID::CH4:
			sensors.emplace_back("ch4_sensor", sink);
			thresholds.push_back(80.0);
			break;
		case SensorID::CO:
			sensors.emplace_back("co_sensor", sink);
			thresholds.push_back(80.0);
			break;
		case SensorID::AIR_FLOW:
			sensors.emplace_back("air_flow_sensor", sink);
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
			}
			else 
			{
				if (errors[i]) // Double error
				{
					logger->error(fmt::format("Sensor {} double error", sensorIDToStr(i)));
					alarms[i] = true;
					continue; // Skips comparison against threshold
				}
				else // Single error
				{
					logger->warn(fmt::format("Sensor {} single error", sensorIDToStr(i)));
					errors[i] = true;
				}
			}

			values[i] = data;

			if ((i == SensorID::AIR_FLOW && data < thresholds[i]) || (i != SensorID::AIR_FLOW && data > thresholds[i]))
			{
				if (!alarms[i]) logger->error(fmt::format("Sensor {} value above threshold", sensorIDToStr(i)));
				alarms[i] = true;
			}
			else
			{
				if (alarms[i]) logger->info(fmt::format("Sensor {} value under threshold", sensorIDToStr(i)));
				alarms[i] = false;
			}
		}
	}

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
