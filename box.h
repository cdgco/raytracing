#ifndef BOX_H
#define BOX_H

#include "object.h"
/** Box creation and intersection functions */
class Box : public Object {
public:
	Box() {}
	/** Box Shaped Object

		Example:

			Box(Vector3D(-1), Vector3D(1), new Lambertian(Vector3D(1, 0, 0)));
	*/
	Box(Vector3D bound1, Vector3D bound2, Material *pm) : pmCurMat(pm) { m_vBounds[0] = bound1; m_vBounds[1] = bound2; };
	virtual bool Hit(const Ray &r, HitRecord &rec, double tMin, double tMax) const;

	Material *pmCurMat; ///< Pointer to Material that the box should render.
	Vector3D m_vBounds[2]; ///< Array of Vector3D bounds for box
};

/** Return boolean value if box is hit by specified ray.

	Example:

		Box::Hit(ray, hitrec, 0.001, DBL_MAX);
*/
bool Box::Hit(const Ray &r, HitRecord &rec, double tMin, double tMax) const {
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


	// Still need to fix the box color (2nd & 3rd parameters in structure).
	rec = { dT, r.PointAtParameter(dT), (r.PointAtParameter(dT) - StdCross(m_vBounds[0], m_vBounds[1])), pmCurMat }; // Renders darkened color if camera off axis, if on axis, renders black
	//rec = { dT, r.PointAtParameter(rec.dT), (r.PointAtParameter(rec.dT) - StdCross(m_vBounds[0], m_vBounds[1])), pmCurMat }; // Renders correct color if there is nothing behind the box and camera is off axis
	return true;
}
#endif // BOXH