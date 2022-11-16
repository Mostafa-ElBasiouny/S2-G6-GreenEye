/*
 * database.h
 * 
 * God have mercy
 *
 * Mostafa Elbasiouny - Nov 16, 2022
 */

#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <vector>

struct Record {
	enum Status { Disabled, Enabled };

	enum Levels { Notice, Warning, Critical };

	enum Sensors {
		None,

		PH,
		CO2,
		UVIndex,
		Temperature,
		AirHumidity,
		Precipitation,
		SoilMoisture,
		SoilFertility
	};

	struct Event {
		std::string timestamp;
		Levels level;
		Sensors sensor;
		std::string message;
	};

private:
	std::vector<float> ph;
	std::vector<float> co2;
	std::vector<float> uv_index;
	std::vector<float> temperature;
	std::vector<float> air_humidity;
	std::vector<float> precipitation;
	std::vector<float> soil_moisture;
	std::vector<float> soil_fertility;

	std::vector<Event> events;

	Status ac;
	Status uv_light;
	Status humidifier;
	Status ph_regulator;
	Status water_sprinkler;

	std::pair<float, float> ph_range;
	std::pair<float, float> co2_range;
	std::pair<float, float> uv_index_range;
	std::pair<float, float> temperature_range;
	std::pair<float, float> air_humidity_range;
	std::pair<float, float> precipitation_range;
	std::pair<float, float> soil_moisture_range;
	std::pair<float, float> soil_fertility_range;

public:
	void Set(Sensors sensor, float value);
	void Set(Sensors sensor, Status status);
	void Set(Sensors sensor, float minimum_value, float maximum_value);
	void Set(Levels level, Sensors sensor, std::string message, std::string timestamp_string = "\0");

	void Get(Sensors sensor, std::vector<float>& out_values);
	void Get(Sensors sensor, Status& out_status);
	void Get(Sensors sensor, float& out_minimum_value, float& out_maximum_value);
	void Get(std::vector<Event>& out_events);
};

class Database : public Record {
	std::string m_file_name;
	std::fstream m_file_stream;
	std::vector<std::pair<std::string, std::string>> m_record_raw_data;
	Record m_record;

public:
	Database(std::string file_name);
	~Database();

private:
	void Read(Sensors sensor);
	void Write(Sensors sensor);
};
