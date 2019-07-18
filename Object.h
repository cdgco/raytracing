#ifndef OBJECT_H
#define OBJECT_H

#include "ray.h"

class Material;

struct HitRecord {
	double dT;
	Vector3D m_vP;
	Vector3D m_vNormal;
	Material *pmCurMat;
};

class Object {
public:
	virtual bool Hit(const Ray &r, double tMin, double tMax, HitRecord &rec) const = 0;
};

#endif // OBJECT_H