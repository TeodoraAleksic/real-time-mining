#ifndef _SIMULATION_H_
#define _SIMULATION_H_

#include "EnvironmentMonitor.h"
#include "Loggable.h"
#include "PumpControl.h"
#include "Sensor.h"
#include "WaterLevelMonitor.h"
#include "WaterTank.h"

class Simulation: public Loggable, public Sleepable, public Runnable
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

	// EnvironmentMonitor
	void setSensorThreshold(EnvironmentMonitor::SensorID sensorID, double threshold);
	void getSensorData(EnvironmentMonitor::SensorID sensorID, double& value, bool& alarm);

	// PumpControl
	bool getPumpAlarm();
	bool isPumpOn();
	void turnPumpOn();
	void turnPumpOff();

	// WaterLevelMonitor
	void getWaterLevelAlarms(bool& highLevelAlarm_, bool& lowLevelAlarm_);
	void setHighLevelThreshold(double threshold);
	void setLowLevelThreshold(double threshold);

	// WaterTank
	double getWaterLevel();

};

#endif
