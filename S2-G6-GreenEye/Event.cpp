#include "Event.h"

void Event::setEventType(char eventT) {
	eventType = eventT;
}
char Event::getEventType() {
	return eventType;
}

void Event::setEventMessage(std::string eventM) {
	eventMessage = eventM;
}
std::string Event::getEventMessage() {
	return eventMessage;
}

/*void Event::setEventTime(float time) {
	eventTime = time;
}
float Event::getEventTime() {
	return eventTime;
}*/

Event::Event() {
	setEventType('V');
	setEventMessage("No Message Here");
	//setEventTime(12.00);
}

Event::Event(char eventType, std::string eventMessage) {
	setEventType(eventType);
	setEventMessage(eventMessage);
	//setEventTime(alertTime);
}