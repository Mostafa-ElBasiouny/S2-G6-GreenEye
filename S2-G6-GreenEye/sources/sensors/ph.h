#pragma once
#include <iostream>

struct Event {
	std::string timestamp;
	Levels level;
	Sensors sensor;
	std::string message;
};

void Set(Sensors sensor, float value) {}
void Set(Sensors sensor, Status status) {}
void Set(Sensors sensor, float minimum_value, float maximum_value) {}
void Set(Levels level, Sensors sensor, std::string message, std::string timestamp_string = "\0") {}

void Get(Sensors sensor, std::vector<float>& out_values) {}
void Get(Sensors sensor, Status& out_status) {}
void Get(Sensors sensor, float& out_minimum_value, float& out_maximum_value) {}
void Get(std::vector<Event>& out_events) {}
#endif

class PhSensor
{
protected:
    Sensors m_sensor = PH;

    float m_value;
    std::pair<float, float> m_ranges;

public:
	PhSensor(uint32_t read_interval);

    void GetRanges();
    void SetValue(float value);
    void SetRanges(float min, float max);

    virtual void Evaluate() = 0;
};

class PhRegulator : protected PhSensor
{
    Status m_status;

private:
    void Switcher();
    void CreateEvent(Levels level, std::string message);

public:
    void Evaluate() override;
};