#pragma once
#include "PhSensor.h"

class PhRegulator {
private:
	PH sensor;
	float presetPhMin;
	float presetPhMax;
	bool onOff;

public:
	void turnOnOff(bool switcher);

	void raiseAlert();

	void setPresetPhMin(float pH);
	float getPresetPhMin();

	void setPresetPhMax(float pH);
	float getPresetPhMax();
};