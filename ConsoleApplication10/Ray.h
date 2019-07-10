#ifndef RAYH
#define RAYH

#include "Vector3D.h"

// 
// Ray(Vector3D A, Vector3D B) 
// 
// Creates an object consisting of two vectors
// Vector A moving to Vector B
// 
// Member Functions:
//	Origin(): Returns Starting Vector
//	Direction(): Returns Ending Vector
//	PointAtParameter(t): Returns Vector at point t along ray
//
class Ray {
public:
	Ray() {}
	Ray(const Vector3D& a, const Vector3D& b) { A = a; B = b; }

	// Starting Vector of Ray
	Vector3D Origin() const { return A; }

	// Ending Vector of Ray
	Vector3D Direction() const { return B; }

	// Vector at point t along Ray
	Vector3D PointAtParameter(double t) const { return A + t * B; }

	Vector3D A;
	Vector3D B;

};

#endif // RAYH