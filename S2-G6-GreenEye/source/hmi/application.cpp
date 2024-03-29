﻿/*
 * application.cpp
 *
 * Mostafa Elbasiouny - Nov 04, 2022
 */

#include "application.h"

#pragma region States

Application::States::States()
{
	this->ph_range = std::make_pair(0.0f, 14.0f);
	this->co2_range = std::make_pair(500.0f, 1500.0f);
	this->uv_index_range = std::make_pair(0.0f, 12.0f);
	this->temperature_range = std::make_pair(20.0f, 30.0f);
	this->air_humidity_range = std::make_pair(60.0f, 100.0f);
	this->precipitation_range = std::make_pair(2500.0f, 7500.0f);
	this->soil_moisture_range = std::make_pair(80.0f, 100.0f);
	this->soil_fertility_range = std::make_pair(80.0f, 100.0f);

	std::fill_n(std::back_inserter(this->ph), this->record_size, (this->ph_range.first + this->ph_range.second) / 2.0f);
	std::fill_n(std::back_inserter(this->co2), this->record_size, (this->co2_range.first + this->co2_range.second) / 2.0f);
	std::fill_n(std::back_inserter(this->uv_index), this->record_size, (this->uv_index_range.first + this->uv_index_range.second) / 2.0f);
	std::fill_n(std::back_inserter(this->temperature), this->record_size, (this->temperature_range.first + this->temperature_range.second) / 2.0f);
	std::fill_n(std::back_inserter(this->air_humidity), this->record_size, (this->air_humidity_range.first + this->air_humidity_range.second) / 2.0f);
	std::fill_n(std::back_inserter(this->air_precipitation), this->record_size, (this->precipitation_range.first + this->precipitation_range.second) / 2.0f);
	std::fill_n(std::back_inserter(this->soil_moisture), this->record_size, (this->soil_moisture_range.first + this->soil_moisture_range.second) / 2.0f);
	std::fill_n(std::back_inserter(this->soil_fertility), this->record_size, (this->soil_fertility_range.first + this->soil_fertility_range.second) / 2.0f);

	this->ac = Disabled;
	this->uv_light = Disabled;
	this->humidifier = Disabled;
	this->ph_regulator = Disabled;
	this->water_sprinkler = Disabled;
}

void Application::States::SetPH(Status status)
{
	this->ph_regulator = status;
}

void Application::States::SetUVIndex(Status status)
{
	this->uv_light = status;
}

void Application::States::SetAirHumidity(Status status)
{
	this->humidifier = status;
}

void Application::States::SetTemperature(Status status)
{
	this->ac = status;
}

void Application::States::SetSoilMoisture(Status status)
{
	this->water_sprinkler = status;
}

void Application::States::SetCO2(std::vector<float> values)
{
	this->co2 = values;
}

void Application::States::SetPrecipitation(std::vector<float> values)
{
	this->air_precipitation = values;
}

void Application::States::SetSoilFertility(std::vector<float> values)
{
	this->soil_fertility = values;
}

void Application::States::SetPH(std::vector<float> values)
{
	this->ph = values;
}

void Application::States::SetUVIndex(std::vector<float> values)
{
	this->uv_index = values;
}

void Application::States::SetTemperature(std::vector<float> values)
{
	this->temperature = values;
}

void Application::States::SetAirHumidity(std::vector<float> values)
{
	this->air_humidity = values;
}

void Application::States::SetSoilMoisture(std::vector<float> values)
{
	this->soil_moisture = values;
}

void Application::States::SetEvent(Event event)
{
	this->events.push_back(event);

	if (event.level == Critical)
		this->alerts.push_back(event);
}

std::vector<float> Application::States::GetCO2()
{
	return this->co2;
}

std::vector<float> Application::States::GetPrecipitation()
{
	return this->air_precipitation;
}

std::vector<float> Application::States::GetSoilFertility()
{
	return this->soil_fertility;
}

std::pair<std::vector<float>, Application::States::Status> Application::States::GetPH()
{
	return std::make_pair(this->ph, this->ph_regulator);
}

std::pair<std::vector<float>, Application::States::Status> Application::States::GetUVIndex()
{
	return std::make_pair(this->uv_index, this->uv_light);
}

std::pair<std::vector<float>, Application::States::Status> Application::States::GetTemperature()
{
	return std::make_pair(this->temperature, this->ac);
}

std::pair<std::vector<float>, Application::States::Status> Application::States::GetAirHumidity()
{
	return std::make_pair(this->air_humidity, this->humidifier);
}

std::pair<std::vector<float>, Application::States::Status> Application::States::GetSoilMoisture()
{
	return std::make_pair(this->soil_moisture, this->water_sprinkler);
}

std::vector<Application::States::Event> Application::States::GetEvents(Sensors sensor)
{
	if (sensor != None) {
		std::vector<Event> filtered_events;

		for (auto& event : this->events) {
			if (event.sensor == sensor) {
				filtered_events.push_back(event);
			}
		}

		return filtered_events;
	}
	else {
		return this->events;
	}
}

void Application::States::ClearEvents() { this->events.clear(); }

std::pair<float, float> Application::States::GetCO2Ranges()
{
	return this->co2_range;
}

std::pair<float, float> Application::States::GetPrecipitationRanges()
{
	return this->precipitation_range;
}

std::pair<float, float> Application::States::GetSoilFertilityRanges()
{
	return this->soil_fertility_range;
}

std::pair<float, float> Application::States::GetPHRanges()
{
	return this->ph_range;
}

std::pair<float, float> Application::States::GetUVIndexRanges()
{
	return this->uv_index_range;
}

std::pair<float, float> Application::States::GetTemperatureRanges()
{
	return this->temperature_range;
}

std::pair<float, float> Application::States::GetAirHumidityRanges()
{
	return this->air_humidity_range;
}

std::pair<float, float> Application::States::GetSoilMoistureRanges()
{
	return this->soil_moisture_range;
}

#pragma endregion

#pragma region Base

bool theme_toggle = false;
bool alert_visible = false;
Application::States::Event current_alert;

inline std::vector<float> RangedPlot(std::vector<float> vector, float average)
{
	std::vector<float> ranged_vector;

	for (auto& element : vector) {
		ranged_vector.push_back(((element / average) - 1) / 2);
	}

	return ranged_vector;
}

inline void Spectrum(float maximum, float value, bool extended_index = true)
{
	float width = ImGui::GetWindowWidth() - 32.0f;
	float height = ImGui::GetWindowHeight() - 152.0f;

	float spectrum_value = ((width / maximum) * value) + 32.0f;

	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	ImVec2 gradient_size = ImVec2(width / 2, height);

	{
		ImVec2 p_minimum = ImGui::GetCursorScreenPos();
		ImVec2 p_maximum = ImVec2(p_minimum.x + gradient_size.x, p_minimum.y + gradient_size.y);
		ImU32 color_start;
		ImU32 color_end;

		if (extended_index) {
			color_start = ImGui::GetColorU32(IM_COL32(255, 0, 0, 196));
			color_end = ImGui::GetColorU32(IM_COL32(0, 255, 0, 196));
		}
		else {
			color_start = ImGui::GetColorU32(IM_COL32(0, 255, 0, 196));
			color_end = ImGui::GetColorU32(IM_COL32(255, 65, 0, 196));
		}

		draw_list->AddRectFilledMultiColor(p_minimum, p_maximum, color_start, color_end, color_end, color_start);

		p_minimum = ImVec2(ImGui::GetCursorScreenPos().x + width / 2, ImGui::GetCursorScreenPos().y);
		p_maximum = ImVec2(p_minimum.x + gradient_size.x, p_minimum.y + gradient_size.y);

		if (extended_index) {
			color_start = ImGui::GetColorU32(IM_COL32(0, 255, 0, 196));
			color_end = ImGui::GetColorU32(IM_COL32(0, 0, 255, 196));
		}
		else {
			color_start = ImGui::GetColorU32(IM_COL32(255, 65, 0, 196));
			color_end = ImGui::GetColorU32(IM_COL32(128, 0, 128, 196));
		}

		draw_list->AddRectFilledMultiColor(p_minimum, p_maximum, color_start, color_end, color_end, color_start);

		gradient_size = ImVec2(10.0f, height * 1.25f);
		p_minimum = ImVec2(spectrum_value, ImGui::GetCursorScreenPos().y);
		p_maximum = ImVec2(p_minimum.x + gradient_size.x, p_minimum.y + gradient_size.y);
		color_start = ImGui::GetColorU32(IM_COL32(0, 0, 0, 255));
		color_end = ImGui::GetColorU32(IM_COL32(0, 0, 0, 255));

		draw_list->AddRectFilledMultiColor(p_minimum, p_maximum, color_start, color_end, color_end, color_start);

		ImGui::SetCursorPosX(p_minimum.x - 22.0f);
		ImGui::SetCursorPosY((height * 1.25f) + 32.0f);
		ImGui::Text("%.1f", value);
	}
}

void Application::Base::Home()
{
	ImGui::Text("Home Panel");

	float width = ImGui::GetWindowWidth() - 32.0f;
	float height = ImGui::GetWindowHeight() - 142.0f;

	{
		ImGui::BeginChild("Plots", ImVec2(width, (height / 3) * 2), true);

		ImVec2 plot_size = ImVec2(width - 32.0f, ((height / 3) * 2) - 32.0f);
		static ImPlotFlags flags = ImPlotFlags_NoTitle | ImPlotFlags_NoMouseText | ImPlotFlags_NoInputs | ImPlotFlags_NoMenus | ImPlotFlags_NoBoxSelect | ImPlotFlags_NoChild;

		if (ImPlot::BeginPlot("Plots", plot_size, flags)) {
			ImPlot::SetupLegend(ImPlotLocation_North, ImPlotLegendFlags_Outside | ImPlotLegendFlags_Horizontal);
                  ImPlot::SetupAxes("Sample", "Change Over Time",
                                    ImPlotAxisFlags_NoGridLines,
                                          ImPlotAxisFlags_NoGridLines);
			ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
			ImPlot::PlotBars("PH", RangedPlot(this->states->GetPH().first, (this->states->ph_range.first + this->states->ph_range.second) / 2.0f).data(), this->states->record_size);
			ImPlot::PlotBars(u8"CO²", RangedPlot(this->states->GetCO2(), (this->states->co2_range.first + this->states->co2_range.second) / 2.0f).data(), this->states->record_size);
			ImPlot::PlotBars("UV Index", RangedPlot(this->states->GetUVIndex().first, (this->states->uv_index_range.first + this->states->uv_index_range.second) / 2.0f).data(), this->states->record_size);
			ImPlot::PlotBars("Temperature", RangedPlot(this->states->GetTemperature().first, (this->states->temperature_range.first + this->states->temperature_range.second) / 2.0f).data(), this->states->record_size);
			ImPlot::PlotBars("Air Humidity", RangedPlot(this->states->GetAirHumidity().first, (this->states->air_humidity_range.first + this->states->air_humidity_range.second) / 2.0f).data(), this->states->record_size);
			ImPlot::PlotBars("Air Precipitation", RangedPlot(this->states->GetPrecipitation(), (this->states->precipitation_range.first + this->states->precipitation_range.second) / 2.0f).data(), this->states->record_size);
			ImPlot::PlotBars("Soil Moisture", RangedPlot(this->states->GetSoilMoisture().first, (this->states->soil_moisture_range.first + this->states->soil_moisture_range.second) / 2.0f).data(), this->states->record_size);
			ImPlot::PlotBars("Soil Fertility", RangedPlot(this->states->GetSoilFertility(), (this->states->soil_fertility_range.first + this->states->soil_fertility_range.second) / 2.0f).data(), this->states->record_size);
			ImPlot::PopStyleVar();
			ImPlot::EndPlot();
		}

		ImGui::EndChild();
	}

	{
		ImGui::BeginChild("Events", ImVec2(width, height / 3), true);

		this->Events(this->states->GetEvents());

		ImGui::EndChild();
	}
}

void Application::Base::PH()
{
	ImGui::Text("PH Panel");

	float width = ImGui::GetWindowWidth() - 32.0f;
	float height = ImGui::GetWindowHeight() - 152.0f;

	{
		ImGui::BeginChild("Plots", ImVec2(width, height / 3), true);

		Spectrum(14.0f, this->states->GetPH().first.back());

		ImGui::EndChild();
	}

	{
		ImGui::BeginChild("Configurations", ImVec2(width, height / 3), true);

		ImGui::BulletText("Average Index Range");
		ImGui::SliderFloat("Minimum", &this->states->ph_range.first, 0.0f, 7.0f, "%.1f");
		ImGui::SliderFloat("Maximum", &this->states->ph_range.second, 7.0f, 14.0f, "%.1f");

		ImGui::BulletText("Current Value: %.1f", this->states->GetPH().first.back());

		ImGui::BulletText("PH Regulator %s", this->states->GetPH().second ? "Enabled" : "Disabled");
		ImGui::SameLine();

		const char* action = this->states->GetPH().second ? "Disable" : "Enable";

		ImGuiStyle* style = &ImGui::GetStyle();

		if (action == "Disable")
			style->Colors[ImGuiCol_Button] = ImVec4(0.71f, 0.32f, 0.3f, 1.00f);
		else
			style->Colors[ImGuiCol_Button] = ImVec4(0.49f, 0.71f, 0.3f, 1.00f);

		if (ImGui::SmallButton(action)) {
			this->states->SetPH(action == "Enable" ? this->states->Enabled : this->states->Disabled);
			time_t _ = time(0);
			this->states->SetEvent({ ctime(&_), this->states->Notice, this->states->PH, action == "Enable" ? "PH regulator enabled by user." : "PH regulator disabled by user." });
			auto status = static_cast<Record::Status>(this->states->GetPH().second);
			this->m_database->Set(Record::PH, status);
		}

		ImGui::EndChild();
	}

	{
		ImGui::BeginChild("Events", ImVec2(width, height / 3), true);

		this->Events(this->states->GetEvents(this->states->PH));

		ImGui::EndChild();
	}
}

void Application::Base::CO2()
{
	ImGui::Text(u8"CO² Panel");

	float width = ImGui::GetWindowWidth() - 32.0f;
	float height = ImGui::GetWindowHeight() - 152.0f;

	{
		ImGui::BeginChild("Plots", ImVec2(width, height / 3), true);

		ImVec2 plot_size = ImVec2(width - 32.0f, (height / 3) - 32.0f);
		static ImPlotFlags flags = ImPlotFlags_NoTitle | ImPlotFlags_NoLegend | ImPlotFlags_NoMouseText | ImPlotFlags_NoInputs | ImPlotFlags_NoMenus | ImPlotFlags_NoBoxSelect | ImPlotFlags_NoChild;

		if (ImPlot::BeginPlot("Plots", plot_size, flags)) {
                  ImPlot::SetupAxes("Time", "Parts Per Million (PPM)",
                                    ImPlotAxisFlags_NoGridLines,
                                    ImPlotAxisFlags_NoGridLines);
			ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.5f);
			ImPlot::PlotScatter(u8"CO²", this->states->GetCO2().data(), this->states->record_size);
			ImPlot::PopStyleVar();
			ImPlot::EndPlot();
		}

		ImGui::EndChild();
	}

	{
		ImGui::BeginChild("Configurations", ImVec2(width, height / 3), true);

		ImGui::BulletText("Average PPM Range");
		ImGui::SliderFloat("Minimum", &this->states->co2_range.first, 500.0f, 1000.0f, "%.1f");
		ImGui::SliderFloat("Maximum", &this->states->co2_range.second, 1000.0f, 1500.0f, "%.1f");

		ImGui::BulletText("Current Value: %.1f PPM", this->states->GetCO2().back());

		ImGui::EndChild();
	}

	{
		ImGui::BeginChild("Events", ImVec2(width, height / 3), true);

		this->Events(this->states->GetEvents(this->states->CO2));

		ImGui::EndChild();
	}
}

void Application::Base::UVIndex()
{
	ImGui::Text("UV Index Panel");

	float width = ImGui::GetWindowWidth() - 32.0f;
	float height = ImGui::GetWindowHeight() - 152.0f;

	{
		ImGui::BeginChild("Plots", ImVec2(width, height / 3), true);

		Spectrum(12.0f, this->states->GetUVIndex().first.back(), false);

		ImGui::EndChild();
	}

	{
		ImGui::BeginChild("Configurations", ImVec2(width, height / 3), true);

		ImGui::BulletText("Average Index Range");
		ImGui::SliderFloat("Minimum", &this->states->uv_index_range.first, 0.0f, 6.0f, "%.1f");
		ImGui::SliderFloat("Maximum", &this->states->uv_index_range.second, 6.0f, 12.0f, "%.1f");

		ImGui::BulletText("Current Value: %.1f", this->states->GetUVIndex().first.back());

		ImGui::BulletText("UV Light %s", this->states->GetUVIndex().second ? "Enabled" : "Disabled");
		ImGui::SameLine();

		const char* action = this->states->GetUVIndex().second ? "Disable" : "Enable";

		ImGuiStyle* style = &ImGui::GetStyle();

		if (action == "Disable")
			style->Colors[ImGuiCol_Button] = ImVec4(0.71f, 0.32f, 0.3f, 1.00f);
		else
			style->Colors[ImGuiCol_Button] = ImVec4(0.49f, 0.71f, 0.3f, 1.00f);

		if (ImGui::SmallButton(action)) {
			this->states->SetUVIndex(action == "Enable" ? this->states->Enabled : this->states->Disabled);
			time_t _ = time(0);
			this->states->SetEvent({ ctime(&_), this->states->Notice, this->states->UVIndex, action == "Enable" ? "UV light enabled by user." : "UV light disabled by user." });
			auto status = static_cast<Record::Status>(this->states->GetUVIndex().second);
			this->m_database->Set(Record::UVIndex, status);
		}

		ImGui::EndChild();
	}

	{
		ImGui::BeginChild("Events", ImVec2(width, height / 3), true);

		this->Events(this->states->GetEvents(this->states->UVIndex));

		ImGui::EndChild();
	}
}

void Application::Base::Temperature()
{
	ImGui::Text("Temperature Panel");

	float width = ImGui::GetWindowWidth() - 32.0f;
	float height = ImGui::GetWindowHeight() - 152.0f;

	{
		ImGui::BeginChild("Plots", ImVec2(width, height / 3), true);

		ImVec2 plot_size = ImVec2(width - 32.0f, (height / 3) - 32.0f);
		static ImPlotFlags flags = ImPlotFlags_NoTitle | ImPlotFlags_NoLegend | ImPlotFlags_NoMouseText | ImPlotFlags_NoInputs | ImPlotFlags_NoMenus | ImPlotFlags_NoBoxSelect | ImPlotFlags_NoChild;

		if (ImPlot::BeginPlot("Plots", plot_size, flags)) {
                  ImPlot::SetupAxes("Time", u8"Celsius (°C)",
                                    ImPlotAxisFlags_NoGridLines,
                                    ImPlotAxisFlags_NoGridLines);
			ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.5f);
			ImPlot::PlotScatter("Temperature", this->states->GetTemperature().first.data(), this->states->record_size);
			ImPlot::PopStyleVar();
			ImPlot::EndPlot();
		}

		ImGui::EndChild();
	}

	{
		ImGui::BeginChild("Configurations", ImVec2(width, height / 3), true);

		ImGui::BulletText("Average Degrees Celsius Range");
		ImGui::SliderFloat("Minimum", &this->states->temperature_range.first, 20.0f, 25.0f, "%.1f");
		ImGui::SliderFloat("Maximum", &this->states->temperature_range.second, 25.0f, 30.0f, "%.1f");

		ImGui::BulletText(u8"Current Value: %.1f °C", this->states->GetTemperature().first.back());

		ImGui::BulletText("AC %s", this->states->GetTemperature().second ? "Enabled" : "Disabled");
		ImGui::SameLine();

		const char* action = this->states->GetTemperature().second ? "Disable" : "Enable";

		ImGuiStyle* style = &ImGui::GetStyle();

		if (action == "Disable")
			style->Colors[ImGuiCol_Button] = ImVec4(0.71f, 0.32f, 0.3f, 1.00f);
		else
			style->Colors[ImGuiCol_Button] = ImVec4(0.49f, 0.71f, 0.3f, 1.00f);

		if (ImGui::SmallButton(action)) {
			this->states->SetTemperature(action == "Enable" ? this->states->Enabled : this->states->Disabled);
			time_t _ = time(0);
			this->states->SetEvent({ ctime(&_), this->states->Notice, this->states->Temperature, action == "Enable" ? "AC enabled by user." : "AC disabled by user." });
			auto status = static_cast<Record::Status>(this->states->GetTemperature().second);
			this->m_database->Set(Record::Temperature, status);
		}

		ImGui::EndChild();
	}

	{
		ImGui::BeginChild("Events", ImVec2(width, height / 3), true);

		this->Events(this->states->GetEvents(this->states->Temperature));

		ImGui::EndChild();
	}
}

void Application::Base::AirHumidity()
{
	ImGui::Text("Air Humidity Panel");

	float width = ImGui::GetWindowWidth() - 32.0f;
	float height = ImGui::GetWindowHeight() - 152.0f;

	{
		ImGui::BeginChild("Plots", ImVec2(width, height / 3), true);

		ImVec2 plot_size = ImVec2(width - 32.0f, (height / 3) - 32.0f);
		static ImPlotFlags flags = ImPlotFlags_NoTitle | ImPlotFlags_NoLegend | ImPlotFlags_NoMouseText | ImPlotFlags_NoInputs | ImPlotFlags_NoMenus | ImPlotFlags_NoBoxSelect | ImPlotFlags_NoChild;

		if (ImPlot::BeginPlot("Plots", plot_size, flags)) {
                  ImPlot::SetupAxes("Time", "Percentage (%)",
                                    ImPlotAxisFlags_NoGridLines,
                                    ImPlotAxisFlags_NoGridLines);
			ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.5f);
			ImPlot::PlotScatter("Air Humidity", this->states->GetAirHumidity().first.data(), this->states->record_size);
			ImPlot::PopStyleVar();
			ImPlot::EndPlot();
		}

		ImGui::EndChild();
	}

	{
		ImGui::BeginChild("Configurations", ImVec2(width, height / 3), true);

		ImGui::BulletText("Average Percentage Range");
		ImGui::SliderFloat("Minimum", &this->states->air_humidity_range.first, 60.0f, 80.0f, "%.1f");
		ImGui::SliderFloat("Maximum", &this->states->air_humidity_range.second, 80.0f, 100.0f, "%.1f");

		ImGui::BulletText("Current Value: %.1f %%", this->states->GetAirHumidity().first.back());

		ImGui::BulletText("Humidifier %s", this->states->GetAirHumidity().second ? "Enabled" : "Disabled");
		ImGui::SameLine();

		const char* action = this->states->GetAirHumidity().second ? "Disable" : "Enable";

		ImGuiStyle* style = &ImGui::GetStyle();

		if (action == "Disable")
			style->Colors[ImGuiCol_Button] = ImVec4(0.71f, 0.32f, 0.3f, 1.00f);
		else
			style->Colors[ImGuiCol_Button] = ImVec4(0.49f, 0.71f, 0.3f, 1.00f);

		if (ImGui::SmallButton(action)) {
			this->states->SetAirHumidity(action == "Enable" ? this->states->Enabled : this->states->Disabled);
			time_t _ = time(0);
			this->states->SetEvent({ ctime(&_), this->states->Notice, this->states->AirHumidity, action == "Enable" ? "Humidifier enabled by user." : "Humidifier disabled by user." });
			auto status = static_cast<Record::Status>(this->states->GetAirHumidity().second);
			this->m_database->Set(Record::AirHumidity, status);
		}

		ImGui::EndChild();
	}

	{
		ImGui::BeginChild("Events", ImVec2(width, height / 3), true);

		this->Events(this->states->GetEvents(this->states->AirHumidity));

		ImGui::EndChild();
	}
}

void Application::Base::Air_Precipitation()
{
	ImGui::Text("Air Precipitation Panel");

	float width = ImGui::GetWindowWidth() - 32.0f;
	float height = ImGui::GetWindowHeight() - 152.0f;

	{
		ImGui::BeginChild("Plots", ImVec2(width, height / 3), true);

		ImVec2 plot_size = ImVec2(width - 32.0f, (height / 3) - 32.0f);
		static ImPlotFlags flags = ImPlotFlags_NoTitle | ImPlotFlags_NoLegend | ImPlotFlags_NoMouseText | ImPlotFlags_NoInputs | ImPlotFlags_NoMenus | ImPlotFlags_NoBoxSelect | ImPlotFlags_NoChild;

		if (ImPlot::BeginPlot("Plots", plot_size, flags)) {
                  ImPlot::SetupAxes("Time", "Millimeter (MM)",
                                    ImPlotAxisFlags_NoGridLines,
                                    ImPlotAxisFlags_NoGridLines);
			ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.5f);
			ImPlot::PlotScatter("Air Precipitation", this->states->GetPrecipitation().data(), this->states->record_size);
			ImPlot::PopStyleVar();
			ImPlot::EndPlot();
		}

		ImGui::EndChild();
	}

	{
		ImGui::BeginChild("Configurations", ImVec2(width, height / 3), true);

		ImGui::BulletText("Average Millimetres Range");
		ImGui::SliderFloat("Minimum", &this->states->precipitation_range.first, 2500.0f, 5000.0f, "%.1f");
		ImGui::SliderFloat("Maximum", &this->states->precipitation_range.second, 5000.0f, 7500.0f, "%.1f");

		ImGui::BulletText("Current Value: %.1f MM", this->states->GetPrecipitation().back());

		ImGui::EndChild();
	}

	{
		ImGui::BeginChild("Events", ImVec2(width, height / 3), true);

		this->Events(this->states->GetEvents(this->states->Air_Precipitation));

		ImGui::EndChild();
	}
}

void Application::Base::SoilMoisture()
{
	ImGui::Text("Soil Moisture Panel");

	float width = ImGui::GetWindowWidth() - 32.0f;
	float height = ImGui::GetWindowHeight() - 152.0f;

	{
		ImGui::BeginChild("Plots", ImVec2(width, height / 3), true);

		ImVec2 plot_size = ImVec2(width - 32.0f, (height / 3) - 32.0f);
		static ImPlotFlags flags = ImPlotFlags_NoTitle | ImPlotFlags_NoLegend | ImPlotFlags_NoMouseText | ImPlotFlags_NoInputs | ImPlotFlags_NoMenus | ImPlotFlags_NoBoxSelect | ImPlotFlags_NoChild;

		if (ImPlot::BeginPlot("Plots", plot_size, flags)) {
                  ImPlot::SetupAxes("Time", "Percentage (%)",
                                    ImPlotAxisFlags_NoGridLines,
                                    ImPlotAxisFlags_NoGridLines);
			ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.5f);
			ImPlot::PlotScatter("Soil Moisture", this->states->GetSoilMoisture().first.data(), this->states->record_size);
			ImPlot::PopStyleVar();
			ImPlot::EndPlot();
		}

		ImGui::EndChild();
	}

	{
		ImGui::BeginChild("Configurations", ImVec2(width, height / 3), true);

		ImGui::BulletText("Average Percentage Range");
		ImGui::SliderFloat("Minimum", &this->states->soil_moisture_range.first, 80.0f, 90.0f, "%.1f");
		ImGui::SliderFloat("Maximum", &this->states->soil_moisture_range.second, 90.0f, 100.0f, "%.1f");

		ImGui::BulletText("Current Value: %.1f %%", this->states->GetSoilMoisture().first.back());

		ImGui::BulletText("Water Sprinkler %s", this->states->GetSoilMoisture().second ? "Enabled" : "Disabled");
		ImGui::SameLine();

		const char* action = this->states->GetSoilMoisture().second ? "Disable" : "Enable";

		ImGuiStyle* style = &ImGui::GetStyle();

		if (action == "Disable")
			style->Colors[ImGuiCol_Button] = ImVec4(0.71f, 0.32f, 0.3f, 1.00f);
		else
			style->Colors[ImGuiCol_Button] = ImVec4(0.49f, 0.71f, 0.3f, 1.00f);

		if (ImGui::SmallButton(action)) {
			this->states->SetSoilMoisture(action == "Enable" ? this->states->Enabled : this->states->Disabled);
			time_t _ = time(0);
			this->states->SetEvent({ ctime(&_), this->states->Notice, this->states->SoilMoisture, action == "Enable" ? "Water sprinkler enabled by user." : "Water sprinkler disabled by user." });
			auto status = static_cast<Record::Status>(this->states->GetSoilMoisture().second);
			this->m_database->Set(Record::SoilMoisture, status);
		}

		ImGui::EndChild();
	}

	{
		ImGui::BeginChild("Events", ImVec2(width, height / 3), true);

		this->Events(this->states->GetEvents(this->states->SoilMoisture));

		ImGui::EndChild();
	}
}

void Application::Base::SoilFertility()
{
	ImGui::Text("Soil Fertility Panel");

	float width = ImGui::GetWindowWidth() - 32.0f;
	float height = ImGui::GetWindowHeight() - 152.0f;

	{
		ImGui::BeginChild("Plots", ImVec2(width, height / 3), true);

		ImVec2 plot_size = ImVec2(width - 32.0f, (height / 3) - 32.0f);
		static ImPlotFlags flags = ImPlotFlags_NoTitle | ImPlotFlags_NoLegend | ImPlotFlags_NoMouseText | ImPlotFlags_NoInputs | ImPlotFlags_NoMenus | ImPlotFlags_NoBoxSelect | ImPlotFlags_NoChild;

		if (ImPlot::BeginPlot("Plots", plot_size, flags)) {
                  ImPlot::SetupAxes("Time", "Percentage (%)",
                                    ImPlotAxisFlags_NoGridLines,
                                    ImPlotAxisFlags_NoGridLines);
			ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.5f);
			ImPlot::PlotScatter("Soil Fertility", this->states->GetSoilFertility().data(), this->states->record_size);
			ImPlot::PopStyleVar();
			ImPlot::EndPlot();
		}

		ImGui::EndChild();
	}

	{
		ImGui::BeginChild("Configurations", ImVec2(width, height / 3), true);

		ImGui::BulletText("Average Percentage Range");
		ImGui::SliderFloat("Minimum", &this->states->soil_fertility_range.first, 80.0f, 90.0f, "%.1f");
		ImGui::SliderFloat("Maximum", &this->states->soil_fertility_range.second, 90.0f, 100.0f, "%.1f");

		ImGui::BulletText("Current Value: %.1f %%", this->states->GetSoilFertility().back());

		ImGui::EndChild();
	}

	{
		ImGui::BeginChild("Events", ImVec2(width, height / 3), true);

		this->Events(this->states->GetEvents(this->states->SoilFertility));

		ImGui::EndChild();
	}
}

void Application::Base::Options()
{
  ImGui::Text("Options Panel");

  float width = ImGui::GetWindowWidth() - 32.0f;
  float height = ImGui::GetWindowHeight() - 152.0f;

  {
    ImGui::BeginChild("Options", ImVec2(width, height), true);

	ImGui::Text("Logs");
	if (ImGui::Button("Clear Logs")) {
          this->states->ClearEvents();
	}
        ImGui::NewLine();
	ImGui::Text("Themes");
	ImGuiStyle *style = &ImGui::GetStyle();
    style->Colors[ImGuiCol_Button] = ImVec4(0.49f, 0.71f, 0.3f, 1.00f);
      if (ImGui::Checkbox("Dark Theme", &theme_toggle)) {
      if (!theme_toggle) {
        ImGui::StyleColorsLight();

        ImGuiStyle &style = ImGui::GetStyle();

        {
          style.GrabMinSize = 20.0f;
          style.ScrollbarSize = 20.0f;
          style.PopupBorderSize = 1.0f;
          style.ChildBorderSize = 1.0f;
          style.WindowBorderSize = 1.0f;

          style.TabRounding = 12.0f;
          style.GrabRounding = 12.0f;
          style.FrameRounding = 12.0f;
          style.ChildRounding = 12.0f;
          style.FrameRounding = 12.0f;
          style.PopupRounding = 12.0f;
          style.ScrollbarRounding = 12.0f;

          style.LogSliderDeadzone = 12.0f;

          style.ItemSpacing = ImVec2(20.0f, 10.0f);
          style.ItemInnerSpacing = ImVec2(20.0f, 4.0f);

          style.FramePadding = ImVec2(20.0f, 16.0f);
          style.WindowPadding = ImVec2(16.0f, 16.0f);

          style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
          style.Colors[ImGuiCol_TextDisabled] =
              ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
          style.Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
          style.Colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
          style.Colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.98f);
          style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.30f);
          style.Colors[ImGuiCol_BorderShadow] =
              ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
          style.Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
          style.Colors[ImGuiCol_FrameBgHovered] =
              ImVec4(0.26f, 0.98f, 0.40f, 0.40f);
          style.Colors[ImGuiCol_FrameBgActive] =
              ImVec4(0.26f, 0.98f, 0.27f, 0.67f);
          style.Colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
          style.Colors[ImGuiCol_TitleBgActive] =
              ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
          style.Colors[ImGuiCol_TitleBgCollapsed] =
              ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
          style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
          style.Colors[ImGuiCol_ScrollbarBg] =
              ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
          style.Colors[ImGuiCol_ScrollbarGrab] =
              ImVec4(0.69f, 0.69f, 0.69f, 0.80f);
          style.Colors[ImGuiCol_ScrollbarGrabHovered] =
              ImVec4(0.49f, 0.49f, 0.49f, 0.80f);
          style.Colors[ImGuiCol_ScrollbarGrabActive] =
              ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
          style.Colors[ImGuiCol_CheckMark] = ImVec4(0.07f, 0.70f, 0.10f, 1.00f);
          style.Colors[ImGuiCol_SliderGrab] =
              ImVec4(0.07f, 0.50f, 0.15f, 0.78f);
          style.Colors[ImGuiCol_SliderGrabActive] =
              ImVec4(0.46f, 0.80f, 0.57f, 0.60f);
          style.Colors[ImGuiCol_Button] = ImVec4(0.49f, 0.71f, 0.3f, 1.00f);
          style.Colors[ImGuiCol_ButtonHovered] =
              ImVec4(0.17f, 0.47f, 0.18f, 0.27f);
          style.Colors[ImGuiCol_ButtonActive] =
              ImVec4(0.24f, 0.74f, 0.05f, 0.72f);
          style.Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.72f, 0.36f, 0.31f);
          style.Colors[ImGuiCol_HeaderHovered] =
              ImVec4(0.10f, 0.67f, 0.34f, 0.80f);
          style.Colors[ImGuiCol_HeaderActive] =
              ImVec4(0.36f, 0.93f, 0.55f, 1.00f);
          style.Colors[ImGuiCol_Separator] = ImVec4(0.39f, 0.39f, 0.39f, 0.62f);
          style.Colors[ImGuiCol_SeparatorHovered] =
              ImVec4(0.10f, 0.65f, 0.18f, 0.78f);
          style.Colors[ImGuiCol_SeparatorActive] =
              ImVec4(0.14f, 0.80f, 0.16f, 1.00f);
          style.Colors[ImGuiCol_ResizeGrip] =
              ImVec4(0.35f, 0.35f, 0.35f, 0.17f);
          style.Colors[ImGuiCol_ResizeGripHovered] =
              ImVec4(0.28f, 0.98f, 0.26f, 0.67f);
          style.Colors[ImGuiCol_ResizeGripActive] =
              ImVec4(0.26f, 0.98f, 0.32f, 0.95f);
          style.Colors[ImGuiCol_Tab] = ImVec4(0.20f, 0.82f, 0.23f, 0.81f);
          style.Colors[ImGuiCol_TabHovered] =
              ImVec4(0.26f, 0.98f, 0.26f, 0.80f);
          style.Colors[ImGuiCol_TabActive] = ImVec4(0.09f, 0.65f, 0.24f, 1.00f);
          style.Colors[ImGuiCol_TabUnfocused] =
              ImVec4(0.92f, 0.93f, 0.94f, 0.99f);
          style.Colors[ImGuiCol_TabUnfocusedActive] =
              ImVec4(0.74f, 0.91f, 0.77f, 1.00f);
          style.Colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
          style.Colors[ImGuiCol_PlotLinesHovered] =
              ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
          style.Colors[ImGuiCol_PlotHistogram] =
              ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
          style.Colors[ImGuiCol_PlotHistogramHovered] =
              ImVec4(1.00f, 0.45f, 0.00f, 1.00f);
          style.Colors[ImGuiCol_TableHeaderBg] =
              ImVec4(0.78f, 0.98f, 0.82f, 1.00f);
          style.Colors[ImGuiCol_TableBorderStrong] =
              ImVec4(0.57f, 0.57f, 0.64f, 1.00f);
          style.Colors[ImGuiCol_TableBorderLight] =
              ImVec4(0.68f, 0.68f, 0.74f, 1.00f);
          style.Colors[ImGuiCol_TableRowBg] =
              ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
          style.Colors[ImGuiCol_TableRowBgAlt] =
              ImVec4(0.30f, 0.30f, 0.30f, 0.09f);
          style.Colors[ImGuiCol_TextSelectedBg] =
              ImVec4(0.26f, 0.98f, 0.30f, 0.35f);
          style.Colors[ImGuiCol_DragDropTarget] =
              ImVec4(0.26f, 0.98f, 0.42f, 0.95f);
          style.Colors[ImGuiCol_NavHighlight] =
              ImVec4(0.26f, 0.98f, 0.36f, 0.80f);
          style.Colors[ImGuiCol_NavWindowingHighlight] =
              ImVec4(0.70f, 0.70f, 0.70f, 0.70f);
          style.Colors[ImGuiCol_NavWindowingDimBg] =
              ImVec4(0.20f, 0.20f, 0.20f, 0.20f);
          style.Colors[ImGuiCol_ModalWindowDimBg] =
              ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
        }
      } else {
        ImGui::StyleColorsDark();

        ImGuiStyle &style = ImGui::GetStyle();

        {
          style.GrabMinSize = 20.0f;
          style.ScrollbarSize = 20.0f;
          style.PopupBorderSize = 1.0f;
          style.ChildBorderSize = 1.0f;
          style.WindowBorderSize = 1.0f;

          style.TabRounding = 12.0f;
          style.GrabRounding = 12.0f;
          style.FrameRounding = 12.0f;
          style.ChildRounding = 12.0f;
          style.FrameRounding = 12.0f;
          style.PopupRounding = 12.0f;
          style.ScrollbarRounding = 12.0f;

          style.LogSliderDeadzone = 12.0f;

          style.ItemSpacing = ImVec2(20.0f, 10.0f);
          style.ItemInnerSpacing = ImVec2(20.0f, 4.0f);

          style.FramePadding = ImVec2(20.0f, 16.0f);
          style.WindowPadding = ImVec2(16.0f, 16.0f);
        }
      }
    }

    ImGui::EndChild();
  }
}

inline void Application::Base::Events(std::vector<Application::States::Event> events)
{
	ImU32 cell_bg_color = ImGui::GetColorU32(ImVec4({}));

	if (ImGui::BeginTable("Events", 3, ImGuiTableFlags_RowBg)) {
		ImGui::TableSetupColumn("      Timestamp", ImGuiTableColumnFlags_WidthFixed, 364.0f);
		ImGui::TableSetupColumn("      Sensor", ImGuiTableColumnFlags_WidthFixed, 364.0f);
		ImGui::TableSetupColumn("      Message");
		ImGui::TableHeadersRow();

		for (int i = (int)events.size() - 1; i >= 0; i--) {
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);

			if (events[i].level == Application::States::Notice) {
				cell_bg_color = ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 1.0f, 0.16f));
			}
			else if (events[i].level == Application::States::Warning) {
				cell_bg_color = ImGui::GetColorU32(ImVec4(1.0f, 0.65f, 0.0f, 0.16f));
			}
			else {
				cell_bg_color = ImGui::GetColorU32(ImVec4(1.0f, 0.0f, 0.0f, 0.16f));
			}

			ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, cell_bg_color);

			ImGui::Text(" ");
			ImGui::SameLine();
			int count = (int)events[i].timestamp.length();
			events[i].timestamp[--count] = '\0';
			ImGui::TextUnformatted(events[i].timestamp.data());

			ImGui::TableSetColumnIndex(1);
			const char* sensor = "";
			switch (events[i].sensor) {
			case 0:
				sensor = "Global";
				break;
			case 1:
				sensor = "PH";
				break;
			case 2:
				sensor = u8"CO²";
				break;
			case 3:
				sensor = "UV Index";
				break;
			case 4:
				sensor = "Temperature";
				break;
			case 5:
				sensor = "Air Humidity";
				break;
			case 6:
				sensor = "Air Precipitation";
				break;
			case 7:
				sensor = "Soil Moisture";
				break;
			case 8:
				sensor = "Soil Fertility";
				break;
			}
			ImGui::Text(" ");
			ImGui::SameLine();
			ImGui::TextUnformatted(sensor);

			ImGui::TableSetColumnIndex(2);
			ImGui::Text(" ");
			ImGui::SameLine();
			ImGui::TextUnformatted(events[i].message.data());
		}

		ImGui::EndTable();
	}
}

inline void Popup()
{
	if (ImGui::BeginPopupModal("Alert", &alert_visible, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize))
	{

		const char* sensor = "";
		switch (current_alert.sensor) {
		case 0:
			sensor = "Global";
			break;
		case 1:
			sensor = "PH";
			break;
		case 2:
			sensor = u8"CO²";
			break;
		case 3:
			sensor = "UV Index";
			break;
		case 4:
			sensor = "Temperature";
			break;
		case 5:
			sensor = "Air Humidity";
			break;
		case 6:
			sensor = "Air Precipitation";
			break;
		case 7:
			sensor = "Soil Moisture";
			break;
		case 8:
			sensor = "Soil Fertility";
			break;
		}

		ImGui::TextUnformatted(current_alert.message.data());
		ImGui::NewLine();

		ImGui::EndPopup();
	}
}

Application::Base::Base(Application& application, Database* database)
{
	this->states = &application.states;
	this->m_database = database;
}

void Application::Base::Render()
{
	static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

	ImGui::SetNextWindowPos(ImGui::GetMainViewport()->WorkPos);
	ImGui::SetNextWindowSize(ImGui::GetMainViewport()->WorkSize);

	ImGui::Begin("HMI", nullptr, flags);

	if (ImGui::BeginTabBar("Panels")) {
		if (ImGui::BeginTabItem("Home")) {
			this->Home();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("PH")) {
			this->PH();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem(u8"CO²")) {
			this->CO2();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("UV Index")) {
			this->UVIndex();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Temperature")) {
			this->Temperature();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Air Humidity")) {
			this->AirHumidity();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Air Precipitation")) {
			this->Air_Precipitation();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Soil Moisture")) {
			this->SoilMoisture();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Soil Fertility")) {
			this->SoilFertility();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Options")) {
                  this->Options();
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
		ImGui::End();

		if (!this->states->alerts.empty() && !alert_visible)
		{
			alert_visible = true;
			current_alert = this->states->alerts.back();
			this->states->alerts.pop_back();
			ImGui::OpenPopup("Alert");
		}

		Popup();
	}
}

#pragma endregion

Application::Application(const char* title, Database* database)
	: Wrapper(title)
	, base(*this, database)
{
}

void Application::Render()
{
	this->NewFrame();
	this->base.Render();
	this->RenderFrame();
}

bool Application::ShouldClose()
{
	return glfwWindowShouldClose(this->window);
}

void Application::Close()
{
	this->Terminate();
}
