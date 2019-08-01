#ifndef BOX_H
#define BOX_H

#include "object.h"

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
	Box(Vector3D bound1, Vector3D bound2, const int type, const Vector3D &color = Vector3D(0), const double dFuzz = 0, const double dRefIdx = 0) : m_vColor(color), m_iType(type), m_dFuzz(dFuzz), m_dRefIdx(dRefIdx) { m_vBounds[0] = bound1; m_vBounds[1] = bound2; };
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