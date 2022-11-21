#pragma once

#include <iostream>

class PrecipitationSensor
{
protected:
    Sensors m_sensor = Precipitation;

    float m_value;
    std::pair<float, float> m_ranges;

public:
    PrecipitationSensor(uint32_t read_interval);

    void GetRanges();
    void SetValue(float value);
    void SetRanges(float min, float max);
    void CreateEvent(Levels level, std::string message);
    void Evaluate();
};