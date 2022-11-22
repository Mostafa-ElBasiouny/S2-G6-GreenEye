#pragma once

#include "../database.h"

class PHRegulator
{
protected:
	Record::Status m_status;
	Record::Sensors m_sensor = Record::PH;
	Database* m_database;

	void Switcher();
	void CreateEvent(Record::Levels level, std::string message);

public:
	virtual void Evaluate() = 0;
};

class PH : protected PHRegulator
{
protected:
	float m_value;
	int m_read_interval;
	pair<float, float> m_ranges;
	string m_file_name;
	fstream m_file_stream;
	clock_t m_internal_clock;

public:
	PH(int read_interval, Database* database);
	~PH();

	void Run();
	void GetRanges();
	void SetValue(float value);
	void SetRanges(float min, float max);
	void Evaluate() override;
};
