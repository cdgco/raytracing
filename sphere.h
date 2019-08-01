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
	Sphere(Vector3D cen, double r, const int type, const Vector3D &color = Vector3D(0), const double dFuzz = 0, const double dRefIdx = 0) : m_vCenter(cen), m_dRadius(r), m_vColor(color), m_iType(type), m_dFuzz(dFuzz), m_dRefIdx(dRefIdx) {};
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
	Vector3D clColor() const {
		return m_vColor;
	}
	virtual int clMType() const {
		return m_iType;
	}
	virtual double clFuzz() const {
		return m_dFuzz;
	}
	virtual double clRefIdx() const {
		return m_dRefIdx;
	}

	int m_iType;
	Vector3D m_vCenter; //!< Vector3D center of Sphere Object
	Vector3D m_vColor; //!< Vector3D center of Sphere Object
	double m_dRadius; //!< Radius of Sphere Object
	double m_dFuzz; //!< Radius of Sphere Object
	double m_dRefIdx; //!< Radius of Sphere Object
	Material *m_pmCurMat; //!< Pointer to Material that the Sphere should render.
};

#endif // SPHERE_H