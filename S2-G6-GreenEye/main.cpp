#include "co2.h"
#include <iostream>

int main(void)
{
	co2 mainSensor;
	float co2_max = 100;
	float co2_min = 0;

	mainSensor.setvalueCO(generateRandomValue());
	mainSensor.setMaxMin(co2_max, co2_min);

	float co2value = mainSensor.getvalueCO();

	int event = checkValid(mainSensor);

	if (event == 0)
	{
		if (co2value >= (85 / 100 * mainSensor.getMax()))
		{
			//WARNING THAT IT IS CLOSE TOO HIGH
		}
		else if (co2value <= (15 / 100 * mainSensor.getMin()))
		{
			//WARNING THAT IT IS CLOSE TOO LOW
		}
		return 0;
	}
	else if (event == 1)
	{
		//EVENT THAT VALUE IS TOO HIGH
	}
	else if (event == -1)
	{
		//EVENT THAT VALUE IS TOO LOW
	}
	else
	{
		//UNDEFINED ERROR
	}
	return 0;
}