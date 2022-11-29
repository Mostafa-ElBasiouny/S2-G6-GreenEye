#include "co2.h"

CO2::CO2(int read_interval, Database* database) :
	m_value(0),
	m_internal_clock(std::clock()),
	m_read_interval(read_interval),
	m_file_name("resources/sensors/CO2.data"),
	m_database(database)
{
	m_file_stream.open(m_file_name, ios::in);
}

CO2::~CO2()
{
	m_file_stream.close();
}

void CO2::Run()
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

void CO2::GetRanges()
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

void CO2::SetValue(float value)
{
	m_value = value;

	Evaluate();

	m_database->Set(m_sensor, m_value);
}

void CO2::SetRanges(float min, float max)
{
	m_ranges = make_pair(min, max);
}

void CO2::CreateEvent(Record::Levels level, string message)
{
	m_database->Set(m_sensor, level, message);
}

void CO2::Evaluate()
{
	if (m_value >= (m_ranges.second - (15 / 100 * (m_ranges.second - m_ranges.first))))
	{
		if (m_value > m_ranges.second)
		{
			CreateEvent(Record::Critical, u8"CO² levels too high!");
			return;
		}

		CreateEvent(Record::Warning, u8"CO² reaching high levels!");
	}

	if (m_value <= ((15 / 100 * (m_ranges.second - m_ranges.first)) + m_ranges.first))
	{
		if (m_value < m_ranges.first)
		{
			CreateEvent(Record::Critical, u8"CO² levels too low!");
			return;
		}

		CreateEvent(Record::Warning, u8"CO² reaching low levels!");
	}
}
