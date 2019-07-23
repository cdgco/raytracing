#ifndef DIELECTRIC_H
#define DIELECTRIC_H

#include "material.h"

/*! Dielectric (water, glass, diamond) refractive type Material. */
class Dielectric : public Material {
public:
	/*! Create Dielectric Material for Object with specified refractive index.
	*
	*	Example:
	*
	*		Dielectric(1.3);
	*/
	Dielectric(double ri) : m_dRefId(ri) {}
	/*! Return boolean value for input vectors. If vector was refracted returns true.
	*
	*	Example:
	*	
	*		Refract(ray_direction, normal, NiOverNt, refracted_vector);
	*/
	static bool Refract(const Vector3D &v, const Vector3D &n, double NiOverNt, Vector3D &refracted) {
		Vector3D vUV = UnitVector(v);
		double dDT = vUV.Dot(n);
		double dDiscriminant = 1.0 - (NiOverNt * NiOverNt * (1 - dDT * dDT));
		if (dDiscriminant > 0) {
			refracted = NiOverNt * (vUV - (n * dDT)) - (n * sqrt(dDiscriminant));
			return true;
		}
		else return false;
	}

	virtual bool Scatter(const Ray &r_in, const HitRecord &rec, Vector3D &attenuation, Ray &scattered) const {
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
	double m_dRefId; ///< Refractive index of Dielectric. Air = 1.0; Glass = 1.3 - 1.7; Diamond = 2.4.
};
#endif // DIELECTRIC_H