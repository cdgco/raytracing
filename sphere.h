#ifndef SPHERE_H
#define SPHERE_H

#include "object.h"

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
	virtual bool clHit(const Ray &r, HitRecord &rec, double tMin, double tMax) const;
	Vector3D m_vCenter; //!< Vector3D center of Sphere Object
	double m_dRadius; //!< Radius of Sphere Object
	Material *m_pmCurMat; //!< Pointer to Material that the Sphere should render.
};

#endif // SPHERE_H