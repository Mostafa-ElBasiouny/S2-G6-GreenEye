/*
 * application.cpp
 *
 * Mostafa Elbasiouny - Nov 04, 2022
 */

#include "application.h"

#pragma region States

inline void VectorAppend(std::vector<float>* vector, float value)
{
	vector->push_back(value);
	vector->erase(vector->begin());
}

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
	std::fill_n(std::back_inserter(this->precipitation), this->record_size, (this->precipitation_range.first + this->precipitation_range.second) / 2.0f);
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

void Application::States::SetCO2(float value)
{
	VectorAppend(&this->co2, value);
}

void Application::States::SetPrecipitation(float value)
{
	VectorAppend(&this->precipitation, value);
}

void Application::States::SetSoilFertility(float value)
{
	VectorAppend(&this->soil_fertility, value);
}

void Application::States::SetPH(float value, Status status)
{
	VectorAppend(&this->ph, value);
	this->ph_regulator = status;
}

void Application::States::SetUVIndex(float value, Status status)
{
	VectorAppend(&this->uv_index, value);
	this->uv_light = status;
}

void Application::States::SetTemperature(float value, Status status)
{
	VectorAppend(&this->temperature, value);
	this->ac = status;
}

void Application::States::SetAirHumidity(float value, Status status)
{
	VectorAppend(&this->air_humidity, value);
	this->humidifier = status;
}

void Application::States::SetSoilMoisture(float value, Status status)
{
	VectorAppend(&this->soil_moisture, value);
	this->water_sprinkler = status;
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
	return this->precipitation;
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

#pragma endregion

#pragma region Base

bool alert_visible = true;

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
			ImPlot::SetupAxes("Sample", "Change Over Time");
			ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
			ImPlot::PlotBars("PH", RangedPlot(this->states->GetPH().first, (this->states->ph_range.first + this->states->ph_range.second) / 2.0f).data(), this->states->record_size);
			ImPlot::PlotBars(u8"CO²", RangedPlot(this->states->GetCO2(), (this->states->co2_range.first + this->states->co2_range.second) / 2.0f).data(), this->states->record_size);
			ImPlot::PlotBars("UV Index", RangedPlot(this->states->GetUVIndex().first, (this->states->uv_index_range.first + this->states->uv_index_range.second) / 2.0f).data(), this->states->record_size);
			ImPlot::PlotBars("Temperature", RangedPlot(this->states->GetTemperature().first, (this->states->temperature_range.first + this->states->temperature_range.second) / 2.0f).data(), this->states->record_size);
			ImPlot::PlotBars("Air Humidity", RangedPlot(this->states->GetAirHumidity().first, (this->states->air_humidity_range.first + this->states->air_humidity_range.second) / 2.0f).data(), this->states->record_size);
			ImPlot::PlotBars("Precipitation", RangedPlot(this->states->GetPrecipitation(), (this->states->precipitation_range.first + this->states->precipitation_range.second) / 2.0f).data(), this->states->record_size);
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
			this->states->SetEvent({ time(0), this->states->Warning, this->states->PH, action == "Enable" ? "PH Regulator: Enabled by user." : "PH Regulator: Disabled by user." });
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
			ImPlot::SetupAxes("Sample", "Parts Per Million (PPM)");
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
			this->states->SetEvent({ time(0), this->states->Warning, this->states->UVIndex, action == "Enable" ? "UV Light: Enabled by user." : "UV Light: Disabled by user." });
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
			ImPlot::SetupAxes("Sample", u8"Celsius (°C)");
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
			this->states->SetEvent({ time(0), this->states->Warning, this->states->Temperature, action == "Enable" ? "AC: Enabled by user." : "AC: Disabled by user." });
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
			ImPlot::SetupAxes("Sample", "Percentage (%)");
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
			this->states->SetEvent({ time(0), this->states->Warning, this->states->AirHumidity, action == "Enable" ? "Humidifier: Enabled by user." : "Humidifier: Disabled by user." });
		}

		ImGui::EndChild();
	}

	{
		ImGui::BeginChild("Events", ImVec2(width, height / 3), true);

		this->Events(this->states->GetEvents(this->states->AirHumidity));

		ImGui::EndChild();
	}
}

void Application::Base::Precipitation()
{
	ImGui::Text("Precipitation Panel");

	float width = ImGui::GetWindowWidth() - 32.0f;
	float height = ImGui::GetWindowHeight() - 152.0f;

	{
		ImGui::BeginChild("Plots", ImVec2(width, height / 3), true);

		ImVec2 plot_size = ImVec2(width - 32.0f, (height / 3) - 32.0f);
		static ImPlotFlags flags = ImPlotFlags_NoTitle | ImPlotFlags_NoLegend | ImPlotFlags_NoMouseText | ImPlotFlags_NoInputs | ImPlotFlags_NoMenus | ImPlotFlags_NoBoxSelect | ImPlotFlags_NoChild;

		if (ImPlot::BeginPlot("Plots", plot_size, flags)) {
			ImPlot::SetupAxes("Sample", "Millimeter (MM)");
			ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.5f);
			ImPlot::PlotScatter("Precipitation", this->states->GetPrecipitation().data(), this->states->record_size);
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

		this->Events(this->states->GetEvents(this->states->Precipitation));

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
			ImPlot::SetupAxes("Sample", "Percentage (%)");
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
			this->states->SetEvent({ time(0), this->states->Warning, this->states->SoilMoisture, action == "Enable" ? "Water Sprinkler: Enabled by user." : "Water Sprinkler: Disabled by user." });
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
			ImPlot::SetupAxes("Sample", "Percentage (%)");
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

inline void Application::Base::Events(std::vector<Application::States::Event> events)
{
	ImU32 cell_bg_color = ImGui::GetColorU32(ImVec4({}));

	if (ImGui::BeginTable("Events", 3, ImGuiTableFlags_RowBg)) {
		ImGui::TableSetupColumn(" Timestamp", ImGuiTableColumnFlags_WidthFixed, 364.0f);
		ImGui::TableSetupColumn(" Sensor", ImGuiTableColumnFlags_WidthFixed, 364.0f);
		ImGui::TableSetupColumn(" Message");
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

			ImGui::Text(" %s", ctime(&events[i].timestamp));

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
				sensor = "Precipitation";
				break;
			case 7:
				sensor = "Soil Moisture";
				break;
			case 8:
				sensor = "Soil Fertility";
				break;
			}
			ImGui::Text(" %s", sensor);

			ImGui::TableSetColumnIndex(2);
			ImGui::Text(" %s", events[i].message);
		}

		ImGui::EndTable();
	}
}

Application::Base::Base(Application& application)
{
	this->states = &application.states;
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

		if (ImGui::BeginTabItem("Precipitation")) {
			this->Precipitation();
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

		ImGui::EndTabBar();
	}

	ImGui::End();

	if (this->states->alerts.size() > 0)
		alert_visible = true;

	for (auto& alert : this->states->alerts) {
		const char* sensor = "";
		switch (alert.sensor) {
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
			sensor = "Precipitation";
			break;
		case 7:
			sensor = "Soil Moisture";
			break;
		case 8:
			sensor = "Soil Fertility";
			break;
		}

		ImGui::BeginPopupModal("Alert", &alert_visible, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);

		ImGui::Text("%s: %s", sensor, alert.message);
		ImGui::NewLine();

		ImGui::OpenPopup("Alert");
		ImGui::EndPopup();

		if (!alert_visible)
			this->states->alerts.clear();
	}
}

#pragma endregion

Application::Application(const char* title)
	: Wrapper(title)
	, base(*this)
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
