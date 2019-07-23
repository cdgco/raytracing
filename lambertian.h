#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include "material.h"

/*! Lambertian diffuse (matte) type Material. */
class Lambertian : public Material {
public:
	/** Create Lambertian Material for object with specified Vector3D color.
	*
	*	Example:
	*	
	*		Lambertian(Vector3D(1, 0, 0));
	*/
	Lambertian(const Vector3D& a) : m_vAlbedo(a) {}
	virtual bool Scatter(const Ray &r_in, const HitRecord &rec, Vector3D &attenuation, Ray &scattered) const {
		Vector3D vTarget = rec.m_vP + rec.m_vNormal + RandomInUnitSphere();
		scattered = Ray(rec.m_vP, vTarget - rec.m_vP);
		attenuation = m_vAlbedo;
		return true;
	}
	Vector3D m_vAlbedo; ///< Vector3D attenuation of light absorbed or reflected from Object.
};
#endif // LAMBERTIAN_H