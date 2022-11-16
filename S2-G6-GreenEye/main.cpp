#include "precipitation.h"


int main(void)
{
	precipitation mainSensor;
	float precipitation_max = 7500;
	float precipitation_min = 2500;

	mainSensor.setvalueCO(generateRandomValue());
	mainSensor.setMaxMin(precipitation_max, precipitation_min);

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