#pragma once
#include <cmath>
#include <memory.h>
#include <iostream>
#define _USE_MATH_DEFINES
#define SQ(x) ((x)*(x))
#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)
//my math

class Vec3
{
public:
	union
	{
		float v[3];
		struct { float x, y, z; };
	};
	Vec3() {
		v[0] = 0;
		v[1] = 0;
		v[2] = 0;
		x = 0;
		y = 0;
		z = 0;

	}
	Vec3(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vec3 operator+(const Vec3& pVec) const
	{
		return Vec3(v[0] + pVec.v[0], v[1] + pVec.v[1], v[2] + pVec.v[2]);
	}

	Vec3& operator+=(const Vec3& pVec)
	{
		v[0] += pVec.v[0];
		v[1] += pVec.v[1];
		v[2] += pVec.v[2];
		return *this;
	}

	Vec3 operator-(const Vec3& pVec) const
	{
		return Vec3(v[0] - pVec.v[0], v[1] - pVec.v[1], v[2] - pVec.v[2]);
	}

	Vec3 operator*(const Vec3& pVec) const
	{
		return Vec3(v[0] * pVec.v[0], v[1] * pVec.v[1], v[2] * pVec.v[2]);
	}

	Vec3 operator/(const Vec3& v) const
	{
		return Vec3(x / v.x, y / v.y, z / v.z);
	}

	Vec3& operator-=(const Vec3& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	Vec3& operator*=(const Vec3& v) {
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}

	Vec3& operator/=(const Vec3& v) {
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}

	Vec3 operator*(const float v) const 
	{
		return Vec3(x * v, y * v, z * v);
	}

	Vec3 operator/(const float v) const
	{
		float iv = 1.0f / v;
		return Vec3(x * iv, y * iv, z * iv);
	}

	Vec3& operator*=(const float v) {
		x *= v;
		y *= v;
		z *= v;
		return *this;
	}

	Vec3& operator/=(const float v) {
		float iv = 1.0f / v;
		x *= v;
		y *= v;
		z *= v;
		return *this;
	}

	Vec3 operator -() const {
		return Vec3(-x, -y, -z);
	}

	float length() const {
		return sqrtf(SQ(x) + SQ(y) + SQ(z));
	}

	float lengthSq() const {
		return (SQ(x) + SQ(y) + SQ(z));
	}


	Vec3 normalize(void)
	{
		float len = 1.0f / sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
		return Vec3(x * len, y * len, z * len);
	}

	float normalize_GetLength()
	{
		float length = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
		float len = 1.0f / length;
		v[0] *= len; v[1] *= len; v[2] *= len;
		return length;
	}


	float Dot(const Vec3& pVec) const
	{
		return v[0] * pVec.v[0] + v[1] * pVec.v[1] + v[2] * pVec.v[2];
	}

	Vec3 Cross(const Vec3& v1)
	{
		return Vec3(v1.v[1] * v[2] - v1.v[2] * v[1],
			v1.v[2] * v[0] - v1.v[0] * v[2],
			v1.v[0] * v[1] - v1.v[1] * v[0]);
	}

	Vec3 Max(const Vec3& v1, const Vec3& v2)
	{
		return Vec3(max(v1.v[0], v2.v[0]),
			max(v1.v[1], v2.v[1]),
			max(v1.v[2], v2.v[2]));
	}

	Vec3 Min(const Vec3& v1, const Vec3& v2)
	{
		return Vec3(min(v1.v[0], v2.v[0]),
			min(v1.v[1], v2.v[1]),
			min(v1.v[2], v2.v[2]));
	}

	float Max() const
	{
		return max(x, max(y, z));
	}

	float Min() const
	{
		return min(x, min(y, z));
	}


};

class Matrix
{
public:
	union
	{
		float a[4][4];
		float m[16];
	};

	Matrix() {
		identity();
	}

	Matrix(float *otherMatrix) {
		memcpy(&m, otherMatrix, 16 * sizeof(float));
	}

	void identity() {
		memset(m, 0, 16 * sizeof(float));
		m[0] = 1.0f;
		m[5] = 1.0f;
		m[10] = 1.0f;
		m[15] = 1.0f;
	}

	Vec3 mulVec(const Vec3& v)
	{
		return Vec3(
			(v.x * m[0] + v.y * m[1] + v.z * m[2]),
			(v.x * m[4] + v.y * m[5] + v.z * m[6]),
			(v.x * m[8] + v.y * m[9] + v.z * m[10]));
	}

	Vec3 mulPoint(const Vec3& v)
	{
		Vec3 v1 = Vec3(
			(v.x * m[0] + v.y * m[1] + v.z * m[2]) + m[3],
			(v.x * m[4] + v.y * m[5] + v.z * m[6]) + m[7],
			(v.x * m[8] + v.y * m[9] + v.z * m[10]) + m[11]);
		float w;
		w = (m[12] * v.x) + (m[13] * v.y) + (m[14] * v.z) + m[15];
		w = 1.0f / w;
		return (v1 * w);
	}

	float& operator[](int index) {
		return m[index];
	}

	static Matrix translation(const Vec3& v)
	{
		Matrix mat;
		mat.a[0][3] = v.x;
		mat.a[1][3] = v.y;
		mat.a[2][3] = v.z;
		return mat;
	}
	
	static Matrix scaling(const Vec3& v) {
		Matrix mat;
		mat.m[0] = v.x;
		mat.m[5] = v.y;
		mat.m[10] = v.z;
		return mat;
	}

	static Matrix rotateX(float theta) {
		Matrix mat;
		float ct = cosf(theta);
		float st = sinf(theta);
		mat.m[5] = ct;
		mat.m[0] = ct;
		mat.m[1] = -st;
		mat.m[4] = st;

	}

	Matrix invert()
	{
		Matrix inv;
		inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
		inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
		inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
		inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
		inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
		inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
		inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
		inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
		inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
		inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
		inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
		inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
		inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
		inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
		inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
		inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];
		float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
		if (det == 0) {
			// Handle this case
		}
		det = 1.0 / det;
		for (int i = 0; i < 16; i++) {
			inv[i] = inv[i] * det;
		}
		return inv;
	}

	Matrix mul(const Matrix& matrix) const
	{
		Matrix ret;
		ret.m[0] = m[0] * matrix.m[0] + m[4] * matrix.m[1] + m[8] * matrix.m[2] + m[12] * matrix.m[3];
		ret.m[1] = m[1] * matrix.m[0] + m[5] * matrix.m[1] + m[9] * matrix.m[2] + m[13] * matrix.m[3];
		ret.m[2] = m[2] * matrix.m[0] + m[6] * matrix.m[1] + m[10] * matrix.m[2] + m[14] * matrix.m[3];
		ret.m[3] = m[3] * matrix.m[0] + m[7] * matrix.m[1] + m[11] * matrix.m[2] + m[15] * matrix.m[3];
		ret.m[4] = m[0] * matrix.m[4] + m[4] * matrix.m[5] + m[8] * matrix.m[6] + m[12] * matrix.m[7];
		ret.m[5] = m[1] * matrix.m[4] + m[5] * matrix.m[5] + m[9] * matrix.m[6] + m[13] * matrix.m[7];
		ret.m[6] = m[2] * matrix.m[4] + m[6] * matrix.m[5] + m[10] * matrix.m[6] + m[14] * matrix.m[7];
		ret.m[7] = m[3] * matrix.m[4] + m[7] * matrix.m[5] + m[11] * matrix.m[6] + m[15] * matrix.m[7];
		ret.m[8] = m[0] * matrix.m[8] + m[4] * matrix.m[9] + m[8] * matrix.m[10] + m[12] * matrix.m[11];
		ret.m[9] = m[1] * matrix.m[8] + m[5] * matrix.m[9] + m[9] * matrix.m[10] + m[13] * matrix.m[11];
		ret.m[10] = m[2] * matrix.m[8] + m[6] * matrix.m[9] + m[10] * matrix.m[10] + m[14] * matrix.m[11];
		ret.m[11] = m[3] * matrix.m[8] + m[7] * matrix.m[9] + m[11] * matrix.m[10] + m[15] * matrix.m[11];
		ret.m[12] = m[0] * matrix.m[12] + m[4] * matrix.m[13] + m[8] * matrix.m[14] + m[12] * matrix.m[15];
		ret.m[13] = m[1] * matrix.m[12] + m[5] * matrix.m[13] + m[9] * matrix.m[14] + m[13] * matrix.m[15];
		ret.m[14] = m[2] * matrix.m[12] + m[6] * matrix.m[13] + m[10] * matrix.m[14] + m[14] * matrix.m[15];
		ret.m[15] = m[3] * matrix.m[12] + m[7] * matrix.m[13] + m[11] * matrix.m[14] + m[15] * matrix.m[15];
		return ret;
	}


	

	Matrix lookAt(Vec3& from, Vec3& to, Vec3& up) {
		Vec3 dir = (from - to).normalize();
		Vec3 right = up.Cross(dir).normalize();
		Vec3 upNew = dir.Cross(right);
		Matrix result;

		result.m[0] = right.x;
		result.m[1] = right.y;
		result.m[2] = right.z;

		result.m[4] = upNew.x;
		result.m[5] = upNew.y;
		result.m[6] = upNew.z;

		result.m[8] = dir.x;
		result.m[9] = dir.y;
		result.m[10] = dir.z;

		result.m[3] = -from.Dot(right);
		result.m[7] = -from.Dot(upNew);
		result.m[11] = -from.Dot(dir);

		result.m[12] = 0.f;
		result.m[13] = 0.f;
		result.m[14] = 0.f;
		result.m[15] = 1.f;

		return result;
	}
	
	Matrix Perspective(float fov, float aspect, float zNear, float zFar) {
		Matrix projection;
		float tanHalfFOV = tanf(fov / 2.0f);

		projection.identity();

		projection.m[0] = 1.0f / (aspect * tanHalfFOV);

		projection.m[5] = 1.0f / tanHalfFOV;

		projection.m[10] = -zFar / (zFar - zNear);
		projection.m[11] = -(zFar * zNear) / (zFar - zNear);

		projection.m[14] = -1.0f;
		projection.m[15] = 0.0f;

		return projection;
	}

	Matrix operator*(const Matrix& matrix)
	{
		return mul(matrix);
	}

	

};

class Vec2 {
public:
	union
	{
		float v[2];
		struct { float x, y; };
	};
	Vec2() {
		v[0] = 0;
		v[1] = 0;
		
		x = 0;
		y = 0;

	}
	Vec2(float _x, float _y) {
		x = _x;
		y = _y;
		
	}

	Vec2 operator+(const Vec2& pVec) const
	{
		return Vec2(v[0] + pVec.v[0], v[1] + pVec.v[1]);
	}

	Vec2 operator-(const Vec3& pVec) const
	{
		return Vec2(v[0] - pVec.v[0], v[1] - pVec.v[1]);
	}

	Vec2 operator*(const Vec3& pVec) const
	{
		return Vec2(v[0] * pVec.v[0], v[1] * pVec.v[1]);
	}

	Vec2 operator/(const Vec3& v) const
	{
		return Vec2(x / v.x, y / v.y);
	}

	Vec2& operator+=(const Vec2& pVec)
	{
		v[0] += pVec.v[0];
		v[1] += pVec.v[1];
		
		return *this;
	}

	Vec2 operator*(const float v) const
	{
		return Vec2(x * v, y * v);
	}

};

class Colour {
public:
	//union {
		struct {
			float r;
			float g;
			float b;
			//float a;
		};
		/*float coords[4];*/
	//};
	

	Colour() {
		r = 0.f;
		g = 0.f;
		b = 0.f;
		//a = 0.f;
	}
	Colour(float _r, float _g, float _b) {
		r = _r;
		g = _g;
		b = _b;
		//a = _a;
	}
	

};


class Quaternion {
public:
	union {
		struct {
			float a;
			float b;
			float c;
			float d;
		};
		float q[4];
	};
	Quaternion() {
		a = 0;
		b = 0;
		c = 0;
		d = 0;
	}
	Quaternion(float _a, float _b, float _c, float _d) {
		a = _a;
		b = _b;
		c = _c;
		d = _d;
	}

	// module
	float mod() {
		
		return sqrt(a * a + b * b + c * c + d * d);
	}

	float Dot(const Quaternion q) const {
		return float(a * q.a + b * q.b + c * q.c + d * q.d);
	}

	Quaternion normalise() {
		Quaternion q1;
		float norm = mod();
		return Quaternion(a / norm, b / norm, c / norm, d / norm);
	}


	static Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t) {
		// Compute the dot product between q0 and q1
		float dot = q0.Dot(q1);

		if (dot < 0.0001f)
		{
			return q0;
		}
		// Ensure the dot product is in the range [-1, 1]
		if (dot < -1.0f) dot = -1.0f;
		if (dot > 1.0f) dot = 1.0f;

		// If the dot product is negative, negate q1 to ensure the shortest path
		Quaternion q1Copy = q1;
		if (dot < 0.0f) {
			dot = -dot;
			q1Copy = Quaternion(-q1.a, -q1.b, -q1.c, -q1.d);
		}

		// If the quaternions are almost the same, perform linear interpolation
		const float DOT_THRESHOLD = 0.9995f;
		if (dot > DOT_THRESHOLD) {
			Quaternion result(
				q0.a + t * (q1Copy.a - q0.a),
				q0.b + t * (q1Copy.b - q0.b),
				q0.c + t * (q1Copy.c - q0.c),
				q0.d + t * (q1Copy.d - q0.d)
			);
			// Normalize the result
			return result.normalise();
		}

		// Calculate the angle between the quaternions
		float theta = acosf(dot);
		float sinTheta = sinf(theta);

		// Compute interpolation weights
		float weight0 = sinf((1.0f - t) * theta) / sinTheta;
		float weight1 = sinf(t * theta) / sinTheta;

		// Interpolate the quaternions
		return Quaternion(
			weight0 * q0.a + weight1 * q1Copy.a,
			weight0 * q0.b + weight1 * q1Copy.b,
			weight0 * q0.c + weight1 * q1Copy.c,
			weight0 * q0.d + weight1 * q1Copy.d
		);
	}


	Matrix toMatrix()
	{
		float xx = q[0] * q[0];
		float xy = q[0] * q[1];
		float xz = q[0] * q[2];
		float yy = q[1] * q[1];
		float zz = q[2] * q[2];
		float yz = q[1] * q[2];
		float wx = q[3] * q[0];
		float wy = q[3] * q[1];
		float wz = q[3] * q[2];
		Matrix matrix;
		matrix[0] = 1.0f - 2.0f * (yy + zz);
		matrix[1] = 2.0f * (xy - wz);
		matrix[2] = 2.0f * (xz + wy);
		matrix[3] = 0.0;
		matrix[4] = 2.0f * (xy + wz);
		matrix[5] = 1.0f - 2.0f * (xx + zz);
		matrix[6] = 2.0f * (yz - wx);
		matrix[7] = 0.0;
		matrix[8] = 2.0f * (xz - wy);
		matrix[9] = 2.0f * (yz + wx);
		matrix[10] = 1.0f - 2.0f * (xx + yy);
		matrix[11] = 0.0;
		matrix[12] = 0;
		matrix[13] = 0;
		matrix[14] = 0;
		matrix[15] = 1;
		return matrix;
	}

	

};




