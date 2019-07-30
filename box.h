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
	virtual bool Hit(const Ray &r, HitRecord &rec, double tMin, double tMax) const;
	virtual Vector3D NormalCalc(const Vector3D vP) const;

	Material *m_pmCurMat; //!< Pointer to Material that the box should render.
	Vector3D m_vBounds[2]; //!< Array of Vector3D bounds for box
	Vector3D m_vCenter = Vector3D((m_vBounds[0].x() + m_vBounds[1].x()) / 2, (m_vBounds[0].y() + m_vBounds[1].y()) / 2, (m_vBounds[0].z() + m_vBounds[1].z()) / 2); // Vector3D center of Box Object
};
#endif // BOXH