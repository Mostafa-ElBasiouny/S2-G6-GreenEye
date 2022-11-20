#pragma once
#include <iostream>
#include "database.h"

class PhSensor
{
protected:
    Sensors m_sensor = PH;

    float m_value;
    std::pair<float, float> m_ranges;

public:
    PH(uint32_t read_interval);

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