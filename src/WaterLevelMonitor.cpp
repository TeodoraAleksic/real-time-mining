#include "WaterLevelMonitor.h"
#include "PumpControl.h"
#include "WaterTank.h"


WaterLevelMonitor::WaterLevelMonitor(WaterTank& waterTank_, PumpControl& pumpControl_): 
	Sleepable(50), waterTank(waterTank_), pumpControl(pumpControl_), 
	highLevelThreshold(80.0), lowLevelThreshold(20.0), highLevelAlarm(false), lowLevelAlarm(false)
{
}


WaterLevelMonitor::~WaterLevelMonitor()
{
}


void WaterLevelMonitor::run()
{
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
				highLevelAlarm = true;
				pumpControl.highLevelInterrupt();
			}
		}
		else highLevelAlarm = false;

		if (waterLevel <= lowLevelThreshold)
		{
			if (!lowLevelAlarm)
			{
				// Raises low water level alarm if not raised
				lowLevelAlarm = true;
				pumpControl.lowLevelInterrupt();
			}
		}
		else lowLevelAlarm = false;
	}
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
