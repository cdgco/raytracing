#ifndef OBJECT_H
#define OBJECT_H

#include "ray.h"
#define CL_TARGET_OPENCL_VERSION 220
#pragma warning(disable : 4996)
#pragma warning(disable : 4244)
#include "CL/cl.h"
#include "CL/cl_platform.h"

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
	/*! Returns type of object as int. Sphere (0) or Box (1).
	*
	*	Example:
	*
	*		clType();
	*/
	virtual int clType() const = 0;
	/*! Returns Vector3D center of object.
	*
	*	Example:
	*
	*		clCenter();
	*/
	virtual Vector3D clCenter() const = 0;
	/*! Returns double radius of object.
	*
	*	Example:
	*
	*		clRadius();
	*/
	virtual double clRadius() const = 0;
	/*! Returns first Vector3D bound of object.
	*
	*	Example:
	*
	*		clBound1();
	*/
	virtual Vector3D clBound1() const = 0;
	/*! Returns second Vector3D bound of object.
	*
	*	Example:
	*
	*		clBound2();
	*/
	virtual Vector3D clBound2() const = 0;
	/*! Returns cl_double8 formatted object material.
	*
	*	Example:
	*
	*		CurMat();
	*/
	virtual cl_double8 CurMat() const = 0;
};

typedef std::vector<Object*> vList;
#endif // OBJECT_H