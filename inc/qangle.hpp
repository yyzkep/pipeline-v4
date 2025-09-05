//
// Created by drev on 15/08/2025.
//

#pragma once
// used: clamp
#include <algorithm>
// used: isfinite, fmodf, remainderf
#include <cmath>

class q_angle_t
{
public:
	constexpr q_angle_t(float x = 0.f, float y = 0.f, float z = 0.f) :
		x(x), y(y), z(z) { }

	constexpr q_angle_t(const float* arrAngles) :
		x(arrAngles[0]), y(arrAngles[1]), z(arrAngles[2]) { }

	bool operator==(const q_angle_t& angBase) const
	{
		return this->is_equal(angBase);
	}

	bool operator!=(const q_angle_t& angBase) const
	{
		return !this->is_equal(angBase);
	}

	constexpr q_angle_t& operator=(const q_angle_t& angBase)
	{
		this->x = angBase.x; this->y = angBase.y; this->z = angBase.z;
		return *this;
	}

	constexpr q_angle_t& operator+=(const q_angle_t& angBase)
	{
		this->x += angBase.x; this->y += angBase.y; this->z += angBase.z;
		return *this;
	}

	constexpr q_angle_t& operator-=(const q_angle_t& angBase)
	{
		this->x -= angBase.x; this->y -= angBase.y; this->z -= angBase.z;
		return *this;
	}

	constexpr q_angle_t& operator*=(const q_angle_t& angBase)
	{
		this->x *= angBase.x; this->y *= angBase.y; this->z *= angBase.z;
		return *this;
	}

	constexpr q_angle_t& operator/=(const q_angle_t& angBase)
	{
		this->x /= angBase.x; this->y /= angBase.y; this->z /= angBase.z;
		return *this;
	}

	constexpr q_angle_t& operator+=(const float flAdd)
	{
		this->x += flAdd; this->y += flAdd; this->z += flAdd;
		return *this;
	}

	constexpr q_angle_t& operator-=(const float flSubtract)
	{
		this->x -= flSubtract; this->y -= flSubtract; this->z -= flSubtract;
		return *this;
	}

	constexpr q_angle_t& operator*=(const float flMultiply)
	{
		this->x *= flMultiply; this->y *= flMultiply; this->z *= flMultiply;
		return *this;
	}

	constexpr q_angle_t& operator/=(const float flDivide)
	{
		this->x /= flDivide; this->y /= flDivide; this->z /= flDivide;
		return *this;
	}

	q_angle_t operator+(const q_angle_t& angAdd) const
	{
		return q_angle_t(this->x + angAdd.x, this->y + angAdd.y, this->z + angAdd.z);
	}

	q_angle_t operator-(const q_angle_t& angSubtract) const
	{
		return q_angle_t(this->x - angSubtract.x, this->y - angSubtract.y, this->z - angSubtract.z);
	}

	q_angle_t operator*(const q_angle_t& angMultiply) const
	{
		return q_angle_t(this->x * angMultiply.x, this->y * angMultiply.y, this->z * angMultiply.z);
	}

	q_angle_t operator/(const q_angle_t& angDivide) const
	{
		return q_angle_t(this->x / angDivide.x, this->y / angDivide.y, this->z / angDivide.z);
	}

	q_angle_t operator+(const float flAdd) const
	{
		return q_angle_t(this->x + flAdd, this->y + flAdd, this->z + flAdd);
	}

	q_angle_t operator-(const float flSubtract) const
	{
		return q_angle_t(this->x - flSubtract, this->y - flSubtract, this->z - flSubtract);
	}

	q_angle_t operator*(const float flMultiply) const
	{
		return q_angle_t(this->x * flMultiply, this->y * flMultiply, this->z * flMultiply);
	}

	q_angle_t operator/(const float flDivide) const
	{
		return q_angle_t(this->x / flDivide, this->y / flDivide, this->z / flDivide);
	}

	[[nodiscard]] bool is_equal(const q_angle_t& angEqual, const float flErrorMargin = std::numeric_limits<float>::epsilon()) const
	{
		return (std::fabs(this->x - angEqual.x) < flErrorMargin &&
			std::fabs(this->y - angEqual.y) < flErrorMargin &&
			std::fabs(this->z - angEqual.z) < flErrorMargin);
	}

	[[nodiscard]] bool is_zero() const
	{
		return (std::fpclassify(this->x) == FP_ZERO &&
			std::fpclassify(this->y) == FP_ZERO &&
			std::fpclassify(this->z) == FP_ZERO);
	}

	q_angle_t clamp()
	{
		this->x = std::clamp(this->x, -89.f, 89.f);
		this->y = std::clamp(this->y, -180.f, 180.f);
		this->z = std::clamp(this->z, -50.f, 50.f);
		return *this;
	}

	q_angle_t normalize()
	{
		this->x = std::isfinite(this->x) ? std::remainder(this->x, 360.f) : 0.f;
		this->y = std::isfinite(this->y) ? std::remainder(this->y, 360.f) : 0.f;
		this->z = std::clamp(this->z, -50.f, 50.f);
		return *this;
	}

	q_angle_t mod(const float flValue)
	{
		this->x = std::fmod(this->x, flValue);
		this->y = std::fmod(this->y, flValue);
		this->z = std::fmod(this->z, flValue);
		return *this;
	}

public:
	float x, y, z;
};