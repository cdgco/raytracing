#ifndef DIELECTRIC_H
#define DIELECTRIC_H

#include "material.h"

bool Refract(const Vector3D &v, const Vector3D &n, double NiOverNt, Vector3D &refracted) {
	Vector3D m_vUV = UnitVector(v);
	double dDT = m_vUV.Dot(n);
	double discriminant = 1.0 - (NiOverNt * NiOverNt * (1 - dDT * dDT));
	if (discriminant > 0) {
		refracted = NiOverNt * (m_vUV - (n * dDT)) - (n * sqrt(discriminant));
		return true;
	}
	else return false;
}

/** Dielectric (water, glass, diamond) refractive type Material. */
class Dielectric : public Material {
public:
	/** Create Dielectric Material for Object with specified refractive index.

	Example:
			Dielectric(1.3);
	*/
	Dielectric(double ri) : dRefId(ri) {}
	virtual bool Scatter(const Ray &r_in, const HitRecord &rec, Vector3D &attenuation, Ray &scattered) const {
		Vector3D m_vOutwardNormal, m_refracted;
		double dNiOverNt, dReflectProb, dCosine;
		Vector3D m_vReflected = Reflect(r_in.Direction(), rec.m_vNormal);
		attenuation = Vector3D(1.0);
		if (r_in.Direction().Dot(rec.m_vNormal) > 0) {
			m_vOutwardNormal = -rec.m_vNormal;
			dNiOverNt = dRefId;
			dCosine = dRefId * StdDot(r_in.Direction(), rec.m_vNormal) / r_in.Direction().Length();
		}
		else {
			m_vOutwardNormal = rec.m_vNormal;
			dNiOverNt = 1.0 / dRefId;
			dCosine = -(r_in.Direction().Dot(rec.m_vNormal)) / r_in.Direction().Length();
		}
		if (Refract(r_in.Direction(), m_vOutwardNormal, dNiOverNt, m_refracted)) {
			double d0 = (1 - dRefId) / (1 + dRefId);
			d0 = d0 * d0;
			dReflectProb = d0 + (1 - d0)*pow((1 - dCosine), 5);
		}
		else dReflectProb = 1.0;
		if (drand48() < dReflectProb) scattered = Ray(rec.m_vP, m_vReflected);
		else scattered = Ray(rec.m_vP, m_refracted);
		return true;
	}
	double dRefId; ///< Refractive index of Dielectric. Air = 1.0; Glass = 1.3 - 1.7; Diamond = 2.4.
};
#endif // DIELECTRIC_H