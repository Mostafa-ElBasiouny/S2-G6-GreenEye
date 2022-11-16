#include "AirHumiditySensor.h"
#include <iostream>
#include <time.h>

#define MIN_AH 65;
#define MAX_AH 85;

void AirHumidity::setSensorRecord() {
	srand((unsigned)time(NULL));
	AirHumidity::sensorRecord = rand() % MAX_AH;
}

float AirHumidity::getSensorRecord() {
	return AirHumidity::sensorRecord;
}