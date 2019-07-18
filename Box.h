#ifndef BOX_H
#define BOX_H

#include "object.h"

class Box : public Object {
public:
	Box() {}
	Box(Vector3D bound1, Vector3D bound2, Material *pm) : pmCurMat(pm) { m_vBounds[0] = bound1; m_vBounds[1] = bound2; };
	virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const;

	Material *pmCurMat; Vector3D m_vBounds[2];
};

bool Box::Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const {
	double tmin = (m_vBounds[r.Sign[0]].x() - r.Origin().x()) * r.InvDir.x();
	double tmax = (m_vBounds[1 - r.Sign[0]].x() - r.Origin().x()) * r.InvDir.x();
	double tymin = (m_vBounds[r.Sign[1]].y() - r.Origin().y()) * r.InvDir.y();
	double tymax = (m_vBounds[1 - r.Sign[1]].y() - r.Origin().y()) * r.InvDir.y();
	double tzmin = (m_vBounds[r.Sign[2]].z() - r.Origin().z()) * r.InvDir.z();
	double tzmax = (m_vBounds[1 - r.Sign[2]].z() - r.Origin().z()) * r.InvDir.z();

	if ((tmin > tymax) || (tymin > tmax)) return false;
	if (tymin > tmin) tmin = tymin;
	if (tymax < tmax) tmax = tymax;

	if ((tmin > tzmax) || (tzmin > tmax)) return false;
	if (tzmin > tmin) tmin = tzmin;
	if (tzmax < tmax) tmax = tzmax;
	
	double dT = tmin;
	if (dT < 0) { dT = tmax; if (dT < 0) return false; }

	rec.dT = dT;
	rec.m_vP = r.PointAtParameter(rec.dT);
	rec.m_vNormal = (rec.m_vP - StdCross(m_vBounds[0], m_vBounds[1])) / 1.3;
	rec.pmCurMat = pmCurMat;
	return true;
}
#endif // BOXH