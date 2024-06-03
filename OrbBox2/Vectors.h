#pragma once
#include <math.h>
//float vector
class fvector {
public:
	float x;
	float y;

	fvector(float a = 0, float b = 0) { x = a; y = b; }

	fvector operator+(const fvector& other) { return fvector(x + other.x, y + other.y); }
	void operator+=(const fvector& other) { x += other.x; y += other.y; }

	fvector operator-(const fvector& other) { return fvector(x - other.x, y - other.y); }
	void operator-=(const fvector& other) { x -= other.x; y -= other.y; }

	float distance() { return sqrt((x * x) + (y * y)); }
};

//integer vector
class ivector {
public:
	int x;
	int y;

	ivector(int a = 0, int b = 0) { x = a; y = b; }
	ivector(fvector _input) { x = static_cast<int>(_input.x); y = static_cast<int>(_input.y); }
};

