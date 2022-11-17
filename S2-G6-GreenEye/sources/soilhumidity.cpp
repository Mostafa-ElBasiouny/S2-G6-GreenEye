#include "soilhumidity.h"
#include <cstdlib>

int checkValid(soilhumidity mainsensor)
{
	float value = mainsensor.getvalue();
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
	float random = rand() % (100 - 80 + 1) + 80;
	return random;
}