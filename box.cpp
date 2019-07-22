#include "box.h"

/** Return boolean value if box is hit by specified ray.

	Example:

		Box::Hit(ray, hitrec, 0.001, DBL_MAX);
*/
bool Box::Hit(const Ray &r, HitRecord &rec, double tMin, double tMax) const {
	double tmin = (m_vBounds[r.m_iSign[0]].x() - r.Origin().x()) * r.m_vInvDir.x();
	double tmax = (m_vBounds[1 - r.m_iSign[0]].x() - r.Origin().x()) * r.m_vInvDir.x();
	double tymin = (m_vBounds[r.m_iSign[1]].y() - r.Origin().y()) * r.m_vInvDir.y();
	double tymax = (m_vBounds[1 - r.m_iSign[1]].y() - r.Origin().y()) * r.m_vInvDir.y();
	double tzmin = (m_vBounds[r.m_iSign[2]].z() - r.Origin().z()) * r.m_vInvDir.z();
	double tzmax = (m_vBounds[1 - r.m_iSign[2]].z() - r.Origin().z()) * r.m_vInvDir.z();

	if ((tmin > tymax) || (tymin > tmax)) return false;
	if (tymin > tmin) tmin = tymin;
	if (tymax < tmax) tmax = tymax;

	if ((tmin > tzmax) || (tzmin > tmax)) return false;
	if (tzmin > tmin) tmin = tzmin;
	if (tzmax < tmax) tmax = tzmax;

	double dT = tmin;
	if (dT < 0) { dT = tmax; if (dT < 0) return false; }

	// Still need to fix the box color (2nd & 3rd parameters in structure).
	rec = { dT, r.PointAtParameter(dT), (r.PointAtParameter(dT) - StdCross(m_vBounds[0], m_vBounds[1])), m_pmCurMat }; // Renders darkened color if camera off axis, if on axis, renders black
	//rec = { dT, r.PointAtParameter(rec.m_dT), (r.PointAtParameter(rec.m_dT) - StdCross(m_vBounds[0], m_vBounds[1])), m_pmCurMat }; // Renders correct color if there is nothing behind the box and camera is off axis
	return true;
}