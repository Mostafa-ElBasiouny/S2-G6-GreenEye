#include "soilhumidity.h"
#include <iostream>

int main(void)
{
	soilhumidity mainSensor;
	float max = 100;
	float min = 0;

	mainSensor.setvalue(generateRandomValue());
	mainSensor.setMaxMin(max, min);

	float value = mainSensor.getvalue();

	int event = checkValid(mainSensor);

	if (event == 0)
	{
		if (value >= (85 / 100 * mainSensor.getMax()))
		{
			//WARNING THAT IT IS CLOSE TOO HIGH
		}
		else if (value <= (15 / 100 * mainSensor.getMin()))
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