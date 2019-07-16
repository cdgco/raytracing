#ifndef OBJECTH
#define OBJECTH

#include "Ray.h"

class Material;

struct HitRecord {
	double dT;
	Vector3D m_p;
	Vector3D m_normal;
	Material *mat_ptr;
};

class Object {
public:
	// Prototype shape function
	virtual bool Hit(const Ray& m_r, double t_min, double t_max, HitRecord& rec) const = 0;
};

#endif // OBJECTH