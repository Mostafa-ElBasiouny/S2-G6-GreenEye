#include "PhSensor.h"
#include <iostream>
#include <time.h>

#define MIN_PH 0;
#define MAX_PH 14;

void PH::setSensorRecord() {
	srand((unsigned)time(NULL));
	PH::sensorRecord = rand() % MAX_PH;
}

float PH::getSensorRecord() {
	return PH::sensorRecord;
}