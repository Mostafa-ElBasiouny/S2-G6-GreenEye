#pragma once
#include <iostream> 
#include <vector>
#include <string>

using namespace std;

static class Database
{
	struct Record {
		enum Status {
			Disabled,
			Enabled
		};

		enum Levels {
			Notice,
			Warning,
			Critical
		};

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
			time_t timestamp;
			Levels level;
			Sensors sensor;
			const char* message;
		};

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
	};
	 

public:
	Database(string filename);
	bool writeToFile(Record data);
	Record readFile();

	~Database();
};
