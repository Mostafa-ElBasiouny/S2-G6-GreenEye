#pragma once
#include <string> //sorry Mostafa

class Event {
private:
	char eventType; // A char is being used for passing the eventType. W - Warning; A - Alert; M - Message; 
	std::string eventMessage;
	//float eventTime;

public:
	void setEventType(char eventT);
	char getEventType();

	void setEventMessage(std::string eventM);
	std::string getEventMessage();

	/*void setEventTime(float time);
	float getEventTime();*/

	Event();
	Event(char eventType, std::string eventMessage);
};