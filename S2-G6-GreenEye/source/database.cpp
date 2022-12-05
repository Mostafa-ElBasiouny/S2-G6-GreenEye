/*
 * database.cpp
 *
 * God have mercy
 *
 * Mostafa Elbasiouny - Nov 16, 2022
 */

#include "database.h"

inline void VectorAppend(std::vector<float>* vector, float value)
{
	vector->push_back(value);
	vector->erase(vector->begin());
}

void Record::Set(Sensors sensor, float value)
{
	switch (sensor)
	{
	case PH: VectorAppend(&ph, value); break;
	case CO2: VectorAppend(&co2, value); break;
	case UVIndex: VectorAppend(&uv_index, value); break;
	case Temperature: VectorAppend(&temperature, value); break;
	case AirHumidity: VectorAppend(&air_humidity, value); break;
	case Air_Precipitation: VectorAppend(&air_precipitation, value); break;
	case SoilMoisture: VectorAppend(&soil_moisture, value); break;
	case SoilFertility: VectorAppend(&soil_fertility, value); break;
	}
}

void Record::Set(Sensors sensor, Status status)
{
	switch (sensor)
	{
	case PH: ph_regulator = status; break;
	case UVIndex: uv_light = status; break;
	case Temperature: ac = status; break;
	case AirHumidity: humidifier = status; break;
	case SoilMoisture: water_sprinkler = status; break;
	}
}

void Record::Set(Sensors sensor, float minimum_value, float maximum_value)
{
	switch (sensor)
	{
	case PH: ph_range = make_pair(minimum_value, maximum_value); break;
	case CO2: co2_range = make_pair(minimum_value, maximum_value); break;
	case UVIndex: uv_index_range = make_pair(minimum_value, maximum_value); break;
	case Temperature: temperature_range = make_pair(minimum_value, maximum_value); break;
	case AirHumidity: air_humidity_range = make_pair(minimum_value, maximum_value); break;
	case Air_Precipitation: precipitation_range = make_pair(minimum_value, maximum_value); break;
	case SoilMoisture: soil_moisture_range = make_pair(minimum_value, maximum_value); break;
	case SoilFertility: soil_fertility_range = make_pair(minimum_value, maximum_value); break;
	}
}

void Record::Set(Sensors sensor, Levels level, string message, string timestamp_string)
{
	if (timestamp_string.empty())
	{
		time_t timestamp = time(0);
		timestamp_string = ctime(&timestamp);
		timestamp_string[strlen(timestamp_string.data()) - 1] = ',';
	}

	events.push_back({ sensor, level, message, timestamp_string });
}

void Record::Get(Sensors sensor, vector<float>& out_values)
{
	switch (sensor)
	{
	case PH: out_values = ph; break;
	case CO2: out_values = co2; break;
	case UVIndex: out_values = uv_index; break;
	case Temperature: out_values = temperature; break;
	case AirHumidity: out_values = air_humidity; break;
	case Air_Precipitation: out_values = air_precipitation; break;
	case SoilMoisture: out_values = soil_moisture; break;
	case SoilFertility: out_values = soil_fertility; break;
	}
}

void Record::Get(Sensors sensor, Status& out_status)
{
	switch (sensor)
	{
	case PH: out_status = ph_regulator; break;
	case UVIndex: out_status = uv_light; break;
	case Temperature: out_status = ac; break;
	case AirHumidity: out_status = humidifier; break;
	case SoilMoisture: out_status = water_sprinkler; break;
	}
}

void Record::Get(Sensors sensor, float& out_minimum_value, float& out_maximum_value)
{
	switch (sensor)
	{
	case PH: out_minimum_value = ph_range.first; out_maximum_value = ph_range.second; break;
	case CO2: out_minimum_value = co2_range.first; out_maximum_value = co2_range.second; break;
	case UVIndex: out_minimum_value = uv_index_range.first; out_maximum_value = uv_index_range.second; break;
	case Temperature: out_minimum_value = temperature_range.first; out_maximum_value = temperature_range.second; break;
	case AirHumidity: out_minimum_value = air_humidity_range.first; out_maximum_value = air_humidity_range.second; break;
	case Air_Precipitation: out_minimum_value = precipitation_range.first; out_maximum_value = precipitation_range.second; break;
	case SoilMoisture: out_minimum_value = soil_moisture_range.first; out_maximum_value = soil_moisture_range.second; break;
	case SoilFertility: out_minimum_value = soil_fertility_range.first; out_maximum_value = soil_fertility_range.second; break;
	}
}

void Record::Get(vector<Event>& out_events)
{
	out_events = events;
}

void Record::ClearEvents()
{
	this->events.clear();
}

inline int ToEnum(string value_raw)
{
	return stoi(value_raw);
}

inline pair<float, float> ToPair(string values_raw)
{
	pair<float, float> values;
	stringstream values_raw_stream(values_raw);

	getline(values_raw_stream, values_raw, ',');
	values.first = stof(values_raw);

	getline(values_raw_stream, values_raw, ',');
	values.second = stof(values_raw);

	return values;
}

inline vector<float> ToVector(string values_raw)
{
	vector<float> values;
	stringstream values_raw_stream(values_raw);

	while (getline(values_raw_stream, values_raw, ','))
		values.push_back(stof(values_raw));

	return values;
}

void Database::Read(Sensors sensor)
{
	Record::Status status;
	pair<float, float> ranges;
	vector<float> values;

	switch (sensor)
	{
	case Record::PH:
		for (const auto& data : m_record_raw_data)
		{
			if (data.first == "ph_regulator") {
				status = static_cast<Record::Status>(ToEnum(data.second));
				Set(Record::PH, status);
			}

			if (data.first == "ph_range") {
				ranges = ToPair(data.second);
				Set(Record::PH, ranges.first, ranges.second);
			}

			if (data.first == "ph") {
				values = ToVector(data.second);
				for (auto& value : values) Set(Record::PH, value);
			}
		}
		break;
	case Record::CO2:
		for (const auto& data : m_record_raw_data)
		{
			if (data.first == "co2_range") {
				ranges = ToPair(data.second);
				Set(Record::CO2, ranges.first, ranges.second);
			}

			if (data.first == "co2") {
				values = ToVector(data.second);
				for (auto& value : values) Set(Record::CO2, value);
			}
		}
		break;
	case Record::UVIndex:
		for (const auto& data : m_record_raw_data)
		{
			if (data.first == "uv_light") {
				status = static_cast<Record::Status>(ToEnum(data.second));
				Set(Record::UVIndex, status);
			}

			if (data.first == "uv_index_range") {
				ranges = ToPair(data.second);
				Set(Record::UVIndex, ranges.first, ranges.second);
			}

			if (data.first == "uv_index") {
				values = ToVector(data.second);
				for (auto& value : values) Set(Record::UVIndex, value);
			}
		}
		break;
	case Record::Temperature:
		for (const auto& data : m_record_raw_data)
		{
			if (data.first == "ac") {
				status = static_cast<Record::Status>(ToEnum(data.second));
				Set(Record::Temperature, status);
			}

			if (data.first == "temperature_range") {
				ranges = ToPair(data.second);
				Set(Record::Temperature, ranges.first, ranges.second);
			}

			if (data.first == "temperature") {
				values = ToVector(data.second);
				for (auto& value : values) Set(Record::Temperature, value);
			}
		}
		break;
	case Record::AirHumidity:
		for (const auto& data : m_record_raw_data)
		{
			if (data.first == "air_humidifier") {
				status = static_cast<Record::Status>(ToEnum(data.second));
				Set(Record::AirHumidity, status);
			}

			if (data.first == "air_humidity_range") {
				ranges = ToPair(data.second);
				Set(Record::AirHumidity, ranges.first, ranges.second);
			}

			if (data.first == "air_humidity") {
				values = ToVector(data.second);
				for (auto& value : values) Set(Record::AirHumidity, value);
			}
		}
		break;
	case Record::Air_Precipitation:
		for (const auto& data : m_record_raw_data)
		{
			if (data.first == "precipitation_range") {
				ranges = ToPair(data.second);
				Set(Record::Air_Precipitation, ranges.first, ranges.second);
			}

			if (data.first == "air_precipitation") {
				values = ToVector(data.second);
				for (auto& value : values) Set(Record::Air_Precipitation, value);
			}
		}
		break;
	case Record::SoilMoisture:
		for (const auto& data : m_record_raw_data)
		{
			if (data.first == "water_sprinkler") {
				status = static_cast<Record::Status>(ToEnum(data.second));
				Set(Record::SoilMoisture, status);
			}

			if (data.first == "soil_moisture_range") {
				ranges = ToPair(data.second);
				Set(Record::SoilMoisture, ranges.first, ranges.second);
			}

			if (data.first == "soil_moisture") {
				values = ToVector(data.second);
				for (auto& value : values) Set(Record::SoilMoisture, value);
			}
		}
		break;
	case Record::SoilFertility:
		for (const auto& data : m_record_raw_data)
		{
			if (data.first == "soil_fertility_range") {
				ranges = ToPair(data.second);
				Set(Record::SoilFertility, ranges.first, ranges.second);
			}

			if (data.first == "soil_fertility") {
				values = ToVector(data.second);
				for (auto& value : values) Set(Record::SoilFertility, value);
			}
		}
		break;
	default:
		for (const auto& data : m_record_raw_data)
		{
			if (data.first == "events") {
				Record::Event event;
				string value;
				stringstream value_raw(data.second);

				getline(value_raw, value, ',');

				while (!value.empty())
				{
					event.sensor = static_cast<Record::Sensors>(ToEnum(value));

					getline(value_raw, value, ',');
					event.level = static_cast<Record::Levels>(ToEnum(value));

					getline(value_raw, value, ',');
					event.message = value;

					getline(value_raw, value, ',');
					event.timestamp = value;

					getline(value_raw, value, ',');

					Set(event.sensor, event.level, event.message, event.timestamp);
				}
			}
		}
		break;
	}
}

void Database::Write(Sensors sensor)
{
	Record::Status status;
	float minimum_value;
	float maximum_value;
	vector<float> values;

	switch (sensor)
	{
	case Record::PH:
		Get(Record::PH, status);
		Get(Record::PH, minimum_value, maximum_value);
		Get(Record::PH, values);
		m_file_stream << "ph_regulator=" << status << "," << "\n";
		m_file_stream << "ph_range=" << minimum_value << "," << maximum_value << "," << "\n";
		m_file_stream << "ph=";
		break;
	case Record::CO2:
		Get(Record::CO2, minimum_value, maximum_value);
		Get(Record::CO2, values);
		m_file_stream << "co2_range=" << minimum_value << "," << maximum_value << "," << "\n";
		m_file_stream << "co2=";
		break;
	case Record::UVIndex:
		Get(Record::UVIndex, status);
		Get(Record::UVIndex, minimum_value, maximum_value);
		Get(Record::UVIndex, values);
		m_file_stream << "uv_light=" << status << "," << "\n";
		m_file_stream << "uv_index_range=" << minimum_value << "," << maximum_value << "," << "\n";
		m_file_stream << "uv_index=";
		break;
	case Record::Temperature:
		Get(Record::Temperature, status);
		Get(Record::Temperature, minimum_value, maximum_value);
		Get(Record::Temperature, values);
		m_file_stream << "ac=" << status << "," << "\n";
		m_file_stream << "temperature_range=" << minimum_value << "," << maximum_value << "," << "\n";
		m_file_stream << "temperature=";
		break;
	case Record::AirHumidity:
		Get(Record::AirHumidity, status);
		Get(Record::AirHumidity, minimum_value, maximum_value);
		Get(Record::AirHumidity, values);
		m_file_stream << "air_humidifier=" << status << "," << "\n";
		m_file_stream << "air_humidity_range=" << minimum_value << "," << maximum_value << "," << "\n";
		m_file_stream << "air_humidity=";
		break;
	case Record::Air_Precipitation:
		Get(Record::Air_Precipitation, minimum_value, maximum_value);
		Get(Record::Air_Precipitation, values);
		m_file_stream << "precipitation_range=" << minimum_value << "," << maximum_value << "," << "\n";
		m_file_stream << "air_precipitation=";
		break;
	case Record::SoilMoisture:
		Get(Record::SoilMoisture, status);
		Get(Record::SoilMoisture, minimum_value, maximum_value);
		Get(Record::SoilMoisture, values);
		m_file_stream << "water_sprinkler=" << status << "," << "\n";
		m_file_stream << "soil_moisture_range=" << minimum_value << "," << maximum_value << "," << "\n";
		m_file_stream << "soil_moisture=";
		break;
	case Record::SoilFertility:
		Get(Record::SoilFertility, minimum_value, maximum_value);
		Get(Record::SoilFertility, values);
		m_file_stream << "soil_fertility_range=" << minimum_value << "," << maximum_value << "," << "\n";
		m_file_stream << "soil_fertility=";
		break;
	default:
		vector<Record::Event> events;

		Get(events);
		m_file_stream << "events=";

		for (auto& element : events)
		{
			if (element.timestamp[strlen(element.timestamp.data()) - 1] != ',')
				element.timestamp += ',';

			m_file_stream << element.sensor << "," << element.level << "," << element.message << "," << element.timestamp;
		}

		m_file_stream << "\n";
		return;
	}

	for (auto& element : values) m_file_stream << element << ",";

	m_file_stream << "\n";
}

Database::Database(string file_name)
{
	std::fill_n(std::back_inserter(this->ph), this->record_size, (this->ph_range.first + this->ph_range.second) / 2.0f);
	std::fill_n(std::back_inserter(this->co2), this->record_size, (this->co2_range.first + this->co2_range.second) / 2.0f);
	std::fill_n(std::back_inserter(this->uv_index), this->record_size, (this->uv_index_range.first + this->uv_index_range.second) / 2.0f);
	std::fill_n(std::back_inserter(this->temperature), this->record_size, (this->temperature_range.first + this->temperature_range.second) / 2.0f);
	std::fill_n(std::back_inserter(this->air_humidity), this->record_size, (this->air_humidity_range.first + this->air_humidity_range.second) / 2.0f);
	std::fill_n(std::back_inserter(this->air_precipitation), this->record_size, (this->precipitation_range.first + this->precipitation_range.second) / 2.0f);
	std::fill_n(std::back_inserter(this->soil_moisture), this->record_size, (this->soil_moisture_range.first + this->soil_moisture_range.second) / 2.0f);
	std::fill_n(std::back_inserter(this->soil_fertility), this->record_size, (this->soil_fertility_range.first + this->soil_fertility_range.second) / 2.0f);

	m_file_name = file_name;
	m_file_stream.open(m_file_name, ios::in);

	if (!m_file_stream.is_open()) exit(1);

	while (!m_file_stream.eof())
	{
		string record_line_raw;
		string record_tag;
		string record_values;

		getline(m_file_stream, record_line_raw);

		stringstream record_line_raw_stream(record_line_raw);

		getline(record_line_raw_stream, record_tag, '=');
		getline(record_line_raw_stream, record_values, '=');

		m_record_raw_data.push_back(make_pair(record_tag, record_values));
	} m_record_raw_data.pop_back();

	Read(PH);
	Read(CO2);
	Read(UVIndex);
	Read(Temperature);
	Read(AirHumidity);
	Read(Air_Precipitation);
	Read(SoilMoisture);
	Read(SoilFertility);

	Read(None);

	m_file_stream.close();
}

Database::~Database()
{
	m_file_stream.open(m_file_name, ios::out | ios::trunc);

	Write(PH);
	Write(CO2);
	Write(UVIndex);
	Write(Temperature);
	Write(AirHumidity);
	Write(Air_Precipitation);
	Write(SoilMoisture);
	Write(SoilFertility);

	Write(None);

	m_file_stream.close();
}
