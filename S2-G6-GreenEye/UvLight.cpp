#include "UvLight.h"
#include "database.h"
#include <time.h>

void UvLight::turnOnOff(bool switcher) {
	onOff = switcher;
}

void UvLight::raiseAlert() {
	
	if (sensor.getSensorRecord() > presetUvMax + 1) {
		database.Set(Record::Critical, Record::UV, "The pH level is too high!");
	}
	else if (sensor.getSensorRecord() < presetUvMin - 1) {
		database.Set(Record::Critical, Record::UV, "The pH level is too low!");
	}
	else if (sensor.getSensorRecord() > presetUvMax + 0.5) {
		database.Set(Record::Warning, Record::UV, "The pH level is a higher than the preset value!");
	}
	else if (sensor.getSensorRecord() < presetUvMin - 0.5) {
		database.Set(Record::Warning, Record::UV, "The pH level is a lower than the preset value!");
	}
	else if (sensor.getSensorRecord() > presetUvMax) {
		database.Set(Record::Notice, Record::UV, "The pH level is little higher than the preset value!");
	}
	else if (sensor.getSensorRecord() < presetUvMin) {
		database.Set(Record::Notice, Record::UV, "The pH level is little lower than the preset value!");
	}
}

void UvLight::setPresetUvMin(float UV) {
	presetUvMin = UV;
}

float UvLight::getPresetUvMin() {
	return presetUvMin;
}

void UvLight::setPresetUvMax(float UV) {
	presetUvMax = UV;
}

float UvLight::getPresetUvMax() {
	return presetUvMax;
}