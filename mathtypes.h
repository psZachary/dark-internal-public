#pragma once

#include <math.h>
#include <cstdint>
#include <d2d1.h>
#include "pch.h"
#include <SDK.h>
typedef unsigned __int64 ULONGLONG;
struct color_m {
	float r, g, b, a;
	operator D2D1::ColorF() const { return D2D1::ColorF{r, g, b, a}; }
};
struct transform_access_read_only_t
{
	ULONGLONG transform_data;
	int			index;
};
struct transform_data_t
{
	ULONGLONG transform_array;
	ULONGLONG transform_indices;
};
class vec4_t
{
public:
	float x;
	float y;
	float z;
	float w;
};
struct matrix3x4_t
{
	vec4_t vec0{};
	vec4_t vec1{};
	vec4_t vec2{};
};
struct matrix4x4_t
{
	matrix4x4_t()
		: m{ { 0, 0, 0, 0 },
			 { 0, 0, 0, 0 },
			 { 0, 0, 0, 0 },
			 { 0, 0, 0, 0 } }
	{}

	matrix4x4_t(const matrix4x4_t&) = default;

	union {
		struct {
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];
	};
};
struct vector2 {
public:
	float x, y;
	inline float magnitude() {
		return (float)sqrt(pow(x, 2) + pow(y, 2));
	}
	operator CG::FVector2D const() {
		return CG::FVector2D(x, y);
	}
	vector2 operator/ (int other) {
		auto res = vector2();
		res.x = this->x / other;
		res.y = this->y / other;
		return res;
	}
	vector2 operator- (vector2 other) {
		auto res = vector2();
		res.x = this->x - other.x;
		res.y = this->y - other.y;
		return res;
	}	
	vector2 operator+ (vector2 other) {
		auto res = vector2();
		res.x = this->x + other.x;
		res.y = this->y + other.y;
		return res;
	}
	vector2 operator+ (float other) {
		auto res = vector2();
		res.x = this->x + other;
		res.y = this->y + other;
		return res;
	}	
	vector2 operator- (float other) {
		auto res = vector2();
		res.x = this->x - other;
		res.y = this->y - other;
		return res;
	}
	bool operator== (vector2 other) {
		if (other.x == this->x && other.y == this->y) return true;
		return false;
	}	
	bool operator< (vector2 other) {
		if (this->x < other.x && this->y < other.y) return true;
		return false;
	}
};
struct vector3 {
public:
	float x, y, z;
	inline float magnitude() {
		return (float)sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	}
	vector3 operator- (vector3 other) {
		auto res = vector3();
		res.x = this->x - other.x;
		res.y = this->y - other.y;
		res.z = this->z - other.z;
		return res;
	}	
	vector3 operator* (float other) {
		auto res = vector3();
		res.x = this->x * other;
		res.y = this->y * other;
		res.z = this->z * other;
		return res;
	}
	vector3 operator+ (vector3 other) {
		auto res = vector3();
		res.x = this->x + other.x;
		res.y = this->y + other.y;
		res.z = this->z + other.z;
		return res;
	}
	inline float dot(vector3 other)
	{
		return x * other.x + y * other.y + z * other.z;
	}
	bool operator== (float other) {
		if (other == this->x && other == this->y && other == this->z) return true;
		return false;
	}
	bool operator== (vector3 other) {
		if (other.x == this->x && other.y == this->y && other.z == this->z) return true;
		return false;
	}
	vector3 operator+ (float other) {
		auto res = vector3();
		res.x = this->x + other;
		res.y = this->y + other;
		res.z = this->z + other;
		return res;
	}
	static vector3 zero() {
		return { 0.f,0.f,0.f };
	}
};

struct box {
	CG::FVector2D position;
	double width;
	double height;
};