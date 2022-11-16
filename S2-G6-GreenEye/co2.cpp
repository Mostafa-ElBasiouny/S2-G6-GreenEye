#include "co2.h"
#include <cstdlib>

int checkValid(co2 mainsensor)
{
	float value = mainsensor.getvalueCO();
	float max = mainsensor.getMax();
	float min = mainsensor.getMin();

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