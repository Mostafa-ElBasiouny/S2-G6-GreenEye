#pragma once
#include "PhSensor.h"

class PhRegulator {
private:
	PH sensor;
	float presetPH;
	bool onOff;

public:
	void turnOnOff(bool switcher);

	void raiseAlert();

	void setPresetPh(float pH);

	float getPresetPh();
};