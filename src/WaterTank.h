#ifndef _WATERTANK_H_
#define _WATERTANK_H_

#include "Loggable.h"
#include "Runnable.h"
#include "Sleepable.h"

#include <mutex>

using namespace std;

class WaterTank: public Loggable, public Sleepable, public Runnable
{
private:

	double waterLevel;

	double maxIncrement;

	double direction;

	std::mutex tankMutex;

	void run();

public:

	WaterTank();
	~WaterTank();

	void increase();
	void decrease();

	double getWaterLevel();

	void changeWaterSpeed(double speed);

};

#endif
