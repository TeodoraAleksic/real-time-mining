#include "WaterLevelMonitor.h"

#include "GlobalException.h"
#include "PumpControl.h"
#include "WaterTank.h"


WaterLevelMonitor::WaterLevelMonitor(WaterTank& waterTank_, PumpControl& pumpControl_):
	Loggable("water_level"), Sleepable(150), waterTank(waterTank_), pumpControl(pumpControl_), 
	highLevelThreshold(80.0), lowLevelThreshold(20.0), highLevelAlarm(false), lowLevelAlarm(false)
{
}


WaterLevelMonitor::~WaterLevelMonitor()
{
}


void WaterLevelMonitor::run()
{
	logger->info("Starting");

	bool deadlineMissed = false;

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
		}
		else deadlineMissed = false;

		std::this_thread::sleep_until(sleepUntil);

		double waterLevel = waterTank.getWaterLevel();

		std::lock_guard<std::mutex> guard(monitorMutex);

		if (waterLevel >= highLevelThreshold)
		{
			if (!highLevelAlarm)
				logger->warn("High water level");

			// Raises high water level alarm
			highLevelAlarm = true;
			pumpControl.highLevelInterrupt();
		}
		else highLevelAlarm = false;

		if (waterLevel <= lowLevelThreshold)
		{
			if (!lowLevelAlarm)
				logger->warn("Low water level");
			
			// Raises low water level alarm
			lowLevelAlarm = true;
			pumpControl.lowLevelInterrupt();
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

	if (threshold > 0 && threshold < 100)
		if (threshold > lowLevelThreshold)
			highLevelThreshold = threshold;
		else
			logger->error("Threshold {} lower than low level threshold {}", threshold, lowLevelThreshold);
	else
		logger->error("Invalid threshold value {}", threshold);
}


void WaterLevelMonitor::setLowLevelThreshold(double threshold)
{
	std::lock_guard<std::mutex> guard(monitorMutex);
	
	if (threshold > 0 && threshold < 100)
		if (threshold < highLevelThreshold)
			lowLevelThreshold = threshold;
		else
			logger->error("Threshold {} higher than high level threshold {}", threshold, highLevelThreshold);
	else
		logger->error("Invalid threshold {}", threshold);
}
