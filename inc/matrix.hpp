#pragma once
#include <xmmintrin.h>
#include <bit>
#include "vector.hpp"

// forward declarations
struct q_angle_t;

using matrix3x3_t = float[3][3];

struct matrix3x4_t
{
	matrix3x4_t() = default;

	constexpr matrix3x4_t(
		const float m00, const float m01, const float m02, const float m03,
		const float m10, const float m11, const float m12, const float m13,
		const float m20, const float m21, const float m22, const float m23)
	{
		arrData[0][0] = m00; arrData[0][1] = m01; arrData[0][2] = m02; arrData[0][3] = m03;
		arrData[1][0] = m10; arrData[1][1] = m11; arrData[1][2] = m12; arrData[1][3] = m13;
		arrData[2][0] = m20; arrData[2][1] = m21; arrData[2][2] = m22; arrData[2][3] = m23;
	}

	constexpr matrix3x4_t(const vector_t& vecForward, const vector_t& vecLeft, const vector_t& vecUp, const vector_t& vecOrigin)
	{
		SetForward(vecForward);
		SetLeft(vecLeft);
		SetUp(vecUp);
		SetOrigin(vecOrigin);
	}

	[[nodiscard]] float* operator[](const int nIndex)
	{
		return arrData[nIndex];
	}

	[[nodiscard]] const float* operator[](const int nIndex) const
	{
		return arrData[nIndex];
	}

	constexpr void SetForward(const vector_t& vecForward)
	{
		arrData[0][0] = vecForward.x;
		arrData[1][0] = vecForward.y;
		arrData[2][0] = vecForward.z;
	}

	constexpr void SetLeft(const vector_t& vecLeft)
	{
		arrData[0][1] = vecLeft.x;
		arrData[1][1] = vecLeft.y;
		arrData[2][1] = vecLeft.z;
	}

	constexpr void SetUp(const vector_t& vecUp)
	{
		arrData[0][2] = vecUp.x;
		arrData[1][2] = vecUp.y;
		arrData[2][2] = vecUp.z;
	}

	constexpr void SetOrigin(const vector_t& vecOrigin)
	{
		arrData[0][3] = vecOrigin.x;
		arrData[1][3] = vecOrigin.y;
		arrData[2][3] = vecOrigin.z;
	}

	[[nodiscard]] constexpr vector_t GetForward() const
	{
		return { arrData[0][0], arrData[1][0], arrData[2][0] };
	}

	[[nodiscard]] constexpr vector_t GetLeft() const
	{
		return { arrData[0][1], arrData[1][1], arrData[2][1] };
	}

	[[nodiscard]] constexpr vector_t GetUp() const
	{
		return { arrData[0][2], arrData[1][2], arrData[2][2] };
	}

	[[nodiscard]] constexpr vector_t GetOrigin() const
	{
		return { arrData[0][3], arrData[1][3], arrData[2][3] };
	}

	constexpr void Invalidate()
	{
		for (auto& arrSubData : arrData)
		{
			for (auto& flData : arrSubData)
				flData = std::numeric_limits<float>::infinity();
		}
	}

	/// concatenate transformations of two matrices into one
	/// @returns: matrix with concatenated transformations
	[[nodiscard]] constexpr matrix3x4_t ConcatTransforms(const matrix3x4_t& matOther) const
	{
		return
		{
			arrData[0][0] * matOther.arrData[0][0] + arrData[0][1] * matOther.arrData[1][0] + arrData[0][2] * matOther.arrData[2][0],
			arrData[0][0] * matOther.arrData[0][1] + arrData[0][1] * matOther.arrData[1][1] + arrData[0][2] * matOther.arrData[2][1],
			arrData[0][0] * matOther.arrData[0][2] + arrData[0][1] * matOther.arrData[1][2] + arrData[0][2] * matOther.arrData[2][2],
			arrData[0][0] * matOther.arrData[0][3] + arrData[0][1] * matOther.arrData[1][3] + arrData[0][2] * matOther.arrData[2][3] + arrData[0][3],

			arrData[1][0] * matOther.arrData[0][0] + arrData[1][1] * matOther.arrData[1][0] + arrData[1][2] * matOther.arrData[2][0],
			arrData[1][0] * matOther.arrData[0][1] + arrData[1][1] * matOther.arrData[1][1] + arrData[1][2] * matOther.arrData[2][1],
			arrData[1][0] * matOther.arrData[0][2] + arrData[1][1] * matOther.arrData[1][2] + arrData[1][2] * matOther.arrData[2][2],
			arrData[1][0] * matOther.arrData[0][3] + arrData[1][1] * matOther.arrData[1][3] + arrData[1][2] * matOther.arrData[2][3] + arrData[1][3],

			arrData[2][0] * matOther.arrData[0][0] + arrData[2][1] * matOther.arrData[1][0] + arrData[2][2] * matOther.arrData[2][0],
			arrData[2][0] * matOther.arrData[0][1] + arrData[2][1] * matOther.arrData[1][1] + arrData[2][2] * matOther.arrData[2][1],
			arrData[2][0] * matOther.arrData[0][2] + arrData[2][1] * matOther.arrData[1][2] + arrData[2][2] * matOther.arrData[2][2],
			arrData[2][0] * matOther.arrData[0][3] + arrData[2][1] * matOther.arrData[1][3] + arrData[2][2] * matOther.arrData[2][3] + arrData[2][3]
		};
	}

	/// @returns: angles converted from this matrix
	[[nodiscard]] q_angle_t ToAngles() const;

	float arrData[3][4] = { };
};