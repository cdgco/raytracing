#ifndef VECTOR3D_H
#define VECTOR3D_H

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

#define drand48() ((double)rand()/RAND_MAX)

class Vector3D {
public:

	Vector3D() {}
	Vector3D(double e0) { e[0] = e0; e[1] = e0; e[2] = e0; }
	Vector3D(double e0, double e1, double e2) { e[0] = e0; e[1] = e1; e[2] = e2; }
	
	inline double x() const { return e[0]; }
	inline double y() const { return e[1]; }
	inline double z() const { return e[2]; }
	inline double r() const { return e[0]; }
	inline double g() const { return e[1]; }
	inline double b() const { return e[2]; }

	inline const Vector3D& operator+() const { return *this; }
	inline Vector3D operator-() const { return Vector3D(-e[0], -e[1], -e[2]); }
	inline double operator[](int i) const { return e[i]; }
	inline double& operator[](int i) { return e[i]; }
	inline Vector3D& operator+=(const Vector3D &v2);
	inline Vector3D& operator-=(const Vector3D &v2);
	inline Vector3D& operator*=(const Vector3D &v2);
	inline Vector3D& operator/=(const Vector3D &v2);
	inline Vector3D& operator*=(const double t);
	inline Vector3D& operator/=(const double t);

	inline double Length() const { return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]); }
	inline double SquaredLength() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }
	inline double Dot(const Vector3D &v2) { return e[0] * v2.e[0] + e[1] * v2.e[1] + e[2] * v2.e[2]; }

	inline Vector3D Cross(const Vector3D &v2) {
		return Vector3D((e[1] * v2.e[2] - e[2] * v2.e[1]),
			(-(e[0] * v2.e[2] - e[2] * v2.e[0])),
			(e[0] * v2.e[1] - e[1] * v2.e[0]));
	}

	inline void MakeUnitVector();

	double e[3];
};

inline std::istream& operator>>(std::istream &is, Vector3D &t) {
	is >> t.e[0] >> t.e[1] >> t.e[2];
	return is;
}

inline std::ostream& operator<<(std::ostream &os, const  Vector3D &t) {
	os << t.e[0] << " " << t.e[1] << " " << t.e[2];
	return os;
}

inline void Vector3D::MakeUnitVector() {
	double k = 1.0 / sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
	e[0] *= k; e[1] *= k; e[2] *= k;
}

inline Vector3D operator+(const  Vector3D &v1, const  Vector3D &v2) {
	return  Vector3D(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline Vector3D operator+(const Vector3D &v1, const int &i2) {
	return Vector3D(v1.e[0] + i2, v1.e[1] + i2, v1.e[2] + i2);
}

inline Vector3D operator+(const Vector3D &v1, const double &i2) {
	return Vector3D(v1.e[0] + i2, v1.e[1] + i2, v1.e[2] + i2);
}

inline Vector3D operator-(const Vector3D &v1, const Vector3D &v2) {
	return Vector3D(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline Vector3D operator-(const Vector3D &v1, const int &i2) {
	return Vector3D(v1.e[0] - i2, v1.e[1] - i2, v1.e[2] - i2);
}

inline Vector3D operator-(const Vector3D &v1, double &i2) {
	return Vector3D(v1.e[0] - i2, v1.e[1] - i2, v1.e[2] - i2);
}

inline Vector3D operator*(const Vector3D &v1, const Vector3D &v2) {
	return Vector3D(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline Vector3D operator/(const Vector3D &v1, const Vector3D &v2) {
	return Vector3D(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

inline Vector3D operator*(double t, const Vector3D &v) {
	return Vector3D(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline Vector3D operator/(Vector3D v, double t) {
	return Vector3D(v.e[0] / t, v.e[1] / t, v.e[2] / t);
}

inline Vector3D operator/(double t, Vector3D v) {
	return Vector3D(t / v.e[0], t / v.e[1], t / v.e[2]);
}

inline Vector3D operator*(const Vector3D &v, double t) {
	return Vector3D(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline Vector3D& Vector3D::operator+=(const Vector3D &v) {
	e[0] += v.e[0];
	e[1] += v.e[1];
	e[2] += v.e[2];
	return *this;
}

inline Vector3D& Vector3D::operator*=(const Vector3D &v) {
	e[0] *= v.e[0];
	e[1] *= v.e[1];
	e[2] *= v.e[2];
	return *this;
}

inline Vector3D& Vector3D::operator/=(const Vector3D &v) {
	e[0] /= v.e[0];
	e[1] /= v.e[1];
	e[2] /= v.e[2];
	return *this;
}

inline Vector3D& Vector3D::operator-=(const Vector3D& v) {
	e[0] -= v.e[0];
	e[1] -= v.e[1];
	e[2] -= v.e[2];
	return *this;
}

inline Vector3D& Vector3D::operator*=(const double t) {
	e[0] *= t;
	e[1] *= t;
	e[2] *= t;
	return *this;
}

inline Vector3D& Vector3D::operator/=(const double t) {
	double k = 1.0 / t;

	e[0] *= k;
	e[1] *= k;
	e[2] *= k;
	return *this;
}

inline Vector3D UnitVector(Vector3D v) {
	return v / v.Length();
}

inline double StdDot(const Vector3D &v1, const Vector3D &v2) { 
	return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2]; 
}

inline Vector3D StdCross(const Vector3D &v1, const Vector3D &v2) {
	return Vector3D((v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1]),
		(-(v1.e[0] * v2.e[2] - v1.e[2] * v2.e[0])),
		(v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]));
}

#endif // VECTOR3D_H