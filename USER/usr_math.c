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

float normpdf_revised(float x, float mu, float sigma)
{
	float temp = 1.0f / (sigma * sqrt(2 * PI)) * exp(-(x - mu) * (x - mu) / (2 * sigma * sigma));
	if (mu < 0)
	{
		if (x < mu)
			return 1.0f;
		else
		{
			if (temp < 0.2f)
				return 0.2f;
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
			if (temp < 0.2f)
				return 0.2f;
			else
				return temp;
		}
	}
}
