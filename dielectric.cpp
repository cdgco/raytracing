#include "dielectric.h"

bool Dielectric::Scatter(const Ray &r_in, const HitRecord &rec, Vector3D &attenuation, Ray &scattered) const {
	Vector3D vOutwardNormal, m_refracted;
	double dNiOverNt, dReflectProb, dCosine;
	Vector3D vReflected = Reflect(r_in.Direction(), rec.m_vNormal);
	attenuation = Vector3D(1.0);
	if (r_in.Direction().Dot(rec.m_vNormal) > 0) {
		vOutwardNormal = -rec.m_vNormal;
		dNiOverNt = m_dRefId;
		dCosine = m_dRefId * StdDot(r_in.Direction(), rec.m_vNormal) / r_in.Direction().Length();
	}
	else {
		vOutwardNormal = rec.m_vNormal;
		dNiOverNt = 1.0 / m_dRefId;
		dCosine = -(r_in.Direction().Dot(rec.m_vNormal)) / r_in.Direction().Length();
	}
	if (Refract(r_in.Direction(), vOutwardNormal, dNiOverNt, m_refracted)) {
		double d0 = (1 - m_dRefId) / (1 + m_dRefId);
		d0 = d0 * d0;
		dReflectProb = d0 + (1 - d0)*pow((1 - dCosine), 5);
	}
	else dReflectProb = 1.0;
	if (drand48() < dReflectProb) scattered = Ray(rec.m_vP, vReflected);
	else scattered = Ray(rec.m_vP, m_refracted);
	return true;
}