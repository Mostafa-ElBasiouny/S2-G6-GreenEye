#include "temperature.h"

Temperature::Temperature(int read_interval, Database* database) :
	m_value(0),
	m_internal_clock(std::clock()),
	m_read_interval(read_interval),
	m_file_name("resources/sensors/Temperature.data")
{
	m_database = database;
	m_file_stream.open(m_file_name, ios::in);
}

Temperature::~Temperature()
{
	m_file_stream.close();
}

void Temperature::Run()
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

void Temperature::GetRanges()
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

void Temperature::SetValue(float value)
{
	m_value = value;

	Evaluate();

	m_database->Set(m_sensor, m_value);
}

void Temperature::SetRanges(float min, float max)
{
	m_ranges = make_pair(min, max);
}

void Temperature::Evaluate()
{
	if (m_value >= (85 / 100 * m_ranges.second))
	{
		if (m_value >= m_ranges.second)
		{
			Switcher();
			CreateEvent(Record::Critical, "Temperature levels too high!");
			return;
		}

		CreateEvent(Record::Warning, "Temperature reaching high levels!");
	}

	if (m_value <= (15 / 100 * m_ranges.first))
	{
		if (m_value <= m_ranges.first)
		{
			Switcher();
			CreateEvent(Record::Critical, "Temperature levels too low!");
			return;
		}

		CreateEvent(Record::Warning, "Temperature reaching low levels!");
	}
}

void AC::Switcher()
{
	m_database->Get(m_sensor, m_status);

	m_status = (m_status == Record::Disabled ? Record::Enabled : Record::Disabled);

	m_database->Set(m_sensor, m_status);
}

void AC::CreateEvent(Record::Levels level, std::string message)
{
	m_database->Set(m_sensor, level, message);
}