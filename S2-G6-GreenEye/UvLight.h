#pragma once

#include "UvIndexSensor.h"

class UvLight {
private:
	UvIndexSensor sensor;
	float presetUvMin;
	float presetUvMax;
	bool onOff;

public:
	void turnOnOff(bool switcher);

	void raiseAlert();

	void setPresetUvMin(float pH);
	float getPresetUvMin();

	void setPresetUvMax(float pH);
	float getPresetUvMax();
};