#include "WaterTank.h"


WaterTank::WaterTank(): Sleepable(50), waterLevel(75.0), increment(0.0), maxIncrement(0.15), direction(0.05)
{
}


WaterTank::~WaterTank()
{
}


void WaterTank::run()
{
	while (running)
	{
		computeNextTime();
		std::this_thread::sleep_until(sleepUntil);

		std::lock_guard<std::mutex> guard(tankMutex);

		if ((direction > 0 && increment < maxIncrement) || (direction < 0 && increment > (-1) * maxIncrement))
			// Speeds up the water level change to max speed
			increment += direction;

		// Changes the water level
		waterLevel += increment;
	}
}


void WaterTank::increase()
{
	std::lock_guard<std::mutex> guard(tankMutex);
	
	if (direction < 0) // Change water direction
	{
		// TODO introduce error
		direction = 0.05;
	}
}


void WaterTank::decrease() 
{
	std::lock_guard<std::mutex> guard(tankMutex);
	
	if (direction > 0) // Change water direction
	{
		// TODO introduce error
		direction = -0.05;
	}
}


double WaterTank::getWaterLevel()
{
	std::lock_guard<std::mutex> guard(tankMutex);
	return waterLevel;
}
