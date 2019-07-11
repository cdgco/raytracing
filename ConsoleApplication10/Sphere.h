#ifndef SPHEREH
#define SPHEREH

#include "Object.h"

class Sphere : public Object {
public: 
	Sphere() {}
	Sphere(Vector3D m_cen, double r, Material *m) : m_center(m_cen), dRadius(r), m_mat_ptr(m) {};
	virtual bool Hit(const Ray& m_r, double tmin, double tmax, HitRecord& rec) const;
	Vector3D m_center;
	double dRadius;
	Material *m_mat_ptr;
};

bool Sphere::Hit(const Ray& m_r, double t_min, double t_max, HitRecord& rec) const {
	Vector3D m_oc = m_r.Origin() - m_center;
	// Define variables for quadratic formula
	double a = m_r.Direction().Dot(m_r.Direction());
	double b = m_oc.Dot(m_r.Direction());
	double c = m_oc.Dot(m_oc) - dRadius * dRadius;
	double dDiscriminant = b * b - a * c;
	if (dDiscriminant > 0) {
		// Use quadratic formula to calculate intersections (-)
		double temp = (-b - sqrt(dDiscriminant)) / a;
		// If intersection falls within limits
		if (temp < t_max && temp > t_min) {
			// Record point
			rec.dT = temp;
			rec.m_p = m_r.PointAtParameter(rec.dT);
			rec.m_normal = (rec.m_p - m_center) / dRadius;
			rec.mat_ptr = m_mat_ptr;
			return true;
		}
		// Use quadratic formula to calculate intersections (+)
		temp = (-b + sqrt(dDiscriminant)) / a;
		// If intersection falls within limits
		if (temp < t_max && temp > t_min) {
			// Record point
			rec.dT = temp;
			rec.m_p = m_r.PointAtParameter(rec.dT);
			rec.m_normal = (rec.m_p - m_center) / dRadius;
			rec.mat_ptr = m_mat_ptr;
			return true;
		}
	}
	return false;
}

#endif // SPHEREH