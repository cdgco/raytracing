#ifndef RAY_H
#define RAY_H

#include "vector.h"

/*! Ray function. */
class Ray {
public:
	Ray() {}
	/*! Creates ray between two Vector3D locations.
	*
	*	Example:
	*
	*		Ray r(Vector3D(3), Vector3D(0));
	*/
	Ray(const Vector3D &a, const Vector3D &b) { 
		m_vA = a; m_vB = b; 
		m_vInvDir = 1 / m_vB; 
		m_iSign[0] = (m_vInvDir.x() < 0); 
		m_iSign[1] = (m_vInvDir.y() < 0); 
		m_iSign[2] = (m_vInvDir.z() < 0); 
		m_vUnitDirection = UnitVector(m_vB); 
	}

	Vector3D Origin() const { return m_vA; } ///< Returns Vector3D starting location of Ray.
	Vector3D Direction() const { return m_vB; } ///< Returns Vector3D ending location of Ray.
	Vector3D PointAtParameter(double t) const { return m_vA + t * m_vB; } ///< Returns Vector3D location on ray based on given t value.
	
	int m_iSign[3]; ///< Array of 0 or 1 values dependent on InvDir of x(0), y(1), or z(2) value of Ray.
	Vector3D m_vA; ///< Starting location of Ray; Origin.
	Vector3D m_vB; ///< Ending location of Ray; Direction.
	Vector3D m_vInvDir; ///< Inverted direction of Ray.
	Vector3D m_vUnitDirection; ///< UnitVector Vector3D of Ray Direction.
};
#endif // RAY_H