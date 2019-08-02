#include "camera.h"

/*! For each pixel sample, create a ray using a given x and y value, mapped from the camera to the target.
	*
	*	Example:
	*
	*		GetRay(x, y);
	*/
Ray Camera::GetRay(double s, double t) {
	Vector3D vRD = (m_dAperture / 2) * RandomInUnitDisk();
	Vector3D vOffset = m_vU * vRD.x() + m_vV * vRD.y();
	//return Ray(m_vOrigin + vOffset, m_vLowerLeftCorner + s * m_vHorizontal + t * m_vVertical - m_vOrigin - vOffset);
	return Ray(m_vOrigin, (Vector3D(-2.0, -1.0, -1.0) + (s * Vector3D(4.0, 0.0, 0.0)) + (t * Vector3D(0.0, 2.0, 0.0))));
}

/*! Returns a random vector with a dot product greater than 1.0.
*
*	Example:
*
*		RandomInUnitDisk();
*/
Vector3D Camera::RandomInUnitDisk() {
	Vector3D vP;
	do {
		vP = 2.0*Vector3D(drand48(), drand48(), 0) - Vector3D(1, 1, 0);
	} while (vP.Dot(vP) >= 1.0);
	return vP;
}