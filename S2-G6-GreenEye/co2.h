#pragma once
class co2
{
private:

	float valueCO;
	float valueCOMax;
	float valueCOMin;

public:

	void setMaxMin(float COMax, float COMin)
	{
		valueCOMax = COMax;
		valueCOMin = COMin;
	}
	float getvalueCO(void)
	{
		return valueCOMax;
	}
	void setvalueCO(float value)
	{
		valueCO = value;
	}
	float getMax(void)
	{
		return valueCO;
	}
	float getMin(void)
	{
		return valueCOMin;
	}
};

float generateRandomValue(void);
int checkValid(co2 mainsensor);