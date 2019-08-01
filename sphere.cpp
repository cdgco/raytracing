#include "sphere.h"

/*! Return boolean value if box is hit by specified ray.
*
*	Example:
*
*		Box::Hit(ray, hitrec, 0.001, DBL_MAX);
*/
bool Sphere::Hit(const Ray &r, HitRecord &rec, double tMin, double tMax) const {
	Vector3D vOC = r.Origin() - m_vCenter;
	double dA = r.Direction().Dot(r.Direction());
	double dB = vOC.Dot(r.Direction());
	double dC = vOC.Dot(vOC) - m_dRadius * m_dRadius;
	double dDiscriminant = dB * dB - dA * dC;
	if (dDiscriminant > 0) {
		double dT = (-dB - sqrt(dDiscriminant)) / dA;
		if (dT < tMax && dT > tMin) {
			rec = { dT,  r.PointAtParameter(dT), (r.PointAtParameter(dT) - m_vCenter) / m_dRadius, m_pmCurMat };
			return true;
		}
		dT = (-dB + sqrt(dDiscriminant)) / dA;
		if (dT < tMax && dT > tMin) {
			rec = { dT,  r.PointAtParameter(dT), (r.PointAtParameter(dT) - m_vCenter) / m_dRadius, m_pmCurMat };
			return true;
		}
	}
	return false;
}
double Sphere::clType() const {
	return 0.0;
}
Vector3D Sphere::clCenter() const {
	return m_vCenter;
}
double Sphere::clRadius() const {
	return m_dRadius;
}
Vector3D Sphere::clBound1() const {
	return Vector3D(0);
}
Vector3D Sphere::clBound2() const {
	return Vector3D(0);
}
Material* Sphere::clMatPtr() const {
	return m_pmCurMat;
}