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

	double increment;
	const double maxIncrement;

	double direction;

	std::mutex tankMutex;

	void run();

public:

	WaterTank(std::shared_ptr<spdlog::sinks::simple_file_sink_mt> sink);
	~WaterTank();

	void increase();
	void decrease();

	double getWaterLevel();

};

#endif
