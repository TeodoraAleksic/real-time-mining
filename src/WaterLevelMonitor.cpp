#include "WaterLevelMonitor.h"

#include "GlobalEvent.h"
#include "PumpControl.h"
#include "WaterTank.h"


WaterLevelMonitor::WaterLevelMonitor(PumpControl& pumpControl_, WaterTank& waterTank_):
	Loggable("water_level"), pumpControl(pumpControl_), waterTank(waterTank_)
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
		GlobalEvent::getInstance().waitWaterLevel();

		bool highLevelAlarm;
		bool lowLevelAlarm;

		// Checks which water level interrupt happened
		waterTank.getAlarms(highLevelAlarm, lowLevelAlarm);

		if (highLevelAlarm)
			pumpControl.turnOn();

		if (lowLevelAlarm)
			pumpControl.turnOff();
	}

	logger->info("Stopping");
}


void WaterLevelMonitor::stop()
{
	running = false;
	GlobalEvent::getInstance().signalWaterLevel();
	if (thread.joinable()) thread.join();
}
