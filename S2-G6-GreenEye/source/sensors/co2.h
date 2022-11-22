#pragma once

#include "../database.h"

class CO2
{
	Record::Sensors m_sensor = Record::CO2;
	Database* m_database;

	float m_value;
	int m_read_interval;
	pair<float, float> m_ranges;
	string m_file_name;
	fstream m_file_stream;
	clock_t m_internal_clock;

public:
	CO2(int read_interval, Database* database);
	~CO2();

	void Run();
	void GetRanges();
	void SetValue(float value);
	void SetRanges(float min, float max);
	void CreateEvent(Record::Levels level, string message);
	void Evaluate();
};
