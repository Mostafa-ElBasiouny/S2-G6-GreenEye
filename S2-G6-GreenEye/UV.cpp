#include "UV.h"
#include <ctime>
#include <sstream>
#include <fstream>


void UvSensor::GetRanges()
{
    /*
        Read ranges from the database.
    */

    float min;
    float max;

    /* DATABASE */ Get(m_sensor, min, max);

    /* Update the sensor's ranges. */
    SetRanges(min, max);
}

void UvSensor::SetValue(float value)
{
    m_value = value;

    Evaluate();

    /* DATABASE */ Set(m_sensor, m_value);
}

void UvSensor::SetRanges(float min, float max)
{
    m_ranges = std::make_pair(min, max);
}

void UvLight::Evaluate()
{
    if (m_value >= (85 / 100 * m_ranges.second))
    {
        if (m_value >= m_ranges.second)
        {
            Switcher();
            CreateEvent(Critical, "");
            return;
        }

        CreateEvent(Warning, "");
    }

    if (m_value <= (15 / 100 * m_ranges.first))
    {
        if (m_value <= m_ranges.first)
        {
            Switcher();
            CreateEvent(Critical, "");
            return;
        }

        CreateEvent(Warning, "");
    }
}

void UvLight::Switcher()
{
    /* DATABASE */ Get(m_sensor, m_status);

    m_status ? Enabled : Disabled;
}

void UvLight::CreateEvent(Levels level, std::string message)
{
    /* DATABASE */ Set(level, m_sensor, message);
}