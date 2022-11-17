#pragma once
class soilhumidity
{
private:

	float value_soilhumidity;
	float max_soilhumidity;
	float min_soilhumidity;

public:

	void setMaxMin(float Max, float Min)
	{
		max_soilhumidity = Max;
		min_soilhumidity = Min;
	}
	float getvalue(void)
	{
		return value_soilhumidity;
	}
	void setvalue(float value)
	{
		value_soilhumidity = value;
	}
	float getMax(void)
	{
		return max_soilhumidity;
	}
	float getMin(void)
	{
		return min_soilhumidity;
	}
};

float generateRandomValue(void);
int checkValid(soilhumidity mainsensor);