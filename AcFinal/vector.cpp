#include "vector.h"
#include <math.h>

float DistanceVec3(vec3 src, vec3 dst){
	return sqrtf(powf(dst.x - src.x, 2) + powf(dst.y - src.y, 2) + powf(dst.z - src.z, 2));
}

#define PI 3.14159265358979323846
vec3 CalcAngle(vec3 src, vec3 dst) {
	vec3 angle;
	angle.x = -atan2f(dst.x - src.x, dst.y - src.y) / (float)PI * 180 + 180;
	angle.y = asinf((dst.z - src.z) / DistanceVec3(src, dst)) * 180 / (float)PI;
	angle.z = 0;

	return angle;
}