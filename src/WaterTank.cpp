#include "WaterTank.h"

#include "GlobalEvent.h"

#include <math.h>


WaterTank::WaterTank():
	Loggable("water_tank"), Sleepable(150), waterLevel(50.0), maxIncrement(0.08), direction(0.02),
	highLevelThreshold(80.0), lowLevelThreshold(20.0), highLevelAlarm(false), lowLevelAlarm(false)
{
}


WaterTank::~WaterTank()
{
}


void WaterTank::run()
{
	logger->info("Starting");

	double increment = 0.0;

	while (running)
	{
		computeNextTime();

		// Checks if thread missed deadline
		if (std::chrono::steady_clock::now() > sleepUntil)
		{
			logger->error("Missed deadline");
			GlobalEvent::getInstance().setGlobalException();
			break;
		}

		std::this_thread::sleep_until(sleepUntil);

		std::lock_guard<std::mutex> guard(tankMutex);

		if ((direction > 0 && increment < maxIncrement) || (direction < 0 && increment > (-1) * maxIncrement))
			// Increases the water level increment to max speed
			increment += direction;

		if (waterLevel + increment >= 0 && waterLevel + increment <= 100)
		{
			// Changes the water level
			waterLevel += increment;
		}
		else if (waterLevel + increment > 100)
		{
			// Sets global exception if tank is overflowing
			logger->error("Water tank overflowing");
			GlobalEvent::getInstance().setGlobalException();
		}

		// Checks if water level is too high
		if (waterLevel >= highLevelThreshold)
		{
			if (!highLevelAlarm)
				logger->warn("High water level");

			// Raises high water level alarm
			highLevelAlarm = true;
			GlobalEvent::getInstance().signalWaterLevel();
		}
		else highLevelAlarm = false;

		// Checks if water level is too low
		if (waterLevel <= lowLevelThreshold)
		{
			if (!lowLevelAlarm)
				logger->warn("Low water level");

			// Raises low water level alarm
			lowLevelAlarm = true;
			GlobalEvent::getInstance().signalWaterLevel();
		}
		else lowLevelAlarm = false;
	}

	logger->info("Stopping");
}


void WaterTank::increase()
{
	std::lock_guard<std::mutex> guard(tankMutex);
	
	if (direction < 0) // Changes water direction
	{
		logger->info("Increasing water level");

		// Introduces new error after at least 5 seconds
		if (std::chrono::steady_clock::now() - lastError > std::chrono::seconds(5) && (int)waterLevel % 3 == 0)
			// Introduces error
			lastError = std::chrono::steady_clock::now();
		else
			direction = direction * -1;
	}
}


void WaterTank::decrease() 
{
	std::lock_guard<std::mutex> guard(tankMutex);
	
	if (direction > 0) // Changes water direction
	{
		logger->info("Decreasing water level");

		// Introduces new error after at least 5 seconds
		if (std::chrono::steady_clock::now() - lastError > std::chrono::seconds(5) && (int)waterLevel % 3 == 0)
			// Introduces error
			lastError = std::chrono::steady_clock::now();
		else
			// Doesn't introduce error
			direction = direction * -1;
	}
}


double WaterTank::getWaterLevel()
{
	std::lock_guard<std::mutex> guard(tankMutex);
	return waterLevel;
}


void WaterTank::changeWaterSpeed(double increment)
{
	std::lock_guard<std::mutex> guard(tankMutex);

	if (increment < 0.0 || increment >= 3.6)
		logger->error(fmt::format("Invalid water speed {}", increment));
	else
	{
		maxIncrement = increment;

		// Calculate direction as a quarter of the max increment
		if (direction > 0)
			direction = std::ceil(increment / 4);
		else
			direction = std::ceil(increment / 4) * -1;
	}
}


void WaterTank::getAlarms(bool& highLevelAlarm_, bool& lowLevelAlarm_)
{
	std::lock_guard<std::mutex> guard(tankMutex);
	highLevelAlarm_ = highLevelAlarm;
	lowLevelAlarm_ = lowLevelAlarm;
}


void WaterTank::setHighLevelThreshold(double threshold)
{
	std::lock_guard<std::mutex> guard(tankMutex);

	if (threshold > 0 && threshold < 100)
		if (threshold > lowLevelThreshold)
			highLevelThreshold = threshold;
		else
			logger->error("Threshold {} lower than low level threshold {}", threshold, lowLevelThreshold);
	else
		logger->error("Invalid threshold value {}", threshold);
}


void WaterTank::setLowLevelThreshold(double threshold)
{
	std::lock_guard<std::mutex> guard(tankMutex);

	if (threshold > 0 && threshold < 100)
		if (threshold < highLevelThreshold)
			lowLevelThreshold = threshold;
		else
			logger->error("Threshold {} higher than high level threshold {}", threshold, highLevelThreshold);
	else
		logger->error("Invalid threshold {}", threshold);
}
