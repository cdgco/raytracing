#ifndef SPHERE_H
#define SPHERE_H

#include "material.h"

/** Sphere creation and intersection functions */
class Sphere : public Object {
public: 
	Sphere() {}
	/** Sphere Shaped Object

		Example:

			Sphere(Vector3D(-1), 1, new Lambertian(Vector3D(1, 0, 0)));
	*/
	Sphere(Vector3D cen, double r, Material *pm) : m_vCenter(cen), m_dRadius(r), m_pmCurMat(pm) {};
	virtual bool Hit(const Ray &r, HitRecord &rec, double tMin, double tMax) const;
	Vector3D m_vCenter; ///< Vector3D center of Sphere Object
	double m_dRadius; ///< Radius of Sphere Object
	Material *m_pmCurMat; ///< Pointer to Material that the Sphere should render.
};

/** Return boolean value if box is hit by specified ray.

	Example:

		Box::Hit(ray, hitrec, 0.001, DBL_MAX);
*/
bool Sphere::Hit(const Ray &r, HitRecord &rec, double tMin, double tMax) const {
	Vector3D m_vOC = r.Origin() - m_vCenter;
	double dA = r.Direction().Dot(r.Direction());
	double dB = m_vOC.Dot(r.Direction());
	double dC = m_vOC.Dot(m_vOC) - m_dRadius * m_dRadius;
	double dDiscriminant = dB * dB - dA * dC;
	if (dDiscriminant > 0) {
		double dT = (-dB - sqrt(dDiscriminant)) / dA;
		if (dT < tMax && dT > tMin) {
			HitRecord tempRec = { dT,  r.PointAtParameter(dT), (r.PointAtParameter(dT) - m_vCenter) / m_dRadius, m_pmCurMat };
			rec = tempRec;
			return true;
		}
		dT = (-dB + sqrt(dDiscriminant)) / dA;
		if (dT < tMax && dT > tMin) {
			HitRecord tempRec = { dT,  r.PointAtParameter(dT), (r.PointAtParameter(dT) - m_vCenter) / m_dRadius, m_pmCurMat };
			rec = tempRec;
			return true;
		}
	}
	return false;
}

#endif // SPHERE_H