#include "soil_fertility.h"

SoilFertility::SoilFertility(int read_interval, Database* database) :
	m_value(0),
	m_internal_clock(std::clock()),
	m_read_interval(read_interval),
	m_file_name("resources/sensors/SoilFertility.data"),
	m_database(database)
{
	m_file_stream.open(m_file_name, ios::in);
}

SoilFertility::~SoilFertility()
{
	m_file_stream.close();
}

void SoilFertility::Run()
{
	if (!m_file_stream.is_open()) exit(-1);
	else if (!m_file_stream.eof() && clock() - m_internal_clock > (m_read_interval * 1000))
	{
		string value_raw;
		getline(m_file_stream, value_raw);

		GetRanges();

		float value = 0.0f;

		if (value_raw != "") value = stof(value_raw);

		SetValue(value);

		m_internal_clock = clock();
	}

	if (m_file_stream.eof()) m_file_stream.seekg(0);
}

void SoilFertility::GetRanges()
{
	/*
		Read ranges from the database.
	*/

	float min;
	float max;

	m_database->Get(m_sensor, min, max);

	/* Update the sensor's ranges. */
	SetRanges(min, max);
}

void SoilFertility::SetValue(float value)
{
	m_value = value;

	Evaluate();

	m_database->Set(m_sensor, m_value);
}

void SoilFertility::SetRanges(float min, float max)
{
	m_ranges = make_pair(min, max);
}

void SoilFertility::CreateEvent(Record::Levels level, string message)
{
	m_database->Set(m_sensor, level, message);
}

void SoilFertility::Evaluate()
{
	if (m_value >= (85 / 100 * m_ranges.second))
	{
		if (m_value >= m_ranges.second)
		{
			CreateEvent(Record::Critical, "Soil fertility levels too high!");
			return;
		}

		CreateEvent(Record::Warning, "Soil fertility reaching high levels!");
	}

	if (m_value <= (15 / 100 * m_ranges.first))
	{
		if (m_value <= m_ranges.first)
		{
			CreateEvent(Record::Critical, "Soil fertility levels too low!");
			return;
		}

		CreateEvent(Record::Warning, "Soil fertility reaching low levels!");
	}
}
