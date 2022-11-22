#pragma once

#include "../database.h"

class Precipitation
{
	Record::Sensors m_sensor = Record::Precipitation;
	Database* m_database;

	float m_value;
	int m_read_interval;
	pair<float, float> m_ranges;
	string m_file_name;
	fstream m_file_stream;
	clock_t m_internal_clock;

public:
	Precipitation(int read_interval, Database* database);
	~Precipitation();

	void Run();
	void GetRanges();
	void SetValue(float value);
	void SetRanges(float min, float max);
	void CreateEvent(Record::Levels level, string message);
	void Evaluate();
};
