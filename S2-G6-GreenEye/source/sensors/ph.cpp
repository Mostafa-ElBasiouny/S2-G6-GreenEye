#include "ph.h"

PH::PH(int read_interval, Database* database) :
	m_value(0),
	m_internal_clock(std::clock()),
	m_read_interval(read_interval),
	m_file_name("resources/sensors/PH.data")
{
	m_database = database;
	m_file_stream.open(m_file_name, ios::in);
}

PH::~PH()
{
	m_file_stream.close();
}

void PH::Run()
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

void PH::GetRanges()
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

void PH::SetValue(float value)
{
	m_value = value;

	Evaluate();

	m_database->Set(m_sensor, m_value);
}

void PH::SetRanges(float min, float max)
{
	m_ranges = make_pair(min, max);
}

void PH::Evaluate()
{
	if (m_value >= (m_ranges.second - (15 / 100 * (m_ranges.second - m_ranges.first))))
	{
		if (m_value > m_ranges.second)
		{
			Switcher();
			CreateEvent(Record::Critical, "PH levels too high!");
			return;
		}

		CreateEvent(Record::Warning, "PH reaching high levels!");
	}

	if (m_value <= ((15 / 100 * (m_ranges.second - m_ranges.first)) + m_ranges.first))
	{
		if (m_value < m_ranges.first)
		{
			Switcher();
			CreateEvent(Record::Critical, "PH levels too low!");
			return;
		}

		CreateEvent(Record::Warning, "PH reaching low levels!");
	}
}

void PHRegulator::Switcher()
{
	m_database->Get(m_sensor, m_status);

	m_status = (m_status == Record::Disabled ? Record::Enabled : Record::Disabled);

	m_database->Set(m_sensor, m_status);

	CreateEvent(Record::Notice, m_status == Record::Enabled ? "PH regulator enabled by system." : "PH regulator disabled by system.");
}

void PHRegulator::CreateEvent(Record::Levels level, std::string message)
{
	m_database->Set(m_sensor, level, message);
}
