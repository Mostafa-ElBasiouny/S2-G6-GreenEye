#include "PhRegulator.h"
#include <time.h>

void PhRegulator::turnOnOff(bool switcher) {
	onOff = switcher;
}

Event PhRegulator::raiseAlert() {
	
	if (sensor.getSensorRecord() > presetPH + 1) {
		Event event('A', "The pH level is too high!");
		return event;
	}
	else if (sensor.getSensorRecord() < presetPH - 1) {
		Event event('A', "The pH level is too low!");
		return event;
	}
	else if (sensor.getSensorRecord() > presetPH) {
		Event event('W', "The pH level is higher than the preset value!");
		return event;
	}
	else if (sensor.getSensorRecord() < presetPH) {
		Event event('W', "The pH level is lower than the preset value!");
		return event;
	}
}

void PhRegulator::setPresetPh(float pH) {
	presetPH = pH;
}

float PhRegulator::getPresetPh() {
	return presetPH;
}