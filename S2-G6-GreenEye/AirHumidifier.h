#pragma once
#pragma once
#include "AirHumiditySensor.h"

class AirHumidifier {
private:
	AirHumidity sensor;
	float presetAH;
	bool onOff;

public:
	void turnOnOff(bool switcher);

	void raiseAlert();

	void setPresetAH(float AH);

	float getPresetAH();
};