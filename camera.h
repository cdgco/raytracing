#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"

/*! Integer structure storing output image size in pixels
*	Example:
*		SDim dimensions = { width, height };
*/
struct SDim {
	int m_iX; //!< Width in pixels
	int m_iY; //!< Height in pixels
};

/*! Perspective and ray generating functions. */
class Camera {
public:
	Camera() {}
	/*! Sets perspective for output image.
	*
	*	Camera location, subject, viewport, field of view, focus and aperture can be adjusted.
	*
	*	Example:
	*
	*		Camera(dimensions, Vector3D(5,0,0));
	*/
	Camera(const SDim &dims, Vector3D lookFrom, Vector3D lookAt = Vector3D(0), Vector3D viewUp = Vector3D(0, 1, 0), double aperture = 0.1, double Fov = 40) : m_vOrigin(lookFrom), m_dAperture(aperture) {

		double dHalfHeight = tan(Fov*M_PI / 360);
		double dHalfWidth = (dims.m_iX / dims.m_iY) * dHalfHeight;
		double dFocusDist = (lookFrom - lookAt).Length();
		m_vW = UnitVector(lookFrom - lookAt);
		m_vU = UnitVector(viewUp.Cross(m_vW));
		m_vV = m_vW.Cross(m_vU);
		m_vLowerLeftCorner = m_vOrigin - dHalfWidth * dFocusDist * m_vU - dHalfHeight * dFocusDist * m_vV - dFocusDist * m_vW;
		m_vHorizontal = 2 * dHalfWidth*dFocusDist*m_vU;
		m_vVertical = 2 * dHalfHeight*dFocusDist*m_vV;
	}
	virtual Ray GetRay(double s, double t);
	virtual Vector3D RandomInUnitDisk();

	Vector3D m_vU; //!< Vector3D distance from camera origin to target
	Vector3D m_vV; //!< Vector3D distance adjusted for viewup
	Vector3D m_vW; //!< Vector3D cross product of distance and viewUp
	Vector3D m_vLowerLeftCorner; //!< Vector3D lower left corner of grid
	Vector3D m_vHorizontal; //!< Vector3D width of grid
	Vector3D m_vVertical; //!< Vector3D height of grid
	Vector3D m_vOrigin; //!< Vector3D center of grid
	double m_dAperture; //!< Aperture of camera lens
};

#endif // CAMERA_H