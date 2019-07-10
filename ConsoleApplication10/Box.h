#ifndef BOXH
#define BOXH

#include "Object.h"

class Box : public Object {
public:
	Box() {}
	Box(Vector3D m_cen, double l) : m_center(m_cen), dSideLength(l) {};
	virtual bool Hit(const Ray& m_r, double tmin, double tmax, HitRecord& rec) const;
	Vector3D m_center;
	double dSideLength;
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