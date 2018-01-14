#include "WaterTank.h"

#include "GlobalException.h"


WaterTank::WaterTank(): 
	Loggable("water_tank"), Sleepable(50), waterLevel(20.0), increment(0.0), maxIncrement(0.15), direction(0.05)
{
}


WaterTank::~WaterTank()
{
}


void WaterTank::run()
{
	logger->info("Starting");

	while (running)
	{
		computeNextTime();
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
			if (!GlobalException::getInstance().getGlobalException())
			{
				// Sets exception if tank is overflowing
				GlobalException::getInstance().setGlobalException();
				logger->error("Water tank overflowing");
			}
		}
	}

	logger->info("Stopping");
}


void WaterTank::increase()
{
	std::lock_guard<std::mutex> guard(tankMutex);
	
	if (direction < 0) // Change water direction
	{
		logger->info("Increasing water level");
		direction = 0.05; // TODO introduce error
	}
}


void WaterTank::decrease() 
{
	std::lock_guard<std::mutex> guard(tankMutex);
	
	if (direction > 0) // Change water direction
	{
		logger->info("Decreasing water level");
		direction = -0.05; // TODO introduce error
	}
}


double WaterTank::getWaterLevel()
{
	std::lock_guard<std::mutex> guard(tankMutex);
	return waterLevel;
}
