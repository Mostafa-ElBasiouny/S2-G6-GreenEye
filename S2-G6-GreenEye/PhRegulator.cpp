#include "PhRegulator.h"
#include "database.h"
#include <time.h>

void PhRegulator::turnOnOff(bool switcher) {
	onOff = switcher;
}

void PhRegulator::raiseAlert() {
	
	if (sensor.getSensorRecord() > presetPH + 1) {
		database.Set(Record::Critical, Record::PH, "The pH level is too high!");
	}
	else if (sensor.getSensorRecord() < presetPH - 1) {
		database.Set(Record::Critical, Record::PH, "The pH level is too low!");
	}
	else if (sensor.getSensorRecord() > presetPH + 0.5) {
		database.Set(Record::Warning, Record::PH, "The pH level is a higher than the preset value!");
	}
	else if (sensor.getSensorRecord() < presetPH - 0.5) {
		database.Set(Record::Warning, Record::PH, "The pH level is a lower than the preset value!");
	}
	else if (sensor.getSensorRecord() > presetPH) {
		database.Set(Record::Notice, Record::PH, "The pH level is little higher than the preset value!");
	}
	else if (sensor.getSensorRecord() < presetPH) {
		database.Set(Record::Notice, Record::PH, "The pH level is little lower than the preset value!");
	}
}

void PhRegulator::setPresetPh(float pH) {
	presetPH = pH;
}

float PhRegulator::getPresetPh() {
	return presetPH;
}