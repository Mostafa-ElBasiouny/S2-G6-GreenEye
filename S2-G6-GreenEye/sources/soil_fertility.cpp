#include "soil_fertility.h"

#include <ctime>
#include <sstream>
#include <fstream>

SoilFertilitySensor::SoilFertilitySensor(uint32_t time_interval) : m_value(0)
{
    float sensor_reading = 0.0f;
    std::string file_name = "SoilFertility.data";
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

void SoilFertilitySensor::GetRanges()
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

void SoilFertilitySensor::SetValue(float value)
{
    m_value = value;

    Evaluate();

    /* DATABASE */ Set(m_sensor, m_value);
}

void SoilFertilitySensor::SetRanges(float min, float max)
{
    m_ranges = std::make_pair(min, max);
}

void SoilFertilitySensor::Evaluate()
{
    if (m_value >= (85 / 100 * m_ranges.second))
    {
        if (m_value >= m_ranges.second)
        {
            CreateEvent(Critical, "DONT ADD FERTILIZER: Soil Fertility too high!");
            return;
        }

        CreateEvent(Warning, "Soil Fertility reaching high levels!");
    }

    if (m_value <= (15 / 100 * m_ranges.first))
    {
        if (m_value <= m_ranges.first)
        {
            CreateEvent(Critical, "ADD FERTILIZER: Soil Fertility too low!");
            return;
        }

        CreateEvent(Warning, "Soil Fertility reaching low levels!");
    }
}
void SoilFertilitySensor::CreateEvent(Levels level, std::string message)
{
    /* DATABASE */ Set(level, m_sensor, message);
}