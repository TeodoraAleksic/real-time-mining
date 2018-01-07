#ifndef _WATERTANK_H_
#define _WATERTANK_H_

#include "Runnable.h"
#include "Sleepable.h"

#include <mutex>

using namespace std;

class WaterTank: public Runnable, public Sleepable
{
private:

	double waterLevel;

	double increment;
	const double maxIncrement;

	double direction;

	std::mutex tankMutex;

	void run();

public:

	WaterTank();
	~WaterTank();

	void increase();
	void decrease();

	double getWaterLevel();

};

#endif
