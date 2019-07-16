#ifndef MATERIALH
#define MATERIALH

struct HitRecord;

#include "Ray.h"
#include "Object.h"

// Produces random Vector3D (i.e. three values with range 0 - 1)
Vector3D RandomInUnitSphere() {
	Vector3D m_p;
	do {
		m_p = 2.0*Vector3D(drand48(), drand48(), drand48()) - Vector3D(1, 1, 1);
	} while (m_p.SquaredLength() >= 1.0);
	return m_p;
}

Vector3D Reflect(const Vector3D& m_v, const Vector3D& m_n) {
	return m_v - 2 * StdDot(m_v, m_n)*m_n;
}

double Schlick(double dCosine, double dRefIdx) {
	double r0 = (1 - dRefIdx) / (1 + dRefIdx);
	r0 = r0 * r0;
	return r0 + (1 - r0)*pow((1 - dCosine), 5);
}

bool Refract(const Vector3D& m_v, const Vector3D& m_n, double ni_over_nt, Vector3D& m_refracted) {
	Vector3D m_uv = UnitVector(m_v);
	double dDT = m_uv.Dot(m_n);
	double discriminant = 1.0 - (ni_over_nt * ni_over_nt * (1 - dDT * dDT));
	if (discriminant > 0) {
		m_refracted = ni_over_nt * (m_uv - (m_n * dDT)) - (m_n * sqrt(discriminant));
		return true;
	}
	else {
		return false;
	}
}

class Material {
public:
	virtual bool Scatter(const Ray& m_r_in, const HitRecord& rec, Vector3D& m_attenuation, Ray& m_scattered) const = 0;

};

class Lambertian : public Material {
public: 
	Lambertian(const Vector3D& m_a) : m_albedo(m_a) {}
	virtual bool Scatter(const Ray& m_r_in, const HitRecord& rec, Vector3D& m_attenuation, Ray& m_scattered) const {
		Vector3D m_target = rec.m_p + rec.m_normal + RandomInUnitSphere();
		m_scattered = Ray(rec.m_p, m_target - rec.m_p);
		m_attenuation = m_albedo;
		return true;
	}
	Vector3D m_albedo;
};
// Vector3D coordinates
// Fuzz level
class Metal : public Material {
public:
	Metal(const Vector3D& m_a, double dF) : m_albedo(m_a) { 
		if (dF < 1) { fuzz = dF; }  
		else { fuzz = 1; }
}
	virtual bool Scatter(const Ray& m_r_in, const HitRecord& rec, Vector3D& m_attenuation, Ray& m_scattered) const {
		Vector3D m_reflected = Reflect(UnitVector(m_r_in.Direction()), rec.m_normal);
		m_scattered = Ray(rec.m_p, m_reflected + fuzz*RandomInUnitSphere());
		m_attenuation = m_albedo;
		return (m_scattered.Direction().Dot(rec.m_normal) > 0);
	}
	Vector3D m_albedo;
	double fuzz;
};

// Refractive Index: 
//	Air: 1.0
//	Glass: 1.3 - 1.7
//	Diamond: 2.4
class Dielectric : public Material {
public:
	Dielectric(double dRI) : dRefIdx(dRI) {}
	virtual bool Scatter(const Ray& m_r_in, const HitRecord& rec, Vector3D& m_attenuation, Ray& m_scattered) const {
		Vector3D m_outward_normal;
		Vector3D m_reflected = Reflect(m_r_in.Direction(), rec.m_normal);
		double ni_over_nt;
		m_attenuation = Vector3D(1.0, 1.0, 1.0);
		Vector3D m_refracted;
		double dReflectProb;
		double dCosine;
		if (m_r_in.Direction().Dot(rec.m_normal) > 0) {
			m_outward_normal = -rec.m_normal;
			ni_over_nt = dRefIdx;
			dCosine = dRefIdx * StdDot(m_r_in.Direction(), rec.m_normal) / m_r_in.Direction().Length();
		}
		else {
			m_outward_normal = rec.m_normal;
			ni_over_nt = 1.0 / dRefIdx;
			dCosine = -(m_r_in.Direction().Dot(rec.m_normal)) / m_r_in.Direction().Length();
		}
		if (Refract(m_r_in.Direction(), m_outward_normal, ni_over_nt, m_refracted)) {
			dReflectProb = Schlick(dCosine, dRefIdx);
		}
		else {
			dReflectProb = 1.0;
		}
		if (drand48() < dReflectProb) {
			m_scattered = Ray(rec.m_p, m_reflected);
		}
		else {
			m_scattered = Ray(rec.m_p, m_refracted);
		}
		return true;
	}
	double dRefIdx;
};

#endif // MATERIALH