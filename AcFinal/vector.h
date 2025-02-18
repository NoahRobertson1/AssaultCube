#pragma once

struct vec4 {
	float x, y, z, w;
};

struct vec3 {
	float x, y, z;
};

struct v3ptr {
	float* x;
	float* y;
	float* z;
};

struct vec2 {
	float x, y;
};

struct v2ptr {
	float* x;
	float* y;
};

float DistanceVec3(vec3 src, vec3 dst);
vec3 CalcAngle(vec3 src, vec3 dst);