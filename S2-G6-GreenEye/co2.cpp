#include "co2.h"
#include <cstdlib>

co2 co2_sensor;

int checkValid(float valueCO2)
{
	float value = co2_sensor.getvalueCO();
	float max = co2_sensor.getMax();
	float min = co2_sensor.getMin();

	if (value > max)
	{
		return 1;
	}
	else if (value < min)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

float generateRandomValue(void)
{
	float random = (rand() % 100);
	return random;
}