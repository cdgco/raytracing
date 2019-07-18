#ifndef RAY_H
#define RAY_H

#include "vector.h"

class Ray {
public:
	Ray() {}
	Ray(const Vector3D& a, const Vector3D& b) { A = a; B = b; InvDir = 1 / B; Sign[0] = (InvDir.x() < 0); Sign[1] = (InvDir.y() < 0); Sign[2] = (InvDir.z() < 0); UnitDirection = UnitVector(B); }

	Vector3D Origin() const { return A; }
	Vector3D Direction() const { return B; }
	Vector3D PointAtParameter(double t) const { return A + t * B; }
	
	int Sign[3];
	Vector3D A, B, InvDir, UnitDirection;
};

#endif // RAY_H