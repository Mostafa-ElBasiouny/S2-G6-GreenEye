#pragma once

#include <iostream>

class TemperatureSensor
{
protected:
    Sensors m_sensor = Temperature;

    float m_value;
    std::pair<float, float> m_ranges;

public:
    TemperatureSensor(uint32_t read_interval);

    void GetRanges();
    void SetValue(float value);
    void SetRanges(float min, float max);

    virtual void Evaluate() = 0;
};

class AC : protected TemperatureSensor
{
    Status m_status;

private:
    void Switcher();
    void CreateEvent(Levels level, std::string message);

public:
    void Evaluate() override;
};
