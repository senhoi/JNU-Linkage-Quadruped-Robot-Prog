#include "usr_math.h"

float normpdf(float x, float mu, float sigma)
{
	return 1.0f / (sigma * sqrt(2 * PI)) * exp(-(x - mu) * (x - mu) / (2 * sigma * sigma));
}

void absLimit(float *num, float Limit)
{
	if (*num > Limit)
	{
		*num = Limit;
	}
	else if (*num < -Limit)
	{
		*num = -Limit;
	}
}

const float ACC_TIME = 0.1f;
const float CUT_OFF_VEL = 0.4f;
float normpdf_revised(float x, float mu, float sigma)
{
	float temp = 1.0f / (sigma * sqrt(2 * PI)) * exp(-(x - mu) * (x - mu) / (2 * sigma * sigma));

	if (mu < 0)
	{
		if (x < mu)
		{
			if (x + 1 < ACC_TIME)
				return (1.0f - CUT_OFF_VEL) / ACC_TIME * (x + 1) + CUT_OFF_VEL;
			else
				return 1.0f;
		}
		else
		{
			if (temp < CUT_OFF_VEL)
				return CUT_OFF_VEL;
			else
				return temp;
		}
	}
	else
	{
		if (x > mu)
			return 1.0f;
		else
		{
			if (temp < CUT_OFF_VEL)
				return CUT_OFF_VEL;
			else
				return temp;
		}
	}
}
