#include "globals.h"

void WinMain()
{
	Application application(u8"Green Eye 🌱", g_database);

	INITIALIZE(PH, ph);
	INITIALIZE(CO2, co2);
	INITIALIZE(UVIndex, uv_index);
	INITIALIZE(Temperature, temperature);
	INITIALIZE(AirHumidity, air_humidity);
	INITIALIZE(SoilMoisture, soil_moisture);
	INITIALIZE(Precipitation, precipitation);
	INITIALIZE(SoilFertility, soil_fertility);

	while (!application.ShouldClose()) {
		application.Render();

		ph->Run();
		co2->Run();
		uv_index->Run();
		temperature->Run();
		air_humidity->Run();
		soil_moisture->Run();
		precipitation->Run();
		soil_fertility->Run();

		// ...

		Record::Status d_ph_regulator;
		g_database->Get(Record::PH, d_ph_regulator);
		auto a_ph_regulator = static_cast<Application::States::Status>(d_ph_regulator);
		application.states.SetPH(a_ph_regulator);

		Record::Status d_uv_light;
		g_database->Get(Record::UVIndex, d_uv_light);
		auto a_uv_light = static_cast<Application::States::Status>(d_uv_light);
		application.states.SetUVIndex(a_uv_light);

		Record::Status d_ac;
		g_database->Get(Record::Temperature, d_ac);
		auto a_ac = static_cast<Application::States::Status>(d_ac);
		application.states.SetTemperature(a_ac);

		Record::Status d_air_humidifier;
		g_database->Get(Record::AirHumidity, d_air_humidifier);
		auto a_air_humidifier = static_cast<Application::States::Status>(d_air_humidifier);
		application.states.SetAirHumidity(a_air_humidifier);

		Record::Status d_water_sprinkler;
		g_database->Get(Record::SoilMoisture, d_water_sprinkler);
		auto a_water_sprinkler = static_cast<Application::States::Status>(d_water_sprinkler);
		application.states.SetSoilMoisture(a_water_sprinkler);

		// ...

		a_ph_regulator = application.states.GetPH().second;
		d_ph_regulator = static_cast<Record::Status>(a_ph_regulator);
		g_database->Set(Record::PH, d_ph_regulator);

		a_uv_light = application.states.GetUVIndex().second;
		d_uv_light = static_cast<Record::Status>(a_uv_light);
		g_database->Set(Record::UVIndex, d_uv_light);

		a_ac = application.states.GetTemperature().second;
		d_ac = static_cast<Record::Status>(a_ac);
		g_database->Set(Record::Temperature, d_ac);

		a_air_humidifier = application.states.GetAirHumidity().second;
		d_air_humidifier = static_cast<Record::Status>(a_air_humidifier);
		g_database->Set(Record::AirHumidity, d_air_humidifier);

		a_water_sprinkler = application.states.GetSoilMoisture().second;
		d_water_sprinkler = static_cast<Record::Status>(a_water_sprinkler);
		g_database->Set(Record::SoilMoisture, d_water_sprinkler);

		// ...

		vector<float> ph;
		g_database->Get(Record::PH, ph);
		application.states.SetPH(ph);

		vector<float> co2;
		g_database->Get(Record::CO2, co2);
		application.states.SetCO2(co2);

		vector<float> uv_index;
		g_database->Get(Record::UVIndex, uv_index);
		application.states.SetUVIndex(uv_index);

		vector<float> temperature;
		g_database->Get(Record::Temperature, temperature);
		application.states.SetTemperature(temperature);

		vector<float> air_humidity;
		g_database->Get(Record::AirHumidity, air_humidity);
		application.states.SetAirHumidity(air_humidity);

		vector<float> precipitation;
		g_database->Get(Record::Precipitation, precipitation);
		application.states.SetPrecipitation(precipitation);

		vector<float> soil_moisture;
		g_database->Get(Record::SoilMoisture, soil_moisture);
		application.states.SetSoilMoisture(soil_moisture);

		vector<float> soil_fertility;
		g_database->Get(Record::SoilFertility, soil_fertility);
		application.states.SetSoilFertility(soil_fertility);

		// ...

		pair<float, float> ph_ranges;
		ph_ranges = application.states.GetPHRanges();
		g_database->Set(Record::PH, ph_ranges.first, ph_ranges.second);

		pair<float, float> co2_ranges;
		co2_ranges = application.states.GetCO2Ranges();
		g_database->Set(Record::CO2, co2_ranges.first, co2_ranges.second);

		pair<float, float> uv_index_ranges;
		uv_index_ranges = application.states.GetUVIndexRanges();
		g_database->Set(Record::UVIndex, uv_index_ranges.first, uv_index_ranges.second);

		pair<float, float> temperature_ranges;
		temperature_ranges = application.states.GetTemperatureRanges();
		g_database->Set(Record::Temperature, temperature_ranges.first, temperature_ranges.second);

		pair<float, float> air_humidity_ranges;
		air_humidity_ranges = application.states.GetAirHumidityRanges();
		g_database->Set(Record::AirHumidity, air_humidity_ranges.first, air_humidity_ranges.second);

		pair<float, float> precipitation_ranges;
		precipitation_ranges = application.states.GetPrecipitationRanges();
		g_database->Set(Record::Precipitation, precipitation_ranges.first, precipitation_ranges.second);

		pair<float, float> soil_moisture_ranges;
		soil_moisture_ranges = application.states.GetSoilMoistureRanges();
		g_database->Set(Record::SoilMoisture, soil_moisture_ranges.first, soil_moisture_ranges.second);

		pair<float, float> soil_fertility_ranges;
		soil_fertility_ranges = application.states.GetSoilFertilityRanges();
		g_database->Set(Record::SoilFertility, soil_fertility_ranges.first, soil_fertility_ranges.second);

		// ...

		vector<Record::Event> events;
		g_database->Get(events);

		if (events.size() > 0)
		{
			for (const auto& event : events)
			{
				Application::States::Event _;
				_.level = static_cast<Application::States::Levels>(event.level);
				_.sensor = static_cast<Application::States::Sensors>(event.sensor);
				_.message = event.message;
				_.timestamp = event.timestamp;

				application.states.SetEvent(_);
			}
		}

		g_database->ClearEvents();
	}

	application.Close();

	DELETE(ph);
	DELETE(co2);
	DELETE(uv_index);
	DELETE(temperature);
	DELETE(air_humidity);
	DELETE(soil_moisture);
	DELETE(precipitation);
	DELETE(soil_fertility);

	DELETE(g_database);
}
