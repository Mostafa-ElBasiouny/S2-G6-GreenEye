#include "UvIndexSensor.h"
#include <iostream>
#include <time.h>

#define MIN_UV 1
#define MAX_UV 11

void UvIndexSensor::setSensorRecord() {
	srand((unsigned)time(NULL));
	sensorRecord = rand() % MAX_UV;
}

float UvIndexSensor::getSensorRecord() {
	return sensorRecord;
}