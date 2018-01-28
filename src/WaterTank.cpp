#include "WaterTank.h"

#include "GlobalException.h"

#include <math.h>

WaterTank::WaterTank(): 
	Loggable("water_tank"), Sleepable(150), waterLevel(50.0), maxIncrement(0.08), direction(0.02)
{
}


WaterTank::~WaterTank()
{
}


void WaterTank::run()
{
	logger->info("Starting");

	bool deadlineMissed = false;
	double increment = 0.0;

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
			GlobalException::getInstance().setGlobalException();
		}
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
