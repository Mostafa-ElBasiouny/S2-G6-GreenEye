#include "soilmoisture.h"

#include <ctime>
#include <sstream>
#include <fstream>

SoilMoisture::SoilMoisture(uint32_t time_interval) : m_value(0)
{
    float sensor_reading = 0.0f;
    std::string file_name = "SoilMoisture.data";
    std::ifstream file_stream;

    file_stream.open(file_name);

    if (!file_stream.is_open()) std::exit(-1);

loop:
    while (!file_stream.eof())
    {
        std::string value_raw;
        std::getline(file_stream, value_raw);

        sensor_reading = std::stof(value_raw);

        GetRanges();
        SetValue(sensor_reading);

        auto time_now = std::clock();

        while (std::clock() - time_now < time_interval * 1000) {}
    }

    goto loop;
}

void SoilMoisture::GetRanges()
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

void SoilMoisture::SetValue(float value)
{
    m_value = value;

    Evaluate();

    /* DATABASE */ Set(m_sensor, m_value);
}

void SoilMoisture::SetRanges(float min, float max)
{
    m_ranges = std::make_pair(min, max);
}

void WaterSprinkler::Evaluate()
{
    if (m_value >= (85 / 100 * m_ranges.second))
    {
        if (m_value >= m_ranges.second)
        {
            Switcher();
            CreateEvent(Critical, "TURNING OFF WATER SPRINKLER: Soil Moisture too high!");
            return;
        }

        CreateEvent(Warning, "Reaching high levels of Soil Moisture!");
    }

    if (m_value <= (15 / 100 * m_ranges.first))
    {
        if (m_value <= m_ranges.first)
        {
            Switcher();
            CreateEvent(Critical, "TURNING ON WATER SPRINKLER: Soil Moisture too low!");
            return;
        }

        CreateEvent(Warning, "Reaching low levels of Soil Moisture!");
    }
}

void WaterSprinkler::Switcher()
{
    /* DATABASE */ Get(m_sensor, m_status);

    m_status ? Enabled : Disabled;
}

void WaterSprinkler::CreateEvent(Levels level, std::string message)
{
    /* DATABASE */ Set(level, m_sensor, message);
}