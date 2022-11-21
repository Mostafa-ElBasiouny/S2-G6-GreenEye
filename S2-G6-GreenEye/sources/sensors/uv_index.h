#pragma once

#include <iostream>

class UVIndexSensor
{
protected:
    Sensors m_sensor = UVIndex;

    float m_value;
    std::pair<float, float> m_ranges;

public:
	UVIndexSensor(uint32_t read_interval);

    void GetRanges();
    void SetValue(float value);
    void SetRanges(float min, float max);

    virtual void Evaluate() = 0;
};

class UVLight : protected UVIndexSensor
{
    Status m_status;

private:
    void Switcher();
    void CreateEvent(Levels level, std::string message);

public:
    void Evaluate() override;
};