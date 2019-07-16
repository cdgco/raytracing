#ifndef BOXH
#define BOXH

#include "Object.h"

inline double ffmin(double a, double b) { return a < b ? a : b; }
inline double ffmax(double a, double b) { return a > b ? a : b; }

class Box {
public:
	Box() {}
	Box(const Vector3D &m_bounds_1, const Vector3D &m_bounds_2) : bound1(m_bounds_1), bound2(m_bounds_2) {};
	Vector3D min() const { return bound1; }
	Vector3D max() const { return bound2; }

	Vector3D bound1, bound2;

};

inline bool Box::Hit(const Ray& m_r, double min, double max) const {

	double t0x = (bound1.x() - m_r.Origin().x()) / m_r.Direction.x();
	double t1x = (bound2.x() - m_r.Origin().x()) / m_r.Direction.x();

	if (t0x > t1x) {
		std::swap(t0x, t1x);
	}
	if (m_r.Direction().x() >= 0) {

	}


	double t0y = (bound1.y() - m_r.Origin().y()) / m_r.Direction.y();
	double t1y = (bound2.y() - m_r.Origin().y()) / m_r.Direction.y();
	
	if (t0y > t1y) {
		std::swap(t0y, t1y);
	}

	if (t0y > t0x) {
		t0x = t0y;
	}
	if (t1y < t1x) {
		t1x = t1y;
	}

	double t0z = (bound1.z() - m_r.Origin().z()) / m_r.Direction.z();
	double t1z = (bound2.z() - m_r.Origin().z()) / m_r.Direction.z();

	if (t0z > t1z) {
		std::swap(t0z, t1z);
	}

	if (t0x > t1z || t0z > t1x) {
		return false;
	}

	if (t0z > t0x) {
		t0x = t0z;
	}
	if (t1z < t1x) {
		t1x = t1z;
	}

	return true;
	/**
	for (int a = 0; a < 3; a++) {
		double invD = 1.0f / m_r.Direction()[a];
		double t0 = (min()[a] - m_r.Origin()[a]) * invD;
		double t1 = (max()[a] - m_r.Origin()[a]) * invD;
		if (invD < 0.0f) {
			std::swap(t0, t1);
		}
		tmin = t0 > tmin ? t0 : tmin;
		tmax = t1 < tmax ? t1 : tmax;
		if (tmax <= tmin) {
			return false;
		}
		return true;
	}

	**/
}

Box SurroundingBox(Box box0, Box box1) {
	Vector3D small(fmin(box0.min().x(), box1.min().x()),
		fmin(box0.min().y(), box1.min().y()),
		fmin(box0.min().z(), box1.min().z()));
	Vector3D big(fmax(box0.max().x(), box1.max().x()),
		fmax(box0.max().y(), box1.max().y()),
		fmax(box0.max().z(), box1.max().z()));
	return Box(small, big);
}

#endif // BOXH