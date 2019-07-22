#ifndef MATERIAL_H
#define MATERIAL_H

struct HitRecord;

#include "object.h"

/** Renderable Material used to color Objects. */
class Material {
public:
	/** Return boolean value if ray scatters based on Material attenuation.

		Example:
		
			Scatter(StartingRay, rec, attenuation, ScatteredRay);
	*/
	virtual bool Scatter(const Ray &r_in, const HitRecord &rec, Vector3D &attenuation, Ray &scattered) const = 0;
	static Vector3D Reflect(const Vector3D &v, const Vector3D &n) {
		return v - 2 * StdDot(v, n)*n;
	}
	/** Returns a random Vector3D with a length greater than 1.0.

		Example:

			RandomInUnitSphere();
	*/
	static Vector3D RandomInUnitSphere() {
		Vector3D m_vP;
		do {
			m_vP = 2.0*Vector3D(drand48(), drand48(), drand48()) - Vector3D(1);
		} while (m_vP.SquaredLength() >= 1.0);
		return m_vP;
	}
};
#endif // MATERIAL_H