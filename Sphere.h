#ifndef SPHERE_H
#define SPHERE_H

#include "material.h"
#include "box.h"

typedef std::vector<Object*> vList;

void NewItem(vList &vector, Object* object) {
	vector.push_back(object);
}

class Sphere : public Object {
public: 
	Sphere() {}
	Sphere(Vector3D cen, double r, Material *pm) : m_vCenter(cen), dRadius(r), pmCurMat(pm) {};
	virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const;
	virtual Box BBox(vList &vector, Material *pm = new Lambertian(Vector3D(0, 0, 0)));
	Vector3D m_vCenter; double dRadius; Material *pmCurMat;
};

bool Sphere::Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const {
	Vector3D m_vOC = r.Origin() - m_vCenter;
	double dA = r.Direction().Dot(r.Direction());
	double dB = m_vOC.Dot(r.Direction());
	double dC = m_vOC.Dot(m_vOC) - dRadius * dRadius;
	double dDiscriminant = dB * dB - dA * dC;
	if (dDiscriminant > 0) {
		double dTemp = (-dB - sqrt(dDiscriminant)) / dA;
		if (dTemp < tMax && dTemp > tMin) {
			rec.dT = dTemp;
			rec.m_vP = r.PointAtParameter(rec.dT);
			rec.m_vNormal = (rec.m_vP - m_vCenter) / dRadius;
			rec.pmCurMat = pmCurMat;
			return true;
		}
		dTemp = (-dB + sqrt(dDiscriminant)) / dA;
		if (dTemp < tMax && dTemp > tMin) {
			rec.dT = dTemp;
			rec.m_vP = r.PointAtParameter(rec.dT);
			rec.m_vNormal = (rec.m_vP - m_vCenter) / dRadius;
			rec.pmCurMat = pmCurMat;
			return true;
		}
	}
	return false;
}

inline Box Sphere::BBox(vList &vector, Material *pm) {
	NewItem(vector, this);
	return Box(m_vCenter - dRadius, m_vCenter + dRadius, pm);
}
#endif // SPHERE_H