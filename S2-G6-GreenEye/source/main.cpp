#include "globals.h"

void WinMain()
{
	Application application(u8"Green Eye 🌱");

	INITIALIZE(PH, ph);
	INITIALIZE(CO2, co2);
	INITIALIZE(UVIndex, uv_index);
	INITIALIZE(Temperature, temperature);
	INITIALIZE(AirHumidity, air_humidity);
	INITIALIZE(SoilMoisture, soil_moisture);
	INITIALIZE(Precipitation, precipitation);
	INITIALIZE(SoilFertility, soil_fertility);

	RUN(application,
		{
			ph->Run();
			co2->Run();
			uv_index->Run();
			temperature->Run();
			air_humidity->Run();
			soil_moisture->Run();
			precipitation->Run();
			soil_fertility->Run();

			// Application flow goes here.
		});

	DELETE(ph);
	DELETE(co2);
	DELETE(uv_index);
	DELETE(temperature);
	DELETE(air_humidity);
	DELETE(soil_moisture);
	DELETE(precipitation);
	DELETE(soil_fertility);

	DELETE(database);
}
