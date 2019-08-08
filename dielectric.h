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

	virtual bool Scatter(const Ray &r_in, const HitRecord &rec, Vector3D &attenuation, Ray &scattered) const;
	virtual int MatType() const {
		return 2;
	}
	virtual Vector3D MatColor() const {
		return Vector3D(0);
	}
	virtual double MatFuzz() const {
		return 0;
	}
	virtual double MatRef() const {
		return m_dRefId;
	}
	double m_dRefId; //!< Refractive index of Dielectric. Air = 1.0; Glass = 1.3 - 1.7; Diamond = 2.4.
};
#endif // DIELECTRIC_H