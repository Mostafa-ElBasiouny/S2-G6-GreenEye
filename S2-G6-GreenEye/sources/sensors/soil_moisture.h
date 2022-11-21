#pragma once

#include <iostream>

class SoilMoistureSensor
{
protected:
    Sensors m_sensor = Soilmoisture;

    float m_value;
    std::pair<float, float> m_ranges;

public:
    SoilMoistureSensor(uint32_t read_interval);

    void GetRanges();
    void SetValue(float value);
    void SetRanges(float min, float max);

    virtual void Evaluate() = 0;
};

class WaterSprinkler : protected SoilMoistureSensor
{
    Status m_status;

private:
    void Switcher();
    void CreateEvent(Levels level, std::string message);

public:
    void Evaluate() override;
};