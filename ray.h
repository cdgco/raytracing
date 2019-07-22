#ifndef RAY_H
#define RAY_H

#include "vector.h"

/** Ray function. */
class Ray {
public:
	Ray() {}
	/** Creates ray between two Vector3D locations.

	Example:

		Ray r(Vector3D(3), Vector3D(0));
	*/
	Ray(const Vector3D &a, const Vector3D &b) { A = a; B = b; InvDir = 1 / B; Sign[0] = (InvDir.x() < 0); Sign[1] = (InvDir.y() < 0); Sign[2] = (InvDir.z() < 0); UnitDirection = UnitVector(B); }

	Vector3D Origin() const { return A; } ///< Returns Vector3D starting location of Ray.
	Vector3D Direction() const { return B; } ///< Returns Vector3D ending location of Ray.
	Vector3D PointAtParameter(double t) const { return A + t * B; } ///< Returns Vector3D location on ray based on given t value.
	
	int Sign[3]; ///< Array of 0 or 1 values dependent on InvDir of x(0), y(1), or z(2) value of Ray.
	Vector3D A; ///< Starting location of Ray; Origin.
	Vector3D B; ///< Ending location of Ray; Direction.
	Vector3D InvDir; ///< Inverted direction of Ray.
	Vector3D UnitDirection; ///< UnitVector Vector3D of Ray Direction.
};
#endif // RAY_H