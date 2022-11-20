/*
 * database.cpp
 *
 * God have mercy
 *
 * Mostafa Elbasiouny - Nov 16, 2022
 */

#include "database.h"

void Record::Set(Sensors sensor, float value)
{
	switch (sensor)
	{
	case PH: ph.push_back(value); break;
	case CO2: co2.push_back(value); break;
	case UVIndex: uv_index.push_back(value); break;
	case Temperature: temperature.push_back(value); break;
	case AirHumidity: air_humidity.push_back(value); break;
	case Precipitation: precipitation.push_back(value); break;
	case SoilMoisture: soil_moisture.push_back(value); break;
	case SoilFertility: soil_fertility.push_back(value); break;
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
	case PH: ph_range = std::make_pair(minimum_value, maximum_value); break;
	case CO2: co2_range = std::make_pair(minimum_value, maximum_value); break;
	case UVIndex: uv_index_range = std::make_pair(minimum_value, maximum_value); break;
	case Temperature: temperature_range = std::make_pair(minimum_value, maximum_value); break;
	case AirHumidity: air_humidity_range = std::make_pair(minimum_value, maximum_value); break;
	case Precipitation: precipitation_range = std::make_pair(minimum_value, maximum_value); break;
	case SoilMoisture: soil_moisture_range = std::make_pair(minimum_value, maximum_value); break;
	case SoilFertility: soil_fertility_range = std::make_pair(minimum_value, maximum_value); break;
	}
}

void Record::Set(Levels level, Sensors sensor, std::string message, std::string timestamp_string)
{
	if (timestamp_string.empty())
	{
		time_t timestamp = time(0);
		timestamp_string = std::ctime(&timestamp);
		timestamp_string[std::strlen(timestamp_string.data()) - 1] = ',';
	}

	events.push_back({ timestamp_string, level, sensor, message });
}

void Record::Get(Sensors sensor, std::vector<float>& out_values)
{
	switch (sensor)
	{
	case PH: out_values = ph; break;
	case CO2: out_values = co2; break;
	case UVIndex: out_values = uv_index; break;
	case Temperature: out_values = temperature; break;
	case AirHumidity: out_values = air_humidity; break;
	case Precipitation: out_values = precipitation; break;
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
	case Precipitation: out_minimum_value = precipitation_range.first; out_maximum_value = precipitation_range.second; break;
	case SoilMoisture: out_minimum_value = soil_moisture_range.first; out_maximum_value = soil_moisture_range.second; break;
	case SoilFertility: out_minimum_value = soil_fertility_range.first; out_maximum_value = soil_fertility_range.second; break;
	}
}

void Record::Get(std::vector<Event>& out_events)
{
	out_events = events;
}

Database::Database(std::string file_name) {
	m_file_name = file_name;
	m_file_stream.open(m_file_name, std::ios::in);

	if (!m_file_stream.is_open()) std::exit(1);

	while (!m_file_stream.eof())
	{
		std::string record_line_raw;
		std::string record_tag;
		std::string record_data;

		getline(m_file_stream, record_line_raw);

		std::stringstream record_line_raw_stream(record_line_raw);

		getline(record_line_raw_stream, record_tag, '=');
		getline(record_line_raw_stream, record_data, '=');

		m_record_raw_data.push_back(std::make_pair(record_tag, record_data));
	} m_record_raw_data.pop_back();

	Read(PH);
	Read(CO2);
	Read(UVIndex);
	Read(Temperature);
	Read(AirHumidity);
	Read(Precipitation);
	Read(SoilMoisture);
	Read(SoilFertility);

	Read(None);

	m_file_stream.close();
}

Database::~Database() {
	m_file_stream.open(m_file_name, std::ios::out | std::ios::trunc);

	Write(PH);
	Write(CO2);
	Write(UVIndex);
	Write(Temperature);
	Write(AirHumidity);
	Write(Precipitation);
	Write(SoilMoisture);
	Write(SoilFertility);

	Write(None);

	m_file_stream.close();
}

inline uint32_t ConvertToEnum(std::string value_raw)
{
	return std::stoi(value_raw);
}

inline std::pair<float, float> ConvertToPair(std::string values_raw)
{
	std::pair<float, float> values;
	std::stringstream values_raw_stream(values_raw);

	getline(values_raw_stream, values_raw, ',');
	values.first = std::stof(values_raw);

	getline(values_raw_stream, values_raw, ',');
	values.second = std::stof(values_raw);

	return values;
}

inline std::vector<float> ConvertToVector(std::string values_raw)
{
	std::vector<float> values;
	std::stringstream values_raw_stream(values_raw);

	while (getline(values_raw_stream, values_raw, ','))
		values.push_back(std::stof(values_raw));

	return values;
}

void Database::Read(Sensors sensor)
{
	Status status;
	std::pair<float, float> ranges;
	std::vector<float> values;

	switch (sensor)
	{
	case PH:
		for (const auto& [tag, data] : m_record_raw_data)
		{
			if (tag == "ph_regulator") {
				status = static_cast<Status>(ConvertToEnum(data));
				Set(PH, status);
			}

			if (tag == "ph_range") {
				ranges = ConvertToPair(data);
				Set(PH, ranges.first, ranges.second);
			}

			if (tag == "ph") {
				values = ConvertToVector(data);
				for (auto& value : values) Set(PH, value);
			}
		}
		break;
	case CO2:
		for (const auto& [tag, data] : m_record_raw_data)
		{
			if (tag == "co2_range") {
				ranges = ConvertToPair(data);
				Set(CO2, ranges.first, ranges.second);
			}

			if (tag == "co2") {
				values = ConvertToVector(data);
				for (auto& value : values) Set(CO2, value);
			}
		}
		break;
	case UVIndex:
		for (const auto& [tag, data] : m_record_raw_data)
		{
			if (tag == "uv_light") {
				status = static_cast<Status>(ConvertToEnum(data));
				Set(UVIndex, status);
			}

			if (tag == "uv_index_range") {
				ranges = ConvertToPair(data);
				Set(UVIndex, ranges.first, ranges.second);
			}

			if (tag == "uv_index") {
				values = ConvertToVector(data);
				for (auto& value : values) Set(UVIndex, value);
			}
		}
		break;
	case Temperature:
		for (const auto& [tag, data] : m_record_raw_data)
		{
			if (tag == "ac") {
				status = static_cast<Status>(ConvertToEnum(data));
				Set(Temperature, status);
			}

			if (tag == "temperature_range") {
				ranges = ConvertToPair(data);
				Set(Temperature, ranges.first, ranges.second);
			}

			if (tag == "temperature") {
				values = ConvertToVector(data);
				for (auto& value : values) Set(Temperature, value);
			}
		}
		break;
	case AirHumidity:
		for (const auto& [tag, data] : m_record_raw_data)
		{
			if (tag == "air_humidifier") {
				status = static_cast<Status>(ConvertToEnum(data));
				Set(AirHumidity, status);
			}

			if (tag == "air_humidity_range") {
				ranges = ConvertToPair(data);
				Set(AirHumidity, ranges.first, ranges.second);
			}

			if (tag == "air_humidity") {
				values = ConvertToVector(data);
				for (auto& value : values) Set(AirHumidity, value);
			}
		}
		break;
	case Precipitation:
		for (const auto& [tag, data] : m_record_raw_data)
		{
			if (tag == "precipitation_range") {
				ranges = ConvertToPair(data);
				Set(Precipitation, ranges.first, ranges.second);
			}

			if (tag == "precipitation") {
				values = ConvertToVector(data);
				for (auto& value : values) Set(Precipitation, value);
			}
		}
		break;
	case SoilMoisture:
		for (const auto& [tag, data] : m_record_raw_data)
		{
			if (tag == "water_sprinkler") {
				status = static_cast<Status>(ConvertToEnum(data));
				Set(SoilMoisture, status);
			}

			if (tag == "soil_moisture_range") {
				ranges = ConvertToPair(data);
				Set(SoilMoisture, ranges.first, ranges.second);
			}

			if (tag == "soil_moisture") {
				values = ConvertToVector(data);
				for (auto& value : values) Set(SoilMoisture, value);
			}
		}
		break;
	case SoilFertility:
		for (const auto& [tag, data] : m_record_raw_data)
		{
			if (tag == "soil_fertility_range") {
				ranges = ConvertToPair(data);
				Set(SoilFertility, ranges.first, ranges.second);
			}

			if (tag == "soil_fertility") {
				values = ConvertToVector(data);
				for (auto& value : values) Set(SoilFertility, value);
			}
		}
		break;
	default:
		for (const auto& [tag, data] : m_record_raw_data)
		{
			if (tag == "events") {
				Event event;
				std::string value;
				std::stringstream value_raw(data);

				getline(value_raw, value, ',');

				while (!value.empty())
				{
					event.timestamp = value;

					getline(value_raw, value, ',');
					event.level = static_cast<Levels>(ConvertToEnum(value));

					getline(value_raw, value, ',');
					event.sensor = static_cast<Sensors>(ConvertToEnum(value));

					getline(value_raw, value, ',');
					event.message = value;

					getline(value_raw, value, ',');

					Set(event.level, event.sensor, event.message, event.timestamp);
				}
			}
		}
		break;
	}
}

void Database::Write(Sensors sensor)
{
	Status status;
	float minimum_value;
	float maximum_value;
	std::vector<float> values;

	switch (sensor)
	{
	case PH:
		Get(PH, status);
		Get(PH, minimum_value, maximum_value);
		Get(PH, values);
		m_file_stream << "ph_regulator=" << status << "," << std::endl;
		m_file_stream << "ph_range=" << minimum_value << "," << maximum_value << "," << std::endl;
		m_file_stream << "ph=";
		break;
	case CO2:
		Get(CO2, minimum_value, maximum_value);
		Get(CO2, values);
		m_file_stream << "co2_range=" << minimum_value << "," << maximum_value << "," << std::endl;
		m_file_stream << "co2=";
		break;
	case UVIndex:
		Get(UVIndex, status);
		Get(UVIndex, minimum_value, maximum_value);
		Get(UVIndex, values);
		m_file_stream << "uv_light=" << status << "," << std::endl;
		m_file_stream << "uv_index_range=" << minimum_value << "," << maximum_value << "," << std::endl;
		m_file_stream << "uv_index=";
		break;
	case Temperature:
		Get(Temperature, status);
		Get(Temperature, minimum_value, maximum_value);
		Get(Temperature, values);
		m_file_stream << "ac=" << status << "," << std::endl;
		m_file_stream << "temperature_range=" << minimum_value << "," << maximum_value << "," << std::endl;
		m_file_stream << "temperature=";
		break;
	case AirHumidity:
		Get(AirHumidity, status);
		Get(AirHumidity, minimum_value, maximum_value);
		Get(AirHumidity, values);
		m_file_stream << "air_humidifier=" << status << "," << std::endl;
		m_file_stream << "air_humidity_range=" << minimum_value << "," << maximum_value << "," << std::endl;
		m_file_stream << "air_humidity=";
		break;
	case Precipitation:
		Get(Precipitation, minimum_value, maximum_value);
		Get(Precipitation, values);
		m_file_stream << "precipitation_range=" << minimum_value << "," << maximum_value << "," << std::endl;
		m_file_stream << "precipitation=";
		break;
	case SoilMoisture:
		Get(SoilMoisture, status);
		Get(SoilMoisture, minimum_value, maximum_value);
		Get(SoilMoisture, values);
		m_file_stream << "water_sprinkler=" << status << "," << std::endl;
		m_file_stream << "soil_moisture_range=" << minimum_value << "," << maximum_value << "," << std::endl;
		m_file_stream << "soil_moisture=";
		break;
	case SoilFertility:
		Get(SoilFertility, minimum_value, maximum_value);
		Get(SoilFertility, values);
		m_file_stream << "soil_fertility_range=" << minimum_value << "," << maximum_value << "," << std::endl;
		m_file_stream << "soil_fertility=";
		break;
	default:
		std::vector<Event> events;

		Get(events);
		m_file_stream << "events=";

		for (auto& element : events)
		{
			if (element.timestamp[std::strlen(element.timestamp.data()) - 1] != ',')
				element.timestamp += ',';

			m_file_stream << element.timestamp << element.level << "," << element.sensor << "," << element.message << ",";
		}

		m_file_stream << std::endl;
		return;
	}

	for (auto& element : values) m_file_stream << element << ",";

	m_file_stream << std::endl;
}
