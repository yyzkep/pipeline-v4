#pragma once
#include "vector.hpp"
#include "matrix.hpp"
#include "qangle.hpp"
#include <algorithm>
#include "sdk/valve/defs.hpp"

#define M_RADPI		57.295779513082	
#define PI			3.14159265358979323846
#define DEG2RAD(x)	((float)(x) * (float)((float)(PI) / 180.0f))
#define RAD2DEG(x)	((float)(x) * (float)(180.0f / (float)(PI)))

namespace mathematics //genius naming ikr
{
	inline float normalize_angle(float ang)
	{
		if (!std::isfinite(ang))
			return 0.0f;

		while (ang > 180.0f)
			ang -= 360.0f;

		while (ang < -180.0f)
			ang += 360.0f;

		return ang;
	}

	inline float normalize_vector(vector_t& v)
	{
		float len = v.length();
		if (!len) v.Set(); else v /= len;
		return len;
	}

	inline void clamp_angles(vector_t& angles)
	{
		angles.x = std::max(-89.f, std::min(89.f, normalize_angle(angles.x)));
		angles.y = normalize_angle(angles.y);
		angles.z = 0.0f;
	}

	inline void sin_cos(float radians, float* sine, float* cosine)
	{
		*sine = std::sin(radians);
		*cosine = std::cos(radians);
	}

	inline void angle_vectors(const vector_t& angles, vector_t* forward)
	{
		float sp, sy, cp, cy;

		sin_cos(DEG2RAD(angles.x), &sp, &cp);
		sin_cos(DEG2RAD(angles.y), &sy, &cy);

		if (forward)
		{
			forward->x = cp * cy;
			forward->y = cp * sy;
			forward->z = -sp;
		}
	}

	inline void angle_vectors(const vector_t& angles, vector_t* forward, vector_t* right, vector_t* up)
	{
		float sr, sp, sy, cr, cp, cy;
		sin_cos(DEG2RAD(angles.x), &sp, &cp);
		sin_cos(DEG2RAD(angles.y), &sy, &cy);
		sin_cos(DEG2RAD(angles.z), &sr, &cr);

		if (forward)
		{
			forward->x = cp * cy;
			forward->y = cp * sy;
			forward->z = -sp;
		}

		if (right)
		{
			right->x = (-1 * sr * sp * cy + -1 * cr * -sy);
			right->y = (-1 * sr * sp * sy + -1 * cr * cy);
			right->z = -1 * sr * cp;
		}

		if (up)
		{
			up->x = (cr * sp * cy + -sr * -sy);
			up->y = (cr * sp * sy + -sr * cy);
			up->z = cr * cp;
		}
	}

	inline Vector calc_angle(const vector_t& source, const vector_t& destination)
	{
		vector_t angles = vector_t(0.0f, 0.0f, 0.0f);
		vector_t delta = (source - destination);
		float fHyp = float(sqrt((delta.x * delta.x) + (delta.y * delta.y)));

		angles.x = (atanf(delta.z / fHyp) * M_RADPI);
		angles.y = (atanf(delta.y / delta.x) * M_RADPI);
		angles.z = 0.0f;

		if (delta.x >= 0.0f)
			angles.y += 180.0f;

		return angles;
	}

	inline float calc_fov(const vector_t& src, const vector_t& dst)
	{
		vector_t v_src = vector_t();
		angle_vectors(src, &v_src);

		vector_t v_dst = vector_t();
		angle_vectors(dst, &v_dst);

		float result = RAD2DEG(acos(v_dst.dot_product(v_src) / v_dst.length_2d()));

		if (!std::isfinite(result) || std::isinf(result) || std::isnan(result))
			result = 0.0f;

		return result;
	}

	inline float dot_product(const vector_t& a, const vector_t& b) {
		return (a.x * b.x
			+ a.y * b.y
			+ a.z * b.z);
	}

	inline void vector_transform(const vector_t& input, const matrix3x4_t& matrix, vector_t& output)
	{
		output[0] = dot_product(input, vector_t(matrix[0][0], matrix[0][1], matrix[0][2])) + matrix[0][3];
		output[1] = dot_product(input, vector_t(matrix[1][0], matrix[1][1], matrix[1][2])) + matrix[1][3];
		output[2] = dot_product(input, vector_t(matrix[2][0], matrix[2][1], matrix[2][2])) + matrix[2][3];
	}



	inline void vector_angles(const vector_t& forward, vector_t& angles)
	{
		float tmp = 0.0f, yaw = 0.0f, pitch = 0.0f;

		if (!forward.y && !forward.x)
		{
			yaw = 0.0f;

			if (forward.z > 0.0f)
				pitch = 270.0f;
			else
				pitch = 90.0f;
		}

		else
		{
			yaw = RAD2DEG(atan2f(forward.y, forward.x));

			if (yaw < 0.0f)
				yaw += 360.0f;

			tmp = forward.length_2d();
			pitch = RAD2DEG(atan2f(-forward.z, tmp));

			if (pitch < 0.0f)
				pitch += 360.0f;
		}

		angles[0] = pitch;
		angles[1] = yaw;
		angles[2] = 0.0f;
	}

	inline void angle_matrix(const vector_t& angles, matrix3x4_t& matrix) {
		float sr, sp, sy, cr, cp, cy;

		sin_cos(DEG2RAD(angles.y), &sy, &cy);
		sin_cos(DEG2RAD(angles.x), &sp, &cp);
		sin_cos(DEG2RAD(angles.z), &sr, &cr);

		matrix[0][0] = cp * cy;
		matrix[1][0] = cp * sy;
		matrix[2][0] = -sp;

		float crcy = cr * cy;
		float crsy = cr * sy;
		float srcy = sr * cy;
		float srsy = sr * sy;

		matrix[0][1] = sp * srcy - crsy;
		matrix[1][1] = sp * srsy + crcy;
		matrix[2][1] = sr * cp;

		matrix[0][2] = (sp * crcy + srsy);
		matrix[1][2] = (sp * crsy - srcy);
		matrix[2][2] = cr * cp;

		matrix[0][3] = 0.0f;
		matrix[1][3] = 0.0f;
		matrix[2][3] = 0.0f;
	}

	inline void matrix_set_column(const vector_t& in, int column, matrix3x4_t& out) {
		out[0][column] = in.x;
		out[1][column] = in.y;
		out[2][column] = in.z;
	}

	inline float normalize_radians(float a) noexcept {
		return std::isfinite(a) ? std::remainder(a, PI * 2) : 0.0f;
	}

	inline float compute_rad_diff(float a1, float a2)
	{
		double delta = normalize_radians(a1 - a2);

		if (a1 > a2) {
			if (delta >= PI)
				delta -= PI * 2;
		}

		else if (delta <= -PI)
			delta += PI * 2;

		return static_cast<float>(delta);
	}

	inline vector_t velocity_to_angles(vector_t& direction)
	{
		auto Magnitude = [&](const vector_t& v) -> float {
			return sqrtf(v.dot_product(v));
			};

		float yaw, pitch;

		if (direction.y == 0.0f && direction.x == 0.0f)
		{
			yaw = 0.0f;

			if (direction.z > 0.0f)
				pitch = 270.0f;

			else pitch = 90.0f;
		}

		else
		{
			yaw = RAD2DEG(atan2f(direction.y, direction.x));
			pitch = RAD2DEG(atan2f(-direction.z, Magnitude(Vector(direction))));

			if (yaw < 0.0f)
				yaw += 360.0f;

			if (pitch < 0.0f)
				pitch += 360.0f;
		}

		return { pitch, yaw, 0.0f };
	}

	inline float remap(float val, float a, float b, float c, float d) {
		if (a == b)
			return val >= b ? d : c;

		float val_ = (val - a) / (b - a);
		val_ = std::clamp(val_, 0.0f, 1.0f);

		return c + (d - c) * val_;
	}

	inline float fraction(const float val, const float max, const float min) {
		return std::clamp((val - min) / (max - min), 0.f, 1.f);
	}

	inline void matrix_copy(const matrix3x4_t& src, matrix3x4_t& dst) {
		if (&src != &dst)
		{
			memcpy(&dst, &src, 12 * sizeof(float));
		}
	}

	inline void concat_transforms(const matrix3x4_t& in1, const matrix3x4_t& in2, matrix3x4_t& out)
	{
		if (&in1 == &out) {
			matrix3x4_t in1b;
			matrix_copy(in1, in1b);
			concat_transforms(in1b, in2, out);
			return;
		}

		if (&in2 == &out) {
			matrix3x4_t in2b;
			matrix_copy(in2, in2b);
			concat_transforms(in1, in2b, out);
			return;
		}

		out[0][0] = in1[0][0] * in2[0][0] + in1[0][1] * in2[1][0] +
			in1[0][2] * in2[2][0];
		out[0][1] = in1[0][0] * in2[0][1] + in1[0][1] * in2[1][1] +
			in1[0][2] * in2[2][1];
		out[0][2] = in1[0][0] * in2[0][2] + in1[0][1] * in2[1][2] +
			in1[0][2] * in2[2][2];
		out[0][3] = in1[0][0] * in2[0][3] + in1[0][1] * in2[1][3] +
			in1[0][2] * in2[2][3] + in1[0][3];
		out[1][0] = in1[1][0] * in2[0][0] + in1[1][1] * in2[1][0] +
			in1[1][2] * in2[2][0];
		out[1][1] = in1[1][0] * in2[0][1] + in1[1][1] * in2[1][1] +
			in1[1][2] * in2[2][1];
		out[1][2] = in1[1][0] * in2[0][2] + in1[1][1] * in2[1][2] +
			in1[1][2] * in2[2][2];
		out[1][3] = in1[1][0] * in2[0][3] + in1[1][1] * in2[1][3] +
			in1[1][2] * in2[2][3] + in1[1][3];
		out[2][0] = in1[2][0] * in2[0][0] + in1[2][1] * in2[1][0] +
			in1[2][2] * in2[2][0];
		out[2][1] = in1[2][0] * in2[0][1] + in1[2][1] * in2[1][1] +
			in1[2][2] * in2[2][1];
		out[2][2] = in1[2][0] * in2[0][2] + in1[2][1] * in2[1][2] +
			in1[2][2] * in2[2][2];
		out[2][3] = in1[2][0] * in2[0][3] + in1[2][1] * in2[1][3] +
			in1[2][2] * in2[2][3] + in1[2][3];
	}

	inline void matrix_angles(const matrix3x4_t& matrix, vector_t& angles) {
		//Vec3 forward, left, up;

		// extract the basis vectors from the matrix. since we only need the z
		// component of the up vector, we don't get x and y.
		const vector_t forward = { matrix[0][0], matrix[1][0], matrix[2][0] };
		const vector_t left = { matrix[0][1], matrix[1][1], matrix[2][1] };
		const vector_t up = { 0.f, 0.f, matrix[2][2] };

		float len = forward.length_2d();

		// enough here to get angles?
		if (len > 0.001f) {
			angles.x = RAD2DEG(std::atan2(-forward.z, len));
			angles.y = RAD2DEG(std::atan2(forward.y, forward.x));
			angles.z = RAD2DEG(std::atan2(left.z, up.z));
		}

		else {
			angles.x = RAD2DEG(std::atan2(-forward.z, len));
			angles.y = RAD2DEG(std::atan2(-left.x, left.y));
			angles.z = 0.f;
		}
	}

	inline void get_matrix_origin(const matrix3x4_t& source, vector_t& target) {
		target.x = source[0][3];
		target.y = source[1][3];
		target.z = source[2][3];
	}

	inline vector_t get_rotated_position(vector_t start, const float rotation, const float distance) {
		const auto rad = RAD2DEG(rotation);
		start.x += cosf(rad) * distance;
		start.y += sinf(rad) * distance;
		return start;
	}

	inline void vector_rotate(const vector_t& in1, const matrix3x4_t& in2, vector_t& out)
	{
		for (int i = 0; i < 3; i++)
		{
			out[i] = in1.dot_product(in2[i]);
		}
	}

	inline void vector_rotate(const vector_t& in1, const vector_t& in2, vector_t& out)
	{
		matrix3x4_t matRotate;
		angle_matrix(in2, matRotate);
		vector_rotate(in1, matRotate, out);
	}

	inline float angle_diff_radians(float a1, float a2) noexcept
	{
		float delta;

		delta = normalize_radians(a1 - a2);

		if (a1 > a2)
		{
			if (delta >= PI)
			{
				delta -= static_cast<float>(PI * 2);
			}
		}
		else
		{
			if (delta <= -PI)
			{
				delta += static_cast<float>(PI * 2);
			}
		}

		return delta;
	}
};
