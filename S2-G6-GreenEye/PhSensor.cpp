#include "PhSensor.h"
#include <iostream>
#include <time.h>

#define MIN_PH 0
#define MAX_PH 14

using namespace std;

void PH::setSensorRecord() {
	srand(time(0));
	sensorRecord = (float) (rand() % MAX_PH);
}

float PH::getSensorRecord() {
	return PH::sensorRecord;
}