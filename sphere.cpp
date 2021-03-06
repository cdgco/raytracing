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