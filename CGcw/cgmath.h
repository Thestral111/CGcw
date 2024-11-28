#pragma once
#include <cmath>
#include <memory.h>
#define _USE_MATH_DEFINES
#define SQ(x) ((x)*(x))
#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)


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
		mat.a[1][3] = v.x;
		mat.a[2][3] = v.z;
		return mat;
	}
	
	static Matrix scaling(const Vec3& v) {
		Matrix mat;
		mat.m[0] = v.x;
		mat.m[5] = v.y;
		mat.m[10] = v.z;
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


	Matrix LookAt(const Vec3& eye, const Vec3& target, const Vec3& up) {
		// Step 1: Calculate the forward (z-axis) vector
		Vec3 z = (eye - target).normalize();

		// Step 2: Calculate the right (x-axis) vector
		Vec3 x = (((Vec3) up).Cross(z)).normalize();

		// Step 3: Calculate the true up (y-axis) vector
		Vec3 y = z.Cross(x);

		// Step 4: Create the rotation matrix
		Matrix rotation;
		rotation.m[0] = x.x; rotation.m[4] = x.y; rotation.m[8] = x.z;  rotation.m[12] = 0.0f;
		rotation.m[1] = y.x; rotation.m[5] = y.y; rotation.m[9] = y.z;  rotation.m[13] = 0.0f;
		rotation.m[2] = z.x; rotation.m[6] = z.y; rotation.m[10] = z.z; rotation.m[14] = 0.0f;
		rotation.m[3] = 0.0f; rotation.m[7] = 0.0f; rotation.m[11] = 0.0f; rotation.m[15] = 1.0f;

		// Step 5: Create the translation matrix
		Matrix translation;
		translation.identity();
		translation.m[12] = -eye.x;
		translation.m[13] = -eye.y;
		translation.m[14] = -eye.z;

		// Step 6: Combine rotation and translation matrices
		return rotation.mul(translation);
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
	/*Colour operator+(const Colour& colour) const {
		return Colour(r + colour.r, g + colour.g, b + colour.b, a + colour.a);
	}
	Colour operator*(const Colour& colour) const {
		return Colour(r - colour.r, g - colour.g, b - colour.b, a - colour.a);
	}
	Colour operator*(const float other) const {
		return Colour(r * other, g * other, b * other, a * other);
	}
	Colour operator/(const float other) const {
		return Colour(r / other, g / other, b / other, a / other);
	}*/

};

