#ifndef MATERIAL_H
#define MATERIAL_H

struct HitRecord;

#include "object.h"

/*! Renderable Material used to color Objects. */
class Material {
public:
	/*! Return boolean value if ray scatters based on Material attenuation.
	*
	*	Example:
	*	
	*		Scatter(StartingRay, rec, attenuation, ScatteredRay);
	*/
	virtual bool Scatter(const Ray &r_in, const HitRecord &rec, Vector3D &attenuation, Ray &scattered) const = 0;
	/*! Return int value based on which material an object has. Used to determine normal calculation for box
	*
	*	Example:
	*
	*		MatType();
	*/
	virtual int MatType() const = 0;
	/*! Return Vector3D value for material color.
	*
	*	Example:
	*
	*		MatColor();
	*/
	virtual Vector3D MatColor() const = 0;
	/*! Return double value for material fuzz.
	*
	*	Example:
	*
	*		MatFuzz();
	*/
	virtual double MatFuzz() const = 0;
	/*! Return double value for material refraction index.
	*
	*	Example:
	*
	*		MatRef();
	*/
	virtual double MatRef() const = 0;
	/*! Return reflected Vector3D for input direction and normal.
	*
	*	Example:
	*
	*		Reflect(ray_direction, normal);
	*/
	static Vector3D Reflect(const Vector3D &v, const Vector3D &n) {
		return v - 2 * StdDot(v, n)*n;
	}
	/*! Returns a random Vector3D with a length greater than 1.0.
	*
	*	Example:
	*
	*		RandomInUnitSphere();
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