/*
 * application.h
 *
 * Mostafa Elbasiouny - Nov 04, 2022
 */

#pragma once

#include "backend_wrapper.h"

#include <iostream>
#include <vector>

#define RUN(APPLICATION, PREDICATE)      \
    while (!APPLICATION.ShouldClose()) { \
        APPLICATION.Render();            \
        PREDICATE                        \
    }                                    \
    APPLICATION.Close();

class Application : private Wrapper {
    struct States {
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

        int record_size = 100;

    private:
        std::vector<float> ph;
        std::vector<float> co2;
        std::vector<float> uv_index;
        std::vector<float> temperature;
        std::vector<float> air_humidity;
        std::vector<float> precipitation;
        std::vector<float> soil_moisture;
        std::vector<float> soil_fertility;

        Status ac;
        Status uv_light;
        Status humidifier;
        Status ph_regulator;
        Status water_sprinkler;

        std::vector<Event> events;

    public:
        States();

        void SetPH(Status status);
        void SetUVIndex(Status status);
        void SetAirHumidity(Status status);
        void SetTemperature(Status status);
        void SetSoilMoisture(Status status);

        void SetCO2(float value);
        void SetPrecipitation(float value);
        void SetSoilFertility(float value);
        void SetPH(float value, Status status);
        void SetUVIndex(float value, Status status);
        void SetTemperature(float value, Status status);
        void SetAirHumidity(float value, Status status);
        void SetSoilMoisture(float value, Status status);

        void SetEvent(Event event);

        std::vector<float> GetCO2();
        std::vector<float> GetPrecipitation();
        std::vector<float> GetSoilFertility();
        std::pair<std::vector<float>, Status> GetPH();
        std::pair<std::vector<float>, Status> GetUVIndex();
        std::pair<std::vector<float>, Status> GetTemperature();
        std::pair<std::vector<float>, Status> GetAirHumidity();
        std::pair<std::vector<float>, Status> GetSoilMoisture();

        std::vector<Event> GetEvents(Sensors sensor = None);

        std::pair<float, float> ph_range;
        std::pair<float, float> co2_range;
        std::pair<float, float> uv_index_range;
        std::pair<float, float> temperature_range;
        std::pair<float, float> air_humidity_range;
        std::pair<float, float> precipitation_range;
        std::pair<float, float> soil_moisture_range;
        std::pair<float, float> soil_fertility_range;

        std::vector<Event> alerts;
    };

    class Base {
        struct States* states;

        void Home();
        void PH();
        void CO2();
        void UVIndex();
        void Temperature();
        void AirHumidity();
        void Precipitation();
        void SoilMoisture();
        void SoilFertility();

        inline void Events(std::vector<Application::States::Event> events);

    public:
        Base(Application& application);

        void Render();
    };

    Base base;

public:
    Application(const char* title);

    void Render();
    States states;

    bool ShouldClose();
    void Close();
};
