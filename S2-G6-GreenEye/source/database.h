/*
 * database.h
 *
 * God have mercy
 *
 * Mostafa Elbasiouny - Nov 16, 2022
 */

#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct Record
{
	enum Status { Disabled, Enabled };

	enum Levels { Notice, Warning, Critical };

	enum Sensors { None, PH, CO2, UVIndex, Temperature, AirHumidity, Precipitation, SoilMoisture, SoilFertility };

	struct Event { Sensors sensor; Levels level; string message; string timestamp; };

protected:
	vector<float> ph;
	vector<float> co2;
	vector<float> uv_index;
	vector<float> temperature;
	vector<float> air_humidity;
	vector<float> precipitation;
	vector<float> soil_moisture;
	vector<float> soil_fertility;

	vector<Event> events;

	Status ac;
	Status uv_light;
	Status humidifier;
	Status ph_regulator;
	Status water_sprinkler;

	pair<float, float> ph_range;
	pair<float, float> co2_range;
	pair<float, float> uv_index_range;
	pair<float, float> temperature_range;
	pair<float, float> air_humidity_range;
	pair<float, float> precipitation_range;
	pair<float, float> soil_moisture_range;
	pair<float, float> soil_fertility_range;

	int record_size = 100;

public:
	void Set(Sensors sensor, float value);
	void Set(Sensors sensor, Status status);
	void Set(Sensors sensor, float minimum_value, float maximum_value);
	void Set(Sensors sensor, Levels level, string message, string timestamp_string = "\0");

	void Get(Sensors sensor, vector<float>& out_values);
	void Get(Sensors sensor, Status& out_status);
	void Get(Sensors sensor, float& out_minimum_value, float& out_maximum_value);
	void Get(vector<Event>& out_events);

	void ClearEvents();
};

class Database : public Record
{
	string m_file_name;
	fstream m_file_stream;
	vector<pair<string, string>> m_record_raw_data;

	void Read(Sensors sensor);
	void Write(Sensors sensor);

public:
	Database(string file_name);
	~Database();
};
