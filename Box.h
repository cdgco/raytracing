#ifndef BOXH
#define BOXH

#include "Object.h"

class Box : public Object {
public:
	Box() {}
	Box(Vector3D m_bounds1, Vector3D m_bounds2, Material *ptr) :  m_mat_ptr(ptr) { bounds[0] = m_bounds1; bounds[1] = m_bounds2; };
	virtual bool Hit(const Ray& m_r, double t_min, double t_max, HitRecord& rec) const;

	inline bool BoxIntersect(const Ray& m_r, double &t) const {
		double tmin, tmax, tymin, tymax, tzmin, tzmax;

		tmin = (bounds[m_r.sign[0]].x() - m_r.Origin().x()) * m_r.InvDir.x();
		tmax = (bounds[1-m_r.sign[0]].x() - m_r.Origin().x()) * m_r.InvDir.x();
		tymin = (bounds[m_r.sign[1]].y() - m_r.Origin().y()) * m_r.InvDir.y();
		tymax = (bounds[1-m_r.sign[1]].y() - m_r.Origin().y()) * m_r.InvDir.y();

		if ((tmin > tymax) || (tymin > tmax)) return false;
		if (tymin > tmin) tmin = tymin;
		if (tymax < tmax) tmax = tymax;

		tzmin = (bounds[m_r.sign[2]].z() - m_r.Origin().z()) * m_r.InvDir.z();
		tzmax = (bounds[1 - m_r.sign[2]].z() - m_r.Origin().z()) * m_r.InvDir.z();

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

	Material *m_mat_ptr;
	Vector3D bounds[2];
};

bool Box::Hit(const Ray& m_r, double t_min, double t_max, HitRecord& rec) const {
	double t = t_min;
	if (BoxIntersect(m_r, t)) {
			rec.dT = t;
			rec.m_p = m_r.PointAtParameter(rec.dT);
			rec.m_normal = (rec.m_p - StdCross(bounds[0], bounds[1]));
			rec.mat_ptr = m_mat_ptr;
			return true;
		}
}

#endif // BOXH