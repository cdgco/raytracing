#ifndef OBJECT_H
#define OBJECT_H

#include "ray.h"

class Material;

/*! Structure to store values when ray hits object.
*
*	Example:
*
*		HitRecord rec { dT, r.PointAtParameter(rec.dT), (rec.m_vP - m_vCenter) / dRadius, pmCurMat };
*/
struct HitRecord {
	double m_dT; //!< Position along ray where intersection takes place.
	Vector3D m_vP; //!< Vector3D location of intersection.
	Vector3D m_vNormal; //!< Vector3D normal to point of intersection.
	Material *m_pmCurMat; //!< Pointer to Material that the specified pixel should render.
};

/*! Renderable object which may be hit by rays */
class Object {
public:
	/*! Return boolean value if member object is hit by specified ray.
	*
	*	Example:
	*
	*		Hit(ray, hitrec, 0.001, DBL_MAX);
	*/
	virtual bool Hit(const Ray &r, HitRecord &rec, double tMin, double tMax) const = 0;
	virtual double clType() const = 0;
	virtual Vector3D clCenter() const = 0;
	virtual double clRadius() const = 0;
	virtual Vector3D clBound1() const = 0;
	virtual Vector3D clBound2() const = 0;
	virtual Material* clMatPtr() const = 0;
};

typedef std::vector<Object*> vList;
#endif // OBJECT_H