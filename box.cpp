#include "box.h"

/*! Return boolean value if box is hit by specified ray.
*
*	Example:
*
*		Box::Hit(ray, hitrec, 0.001, DBL_MAX);
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

	//rec = { dT, r.PointAtParameter(dT), (r.PointAtParameter(dT) - m_vCenter), m_pmCurMat }; // Renders darkened color if camera off axis, if on axis, renders black
	rec = { dT, r.PointAtParameter(rec.m_dT), BoxNormal(r.PointAtParameter(rec.m_dT)), m_pmCurMat }; // Renders color if camera off axis and no objects behind, if on axis, renders black
	return true;
}
Vector3D Box::BoxNormal(Vector3D inter) const {
	static const Vector3D normals[] = {
		Vector3D(1,0,0),
		Vector3D(0,1,0),
		Vector3D(0,0,1)
	};
	const Vector3D interRelative = inter - m_vCenter;
	const double xyCoef = interRelative.y() / interRelative.x();
	const double zyCoef = interRelative.y() / interRelative.z();
	int sign = 1;
	const int coef = ((xyCoef <= 1 && xyCoef >= -1) ? 1 : ((zyCoef < 1 && zyCoef > -1) ? 2 : 0));
	if (interRelative.x() < 0 || interRelative.y() < 0 || interRelative.z() < 0) {
		sign = -1;
	}
	return normals[coef] * sign * interRelative;
}