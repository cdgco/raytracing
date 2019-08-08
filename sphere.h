#ifndef SPHERE_H
#define SPHERE_H

#include "object.h"
#include "material.h"
class Material;

/*! Sphere creation and intersection functions */
class Sphere : public Object {
public: 
	Sphere() {}
	/*! Sphere Shaped Object
	*
	*	Example:
	*
	*		Sphere(Vector3D(-1), 1, new Lambertian(Vector3D(1, 0, 0)));
	*/
	Sphere(Vector3D cen, double r, Material *pm) : m_vCenter(cen), m_dRadius(r), m_pmCurMat(pm) {};
	virtual bool Hit(const Ray &r, HitRecord &rec, double tMin, double tMax) const;
	
	int clType() const {
		return 0;
	}
	Vector3D clCenter() const {
		return m_vCenter;
	}
	double clRadius() const {
		return m_dRadius;
	}
	Vector3D clBound1() const {
		return Vector3D(0);
	}
	Vector3D clBound2() const {
		return Vector3D(0);
	}
	virtual cl_double8 CurMat() const {
		return { cl_double(m_pmCurMat->MatColor().x()), cl_double(m_pmCurMat->MatColor().y()), cl_double(m_pmCurMat->MatColor().z()), cl_double(m_pmCurMat->MatFuzz()), cl_double(m_pmCurMat->MatRef()), cl_double(m_pmCurMat->MatType()), 0, 0 };
	}
	Vector3D m_vCenter; //!< Vector3D center of Sphere Object
	double m_dRadius; //!< Radius of Sphere Object
	Material *m_pmCurMat; //!< Pointer to Material that the Sphere should render.
};

#endif // SPHERE_H