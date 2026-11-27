#ifndef MATH_H
#define MATH_H

#define M_PI 3.14159265358979323846
#define MAXFLOAT ((float)3.40282346638528860e+38) /* as in IRIX <math.h>; a float literal gives cfe warning 763 */

float sinf(float);
double sin(double);
float cosf(float);
double cos(double);

float sqrtf(float);

#endif
