#ifndef USR_MATH_H
#define USR_MATH_H

#include "math.h"

#define PI 3.1416f

float normpdf(float x, float mu, float sigma);
float normpdf_revised(float x, float mu, float sigma);
void absLimit(float *num, float Limit);

#endif
