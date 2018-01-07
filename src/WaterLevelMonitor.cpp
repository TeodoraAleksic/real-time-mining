#include "WaterLevelMonitor.h"
#include "PumpControl.h"
#include "WaterTank.h"


WaterLevelMonitor::WaterLevelMonitor(std::shared_ptr<spdlog::sinks::simple_file_sink_mt> sink, WaterTank& waterTank_, PumpControl& pumpControl_):
	Loggable("water_level", sink), Sleepable(50), waterTank(waterTank_), pumpControl(pumpControl_), 
	highLevelThreshold(80.0), lowLevelThreshold(20.0), highLevelAlarm(false), lowLevelAlarm(false)
{
}


WaterLevelMonitor::~WaterLevelMonitor()
{
}


void WaterLevelMonitor::run()
{
	logger->info("Starting");

	while (running)
	{
		computeNextTime();
		std::this_thread::sleep_until(sleepUntil);

		double waterLevel = waterTank.getWaterLevel();

		std::lock_guard<std::mutex> guard(monitorMutex);

		if (waterLevel >= highLevelThreshold)
		{
			if (!highLevelAlarm)
			{
				// Raises high water level alarm if not raised
				logger->error("High water level");
				highLevelAlarm = true;
				pumpControl.highLevelInterrupt();
			}
		}
		else highLevelAlarm = false;

		if (waterLevel <= lowLevelThreshold)
		{
			if (!lowLevelAlarm)
			{
				logger->error("Low water level");
				// Raises low water level alarm if not raised
				lowLevelAlarm = true;
				pumpControl.lowLevelInterrupt();
			}
		}
		else lowLevelAlarm = false;
	}

	logger->info("Stopping");
}


void WaterLevelMonitor::getAlarms(bool& highLevelAlarm_, bool& lowLevelAlarm_)
{
	std::lock_guard<std::mutex> guard(monitorMutex);
	highLevelAlarm_ = highLevelAlarm;
	lowLevelAlarm_ = lowLevelAlarm;
}


void WaterLevelMonitor::setHighLevelThreshold(double threshold)
{
	std::lock_guard<std::mutex> guard(monitorMutex);
	highLevelThreshold = threshold;
}


void WaterLevelMonitor::setLowLevelThreshold(double threshold)
{
	std::lock_guard<std::mutex> guard(monitorMutex);
	lowLevelThreshold = threshold;
}
