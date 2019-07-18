#ifndef MATERIAL_H
#define MATERIAL_H

struct HitRecord;

#include "object.h"

Vector3D RandomInUnitSphere() {
	Vector3D m_vP;
	do {
		m_vP = 2.0*Vector3D(drand48(), drand48(), drand48()) - Vector3D(1);
	} while (m_vP.SquaredLength() >= 1.0);
	return m_vP;
}

Vector3D Reflect(const Vector3D& v, const Vector3D& n) {
	return v - 2 * StdDot(v, n)*n;
}

double Schlick(double cosine, double refid) {
	double d0 = (1 - refid) / (1 + refid);
	d0 = d0 * d0;
	return d0 + (1 - d0)*pow((1 - cosine), 5);
}

bool Refract(const Vector3D& v, const Vector3D& n, double NiOverNt, Vector3D& refracted) {
	Vector3D m_vUV = UnitVector(v);
	double dDT = m_vUV.Dot(n);
	double discriminant = 1.0 - (NiOverNt * NiOverNt * (1 - dDT * dDT));
	if (discriminant > 0) {
		refracted = NiOverNt * (m_vUV - (n * dDT)) - (n * sqrt(discriminant));
		return true;
	}
	else return false;
}

class Material {
public:
	virtual bool Scatter(const Ray& r_in, const HitRecord& rec, Vector3D& attenuation, Ray& scattered) const = 0;

};

class Lambertian : public Material {
public: 
	Lambertian(const Vector3D& a) : m_vAlbedo(a) {}
	virtual bool Scatter(const Ray& r_in, const HitRecord& rec, Vector3D& attenuation, Ray& scattered) const {
		Vector3D m_vTarget = rec.m_vP + rec.m_vNormal + RandomInUnitSphere();
		scattered = Ray(rec.m_vP, m_vTarget - rec.m_vP);
		attenuation = m_vAlbedo;
		return true;
	}
	Vector3D m_vAlbedo;
};
class Metal : public Material {
public:
	Metal(const Vector3D& a, double f) : m_vAlbedo(a) { 
		if (f < 1) { dFuzz = f; }  
		else { dFuzz = 1; }
}
	virtual bool Scatter(const Ray& r_in, const HitRecord& rec, Vector3D& attenuation, Ray& scattered) const {
		Vector3D m_vReflected = Reflect(UnitVector(r_in.Direction()), rec.m_vNormal);
		scattered = Ray(rec.m_vP, m_vReflected + dFuzz*RandomInUnitSphere());
		attenuation = m_vAlbedo;
		return (scattered.Direction().Dot(rec.m_vNormal) > 0);
	}
	Vector3D m_vAlbedo;
	double dFuzz;
};

class Dielectric : public Material {
public:
	Dielectric(double ri) : dRefId(ri) {}
	virtual bool Scatter(const Ray& r_in, const HitRecord& rec, Vector3D& attenuation, Ray& scattered) const {
		Vector3D m_vOutwardNormal;
		Vector3D m_vReflected = Reflect(r_in.Direction(), rec.m_vNormal);
		double dNiOverNt;
		attenuation = Vector3D(1.0);
		Vector3D m_refracted;
		double dReflectProb;
		double dCosine;
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
			dReflectProb = Schlick(dCosine, dRefId);
		}
		else dReflectProb = 1.0;
		if (drand48() < dReflectProb) scattered = Ray(rec.m_vP, m_vReflected);
		else scattered = Ray(rec.m_vP, m_refracted);
		return true;
	}
	double dRefId;
};
#endif // MATERIAL_H