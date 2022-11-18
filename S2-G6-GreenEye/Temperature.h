#pragma once

#include <iostream>

#define DATABASE_STUBS

/* Database helper methods. */
#ifdef DATABASE_STUBS
#include <vector>

enum Status { Disabled, Enabled };

enum Levels { Notice, Warning, Critical };

enum Sensors {
    None,

    PH,
    CO2,
    UVIndex,
    Temperature,
    AirHumidity,
    Precipitation,
    SoilMoisture,
    SoilFertility
};

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

class TemperatureSens
{
protected:
    Sensors m_sensor = Temperature;

    float m_value;
    std::pair<float, float> m_ranges;

public:
    TemperatureSens(uint32_t read_interval);

    void GetRanges();
    void SetValue(float value);
    void SetRanges(float min, float max);

    virtual void Evaluate() = 0;
};

class AC : protected TemperatureSens
{
    Status m_status;

private:
    void Switcher();
    void CreateEvent(Levels level, std::string message);

public:
    void Evaluate() override;
};