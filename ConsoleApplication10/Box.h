#ifndef BOXH
#define BOXH

#include "Object.h"

class Box : public Object {
public:
	Box() {}
	Box(const Vector3D &m_bounds_1, const Vector3D &m_bounds_2, Material *m) : bound1(m_bounds_1), bound2(m_bounds_2), m_mat_ptr(m) {};
	virtual bool Hit(const Ray& m_r, double tmin, double tmax, HitRecord& rec) const;
	Vector3D bound1, bound2;
	Material *m_mat_ptr;
};

bool Box::Hit(const Ray& m_r, double t_min, double t_max, HitRecord& rec) const {
	Vector3D m_oc = m_r.Origin() - m_center;
	double a = m_r.Direction().Dot(m_r.Direction());
	double b = m_oc.Dot(m_r.Direction());
	double c = m_oc.Dot(m_oc) - dSideLength * dSideLength;
	double dDiscriminant = b * b - a * c;
	if (dDiscriminant > 0) {
		double temp = (-b - sqrt(b*b - a * c)) / a;
		if (temp < t_max && temp > t_min) {
			rec.dT = temp;
			rec.m_p = m_r.PointAtParameter(rec.dT);
			rec.m_normal = (rec.m_p - m_center) / dSideLength;
			return true;
		}
		temp = (-b + sqrt(b*b - a * c)) / a;
		if (temp < t_max && temp > t_min) {
			rec.dT = temp;
			rec.m_p = m_r.PointAtParameter(rec.dT);
			rec.m_normal = (rec.m_p - m_center) / dSideLength;
			return true;
		}
	}
	return false;
}

#endif // BOXH