#ifndef MATERIAL_H
#define MATERIAL_H

struct HitRecord;

#include "object.h"

/** Returns a random Vector3D with a length greater than 1.0.

	Example:

		RandomInUnitSphere();
*/
Vector3D RandomInUnitSphere() {
	Vector3D m_vP;
	do {
		m_vP = 2.0*Vector3D(drand48(), drand48(), drand48()) - Vector3D(1);
	} while (m_vP.SquaredLength() >= 1.0);
	return m_vP;
}

Vector3D Reflect(const Vector3D &v, const Vector3D &n) {
	return v - 2 * StdDot(v, n)*n;
}

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

/** Renderable Material used to color Objects. */
class Material {
public:
	/** Return boolean value if ray scatters based on Material attenuation.

		Example:
		
			Scatter(StartingRay, rec, attenuation, ScatteredRay);
	*/
	virtual bool Scatter(const Ray &r_in, const HitRecord &rec, Vector3D &attenuation, Ray &scattered) const = 0;
};
/** Lambertian diffuse (matte) type Material. */
class Lambertian : public Material {
public: 
	/** Create Lambertian Material for object with specified Vector3D color.
	
	Example:
			Lambertian(Vector3D(1, 0, 0));
	*/
	Lambertian(const Vector3D& a) : m_vAlbedo(a) {}
	virtual bool Scatter(const Ray &r_in, const HitRecord &rec, Vector3D &attenuation, Ray &scattered) const {
		Vector3D m_vTarget = rec.m_vP + rec.m_vNormal + RandomInUnitSphere();
		scattered = Ray(rec.m_vP, m_vTarget - rec.m_vP);
		attenuation = m_vAlbedo;
		return true;
	}
	Vector3D m_vAlbedo; ///< Vector3D attenuation of light absorbed or reflected from Object.
};
/** Metal reflective type Material. */
class Metal : public Material {
public:
	/** Create Metal Material for Object with specified Vector3D color and fuzz value.

	Example:
			Lambertian(Vector3D(1, 0, 0), 0.9);
	*/
	Metal(const Vector3D &a, double f) : m_vAlbedo(a) {
		if (f < 1) { dFuzz = f; }
		else { dFuzz = 1; }
}
	virtual bool Scatter(const Ray &r_in, const HitRecord &rec, Vector3D &attenuation, Ray &scattered) const {
		Vector3D m_vReflected = Reflect(UnitVector(r_in.Direction()), rec.m_vNormal);
		scattered = Ray(rec.m_vP, m_vReflected + dFuzz * RandomInUnitSphere());
		attenuation = m_vAlbedo;
		return (scattered.Direction().Dot(rec.m_vNormal) > 0);
	}
	Vector3D m_vAlbedo; ///< Vector3D attenuation of light absorbed or reflected from object.
	double dFuzz; ///< 0.3 - 1.0 (double) value setting the randomness of reflections, thus blurring the metal surface.
};
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

#endif // MATERIAL_H