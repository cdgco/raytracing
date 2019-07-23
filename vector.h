#ifndef VECTOR_H
#define VECTOR_H

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <cfloat>
#include <iostream>
#include <vector>

#define drand48() ((double)rand()/RAND_MAX)
/*! 3 Paramater Vector3D object, operators and functions
*
*	object can be initialized with 0, 1, or 3 values.
*/
class Vector3D {
public:
	/*! Three value object for location or color
	*
	*	Example:
	*
	*		Vector3D zero;
	*/
	Vector3D() {}
	/*! Three value object for location or color
	*
	*	Example:
	*
	*		Vector3D one(1) // Vector3D one(1, 1, 1);
	*/
	Vector3D(double e0) { m_dE[0] = e0; m_dE[1] = e0; m_dE[2] = e0; }
	/*! Three value object for location or color
	*
	*	Example:
	*
	*		Vector3D three(1, 2, 3);
	*/
	Vector3D(double e0, double e1, double e2) { m_dE[0] = e0; m_dE[1] = e1; m_dE[2] = e2; }
	
	inline double x() const { return m_dE[0]; } ///< Returns first (X) location parameter in Vector3D
	inline double y() const { return m_dE[1]; } ///< Returns second (Y) location parameter in Vector3D
	inline double z() const { return m_dE[2]; } ///< Returns third (Z) location parameter in Vector3D
	inline double r() const { return m_dE[0]; } ///< Returns first (R) color parameter in Vector3D
	inline double g() const { return m_dE[1]; } ///< Returns second (G) color parameter in Vector3D
	inline double b() const { return m_dE[2]; } ///< Returns third (B) color parameter in Vector3D

	inline const Vector3D &operator+() const { return *this; } ///< Add Vector3D Operator
	inline Vector3D operator-() const { return Vector3D(-m_dE[0], -m_dE[1], -m_dE[2]); } ///< Subtract Vector3D Operator
	inline double operator[](int i) const { return m_dE[i]; } ///< Constant Vector3D Array Operator
	inline double &operator[](int i) { return m_dE[i]; } ///< Vector3D Array Operator
	inline Vector3D &operator+=(const Vector3D &v2); ///< Add Assign Vector3D Operator
	inline Vector3D &operator-=(const Vector3D &v2); ///< Subtract Assign Assign Operator
	inline Vector3D &operator*=(const Vector3D &v2); ///< Multiply Assign Vector3D Operator
	inline Vector3D &operator/=(const Vector3D &v2); ///< Divide Assign Vector3D Operator
	inline Vector3D &operator*=(const double t); ///< Multiply Double Operator
	inline Vector3D &operator/=(const double t); ///< Divide Double Operator

	inline double Length() const { return sqrt(m_dE[0] * m_dE[0] + m_dE[1] * m_dE[1] + m_dE[2] * m_dE[2]); } ///< Return length (double) of Vector3D 
	inline double SquaredLength() const { return m_dE[0] * m_dE[0] + m_dE[1] * m_dE[1] + m_dE[2] * m_dE[2]; } ///< Return squared length (double) of Vector3D
	inline double Dot(const Vector3D &v2) { return m_dE[0] * v2.m_dE[0] + m_dE[1] * v2.m_dE[1] + m_dE[2] * v2.m_dE[2]; } ///< Return dot product (double) of two Vector3D

	/*! Return cross product (Vector3D) of two Vector3D */
	inline Vector3D Cross(const Vector3D &v2) {
		return Vector3D((m_dE[1] * v2.m_dE[2] - m_dE[2] * v2.m_dE[1]),
			(-(m_dE[0] * v2.m_dE[2] - m_dE[2] * v2.m_dE[0])),
			(m_dE[0] * v2.m_dE[1] - m_dE[1] * v2.m_dE[0]));
	}

	inline void MakeUnitVector(); ///< Convert Vector3D into unit vector

	double m_dE[3]; ///< Value array (double) for color or location parameters
};

inline std::istream &operator>>(std::istream &is, Vector3D &t) {
	is >> t.m_dE[0] >> t.m_dE[1] >> t.m_dE[2];
	return is;
}

inline std::ostream &operator<<(std::ostream &os, const  Vector3D &t) {
	os << t.m_dE[0] << " " << t.m_dE[1] << " " << t.m_dE[2];
	return os;
}

inline void Vector3D::MakeUnitVector() {
	double k = 1.0 / sqrt(m_dE[0] * m_dE[0] + m_dE[1] * m_dE[1] + m_dE[2] * m_dE[2]);
	m_dE[0] *= k; m_dE[1] *= k; m_dE[2] *= k;
}

inline Vector3D operator+(const  Vector3D &v1, const  Vector3D &v2) {
	return  Vector3D(v1.m_dE[0] + v2.m_dE[0], v1.m_dE[1] + v2.m_dE[1], v1.m_dE[2] + v2.m_dE[2]);
}

inline Vector3D operator+(const Vector3D &v1, const int &i2) {
	return Vector3D(v1.m_dE[0] + i2, v1.m_dE[1] + i2, v1.m_dE[2] + i2);
}

inline Vector3D operator+(const Vector3D &v1, const double &i2) {
	return Vector3D(v1.m_dE[0] + i2, v1.m_dE[1] + i2, v1.m_dE[2] + i2);
}

inline Vector3D operator-(const Vector3D &v1, const Vector3D &v2) {
	return Vector3D(v1.m_dE[0] - v2.m_dE[0], v1.m_dE[1] - v2.m_dE[1], v1.m_dE[2] - v2.m_dE[2]);
}

inline Vector3D operator-(const Vector3D &v1, const int &i2) {
	return Vector3D(v1.m_dE[0] - i2, v1.m_dE[1] - i2, v1.m_dE[2] - i2);
}

inline Vector3D operator-(const Vector3D &v1, double &i2) {
	return Vector3D(v1.m_dE[0] - i2, v1.m_dE[1] - i2, v1.m_dE[2] - i2);
}

inline Vector3D operator*(const Vector3D &v1, const Vector3D &v2) {
	return Vector3D(v1.m_dE[0] * v2.m_dE[0], v1.m_dE[1] * v2.m_dE[1], v1.m_dE[2] * v2.m_dE[2]);
}

inline Vector3D operator/(const Vector3D &v1, const Vector3D &v2) {
	return Vector3D(v1.m_dE[0] / v2.m_dE[0], v1.m_dE[1] / v2.m_dE[1], v1.m_dE[2] / v2.m_dE[2]);
}

inline Vector3D operator*(double t, const Vector3D &v) {
	return Vector3D(t*v.m_dE[0], t*v.m_dE[1], t*v.m_dE[2]);
}

inline Vector3D operator/(Vector3D v, double t) {
	return Vector3D(v.m_dE[0] / t, v.m_dE[1] / t, v.m_dE[2] / t);
}

inline Vector3D operator/(double t, Vector3D v) {
	return Vector3D(t / v.m_dE[0], t / v.m_dE[1], t / v.m_dE[2]);
}

inline Vector3D operator*(const Vector3D &v, double t) {
	return Vector3D(t*v.m_dE[0], t*v.m_dE[1], t*v.m_dE[2]);
}

inline Vector3D &Vector3D::operator+=(const Vector3D &v) {
	m_dE[0] += v.m_dE[0];
	m_dE[1] += v.m_dE[1];
	m_dE[2] += v.m_dE[2];
	return *this;
}

inline Vector3D &Vector3D::operator*=(const Vector3D &v) {
	m_dE[0] *= v.m_dE[0];
	m_dE[1] *= v.m_dE[1];
	m_dE[2] *= v.m_dE[2];
	return *this;
}

inline Vector3D &Vector3D::operator/=(const Vector3D &v) {
	m_dE[0] /= v.m_dE[0];
	m_dE[1] /= v.m_dE[1];
	m_dE[2] /= v.m_dE[2];
	return *this;
}

inline Vector3D &Vector3D::operator-=(const Vector3D& v) {
	m_dE[0] -= v.m_dE[0];
	m_dE[1] -= v.m_dE[1];
	m_dE[2] -= v.m_dE[2];
	return *this;
}

inline Vector3D &Vector3D::operator*=(const double t) {
	m_dE[0] *= t;
	m_dE[1] *= t;
	m_dE[2] *= t;
	return *this;
}

inline Vector3D &Vector3D::operator/=(const double t) {
	double k = 1.0 / t;

	m_dE[0] *= k;
	m_dE[1] *= k;
	m_dE[2] *= k;
	return *this;
}

inline Vector3D UnitVector(Vector3D v) {
	return v / v.Length();
}

inline double StdDot(const Vector3D &v1, const Vector3D &v2) { 
	return v1.m_dE[0] * v2.m_dE[0] + v1.m_dE[1] * v2.m_dE[1] + v1.m_dE[2] * v2.m_dE[2];
}

inline Vector3D StdCross(const Vector3D &v1, const Vector3D &v2) {
	return Vector3D((v1.m_dE[1] * v2.m_dE[2] - v1.m_dE[2] * v2.m_dE[1]),
		(-(v1.m_dE[0] * v2.m_dE[2] - v1.m_dE[2] * v2.m_dE[0])),
		(v1.m_dE[0] * v2.m_dE[1] - v1.m_dE[1] * v2.m_dE[0]));
}

#endif // VECTOR_H