#ifndef _WATERTANK_H_
#define _WATERTANK_H_

#include "Loggable.h"
#include "Runnable.h"
#include "Sleepable.h"

#include <chrono>
#include <mutex>

using namespace std;

class WaterTank: public Loggable, public Sleepable, public Runnable
{
private:

	double waterLevel;

	double maxIncrement;
	double direction;

	std::mutex tankMutex;

	std::chrono::steady_clock::time_point lastError;

	double highLevelThreshold;
	double lowLevelThreshold;

	bool highLevelAlarm;
	bool lowLevelAlarm;

	void run();

public:

	WaterTank();
	~WaterTank();

	void increase();
	void decrease();

	double getWaterLevel();

	void changeWaterSpeed(double speed);

	void getAlarms(bool& highLevelAlarm_, bool& lowLevelAlarm_);

	void setHighLevelThreshold(double threshold);
	void setLowLevelThreshold(double threshold);

};

#endif
