#pragma once
class precipitation
{
private:

	float value_precipitation;
	float max_precipitation;
	float min_precipitation;

public:

	void setMaxMin(float Max, float Min)
	{
		max_precipitation = Max;
		min_precipitation = Min;
	}
	float getvalueCO(void)
	{
		return max_precipitation;
	}
	void setvalueCO(float value)
	{
		value_precipitation = value;
	}
	float getMax(void)
	{
		return value_precipitation;
	}
	float getMin(void)
	{
		return min_precipitation;
	}
};

float generateRandomValue(void);
int checkValid(precipitation mainsensor);

