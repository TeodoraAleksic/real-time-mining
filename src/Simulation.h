#ifndef _SIMULATION_H_
#define _SIMULATION_H_

#include "EnvironmentMonitor.h"
#include "PumpControl.h"
#include "Sensor.h"
#include "WaterLevelMonitor.h"
#include "WaterTank.h"

class Simulation: public Sleepable, Runnable
{
private:

	EnvironmentMonitor em;
	WaterTank wt;
	PumpControl pc;
	WaterLevelMonitor wlm;

	void run();

public:

	Simulation();
	~Simulation();

};

#endif
