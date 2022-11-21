#pragma once

#include <iostream>

class SoilFertilitySensor
{
protected:
    Sensors m_sensor = Soilfertility;

    float m_value;
    std::pair<float, float> m_ranges;

public:
    SoilFertilitySensor(uint32_t read_interval);

    void GetRanges();
    void SetValue(float value);
    void SetRanges(float min, float max);
    void CreateEvent(Levels level, std::string message);
    void Evaluate();
};