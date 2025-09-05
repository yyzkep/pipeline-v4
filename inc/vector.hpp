#pragma once
#include <limits>
#include <cmath>

// forward declarations
struct q_angle_t;
struct matrix3x4_t;

// thankb volvo
//master/public/mathlib/vector.h

struct vector2d_t
{
	constexpr vector2d_t(const float x = 0.0f, const float y = 0.0f) :
		x(x), y(y) {
	}

	[[nodiscard]] bool IsZero() const
	{
		// @note: to make this implementation right, we should use fpclassify here, but game aren't doing same, probably it's better to keep this same, just ensure that it will be compiled same
		return (this->x == 0.0f && this->y == 0.0f);
	}

	float x = 0.0f, y = 0.0f;
};

struct vector_t
{
	constexpr vector_t(const float x = 0.0f, const float y = 0.0f, const float z = 0.0f) :
		x(x), y(y), z(z) {
	}

	constexpr vector_t(const float* arrVector) :
		x(arrVector[0]), y(arrVector[1]), z(arrVector[2]) {
	}

	constexpr vector_t(const vector2d_t& vecBase2D) :
		x(vecBase2D.x), y(vecBase2D.y) {
	}

#pragma region vector_array_operators
	[[nodiscard]] float& operator[](const int nIndex)
	{
		return reinterpret_cast<float*>(this)[nIndex];
	}

	[[nodiscard]] const float& operator[](const int nIndex) const
	{
		return reinterpret_cast<const float*>(this)[nIndex];
	}
#pragma endregion

#pragma region vector_relational_operators
	bool operator==(const vector_t& vecBase) const
	{
		return this->is_equal(vecBase);
	}

	bool operator!=(const vector_t& vecBase) const
	{
		return !this->is_equal(vecBase);
	}
#pragma endregion

#pragma region vector_assignment_operators
	constexpr vector_t& operator=(const vector_t& vecBase)
	{
		this->x = vecBase.x; this->y = vecBase.y; this->z = vecBase.z;
		return *this;
	}

	constexpr vector_t& operator=(const vector2d_t& vecBase2D)
	{
		this->x = vecBase2D.x; this->y = vecBase2D.y; this->z = 0.0f;
		return *this;
	}
#pragma endregion

#pragma region vector_arithmetic_assignment_operators
	constexpr vector_t& operator+=(const vector_t& vecBase)
	{
		this->x += vecBase.x; this->y += vecBase.y; this->z += vecBase.z;
		return *this;
	}

	constexpr vector_t& operator-=(const vector_t& vecBase)
	{
		this->x -= vecBase.x; this->y -= vecBase.y; this->z -= vecBase.z;
		return *this;
	}

	constexpr vector_t& operator*=(const vector_t& vecBase)
	{
		this->x *= vecBase.x; this->y *= vecBase.y; this->z *= vecBase.z;
		return *this;
	}

	constexpr vector_t& operator/=(const vector_t& vecBase)
	{
		this->x /= vecBase.x; this->y /= vecBase.y; this->z /= vecBase.z;
		return *this;
	}

	constexpr vector_t& operator+=(const float flAdd)
	{
		this->x += flAdd; this->y += flAdd; this->z += flAdd;
		return *this;
	}

	constexpr vector_t& operator-=(const float flSubtract)
	{
		this->x -= flSubtract; this->y -= flSubtract; this->z -= flSubtract;
		return *this;
	}

	constexpr vector_t& operator*=(const float flMultiply)
	{
		this->x *= flMultiply; this->y *= flMultiply; this->z *= flMultiply;
		return *this;
	}

	constexpr vector_t& operator/=(const float flDivide)
	{
		this->x /= flDivide; this->y /= flDivide; this->z /= flDivide;
		return *this;
	}
#pragma endregion

#pragma region vector_arithmetic_unary_operators
	constexpr vector_t& operator-()
	{
		this->x = -this->x;
		this->y = -this->y;
		this->z = -this->z;
		return *this;
	}

	constexpr vector_t operator-() const
	{
		return { -this->x, -this->y, -this->z };
	}
#pragma endregion

#pragma region vector_arithmetic_ternary_operators
	vector_t operator+(const vector_t& vecAdd) const
	{
		return { this->x + vecAdd.x, this->y + vecAdd.y, this->z + vecAdd.z };
	}

	vector_t operator-(const vector_t& vecSubtract) const
	{
		return { this->x - vecSubtract.x, this->y - vecSubtract.y, this->z - vecSubtract.z };
	}

	vector_t operator*(const vector_t& vecMultiply) const
	{
		return { this->x * vecMultiply.x, this->y * vecMultiply.y, this->z * vecMultiply.z };
	}

	vector_t operator/(const vector_t& vecDivide) const
	{
		return { this->x / vecDivide.x, this->y / vecDivide.y, this->z / vecDivide.z };
	}

	vector_t operator+(const float flAdd) const
	{
		return { this->x + flAdd, this->y + flAdd, this->z + flAdd };
	}

	vector_t operator-(const float flSubtract) const
	{
		return { this->x - flSubtract, this->y - flSubtract, this->z - flSubtract };
	}

	vector_t operator*(const float flMultiply) const
	{
		return { this->x * flMultiply, this->y * flMultiply, this->z * flMultiply };
	}

	vector_t operator/(const float flDivide) const
	{
		return { this->x / flDivide, this->y / flDivide, this->z / flDivide };
	}
#pragma endregion

	/// @returns: true if each component of the vector is finite, false otherwise
	[[nodiscard]] bool is_valid() const
	{
		return std::isfinite(this->x) && std::isfinite(this->y) && std::isfinite(this->z);
	}

	constexpr void invalidate()
	{
		this->x = this->y = this->z = std::numeric_limits<float>::infinity();
	}

	/// @returns: true if each component of the vector equals to another, false otherwise
	[[nodiscard]] bool is_equal(const vector_t& vecEqual, const float flErrorMargin = std::numeric_limits<float>::epsilon()) const
	{
		return (std::fabs(this->x - vecEqual.x) < flErrorMargin && std::fabs(this->y - vecEqual.y) < flErrorMargin && std::fabs(this->z - vecEqual.z) < flErrorMargin);
	}

	/// @returns: true if each component of the vector equals to zero, false otherwise
	[[nodiscard]] bool is_zero() const
	{
		// @note: to make this implementation right, we should use fpclassify here, but game aren't doing same, probably it's better to keep this same, just ensure that it will be compiled same
		return (this->x == 0.0f && this->y == 0.0f && this->z == 0.0f);
	}

	[[nodiscard]] float length() const
	{
		return std::sqrt(this->length_sqr());
	}

	[[nodiscard]] constexpr float length_sqr() const
	{
		return dot_product(*this);
	}

	[[nodiscard]] float length_2d() const
	{
		return std::sqrt(this->length_2d_sqr());
	}

	[[nodiscard]] constexpr float length_2d_sqr() const
	{
		return (this->x * this->x + this->y * this->y);
	}

	[[nodiscard]] float dist_to(const vector_t& vecEnd) const
	{
		return (*this - vecEnd).length();
	}

	[[nodiscard]] constexpr float dist_to_sqr(const vector_t& vecEnd) const
	{
		return (*this - vecEnd).length_sqr();
	}

	/// normalize magnitude of each component of the vector
	/// @returns: length of the vector
	float normalize_in_place()
	{
		const float flLength = this->length();
		const float flRadius = 1.0f / (flLength + std::numeric_limits<float>::epsilon());

		this->x *= flRadius;
		this->y *= flRadius;
		this->z *= flRadius;

		return flLength;
	}

	/// normalize magnitude of each component of the vector
	/// @returns: copy of the vector with normalized components
	[[nodiscard]] vector_t normalized() const
	{
		vector_t vecOut = *this;
		vecOut.normalize_in_place();
		return vecOut;
	}

	[[nodiscard]] constexpr float dot_product(const vector_t& vecDot) const
	{
		return (this->x * vecDot.x + this->y * vecDot.y + this->z * vecDot.z);
	}

	[[nodiscard]] constexpr vector_t cross_product(const vector_t& vecCross) const
	{
		return { this->y * vecCross.z - this->z * vecCross.y, this->z * vecCross.x - this->x * vecCross.z, this->x * vecCross.y - this->y * vecCross.x };
	}

	/// @returns: transformed vector by given transformation matrix
	[[nodiscard]] vector_t transform(const matrix3x4_t& matTransform) const;

	[[nodiscard]] vector2d_t to_vector_2_d() const
	{
		return { this->x, this->y };
	}

	/// initializes the vector to zero
	void init( float ix = 0, float iy = 0, float iz = 0 )
	{
		x = ix; y = iy; z = iz;
	}

	void Set( float X = 0.0f, float Y = 0.0f, float Z = 0.0f )
	{
		x = X; y = Y; z = Z;
	}


	/// convert forward direction vector to other direction vectors
	/// @param[out] pvecRight [optional] output for converted right vector
	/// @param[out] pvecUp [optional] output for converted up vector
	void to_directions(vector_t* pvecRight, vector_t* pvecUp) const
	{
		if (std::fabs(this->x) < 1e-6f && std::fabs(this->y) < 1e-6f)
		{
			// pitch 90 degrees up/down from identity
			if (pvecRight != nullptr)
			{
				pvecRight->x = 0.0f;
				pvecRight->y = -1.0f;
				pvecRight->z = 0.0f;
			}

			if (pvecUp != nullptr)
			{
				pvecUp->x = -this->z;
				pvecUp->y = 0.0f;
				pvecUp->z = 0.0f;
			}
		}
		else
		{
			if (pvecRight != nullptr)
			{
				pvecRight->x = this->y;
				pvecRight->y = -this->x;
				pvecRight->z = 0.0f;
				pvecRight->normalize_in_place();
			}

			if (pvecUp != nullptr)
			{
				pvecUp->x = (-this->x) * this->z;
				pvecUp->y = -(this->y * this->z);
				pvecUp->z = this->y * this->y - (-this->x) * this->x;
				pvecUp->normalize_in_place();
			}
		}
	}

	/// @returns: 2D angles converted from direction vector
	[[nodiscard]] q_angle_t to_angles() const;

	/// @returns: matrix converted from forward direction vector
	[[nodiscard]] matrix3x4_t to_matrix() const;

	float x = 0.0f, y = 0.0f, z = 0.0f;
};

struct Vector4D_t
{
	constexpr Vector4D_t(const float x = 0.0f, const float y = 0.0f, const float z = 0.0f, const float w = 0.0f) :
		x(x), y(y), z(z), w(w) {
	}

	float x = 0.0f, y = 0.0f, z = 0.0f, w = 0.0f;
};

struct alignas(16) VectorAligned_t : vector_t
{
	VectorAligned_t() = default;

	explicit VectorAligned_t(const vector_t& vecBase)
	{
		this->x = vecBase.x; this->y = vecBase.y; this->z = vecBase.z; this->w = 0.0f;
	}

	constexpr VectorAligned_t& operator=(const vector_t& vecBase)
	{
		this->x = vecBase.x; this->y = vecBase.y; this->z = vecBase.z; this->w = 0.0f;
		return *this;
	}



	float w = 0.0f;
};
static_assert(alignof(VectorAligned_t) == 16);

using Vector = vector_t;