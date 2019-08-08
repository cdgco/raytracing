#ifndef BOX_H
#define BOX_H

#include "object.h"
#include "material.h"
class Material;

/*! Box creation and intersection functions */
class Box : public Object {
public:
	Box() {}
	/*! Box Shaped Object
	*
	*	Example:
	*
	*		Box(Vector3D(-1), Vector3D(1), new Lambertian(Vector3D(1, 0, 0)));
	*/
	Box(Vector3D bound1, Vector3D bound2, Material *pm) : m_pmCurMat(pm) { m_vBounds[0] = bound1; m_vBounds[1] = bound2; };
	virtual bool Hit(const Ray &r, HitRecord &rec, double tMin, double tMax) const;

	int clType() const {
		return 1;
	}
	Vector3D clCenter() const {
		return m_vCenter;
	}
	double clRadius() const {
		return 0;
	}
	Vector3D clBound1() const {
		return m_vBounds[0];
	}
	Vector3D clBound2() const {
		return m_vBounds[1];
	}
	virtual cl_double8 CurMat() const {
		return { cl_double(m_pmCurMat->MatColor().x()), cl_double(m_pmCurMat->MatColor().y()), cl_double(m_pmCurMat->MatColor().z()), cl_double(m_pmCurMat->MatFuzz()), cl_double(m_pmCurMat->MatRef()), cl_double(m_pmCurMat->MatType()), 0, 0 };
	}
	virtual Vector3D NormalCalc(const Vector3D inter) const;
	Material *m_pmCurMat; //!< Pointer to Material that the box should render.
	Vector3D m_vBounds[2]; //!< Array of Vector3D bounds for box
	Vector3D m_vCenter = Vector3D((m_vBounds[0].x() + m_vBounds[1].x()) / 2, (m_vBounds[0].y() + m_vBounds[1].y()) / 2, (m_vBounds[0].z() + m_vBounds[1].z()) / 2); // Vector3D center of Box Object
	int m_iType;
	Vector3D m_vColor; //!< Vector3D center of Sphere Object
	double m_dFuzz; //!< Radius of Sphere Object
	double m_dRefIdx; //!< Radius of Sphere Object

};
#endif // BOXH