#include "soilfertility.h"
#include <iostream>

int main(void)
{
	soilfertility mainSensor;
	float max_soilfertility = 100;
	float min_soilfertility = 0;

	mainSensor.setvalue(generateRandomValue());
	mainSensor.setMaxMin(max_soilfertility, min_soilfertility);

	float co2value = mainSensor.getvalue();

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