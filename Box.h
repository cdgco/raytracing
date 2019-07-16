#ifndef BOXH
#define BOXH

#include "Object.h"

class Box : public Object {
public:
	Box() {}
	Box(Vector3D m_bounds1, Vector3D m_bounds2, Material *ptr) : min(m_bounds1), max(m_bounds2), m_mat_ptr(ptr) {};
	virtual bool Hit(const Ray& m_r, double t_min, double t_max, HitRecord& rec) const;
	Vector3D min, max;
	Material *m_mat_ptr;

	inline bool BoxIntersect(const Ray& m_r, double &t) const {
		double tmin = (min.x() - m_r.Origin().x()) / m_r.Direction().x();
		double tmax = (max.x() - m_r.Origin().x()) / m_r.Direction().x();

		if (tmin > tmax) std::swap(tmin, tmax);

		double tymin = (min.y() - m_r.Origin().y()) / m_r.Direction().y();
		double tymax = (max.y() - m_r.Origin().y()) / m_r.Direction().y();

		if (tymin > tymax) std::swap(tymin, tymax);
		if ((tmin > tymin) || (tymin > tmax)) return false;
		if (tymin > tmin) tmin = tymin;
		if (tymax < tmax) tmax = tymax;

		double tzmin = (min.z() - m_r.Origin().z()) / m_r.Direction().z();
		double tzmax = (max.z() - m_r.Origin().z()) / m_r.Direction().z();

		if (tzmin > tzmax) std::swap(tzmin, tzmax);
		if ((tmin > tzmax) || (tzmin > tmax)) return false;
		if (tzmin > tmin) tmin = tzmin;
		if (tzmax < tmax) tmax = tzmax;

		t = tmin;

		if (t < 0) {
			t = tmax;
			if (t < 0) return false;
		}

		return true;
	}
};

bool Box::Hit(const Ray& m_r, double t_min, double t_max, HitRecord& rec) const {
	if (BoxIntersect(m_r, t_min)) {
		rec.dT = t_min;
		rec.m_p = m_r.PointAtParameter(rec.dT);
		rec.mat_ptr = m_mat_ptr;
		return true;
	}
	
}
#endif // BOXH