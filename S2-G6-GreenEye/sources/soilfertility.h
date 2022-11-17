class soilfertility
{
private:

	float value_soilfertility;
	float max_soilfertility;
	float min_soilfertility;

public:

	void setMaxMin(float Max, float Min)
	{
		max_soilfertility = Max;
		min_soilfertility = Min;
	}
	float getvalue(void)
	{
		return value_soilfertility;
	}
	void setvalue(float value)
	{
		value_soilfertility = value;
	}
	float getMax(void)
	{
		return max_soilfertility;
	}
	float getMin(void)
	{
		return min_soilfertility;
	}
};

float generateRandomValue(void);
int checkValid(soilfertility mainsensor);