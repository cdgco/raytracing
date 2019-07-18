#ifndef SPHERE_H
#define SPHERE_H

#include "material.h"
#include "box.h"
typedef std::vector<Object*> vList;

void NewItem(vList &vector, Object* object) {
	vector.push_back(object);
}

/** Sphere creation and intersection functions */
class Sphere : public Object {
public: 
	Sphere() {}
	/** Sphere Shaped Object

		Example:

			Sphere(Vector3D(-1), 1, new Lambertian(Vector3D(1, 0, 0)));
	*/
	Sphere(Vector3D cen, double r, Material *pm) : m_vCenter(cen), dRadius(r), pmCurMat(pm) {};
	virtual bool Hit(const Ray &r, HitRecord &rec, double tMin, double tMax) const;
	virtual Box BBox(vList &vector, Material *pm = new Lambertian(Vector3D(0, 0, 0)));
	Vector3D m_vCenter; ///< Vector3D center of Sphere Object
	double dRadius; ///< Radius of Sphere Object
	Material *pmCurMat; ///< Pointer to Material that the Sphere should render.
};

/** Return boolean value if box is hit by specified ray.

	Example:

		Box::Hit(ray, hitrec, 0.001, DBL_MAX);
*/
bool Sphere::Hit(const Ray &r, HitRecord &rec, double tMin, double tMax) const {
	Vector3D m_vOC = r.Origin() - m_vCenter;
	double dA = r.Direction().Dot(r.Direction());
	double dB = m_vOC.Dot(r.Direction());
	double dC = m_vOC.Dot(m_vOC) - dRadius * dRadius;
	double dDiscriminant = dB * dB - dA * dC;
	if (dDiscriminant > 0) {
		double dTemp = (-dB - sqrt(dDiscriminant)) / dA;
		if (dTemp < tMax && dTemp > tMin) {
			rec.dT = dTemp;
			rec.m_vP = r.PointAtParameter(rec.dT);
			rec.m_vNormal = (rec.m_vP - m_vCenter) / dRadius;
			rec.pmCurMat = pmCurMat;
			return true;
		}
		dTemp = (-dB + sqrt(dDiscriminant)) / dA;
		if (dTemp < tMax && dTemp > tMin) {
			rec.dT = dTemp;
			rec.m_vP = r.PointAtParameter(rec.dT);
			rec.m_vNormal = (rec.m_vP - m_vCenter) / dRadius;
			rec.pmCurMat = pmCurMat;
			return true;
		}
	}
	return false;
}

/** Return Box shaped Object around Sphere Object attached to given vector list.

	Box is a black Lambertian Material by default.

	Example:

		Sphere::BBox(vList);

		Sphere::BBox(vList, new Lambertian(Vector3D(1, 0, 0));
*/
inline Box Sphere::BBox(vList &vector, Material *pm) {
	NewItem(vector, this);
	return Box(m_vCenter - dRadius, m_vCenter + dRadius, pm);
}
#endif // SPHERE_H