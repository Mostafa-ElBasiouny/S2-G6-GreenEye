#pragma once
#include "PhSensor.h"
#include "Event.h"

class PhRegulator {
private:
	PH sensor;
	float presetPH;
	bool onOff;

public:
	void turnOnOff(bool switcher);

	Event raiseAlert();

	void setPresetPh(float pH);

	float getPresetPh();
};