#include "AirHumidifier.h"
#include "database.h"
#include <time.h>


void AirHumidifier::turnOnOff(bool switcher) {
	onOff = switcher;
}

void AirHumidifier::raiseAlert() {

	if (sensor.getSensorRecord() >= presetAH ) {
		database.Set(Record::Critical, Record::AirHumidity, "The Air Humidity level is too high!");
	}
	
	else if (sensor.getSensorRecord() >= presetAH + 10) {
		database.Set(Record::Warning, Record::AirHumidity, "The Air Humidity level is higher than the average");
	}

	else if (sensor.getSensorRecord() < presetAH) {
		database.Set(Record::Notice, Record::AirHumidity, "The Air Humidity level is little lower than the preset value!");
	}
}

void AirHumidifier::setPresetAH(float AH) {
	presetAH = AH;
}

float AirHumidifier::getPresetAH() {
	return presetAH;
}