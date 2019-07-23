#ifndef METAL_H
#define METAL_H

#include "material.h"

/*! Metal reflective type Material. */
class Metal : public Material {
public:
	/** Create Metal Material for Object with specified Vector3D color and fuzz value.
	*
	*	Example:
	*		Lambertian(Vector3D(1, 0, 0), 0.9);
	*/
	Metal(const Vector3D &a, double f) : m_vAlbedo(a) {
		if (f < 1) { m_dFuzz = f; } // Shift ray scatter by dFuzz amount to blur reflection.
		else { m_dFuzz = 1; } // If fuzz is disabled, do not alter reflection.
	}
	virtual bool Scatter(const Ray &r_in, const HitRecord &rec, Vector3D &attenuation, Ray &scattered) const {
		Vector3D vReflected = Reflect(UnitVector(r_in.Direction()), rec.m_vNormal);
		scattered = Ray(rec.m_vP, vReflected + m_dFuzz * RandomInUnitSphere()); 
		attenuation = m_vAlbedo;
		return (scattered.Direction().Dot(rec.m_vNormal) > 0);
	}
	Vector3D m_vAlbedo; //!< Vector3D attenuation of light absorbed or reflected from object.
	double m_dFuzz; //!< 0.3 - 1.0 (double) value setting the randomness of reflections, thus blurring the metal surface.
};
#endif // METAL_H