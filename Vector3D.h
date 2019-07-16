#ifndef VECTOR3DH
#define VECTOR3DH

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>
#include <iostream>

#define drand48() ((double)rand()/RAND_MAX)

class Vector3D {
public:

	Vector3D() {}
	// 
	// Creates an object consisting of three doubles representing location or color
	// 
	// Member Functions:
	//	x() / r(): First vector parameter
	//	y() / g(): Second vector parameter
	//	z() / b(): Third vector parameter
	//	Length(): Length of Vector
	//	SquaredLength(): Squared length of vector
	//	Dot(Vector3D v2): Dot product of two vectors
	//	Cross(Vector3D v2): Cross Product of two vectors
	//	MakeUnitVector(): Synonym for global function UnitVector(v)
	//
	Vector3D(double e0, double e1, double e2) { e[0] = e0; e[1] = e1; e[2] = e2; }
	// Return numerical paramater for location or color
	inline double x() const { return e[0]; }
	inline double y() const { return e[1]; }
	inline double z() const { return e[2]; }
	inline double r() const { return e[0]; }
	inline double g() const { return e[1]; }
	inline double b() const { return e[2]; }

	// Prototype Operator Functions
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

	// Returns length of Vector
	inline double Length() const { return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]); }
	
	// Returns squared lenght of vector
	inline double SquaredLength() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }
	
	// Returns dot product of two vectors
	inline double Dot(const Vector3D &v2) { return e[0] * v2.e[0] + e[1] * v2.e[1] + e[2] * v2.e[2]; }

	// Returns cross product of two vectors
	inline Vector3D Cross(const Vector3D &v2) {
		return Vector3D((e[1] * v2.e[2] - e[2] * v2.e[1]),
			(-(e[0] * v2.e[2] - e[2] * v2.e[0])),
			(e[0] * v2.e[1] - e[1] * v2.e[0]));
	}

	// Prototype Unit Vector Member Function
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

inline Vector3D operator-(const Vector3D &v1, const Vector3D &v2) {
	return Vector3D(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
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

#endif // VECTOR3DH